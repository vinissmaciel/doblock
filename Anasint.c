#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Analex.h"
#include "Anasint.h"
#include "Funcaux.h"

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

void Prog(){
  catId = VG;
  while(t.cat == PR && (t.codigo == CONST || t.codigo == CHAR || t.codigo == INT || t.codigo == REAL || t.codigo == BOOL)){
    Decl_list_var();
  }

  if(t.cat != PR || t.codigo != BLOCK){
    erroanasint("Esperado declaração de variável ou PR block");
  }

  t = AnaLex(fd);
  while(t.cat == ID){
    catId = BLK;
    strcpy(nomeId, t.lexema);
    escopo = GLOBAL;
    InsereTabIdentif(nomeId, catId, escopo, tipo, isConst, zombie, ref, dimensoes, parametros);
    strcpy(blockAux, t.lexema);
    
    t = AnaLex(fd);
    Decl_block_prot();

    if(t.cat != PR || t.codigo != BLOCK){
      erroanasint("Esperado PR block");
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
    erroanasint("PRs esperadas: char, int, real, bool"); 
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
  
  catId = BLK;
  
  t = AnaLex(fd);
  if(t.cat != ID){
    erroanasint("Identificador esperado");
  }

  strcpy(blockAux, t.lexema);
  int i = BuscaTabIdetifBlk(blockAux);
  //if(i == -1) erro("Bloco '%s' não declarado");
  i++;

  t = AnaLex(fd);
  if(t.cat == PR && t.codigo == WITH){
    t = AnaLex(fd);
    Tipo();

    t = AnaLex(fd);
    if(t.cat != ID){
      erroanasint("Identificador esperado");
    }

    EditaNomeArg(t.lexema, i);
    i++;

    t = AnaLex(fd);
    while(t.cat == SN && t.codigo == ABRE_COLC){
      t = AnaLex(fd);
      if(t.cat != INTCON && t.cat != ID){
        erroanasint("Constante inteira ou identificador inteiro constante (const int id) esperado");
      }

      t = AnaLex(fd);
      if (t.cat != SN || t.codigo != FECHA_COLC){
        erroanasint("Fecha colchetes esperado");
      }

      t = AnaLex(fd);
    }

    while(t.cat == SN && t.codigo == VIRGULA){
      t = AnaLex(fd);
      Tipo();

      t = AnaLex(fd);
      if(t.cat != ID){
        erroanasint("Identificador esperado");
      }

      EditaNomeArg(t.lexema, i);
      i++;

      t = AnaLex(fd);
      while(t.cat == SN && t.codigo == ABRE_COLC){
        t = AnaLex(fd);
        if(t.cat != INTCON && t.cat != ID){
          erroanasint("Constante inteira ou identificador inteiro constante (const int id) esperado");
        }

        t = AnaLex(fd);
        if (t.cat != SN || t.codigo != FECHA_COLC){
          erroanasint("Fecha colchetes esperado");
        }

        t = AnaLex(fd);
      }
    }
  }

  catId = VL;
  escopo = LOCAL;

  while(t.cat == PR && (t.codigo == CONST || t.codigo == CHAR || t.codigo == INT || t.codigo == REAL || t.codigo == BOOL)){
    Decl_list_var();
  }

  while((t.cat == PR && (t.codigo == DO || t.codigo == IF || t.codigo == WHILE || t.codigo == GOBACK || t.codigo == GETINT || t.codigo == GETREAL || t.codigo == GETCHAR || t.codigo == PUTINT || t.codigo == PUTREAL || t.codigo == PUTCHAR )) || t.cat == ID){
    Cmd();
  }

  if(t.cat != PR || t.codigo != ENDBLOCK){
    erroanasint("Esperado PR endblock");
  }

  ExcluiVL(blockAux);
  ZombieARG(blockAux);

  t = AnaLex(fd);
}

////////////////////////////////////////////////////////////////////////////////////////

void Decl_var(){
  if(t.cat != ID){
    erroanasint("Identificador esperado");
  }

  strcpy(nomeId, t.lexema);

  t = AnaLex(fd);
  while(t.cat == SN && t.codigo == ABRE_COLC){
    t = AnaLex(fd);
    if(t.cat != INTCON && t.cat != ID){
      erroanasint("Constante inteira ou identificador inteiro constante (const int id) esperado");
    }

    t = AnaLex(fd);
    if (t.cat != SN || t.codigo != FECHA_COLC){
      erroanasint("Fecha colchetes esperado");
    }

    dimensoes++;

    t = AnaLex(fd);
  }

  if(t.cat == SN && t.codigo == ATRIB){
    t = AnaLex(fd);
    if(t.cat == INTCON || t.cat == REALCON || t.cat == CHARCON || t.cat == STRINGCON){
      t = AnaLex(fd);
    }else if(t.cat == SN && t.codigo == ABRE_CHAVE){
      t = AnaLex(fd);
      if(t.cat != INTCON && t.cat != REALCON && t.cat != CHARCON && t.cat != STRINGCON){
        erroanasint("constante inteira, real, char ou string esperado");
      }

      t = AnaLex(fd);
      while(t.cat == SN && t.codigo == VIRGULA){
        t = AnaLex(fd);
        if(t.cat != INTCON && t.cat != REALCON && t.cat != CHARCON && t.cat != STRINGCON){
          erroanasint("constante inteira, real, char ou string esperado");
        }

        t = AnaLex(fd);
      }

      if(t.cat != SN || t.codigo != FECHA_CHAVE){
        erroanasint("Fecha chaves esperado");
      }

      t = AnaLex(fd);
    }else{
      erroanasint("abre chaves, constante inteira, real, char ou string esperado");
    }
  }

  InsereTabIdentif(nomeId, catId, escopo, tipo, isConst, zombie, ref, dimensoes, parametros);
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
        erroanasint("Fecha colchetes esperado");
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
          erroanasint("Fecha colchetes esperado");
        }

        t = AnaLex(fd);
      }

      InsereTabIdentif(nomeId, catId, escopo, tipo, isConst, zombie, ref, dimensoes, parametros);
      dimensoes = 0;
      
    }

    EditaQtdParams(blockAux, parametros);

    ref = 0;
    parametros = 0;
    
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void Block_main(){
  if(t.cat != PR || t.codigo != MAIN){
    erroanasint("Esperado PR main");
  }

  strcpy(nomeId, palavras_reservadas[t.codigo]);

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
    erroanasint("Esperado PR endblock");
  }

  ExcluiVL("main");
}

