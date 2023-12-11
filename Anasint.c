#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Analex.h"
#include "Anasint.h"
#include "Funcaux.h"
#include "Tabidentific.h"

char nomeId[TAM_MAX_LEXEMA];
char blockAux[TAM_MAX_LEXEMA];
int catId;
int escopo = GLOBAL;
int tipo;
bool isConst = 0;
bool zombie = 0;
bool ref = 0;
int dimensoes = 0;
int parametros = 0;
char cmdObj[20];
char label[TAM_MAX_LABEL];

void Prog(){
  fputs("INIP\n", fd_out);
  
  catId = VG;
  while(t.cat == PR && (t.codigo == CONST || t.codigo == CHAR || t.codigo == INT || t.codigo == REAL || t.codigo == BOOL)){
    Decl_list_var();
  }

  if(t.cat != PR || t.codigo != BLOCK){
    erroanasint(5);
  }

  t = AnaLex(fd);
  while(t.cat == ID){
    catId = BLK;
    strcpy(nomeId, t.lexema);
    escopo = GLOBAL;

    if(BuscaTabIdetifPorEscopo(nomeId, escopo) != -1) errosemantico(6);
    
    InsereTabIdentif(nomeId, catId, escopo, tipo, isConst, zombie, ref, dimensoes, parametros);
    strcpy(blockAux, t.lexema);
    
    t = AnaLex(fd);
    Decl_block_prot();

    if(t.cat != PR || t.codigo != BLOCK){
      erroanasint(7);
    }

    t = AnaLex(fd);
  }

  escopo = GLOBAL;
  catId = BLK;
  
  Block_main();

  t = AnaLex(fd);
  while(t.cat == PR && t.codigo == BLOCK){
    Block_def();
  }

  fputs("HALT\n", fd_out);
}

////////////////////////////////////////////////////////////////////////////////////////

void Decl_list_var(){
  if(t.cat == PR && t.codigo == CONST){
    t = AnaLex(fd);
    isConst = 1;
  }
  
  Tipo();
  
  t = AnaLex(fd);
  Decl_var();

  while(t.cat == SN && t.codigo == VIRGULA){
    t = AnaLex(fd);
    Decl_var();
  }

  isConst = 0;
}

////////////////////////////////////////////////////////////////////////////////////////

void Tipo(){
  if(t.cat != PR || !(t.codigo == CHAR || t.codigo == INT || t.codigo == REAL || t.codigo == BOOL)){
    erroanasint(8); 
  }

  if(t.codigo == CHAR){
    tipo = CHARACTER;
  }else if(t.codigo == INT){
    tipo = INTEIRO;
  }else if(t.codigo == REAL){
    tipo = REAL_TIPO;
  }else if(t.codigo == BOOL){
    tipo = BOOLEAN;
  }
  
}

////////////////////////////////////////////////////////////////////////////////////////

