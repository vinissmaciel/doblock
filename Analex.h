#ifndef _ANALEX_
#define _ANALEX_
#define TAM_MAX_LEXEMA 31 
#define TAM_MAX_STRING 201
#define NUM_PR 29

#include <stdio.h>
#include <stdbool.h>


enum TOKEN_CAT {ID=1, SN, CHARCON, REALCON, INTCON, STRINGCON, PR, FIM_ARQ};  /* Onde: ID: Identificador, SN: Sinal; etc */

enum SINAIS {ATRIB = 1, ADICAO, SUBTRACAO, MULTIPLIC, DIVISAO, ABRE_PAR, FECHA_PAR, NOT, DIFERENTE, MENOR_Q, MENOR_IGUAL, MAIOR_Q, MAIOR_IGUAL, IGUAL, 
            E_COMERCIAL, AND, OR, ABRE_CHAVE, FECHA_CHAVE, ABRE_COLC, FECHA_COLC, VIRGULA}; // Sinais válidos da linguagem 

enum PALAVRAS_RESERVADAS {CONST, BLOCK, MAIN, ENDBLOCK, CHAR, INT, REAL, BOOL, WITH, DO, VARYING, FROM, TO, DOWNTO, WHILE, FOR, TIMES, IF, ELSEIF, ELSE, ENDIF, 
                        ENDWHILE, GOBACK, GETINT, GETREAL, GETCHAR, PUTINT, PUTREAL, PUTCHAR}; // Códigos das palavras reservadas

enum NLBARRA0 {BARRA_N = 10, BARRA_0 = 0}; // ascii do '\n' e '\0'

typedef 
    struct {
        enum TOKEN_CAT cat; // deve receber uma das constantes de enum TOKEN_CAT 
        union { // parte variável do registro 
            int codigo; // para tokens das categorias SN e PR 
            char lexema[TAM_MAX_LEXEMA];    // cadeia de caractares que corresponde o nome do token da cat. ID
            int valInt; // valor da constante inteira em tokens da cat. INTCON
            char string[TAM_MAX_STRING]; // strings para token da cat. STRINGCON
            float valReal; // valor da constante real em tokens da cat. REALCON
            char caracter; // caracter da constate char da cat. CHARCON
        }; 
    } TOKEN;    // Tipo TOKEN 

/* Variaveis globais */
extern TOKEN t;
extern TOKEN lookahead;
extern FILE *fd;
extern int contLinha; /* Contador de linhas do código fonte */
extern char *palavras_reservadas[];

/* Assinaturas de funcoes */
TOKEN AnaLex(FILE *);
TOKEN AnaLexTLA(FILE *);

#endif