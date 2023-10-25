#ifndef ANALEX
#define ANALEX
#define TAM_MAX_LEXEMA 31 
#define TAM_MAX_STRING 201
#define NUM_PR 29

char *palavras_reservadas[] = {"const", "block", "id", "&", "main", "endblock", "intcon", "realcon", "charcon", "stringcon", 
                                "char", "int", "real", "bool", "with", "do", "varying", "from", "to", "downto", "while", "for", 
                                "times", "if", "elseif", "else", "endif", "endwhile", "goback"};

enum TOKEN_CAT {ID=1, SN, CHARCON, REALCON, INTCON, STRINGCON, PR, FIM_ARQ};  /* Onde: ID: Identificador, SN: Sinal; */

enum SINAIS {ATRIB = 1, ADICAO, SUBTRACAO, MULTIPLIC, DIVISAO, ABRE_PAR, FECHA_PAR}; // Sinais válidos da linguagem 

typedef 
    struct {
        enum TOKEN_CAT cat; // deve receber uma das constantes de enum TOKEN_CAT 
        union { // parte variável do registro 
            int codigo; // para tokens das categorias SN
            char lexema[TAM_MAX_LEXEMA];    // cadeia de caractares que corresponde o nome do token da cat. ID ou PR
            int valInt; // valor da constante ineira em tokens da cat. INTCON
            char string[TAM_MAX_STRING]; 
            float valReal;
            char caracter;
        }; 
} TOKEN;    // Tipo TOKEN 

#endif

/* Contador de linhas do código fonte */ 

int contLinha = 1;