void Block_def(){
  int aux, i, dimensoes = 0, cont_param = 0, i_blk;
  char lista_params[TAM_MAX_PARAMS][TAM_MAX_LEXEMA];
  
  catId = BLK;
  
  t = AnaLex(fd);
  if(t.cat != ID){
    erroanasint(9);
  }

  strcpy(blockAux, t.lexema);
  i = BuscaTabIdetifBlk(blockAux);
  if(i == -1) errosemantico(4);
  i_blk = i;

  if(tabIdentif.identificador[i].def == 1) errosemantico(10);
  tabIdentif.identificador[i].def = 1;
  
  i++;

  t = AnaLex(fd);
  if(t.cat == PR && t.codigo == WITH){
    t = AnaLex(fd);
    Tipo();

    t = AnaLex(fd);
    if(t.cat != ID){
      erroanasint(9);
    }

    EditaNomeArg(t.lexema, i);
    strcpy(lista_params[cont_param], t.lexema);

    t = AnaLex(fd);
    while(t.cat == SN && t.codigo == ABRE_COLC){
      t = AnaLex(fd);
      if(t.cat != INTCON && t.cat != ID){
        erroanasint(11);
      }

      if(t.cat == ID){
        aux = BuscaTabIdetif(t.lexema);
        if(aux == -1) errosemantico(12);
        if(tabIdentif.identificador[aux].tipo != INTEIRO || tabIdentif.identificador[aux].isConst == 0) errosemantico(13);

        if(dimensoes == 0){
          tabIdentif.identificador[i].tam_dim1 = tabIdentif.identificador[aux].valorInt;
        }else if(dimensoes == 1){
          tabIdentif.identificador[i].tam_dim2 = tabIdentif.identificador[aux].valorInt;
        }else{
          tabIdentif.identificador[i].tam_dim3 = tabIdentif.identificador[aux].valorInt;
        }

      }else{
        if(dimensoes == 0){
          tabIdentif.identificador[i].tam_dim1 = t.valInt;
        }else if(dimensoes == 1){
          tabIdentif.identificador[i].tam_dim2 = t.valInt;
        }else{
          tabIdentif.identificador[i].tam_dim3 = t.valInt;
        }
      }

      t = AnaLex(fd);
      if (t.cat != SN || t.codigo != FECHA_COLC){
        erroanasint(14);
      }

      dimensoes++;

      if(dimensoes > 3) errosemantico(15);

      t = AnaLex(fd);
    }

    if(tipo != tabIdentif.identificador[i].tipo) errosemantico(16);
    if(dimensoes != tabIdentif.identificador[i].dimensoes) errosemantico(17);
    
    i++;
    cont_param++;

    while(t.cat == SN && t.codigo == VIRGULA){
      dimensoes = 0;
      t = AnaLex(fd);
      Tipo();

      t = AnaLex(fd);
      if(t.cat != ID){
        erroanasint(9);
      }

      EditaNomeArg(t.lexema, i);
      for(int j = 0; j < cont_param; j++){
        if(strcmp(lista_params[j], t.lexema) == 0){
          errosemantico(18);
        }
      }
      strcpy(lista_params[cont_param], t.lexema);

      t = AnaLex(fd);
      while(t.cat == SN && t.codigo == ABRE_COLC){
        t = AnaLex(fd);
        if(t.cat != INTCON && t.cat != ID){
          erroanasint(11);
        }

        if(t.cat == ID){
          aux = BuscaTabIdetif(t.lexema);
          if(aux == -1) errosemantico(12);
          if(tabIdentif.identificador[aux].tipo != INTEIRO || tabIdentif.identificador[aux].isConst == 0) errosemantico(13);

          if(dimensoes == 0){
            tabIdentif.identificador[i].tam_dim1 = tabIdentif.identificador[aux].valorInt;
          }else if(dimensoes == 1){
            tabIdentif.identificador[i].tam_dim2 = tabIdentif.identificador[aux].valorInt;
          }else{
            tabIdentif.identificador[i].tam_dim3 = tabIdentif.identificador[aux].valorInt;
          }

        }else{
          if(dimensoes == 0){
            tabIdentif.identificador[i].tam_dim1 = t.valInt;
          }else if(dimensoes == 1){
            tabIdentif.identificador[i].tam_dim2 = t.valInt;
          }else{
            tabIdentif.identificador[i].tam_dim3 = t.valInt;
          }
        }

        t = AnaLex(fd);
        if (t.cat != SN || t.codigo != FECHA_COLC){
          erroanasint(14);
        }

        dimensoes++;

        if(dimensoes > 3) errosemantico(15);

        t = AnaLex(fd);
      }

      if(tipo != tabIdentif.identificador[i].tipo) errosemantico(16);
      if(dimensoes != tabIdentif.identificador[i].dimensoes) errosemantico(17);

      i++;
      cont_param++;
    }
  }

  if(cont_param != tabIdentif.identificador[i_blk].parametros) errosemantico(19);

  catId = VL;
  escopo = LOCAL;

  while(t.cat == PR && (t.codigo == CONST || t.codigo == CHAR || t.codigo == INT || t.codigo == REAL || t.codigo == BOOL)){
    Decl_list_var();
  }

  while((t.cat == PR && (t.codigo == DO || t.codigo == IF || t.codigo == WHILE || t.codigo == GOBACK || t.codigo == GETINT || t.codigo == GETREAL || t.codigo == GETCHAR || t.codigo == PUTINT || t.codigo == PUTREAL || t.codigo == PUTCHAR )) || t.cat == ID){
    Cmd();
  }

  if(t.cat != PR || t.codigo != ENDBLOCK){
    erroanasint(20);
  }

  ExcluiVL();
  ZombieARG(blockAux);

  t = AnaLex(fd);
}

////////////////////////////////////////////////////////////////////////////////////////

