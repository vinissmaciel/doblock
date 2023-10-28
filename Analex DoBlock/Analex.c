#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "Analex.h" 
#define TAM_LEXEMA 50 
#define TAM_NUM 20 

void error(char msg[]) { 

    printf("%s\n", msg); 
    exit(1); 

} 

TOKEN AnaLex(FILE *fd){

    int estado = 0;
    char lexema[TAM_LEXEMA] = "";
    int tamL = 0;
    char digitos[TAM_NUM] = "";
    char string[TAM_MAX_STRING] = "";
    int tamD = 0;
    int pr = 0, indexpr;
    char caracter;

    TOKEN t;

    while(true){

        char c = fgetc(fd);
        switch(estado){
            case 0: if (c == ' ' || c == '\t') estado = 0;
                    else if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
                        estado = 1;
                        lexema[tamL] = c;
                        lexema[++tamL] = '\0';
                    }
                    else if(c == '_'){
                        estado = 2;
                        lexema[tamL] = c;
                        lexema[++tamL] = '\0';
                    }
                    else if(c == '\n'){
                        estado = 0;
                        contLinha++;
                        printf("\nLINHA %d: ", contLinha);
                    }
                    else if (c == EOF){
                        t.cat = FIM_ARQ;
                        return t;
                    }
                    else if(c >= '0' && c <= '9'){
                        estado = 4;
                        digitos[tamD] = c;
                        digitos[++tamD] = '\0';
                    }
                    else if(c == '\''){
                        estado = 9;
                    }
                    else if(c == '/'){
                        estado = 14;
                    }
                    else if(c == '\"'){
                        estado = 17;
                        string[tamL] = '\0';
                    }
                    else if(c == '*'){
                        estado = 21;
                        t.cat = SN;
                        t.codigo = MULTIPLIC;
                        return t;
                    }
                    else if(c == '-'){
                        estado = 20;
                        t.cat = SN;
                        t.codigo = SUBTRACAO;
                        return t;
                    }
                    else if(c == '+'){
                        estado = 23;
                        t.cat = SN;
                        t.codigo = ADICAO;
                        return t;
                    }
                    else if(c == ','){
                        estado = 46;
                        t.cat = SN;
                        t.codigo = VIRGULA;
                        return t;
                    }
                    else if(c == '['){
                        estado = 44;
                        t.cat = SN;
                        t.codigo = ABRE_COLC;
                        return t;
                    }
                    else if(c == ']'){
                        estado = 47;
                        t.cat = SN;
                        t.codigo = FECHA_COLC;
                        return t;
                    }
                    else if(c == '{'){
                        estado = 41;
                        t.cat = SN;
                        t.codigo = ABRE_CHAVE;
                        return t;
                    }
                    else if(c == '}'){
                        estado = 43;
                        t.cat = SN;
                        t.codigo = FECHA_CHAVE;
                        return t;
                    }
                    else if(c == '('){
                        estado = 42;
                        t.cat = SN;
                        t.codigo = ABRE_PAR;
                        return t;
                    }
                    else if(c == ')'){
                        estado = 40;
                        t.cat = SN;
                        t.codigo = FECHA_PAR;
                        return t;
                    }
                    else{
                        error("Caracter inválido!");
                    }
                    break;
            case 1:
                    if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_')){
                        estado = 1;
                        lexema[tamL] = c;
                        lexema[++tamL] = '\0';
                    }else{
                        estado = 3;
                        ungetc(c, fd);

                        for (int i = 0; i < NUM_PR; i++){ // VERIFICA SE É PR
                            if(strcmp(palavras_reservadas[i], lexema) == 0){
                                pr = 1;
                                indexpr = i;
                            }
                        }

                        if(pr==0){
                            t.cat = ID;
                            strcpy(t.lexema, lexema);
                        }else{
                            t.cat = PR;
                            t.codigo = indexpr;
                        }
                        return t;
                    }
                    break;
            case 2:
                    if(c == '_'){
                        estado = 2;
                        lexema[tamL] = c;
                        lexema[++tamL] = '\0';
                    }
                    else if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
                        estado = 1;
                        lexema[tamL] = c;
                        lexema[++tamL] = '\0';
                    }
                    else{
                        error("Caracter inválido!");
                    }
                    break;
            case 4: 
                    if(c >= '0' && c <= '9'){
                        estado = 4;
                        digitos[tamD] = c;
                        digitos[++tamD] = '\0';
                    }
                    else if(c == '.'){
                        estado = 6;
                        digitos[tamD] = c;
                        digitos[++tamD] = '\0';
                    }
                    else{
                        estado = 5;
                        ungetc(c, fd);
                        t.cat = INTCON;
                        t.valInt = atoi(digitos);
                        return t;
                    }
                    break;
            case 6:
                    if(c >= '0' && c <= '9'){
                        estado = 7;
                        digitos[tamD] = c;
                        digitos[++tamD] = '\0';
                    }
                    else{
                        error("Caracter inválido!");
                    }
                    break;
            case 7:
                    if(c >= '0' && c <= '9'){
                        estado = 7;
                        digitos[tamD] = c;
                        digitos[++tamD] = '\0';
                    }
                    else{
                        estado = 8;
                        ungetc(c, fd);
                        t.cat = REALCON;
                        t.valReal = atof(digitos);
                        return t;
                    }
                    break;
            case 9:
                    if(c == '\\'){
                        estado = 11;
                    }
                    else if((isprint(c) != 0) && (c != '\'')){
                        estado = 10;
                        caracter = c;
                    }
                    else{
                        error("Caracter inválido!");
                    }
                    break;
            case 10:
                    if(c == '\''){
                        estado = 13;
                        t.cat = CHARCON;
                        t.caracter = caracter;
                        return t;
                    }
                    else{
                        error("Caracter inválido!");
                    }
                    break;
            case 11:
                    if(c == 'n' || c == '0'){
                        estado = 12;
                        if(c == 'n'){
                            caracter = BARRA_N;
                        }else{
                            caracter = BARRA_0;
                        }
                    }
                    else{
                        error("Caracter inválido!");
                    }
                    break;
            case 12:
                    if(c == '\''){
                        estado = 16;
                        t.cat = CHARCON;
                        t.caracter = caracter;
                        return t;
                    }
                    else{
                        error("Caracter inválido!");
                    }
                    break;
            case 14:
                    if(c == '/'){
                        estado = 15;
                    }
                    else{
                        estado = 24;
                        ungetc(c, fd);
                        t.cat = SN;
                        t.codigo = DIVISAO;
                        return t;
                    }
                    break;
            case 15:
                    if(c == '\n'){
                        ungetc(c, fd);
                        estado = 0;
                    }
                    else{
                        estado = 15;
                    }
                    break;
            case 17:
                    if(isprint(c) != 0 && c != '\"'){
                        estado = 17;
                        string[tamL] = c;
                        string[++tamL] = '\0';
                    }
                    else if(c == '\"'){
                        estado = 18;
                        t.cat = STRINGCON;
                        strcpy(t.string, string);
                        return t;
                    }else{
                        error("Caracter inválido!");
                    }
                    break;
        }
    }
}

