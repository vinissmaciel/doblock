#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "Analex.h"
#include "Anasint.h"
#include "Funcaux.h"
#include "Tabidentific.h"

/* Variaveis globais */
TOKEN t;
TOKEN lookahead;
FILE *fd;
int contLinha;
TAB_IDENTIF tabIdentif;


void TestaAnalex() {
    
    if ((fd=fopen("codigo.dat", "r")) == NULL) 
        erro("Arquivo de entrada do código nao encontrado!"); 

    printf("LINHA %d: ", contLinha);
    while(1){
        t = AnaLex(fd);
        
        switch (t.cat) {
            case ID: printf("<ID, %s> ", t.lexema);
                    break;
            case PR:  
                    printf("<PR, ");
                    int i = 0;
                    while((palavras_reservadas[t.codigo])[i]){
                        putchar(toupper((palavras_reservadas[t.codigo])[i])); // ROTINA PARA DEIXAR STRING MAISCULA
                        i++;
                    }
                    printf("> ");
                    break;
            case INTCON: printf("<INTCON, %d> ", t.valInt);
                    break;
            case REALCON: printf("<REALCON, %g> ", t.valReal);
                    break;
            case CHARCON: switch (t.caracter){
                                case BARRA_N: printf("<CHARCON, \'\\n\'> ");
                                        break;
                                case BARRA_0: printf("<CHARCON, \'\\0\'> ");
                                        break;
                                default: printf("<CHARCON, \'%c\'> ", t.caracter);
                            }
                        break;
            case STRINGCON: printf("<STRINGCON, \"%s\"> ", t.string);
                    break;
            case SN: switch (t.codigo){
                                case ADICAO: printf("<SN, ADICAO> ");
                                        break;
                                case SUBTRACAO: printf("<SN, SUBTRACAO> ");
                                        break;
                                case DIVISAO: printf("<SN, DIVISAO> ");
                                        break;
                                case MULTIPLIC: printf("<SN, MULTIPLICACAO> ");
                                        break;
                                case VIRGULA: printf("<SN, VIRGULA> ");
                                        break;
                                case ABRE_CHAVE: printf("<SN, ABRE_CHAVES> ");
                                        break;
                                case FECHA_CHAVE: printf("<SN, FECHA_CHAVES> ");
                                        break;
                                case ABRE_PAR: printf("<SN, ABRE_PARENTESES> ");
                                        break;
                                case FECHA_PAR: printf("<SN, FECHA_PARENTESES> ");
                                        break;
                                case ABRE_COLC: printf("<SN, ABRE_COLCHETES> ");
                                        break;
                                case FECHA_COLC: printf("<SN, FECHA_COLCHETES> ");
                                        break;
                                case DIFERENTE: printf("<SN, DIFERENTE> ");
                                        break;
                                case NOT: printf("<SN, NOT> ");
                                        break;
                                case MENOR_IGUAL: printf("<SN, MENOR_IGUAL> ");
                                        break;
                                case MENOR_Q: printf("<SN, MENOR_QUE> ");
                                        break;
                                case MAIOR_IGUAL: printf("<SN, MAIOR_IGUAL> ");
                                        break;
                                case MAIOR_Q: printf("<SN, MAIOR_QUE> ");
                                        break;
                                case IGUAL: printf("<SN, IGUAL> ");
                                        break;
                                case ATRIB: printf("<SN, ATRIBUICAO> ");
                                        break;
                                case AND: printf("<SN, AND> ");
                                        break;
                                case E_COMERCIAL: printf("<SN, &> ");
                                        break;
                                case OR: printf("<SN, OR> ");
                                        break;
                            }
                        break;
            case FIM_ARQ: printf("<Fim do arquivo encontrado>\n");
        }
        if (t.cat == FIM_ARQ) break;
    }
    
    
    
    fclose(fd);
}

void TestaAnasint() {
    if ((fd=fopen("codigo.dat", "r")) == NULL)
        erro("Arquivo de entrada do código nao encontrado!");

    IniciaTabIdentif();
  
    
    t = AnaLex(fd);
    if (t.cat == FIM_ARQ) {
        printf("\nFim do arquivo fonte encontrado!\n");
    }
    Prog();
    if (t.cat==FIM_ARQ)
        printf("\nCódigo fonte sintaticamente correto!\n");
    else {
      printf("%d\n", t.cat);
      if(t.cat == ID){
        printf("%s\n", t.lexema);
      }else if(t.cat == PR){
        printf("%s\n", palavras_reservadas[t.codigo]);
      }
      printf("%d", contLinha);
      erro("Erro de sintaxe!");
    }
  
    fclose(fd);
}

int main() {

  system("chcp 65001");  // Troca a página de códigos de caracteres 
                         // da console para UTF-8 na execução

  contLinha = 1;
  printf("\n\n[Análise Léxica -------------------]\n");
  //TestaAnalex();
  printf("Lookahead não deixa testar léxico e sintático ao mesmo tempo por causa do bool primVez");

  contLinha = 1;
  printf("\n\n[Análise Sintática ----------------]\n");
  TestaAnasint();

  for(int i=0; i<tabIdentif.tamTab; i++){
    REG_IDENTIF ident = tabIdentif.identificador[i];
    printf("\n\nnomeId: %s\nEndereço: %d\ncatId: %d\nescopo: %d\ntipo: %d\nisConst: %d\nzombie: %d\nref: %d\ndimensões: %d\nparametros: %d", ident.nomeId, ident.ender, ident.catId, ident.escopo, ident.tipo, ident.isConst, ident.zombie, ident.ref, ident.dimensoes, ident.parametros);
  }
}