void Decl_var(){
  int atrib;
  int aux;
  int valorInt;
  float valorReal;
  char valorChar;
  char valorString[TAM_MAX_STRING];
  int tamDim1 = 0, tamDim2 = 0, tamDim3 = 0;
  
  if(t.cat != ID){
    erroanasint(9);
  }

  strcpy(nomeId, t.lexema);

  fputs("AMEM 1\n", fd_out);

  t = AnaLex(fd);
  while(t.cat == SN && t.codigo == ABRE_COLC){
    t = AnaLex(fd);
    if(t.cat != INTCON && t.cat != ID){
      erroanasint(11);
    }

    if(t.cat == ID){
      aux = BuscaTabIdetif(t.lexema);
      if(aux == -1) errosemantico(12);
      if(tabIdentif.identificador[aux].tipo != INTEIRO || tabIdentif.identificador[aux].isConst == 0) errosemantico(13);

      if(dimensoes == 0){
        tamDim1 = tabIdentif.identificador[aux].valorInt;
      }else if(dimensoes == 1){
        tamDim2 = tabIdentif.identificador[aux].valorInt;
      }else{
        tamDim3 = tabIdentif.identificador[aux].valorInt;
      }
      
    }else{
      if(dimensoes == 0){
        tamDim1 = t.valInt;
      }else if(dimensoes == 1){
        tamDim2 = t.valInt;
      }else{
        tamDim3 = t.valInt;
      }
    }

    t = AnaLex(fd);
    if (t.cat != SN || t.codigo != FECHA_COLC){
      erroanasint(14);
    }

    dimensoes++;
    if(dimensoes > 3) errosemantico(21);

    t = AnaLex(fd);
  }

  if(isConst == 1){
    if(t.cat != SN || t.codigo != ATRIB) errosemantico(22);
  }

  if(t.cat == SN && t.codigo == ATRIB){
    atrib = 1;
    t = AnaLex(fd);
    if(t.cat == INTCON || t.cat == REALCON || t.cat == CHARCON || t.cat == STRINGCON){

      if(t.cat == INTCON){
        sprintf(cmdObj, "PUSH %d\n", t.valInt);
        fputs(cmdObj, fd_out);
      }else if(t.cat == REALCON){
        sprintf(cmdObj, "PUSH %f\n", t.valReal);
        fputs(cmdObj, fd_out);
      }else if(t.cat == CHARCON){
        sprintf(cmdObj, "PUSH %c\n", t.caracter);
        fputs(cmdObj, fd_out);
      }else if(t.cat == STRINGCON){
        sprintf(cmdObj, "PUSH %s\n", t.string);
        fputs(cmdObj, fd_out);
      }

      if(isConst == 1){
        if(t.cat == INTCON || tipo == BOOLEAN){
          valorInt = t.valInt;
        }else if(t.cat == REALCON){
          valorReal = t.valReal;
        }else if(t.cat == CHARCON){
          valorChar = t.caracter;
        }else{
          strcpy(valorString, t.string);
        }
      }
      
      t = AnaLex(fd);
    }else if(t.cat == SN && t.codigo == ABRE_CHAVE){
      t = AnaLex(fd);
      if(t.cat != INTCON && t.cat != REALCON && t.cat != CHARCON && t.cat != STRINGCON){
        erroanasint(23);
      }

      t = AnaLex(fd);
      while(t.cat == SN && t.codigo == VIRGULA){
        t = AnaLex(fd);
        if(t.cat != INTCON && t.cat != REALCON && t.cat != CHARCON && t.cat != STRINGCON){
          erroanasint(23);
        }

        t = AnaLex(fd);
      }

      if(t.cat != SN || t.codigo != FECHA_CHAVE){
        erroanasint(24);
      }

      t = AnaLex(fd);
    }else{
      erroanasint(25);
    }
  }

  aux = BuscaTabIdetifPorEscopo(nomeId, escopo);
  
  if(aux != -1){
    if(tabIdentif.identificador[aux].catId == ARG && tabIdentif.identificador[aux].zombie == 1){
      // PASSA
    }else{
      errosemantico(6);
    }
  }
  
  aux = InsereTabIdentif(nomeId, catId, escopo, tipo, isConst, zombie, ref, dimensoes, parametros);

  if(isConst == 1){
    if(tipo == CHARACTER){
      tabIdentif.identificador[aux].valorChar = valorChar;
    }else if(tipo == INTEIRO){
      tabIdentif.identificador[aux].valorInt = valorInt;
    }else if(tipo == REAL_TIPO){
      tabIdentif.identificador[aux].valorReal = valorReal;
    }else if(tipo == BOOLEAN){
      tabIdentif.identificador[aux].valorInt = valorInt;
    }
  }

  if(dimensoes>0){
    tabIdentif.identificador[aux].tam_dim1 = tamDim1;
    tabIdentif.identificador[aux].tam_dim2 = tamDim2;
    tabIdentif.identificador[aux].tam_dim3 = tamDim3;
  }

  if(atrib == 1){
    sprintf(cmdObj, "STOR %d\n", tabIdentif.identificador[aux].ender);
    fputs(cmdObj, fd_out);
  }
  
  dimensoes = 0;
}