int main() { 

    FILE *fd;
    TOKEN tk;

    if ((fd=fopen("codigo.dat", "r")) == NULL) 
        error("Arquivo de entrada do código nao encontrado!"); 

    printf("LINHA %d: ", contLinha);
    while(1){
        tk = AnaLex(fd);
        
        switch (tk.cat) {
            case ID: printf("<ID, %s> ", tk.lexema);
                    break;
            case PR:  
                    printf("<PR, ");
                    int i = 0;
                    while((palavras_reservadas[tk.codigo])[i]){
                        putchar(toupper((palavras_reservadas[tk.codigo])[i])); // ROTINA PARA DEIXAR STRING MAISCULA
                        i++;
                    }
                    printf("> ");
                    break;
            case INTCON: printf("<INTCON, %d> ", tk.valInt);
                    break;
            case REALCON: printf("<REALCON, %g> ", tk.valReal);
                    break;
            case CHARCON: switch (tk.caracter){
                                case BARRA_N: printf("<CHARCON, \'\\n\'> ");
                                        break;
                                case BARRA_0: printf("<CHARCON, \'\\0\'> ");
                                        break;
                                default: printf("<CHARCON, \'%c\'> ", tk.caracter);
                            }
                        break;
            case STRINGCON: printf("<STRINGCON, \"%s\"> ", tk.string);
                    break;
            case SN: switch (tk.codigo){
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
                            }
                        break;
            case FIM_ARQ: printf("<Fim do arquivo encontrado>\n");
        }
        if (tk.cat == FIM_ARQ) break;
    }
    
    
    
    fclose(fd);
    return 0;
}