////////////////////////////////////////////////////////////////////////////////////////

void Cmd(){ // SEMPRE RETORNA PROX TOKEN
  if(t.cat == PR && t.codigo == DO){
    t = AnaLex(fd);
    if(t.cat == ID){
      if(lookahead.cat == SN && (lookahead.codigo == ABRE_COLC || lookahead.codigo == ATRIB)){
        Cmd();
      }else{
        t = AnaLex(fd);
        if(t.cat == PR && t.codigo == WITH){

          t = AnaLex(fd);
          if(t.cat != ID){
            erroanasint("Identificador esperado");
          }

          t = AnaLex(fd);
          while(t.cat == SN && t.codigo == VIRGULA){

            t = AnaLex(fd);
            if(t.cat != ID){
              erroanasint("Identificador esperado");
            }

            t = AnaLex(fd);
          }
        }
      }
    }else if(t.cat == PR && (t.codigo == DO || t.codigo == IF || t.codigo == WHILE || t.codigo == GOBACK || t.codigo == GETINT || t.codigo == GETREAL || t.codigo == GETCHAR || t.codigo == PUTINT || t.codigo == PUTREAL || t.codigo == PUTCHAR)){
      Cmd();
    }else{
      erroanasint("Identificador esperado ou PRs esperadas: do, if, while, goback, getint, getreal, getchar, putint, putreal, putchar");
    }

    if(t.cat == PR && t.codigo == VARYING){
      t = AnaLex(fd);
      if(t.cat != ID){
        erroanasint("Identificador esperado");
      }

      t = AnaLex(fd);
      if(t.cat != PR || t.codigo != FROM){
        erroanasint("Esperado PR from");
      }

      t = AnaLex(fd);
      Expr();

      if(t.cat != PR || (t.codigo != TO && t.codigo != DOWNTO)){
        erroanasint("Esperado PR to ou PR downto");
      }

      t = AnaLex(fd);
      Expr();
    }else if(t.cat == PR && t.codigo == WHILE){
      t = AnaLex(fd);
      if(t.cat != SN || t.codigo != ABRE_PAR){
        erroanasint("Esperado abre parenteses");
      }

      t = AnaLex(fd);
      Expr();

      if(t.cat != SN || t.codigo != FECHA_PAR){
        erroanasint("Esperado fecha parenteses");
      }

      t = AnaLex(fd);
    }else if(t.cat == PR && t.codigo == FOR){
      t = AnaLex(fd);
      Expr();

      if(t.cat != PR || t.codigo != TIMES){
        erroanasint("Esperado PR times");
      }

      t = AnaLex(fd);
    }
    
  }else if(t.cat == PR && t.codigo == IF){
    t = AnaLex(fd);
    if(t.cat != SN || t.codigo != ABRE_PAR){
      erroanasint("Esperado abre parenteses");
    }

    t = AnaLex(fd);
    Expr();

    if(t.cat != SN || t.codigo != FECHA_PAR){
      erroanasint("Esperado fecha parenteses");
    }

    t = AnaLex(fd);
    while((t.cat == PR && (t.codigo == DO || t.codigo == IF || t.codigo == WHILE || t.codigo == GOBACK || t.codigo == GETINT || t.codigo == GETREAL || t.codigo == GETCHAR || t.codigo == PUTINT || t.codigo == PUTREAL || t.codigo == PUTCHAR )) || t.cat == ID){
      Cmd();
    }

    while(t.cat == PR && t.codigo == ELSEIF){
      t = AnaLex(fd);
      if(t.cat != SN || t.codigo != ABRE_PAR){
        erroanasint("Esperado abre parenteses");
      }

      t = AnaLex(fd);
      Expr();

      if(t.cat != SN || t.codigo != FECHA_PAR){
        erroanasint("Esperado fecha parenteses");
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
      erroanasint("esperado PR endif");
    }

    t = AnaLex(fd);
    
  }else if(t.cat == PR && t.codigo == WHILE){
    t = AnaLex(fd);
    if(t.cat != SN || t.codigo != ABRE_PAR){
      erroanasint("Esperado abre parenteses");
    }

    t = AnaLex(fd);
    Expr();

    if(t.cat != SN || t.codigo != FECHA_PAR){
      erroanasint("Esperado fecha parenteses");
    }

    t = AnaLex(fd);
    while((t.cat == PR && (t.codigo == DO || t.codigo == IF || t.codigo == WHILE || t.codigo == GOBACK || t.codigo == GETINT || t.codigo == GETREAL || t.codigo == GETCHAR || t.codigo == PUTINT || t.codigo == PUTREAL || t.codigo == PUTCHAR)) || t.cat == ID){
      Cmd();
    }

    if(t.cat != PR || t.codigo != ENDWHILE){
      erroanasint("esperado PR endwhile");
    }

    t = AnaLex(fd);
  }else if(t.cat == ID){
    Atrib();
    
  }else if(t.cat == PR && t.cat == GOBACK){
    t = AnaLex(fd);
    
  }else if(t.cat == PR && (t.codigo == GETINT || t.codigo == GETREAL || t.codigo == GETCHAR || t.codigo == PUTINT || t.codigo == PUTREAL || t.codigo == PUTCHAR)){
    t = AnaLex(fd);
    if(t.cat != ID){
      erroanasint("Identificador esperado");
    }

    t = AnaLex(fd);
    
  }else{
    erroanasint("Identificador esperado ou PRs esperadas: do, if, while, goback, getint, getreal, getchar, putint, putreal, putchar");
  }
}


////////////////////////////////////////////////////////////////////////////////////////




void Atrib(){
  /*if(t.cat != ID){
    erroanasint("Identificador esperado");
  }*/ // VER SE É NECESSÁRIO

  t = AnaLex(fd);
  while(t.cat == SN && t.codigo == ABRE_COLC){
    t = AnaLex(fd);
    Expr();

    if(t.cat != SN || t.codigo != FECHA_COLC){
      erroanasint("Fecha colchetes esperado");
    }

    t = AnaLex(fd);
  }

  if(t.cat != SN || t.codigo != ATRIB){
    erroanasint("Sinal de atribuição (=) esperado");
  }

  t = AnaLex(fd);
  Expr();
}

////////////////////////////////////////////////////////////////////////////////////////

void Expr(){
  Expr_simp();

  if(t.cat == SN && (t.codigo == IGUAL || t.codigo == DIFERENTE || t.codigo == MENOR_IGUAL || t.codigo == MENOR_Q || t.codigo == MAIOR_IGUAL || t.codigo == MAIOR_Q)){
    Op_rel();

    t = AnaLex(fd);
    Expr_simp();
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void Expr_simp(){
  if((t.cat == SN && t.codigo == ADICAO) || (t.cat == SN && t.codigo == SUBTRACAO)){
    t = AnaLex(fd);
  }

  Termo();

  while(t.cat == SN && (t.codigo == ADICAO || t.codigo == SUBTRACAO || t.codigo == OR)){
    t = AnaLex(fd);
    Termo();
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void Termo(){
  Fator();

  while(t.cat == SN && (t.codigo == MULTIPLIC || t.codigo == DIVISAO || t.codigo == AND)){
    t = AnaLex(fd);
    Fator();
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void Fator(){
  if(t.cat == ID){
    t = AnaLex(fd);
    while(t.cat == SN && t.codigo == ABRE_COLC){
      t = AnaLex(fd);
      Expr();

      if(t.cat != SN || t.codigo != FECHA_COLC){
        erroanasint("Fecha colchetes esperado");
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
      erroanasint("Fecha parenteses esperado");
    }

    t = AnaLex(fd);
  }else if(t.cat == INTCON || t.cat == REALCON || t.cat == CHARCON){
    t = AnaLex(fd);
  }else{
    erroanasint("Esperado: Identificador, constante inteira, real ou char, abre parenteses ou !");
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void Op_rel(){
  if(t.cat != SN || (t.codigo != IGUAL && t.codigo != DIFERENTE && t.codigo != MENOR_IGUAL && t.codigo != MENOR_Q && t.codigo != MAIOR_IGUAL && t.codigo != MAIOR_Q)){
    erroanasint("Operador relacional esperado (=, !=, <, <=, >, >=)");
  }
}