////////////////////////////////////////////////////////////////////////////////////////

void Decl_block_prot(){
  if(t.cat == PR && t.codigo == WITH){
    escopo = LOCAL;
    catId = ARG;
    t = AnaLex(fd);
    parametros = 1;
    if(t.cat == SN && t.codigo == E_COMERCIAL){
      ref = 1;
      t = AnaLex(fd);
    }

    Tipo();

    strcpy(nomeId, "");

    t = AnaLex(fd);
    while(t.cat == SN && t.codigo == ABRE_COLC){
      dimensoes++;
      t = AnaLex(fd);
      if(t.cat != SN || t.codigo != FECHA_COLC){
        erroanasint(14);
      }

      t = AnaLex(fd);
    }

    InsereTabIdentif(nomeId, catId, escopo, tipo, isConst, zombie, ref, dimensoes, parametros);
    dimensoes = 0;

    while(t.cat == SN && t.codigo == VIRGULA){
      parametros++;
      ref = 0;
      t = AnaLex(fd);
      if(t.cat == SN && t.codigo == E_COMERCIAL){
        ref = 1;
        t = AnaLex(fd);
      }

      Tipo();

      strcpy(nomeId, "");

      t = AnaLex(fd);
      while(t.cat == SN && t.codigo == ABRE_COLC){
        dimensoes++;
        t = AnaLex(fd);
        if(t.cat != SN || t.codigo != FECHA_COLC){
          erroanasint(14);
        }

        t = AnaLex(fd);
      }

      InsereTabIdentif(nomeId, catId, escopo, tipo, isConst, zombie, ref, dimensoes, parametros);
      dimensoes = 0;
      
    }

    if(parametros > TAM_MAX_PARAMS) errosemantico(26);

    EditaQtdParams(blockAux, parametros);

    ref = 0;
    parametros = 0;
    
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void Block_main(){
  if(t.cat != PR || t.codigo != MAIN){
    erroanasint(27);
  }

  strcpy(nomeId, palavras_reservadas[t.codigo]);

  if(BuscaTabIdetifPorEscopo(nomeId, escopo) != -1) errosemantico(6);

  InsereTabIdentif(nomeId, catId, escopo, tipo, isConst, zombie, ref, dimensoes, parametros);

  catId = VL;
  escopo = LOCAL;

  t = AnaLex(fd);
  while(t.cat == PR && (t.codigo == CONST || t.codigo == CHAR || t.codigo == INT || t.codigo == REAL || t.codigo == BOOL)){
    Decl_list_var();
  }

  while((t.cat == PR && (t.codigo == DO || t.codigo == IF || t.codigo == WHILE || t.codigo == GOBACK || t.codigo == GETINT || t.codigo == GETREAL || t.codigo == GETCHAR || t.codigo == PUTINT || t.codigo == PUTREAL || t.codigo == PUTCHAR )) || t.cat == ID){
    Cmd();
  }

  if(t.cat != PR || t.codigo != ENDBLOCK){
    erroanasint(20);
  }

  ExcluiVL();
}

////////////////////////////////////////////////////////////////////////////////////////

void Cmd(){ // SEMPRE RETORNA PROX TOKEN
  int aux;
  int idaux;
  char l1[TAM_MAX_LABEL];
  char l2[TAM_MAX_LABEL];
  char l3[TAM_MAX_LABEL];
  char l4[TAM_MAX_LABEL];

  gera_label();
  strcpy(l1, label);
  gera_label();
  strcpy(l2, label);
  
  if(t.cat == PR && t.codigo == DO){

    sprintf(cmdObj, "GOTO %s\n", l1);
    fputs(cmdObj, fd_out);
    sprintf(cmdObj, "LABEL %s\n", l2);
    fputs(cmdObj, fd_out);
    
    t = AnaLex(fd);
    if(t.cat == ID){
      if(lookahead.cat == SN && (lookahead.codigo == ABRE_COLC || lookahead.codigo == ATRIB)){
        Cmd();
      }else{

        aux = BuscaTabIdetifBlk(t.lexema);
        if(aux == -1) errosemantico(4);
        
        t = AnaLex(fd);
        if(t.cat == PR && t.codigo == WITH){

          t = AnaLex(fd);
          if(t.cat != ID){
            erroanasint(9);
          }

          aux = BuscaTabIdetifVAR(t.lexema);
          if(aux == -1) errosemantico(28);

          t = AnaLex(fd);
          while(t.cat == SN && t.codigo == VIRGULA){

            t = AnaLex(fd);
            if(t.cat != ID){
              erroanasint(9);
            }

            aux = BuscaTabIdetifVAR(t.lexema);
            if(aux == -1) errosemantico(28);

            t = AnaLex(fd);
          }
        }
      }
    }else if(t.cat == PR && (t.codigo == DO || t.codigo == IF || t.codigo == WHILE || t.codigo == GOBACK || t.codigo == GETINT || t.codigo == GETREAL || t.codigo == GETCHAR || t.codigo == PUTINT || t.codigo == PUTREAL || t.codigo == PUTCHAR)){
      Cmd();
    }else{
      erroanasint(29);
    }

    if(t.cat == PR && t.codigo == VARYING){

      gera_label();
      strcpy(l3, label);
      gera_label();
      strcpy(l4, label);

      sprintf(cmdObj, "GOTO %s\n", l3);
      fputs(cmdObj, fd_out);
      sprintf(cmdObj, "LABEL %s\n", l1);
      fputs(cmdObj, fd_out);
      
      t = AnaLex(fd);
      if(t.cat != ID){
        erroanasint(9);
      }

      aux = BuscaTabIdetifVAR(t.lexema);
      if(aux == -1) errosemantico(28);
      idaux = aux;

      t = AnaLex(fd);
      if(t.cat != PR || t.codigo != FROM){
        erroanasint(30);
      }

      t = AnaLex(fd);
      Expr();

      fputs("COPY\n", fd_out);
      sprintf(cmdObj, "STOR %d\n", tabIdentif.identificador[idaux].ender);
      fputs(cmdObj, fd_out);
      sprintf(cmdObj, "GOTO %s\n", l4);
      fputs(cmdObj, fd_out);
      sprintf(cmdObj, "LABEL %s\n", l3);
      fputs(cmdObj, fd_out);

      if(t.cat != PR || (t.codigo != TO && t.codigo != DOWNTO)){
        erroanasint(31);
      }

      fputs("PUSH 1\n", fd_out);
      sprintf(cmdObj, "LOAD %d\n", tabIdentif.identificador[idaux].ender);
      fputs(cmdObj, fd_out);
      if(t.codigo == TO) fputs("ADD\n", fd_out);
      else if(t.codigo == DOWNTO) fputs("SUB\n", fd_out);
      fputs("COPY\n", fd_out);
      sprintf(cmdObj, "STOR %d\n", tabIdentif.identificador[idaux].ender);
      fputs(cmdObj, fd_out);
      sprintf(cmdObj, "LABEL %s\n", l4);
      fputs(cmdObj, fd_out);

      t = AnaLex(fd);
      Expr();

      fputs("SUB\n", fd_out);
      // <6>
      sprintf(cmdObj, "GOTRUE %s\n", l2);
      fputs(cmdObj, fd_out);
      
    }else if(t.cat == PR && t.codigo == WHILE){

      sprintf(cmdObj, "LABEL %s\n", l1);
      fputs(cmdObj, fd_out);
      
      t = AnaLex(fd);
      if(t.cat != SN || t.codigo != ABRE_PAR){
        erroanasint(32);
      }

      t = AnaLex(fd);
      Expr();

      if(t.cat != SN || t.codigo != FECHA_PAR){
        erroanasint(33);
      }

      sprintf(cmdObj, "GOTRUE %s\n", l2);
      fputs(cmdObj, fd_out);

      t = AnaLex(fd);
    }else if(t.cat == PR && t.codigo == FOR){
      
      gera_label();
      strcpy(l3, label);
      gera_label();
      strcpy(l4, label);

      sprintf(cmdObj, "GOTO %s\n", l3);
      fputs(cmdObj, fd_out);
      sprintf(cmdObj, "LABEL %s\n", l1);
      fputs(cmdObj, fd_out);
      
      t = AnaLex(fd);
      Expr();

      sprintf(cmdObj, "GOTO %s\n", l4);
      fputs(cmdObj, fd_out);
      sprintf(cmdObj, "LABEL %s\n", l3);
      fputs(cmdObj, fd_out);
      fputs("SUB\n", fd_out);
      sprintf(cmdObj, "LABEL %s\n", l4);
      fputs(cmdObj, fd_out);
      fputs("COPY\n", fd_out);
      sprintf(cmdObj, "GOTRUE %s\n", l2);
      fputs(cmdObj, fd_out);
      fputs("POP\n", fd_out);

      if(t.cat != PR || t.codigo != TIMES){
        erroanasint(34);
      }

      t = AnaLex(fd);
    }else{

      gera_label();
      strcpy(l3, label);
      sprintf(cmdObj, "GOTO %s\n", l3);
      fputs(cmdObj, fd_out);
      sprintf(cmdObj, "LABEL %s\n", l1);
      fputs(cmdObj, fd_out);
      sprintf(cmdObj, "GOTO %s\n", l2);
      fputs(cmdObj, fd_out);
      sprintf(cmdObj, "LABEL %s\n", l3);
      fputs(cmdObj, fd_out);
      
    }
    
  }else if(t.cat == PR && t.codigo == IF){
    t = AnaLex(fd);
    if(t.cat != SN || t.codigo != ABRE_PAR){
      erroanasint(32);
    }

    t = AnaLex(fd);
    Expr();

    if(t.cat != SN || t.codigo != FECHA_PAR){
      erroanasint(33);
    }

    t = AnaLex(fd);
    while((t.cat == PR && (t.codigo == DO || t.codigo == IF || t.codigo == WHILE || t.codigo == GOBACK || t.codigo == GETINT || t.codigo == GETREAL || t.codigo == GETCHAR || t.codigo == PUTINT || t.codigo == PUTREAL || t.codigo == PUTCHAR )) || t.cat == ID){
      Cmd();
    }

    while(t.cat == PR && t.codigo == ELSEIF){
      t = AnaLex(fd);
      if(t.cat != SN || t.codigo != ABRE_PAR){
        erroanasint(32);
      }

      t = AnaLex(fd);
      Expr();

      if(t.cat != SN || t.codigo != FECHA_PAR){
        erroanasint(33);
      }

      t = AnaLex(fd);
      while((t.cat == PR && (t.codigo == DO || t.codigo == IF || t.codigo == WHILE || t.codigo == GOBACK || t.codigo == GETINT || t.codigo == GETREAL || t.codigo == GETCHAR || t.codigo == PUTINT || t.codigo == PUTREAL || t.codigo == PUTCHAR )) || t.cat == ID){
        Cmd();
      }
    }

    if(t.cat == PR && t.codigo == ELSE){
      t = AnaLex(fd);
      while((t.cat == PR && (t.codigo == DO || t.codigo == IF || t.codigo == WHILE || t.codigo == GOBACK || t.codigo == GETINT || t.codigo == GETREAL || t.codigo == GETCHAR || t.codigo == PUTINT || t.codigo == PUTREAL || t.codigo == PUTCHAR )) || t.cat == ID){
        Cmd();
      }
    }

    if(t.cat != PR || t.codigo != ENDIF){
      erroanasint(35);
    }

    t = AnaLex(fd);
    
  }else if(t.cat == PR && t.codigo == WHILE){
    t = AnaLex(fd);
    if(t.cat != SN || t.codigo != ABRE_PAR){
      erroanasint(32);
    }

    t = AnaLex(fd);
    Expr();

    if(t.cat != SN || t.codigo != FECHA_PAR){
      erroanasint(33);
    }

    t = AnaLex(fd);
    while((t.cat == PR && (t.codigo == DO || t.codigo == IF || t.codigo == WHILE || t.codigo == GOBACK || t.codigo == GETINT || t.codigo == GETREAL || t.codigo == GETCHAR || t.codigo == PUTINT || t.codigo == PUTREAL || t.codigo == PUTCHAR)) || t.cat == ID){
      Cmd();
    }

    if(t.cat != PR || t.codigo != ENDWHILE){
      erroanasint(36);
    }

    t = AnaLex(fd);
  }else if(t.cat == ID){
    Atrib();
    
  }else if(t.cat == PR && t.cat == GOBACK){
    t = AnaLex(fd);
    
  }else if(t.cat == PR && (t.codigo == GETINT || t.codigo == GETREAL || t.codigo == GETCHAR || t.codigo == PUTINT || t.codigo == PUTREAL || t.codigo == PUTCHAR)){
    t = AnaLex(fd);
    if(t.cat != ID){
      erroanasint(9);
    }

    aux = BuscaTabIdetifVAR(t.lexema);
    if(aux == -1) errosemantico(28);

    t = AnaLex(fd);
    
  }else{
    erroanasint(29);
  }
}


////////////////////////////////////////////////////////////////////////////////////////




void Atrib(){
  int aux;
  aux = BuscaTabIdetifVAR(t.lexema);
  if(aux == -1) errosemantico(28);

  if(tabIdentif.identificador[aux].isConst == 1) errosemantico(37);

  t = AnaLex(fd);
  while(t.cat == SN && t.codigo == ABRE_COLC){
    t = AnaLex(fd);
    Expr();

    if(t.cat != SN || t.codigo != FECHA_COLC){
      erroanasint(14);
    }

    t = AnaLex(fd);
  }

  if(t.cat != SN || t.codigo != ATRIB){
    erroanasint(38);
  }

  t = AnaLex(fd);
  Expr();
}

////////////////////////////////////////////////////////////////////////////////////////

void Expr(){
  int codOp = 0;
  char l1[TAM_MAX_LABEL];
  char l2[TAM_MAX_LABEL];
  char l3[TAM_MAX_LABEL];
  Expr_simp();

  if(t.cat == SN && (t.codigo == IGUAL || t.codigo == DIFERENTE || t.codigo == MENOR_IGUAL || t.codigo == MENOR_Q || t.codigo == MAIOR_IGUAL || t.codigo == MAIOR_Q)){
    codOp = t.codigo;
    Op_rel();

    t = AnaLex(fd);
    Expr_simp();

    gera_label();
    strcpy(l1, label);
    gera_label();
    strcpy(l2, label);

    fputs("SUB\n", fd_out);
    if(codOp == MAIOR_Q){
      sprintf(cmdObj, "GOTRUE %s\n", l1);
      fputs(cmdObj, fd_out);
      fputs("PUSH 0\n", fd_out);
      sprintf(cmdObj, "GOTO %s\n", l2);
      fputs(cmdObj, fd_out);
      sprintf(cmdObj, "LABEL %s\n", l1);
      fputs(cmdObj, fd_out);
      fputs("PUSH 1\n", fd_out);
      sprintf(cmdObj, "LABEL %s\n", l2);
      fputs(cmdObj, fd_out);
      
    }else if(codOp == IGUAL){
      sprintf(cmdObj, "GOFALSE %s\n", l1);
      fputs(cmdObj, fd_out);
      fputs("PUSH 0\n", fd_out);
      sprintf(cmdObj, "GOTO %s\n", l2);
      fputs(cmdObj, fd_out);
      sprintf(cmdObj, "LABEL %s\n", l1);
      fputs(cmdObj, fd_out);
      fputs("PUSH 1\n", fd_out);
      sprintf(cmdObj, "LABEL %s\n", l2);
      fputs(cmdObj, fd_out);
      
    }else if(codOp == MENOR_Q){
      gera_label();
      strcpy(l3, label);

      fputs("COPY\n", fd_out);
      sprintf(cmdObj, "GOFALSE %s\n", l1);
      fputs(cmdObj, fd_out);
      sprintf(cmdObj, "GOTRUE %s\n", l2);
      fputs(cmdObj, fd_out);
      fputs("PUSH 1\n", fd_out);
      sprintf(cmdObj, "GOTO %s\n", l3);
      fputs(cmdObj, fd_out);
      sprintf(cmdObj, "LABEL %s\n", l1);
      fputs(cmdObj, fd_out);
      fputs("POP\n", fd_out);
      sprintf(cmdObj, "LABEL %s\n", l2);
      fputs(cmdObj, fd_out);
      fputs("PUSH 0\n", fd_out);
      sprintf(cmdObj, "LABEL %s\n", l3);
      fputs(cmdObj, fd_out);
      
    }else if(codOp == MAIOR_IGUAL){
      gera_label();
      strcpy(l3, label);
      
      fputs("COPY\n", fd_out);
      sprintf(cmdObj, "GOFALSE %s\n", l1);
      fputs(cmdObj, fd_out);
      sprintf(cmdObj, "GOTRUE %s\n", l2);
      fputs(cmdObj, fd_out);
      fputs("PUSH 0\n", fd_out);
      sprintf(cmdObj, "GOTO %s\n", l3);
      fputs(cmdObj, fd_out);
      sprintf(cmdObj, "LABEL %s\n", l1);
      fputs(cmdObj, fd_out);
      fputs("POP\n", fd_out);
      sprintf(cmdObj, "LABEL %s\n", l2);
      fputs(cmdObj, fd_out);
      fputs("PUSH 1\n", fd_out);
      sprintf(cmdObj, "LABEL %s\n", l3);
      fputs(cmdObj, fd_out);
      
    }else if(codOp == MENOR_IGUAL){
      sprintf(cmdObj, "GOTRUE %s\n", l1);
      fputs(cmdObj, fd_out);
      fputs("PUSH 1\n", fd_out);
      sprintf(cmdObj, "GOTO %s\n", l2);
      fputs(cmdObj, fd_out);
      sprintf(cmdObj, "LABEL %s\n", l1);
      fputs(cmdObj, fd_out);
      fputs("PUSH 0\n", fd_out);
      sprintf(cmdObj, "LABEL %s\n", l2);
      fputs(cmdObj, fd_out);
      
    }else if(codOp == DIFERENTE){
      sprintf(cmdObj, "GOFALSE %s\n", l1);
      fputs(cmdObj, fd_out);
      fputs("PUSH 1\n", fd_out);
      sprintf(cmdObj, "GOTO %s\n", l2);
      fputs(cmdObj, fd_out);
      sprintf(cmdObj, "LABEL %s\n", l1);
      fputs(cmdObj, fd_out);
      fputs("PUSH 0\n", fd_out);
      sprintf(cmdObj, "LABEL %s\n", l2);
      fputs(cmdObj, fd_out);
      
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void Expr_simp(){
  int codOp = 0;
  char l1[TAM_MAX_LABEL];
  
  if((t.cat == SN && t.codigo == ADICAO) || (t.cat == SN && t.codigo == SUBTRACAO)){
    t = AnaLex(fd);
  }

  Termo();

  while(t.cat == SN && (t.codigo == ADICAO || t.codigo == SUBTRACAO || t.codigo == OR)){
    codOp = t.codigo;

    if(codOp == OR){
      gera_label();
      strcpy(l1, label);
      fputs("COPY\n", fd_out);
      sprintf(cmdObj, "GOTRUE %s\n", l1);
      fputs(cmdObj, fd_out);
      fputs("POP\n", fd_out);
    }
    
    t = AnaLex(fd);
    Termo();

    if(codOp == ADICAO){
      fputs("ADD\n", fd_out);
    }else if(codOp == SUBTRACAO){
      fputs("SUB\n", fd_out);
    }else if(codOp == OR){
      sprintf(cmdObj, "LABEL %s\n", l1);
      fputs(cmdObj, fd_out);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void Termo(){
  int codOp = 0;
  char l1[TAM_MAX_LABEL];
  Fator();

  while(t.cat == SN && (t.codigo == MULTIPLIC || t.codigo == DIVISAO || t.codigo == AND)){
    codOp = t.codigo;

    if(codOp == AND){
      gera_label();
      strcpy(l1, label);
      fputs("COPY\n", fd_out);
      sprintf(cmdObj, "GOFALSE %s\n", l1);
      fputs(cmdObj, fd_out);
      fputs("POP\n", fd_out);
    }
    
    t = AnaLex(fd);
    Fator();

    if(codOp == MULTIPLIC){
      fputs("MUL\n", fd_out);
    }else if(codOp == DIVISAO){
      fputs("DIV\n", fd_out);
    }else if(codOp == AND){
      sprintf(cmdObj, "LABEL %s\n", l1);
      fputs(cmdObj, fd_out);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void Fator(){
  int aux;
  if(t.cat == ID){

    aux = BuscaTabIdetifVAR(t.lexema);
    if(aux == -1) errosemantico(28);

    sprintf(cmdObj, "LOAD %d\n", tabIdentif.identificador[aux].ender);
    fputs(cmdObj, fd_out);
    
    t = AnaLex(fd);
    while(t.cat == SN && t.codigo == ABRE_COLC){
      t = AnaLex(fd);
      Expr();

      if(t.cat != SN || t.codigo != FECHA_COLC){
        erroanasint(14);
      }

      t = AnaLex(fd);
    }
  }else if(t.cat == SN && t.codigo == NOT){
    t = AnaLex(fd);
    Fator();
  }else if(t.cat == SN && t.codigo == ABRE_PAR){
    t = AnaLex(fd);
    Expr();

    if(t.cat != SN || t.codigo != FECHA_PAR){
      erroanasint(33);
    }

    t = AnaLex(fd);
  }else if(t.cat == INTCON || t.cat == REALCON || t.cat == CHARCON){
    if(t.cat == INTCON){
      sprintf(cmdObj, "PUSH %d\n", t.valInt);
      fputs(cmdObj, fd_out);
    }else if(t.cat == REALCON){
      sprintf(cmdObj, "PUSH %f\n", t.valReal);
      fputs(cmdObj, fd_out);
    }else{
      sprintf(cmdObj, "PUSH %d\n", t.caracter);
      fputs(cmdObj, fd_out);
    }
    t = AnaLex(fd);
  }else{
    erroanasint(39);
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void Op_rel(){
  if(t.cat != SN || (t.codigo != IGUAL && t.codigo != DIFERENTE && t.codigo != MENOR_IGUAL && t.codigo != MENOR_Q && t.codigo != MAIOR_IGUAL && t.codigo != MAIOR_Q)){
    erroanasint(40);
  }
}

