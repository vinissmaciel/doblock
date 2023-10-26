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
    int tamD = 0;
    int pr = 0;
    char ascii;

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
                    else
                        error("Caracter inválido!");
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
                                pr=1;
                            }
                        }

                        if(pr==0){
                            t.cat = ID;
                        }else{
                            t.cat = PR;
                        }
                        strcpy(t.lexema, lexema);
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
                        ascii = c;
                    }
                    else{
                        error("Caracter inválido!");
                    }
                    break;
            case 10:
                    if(c == '\''){
                        estado = 13;
                        t.cat = CHARCON;
                        t.caracter = ascii;
                        return t;
                    }
                    else{
                        error("Caracter inválido!");
                    }
                    break;
            case 11:
                    if(c == 'n' || c == '0'){
                        estado = 10;
                        if(c == 'n'){
                            ascii = '\n';
                        }else{
                            ascii = '\0';
                        }
                    }
                    else{
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
            case PR: printf("<PR, %s> ", tk.lexema);
                    break;
            case INTCON: printf("<INTCON, %d> ", tk.valInt);
                    break;
            case REALCON: printf("<REALCON, %g> ", tk.valReal);
                    break;
            case CHARCON: printf("<CHARCON, %c> ", tk.caracter);
                    break;
            case FIM_ARQ: printf("<Fim do arquivo encontrado>\n");
        }
        if (tk.cat == FIM_ARQ) break;
    }
    
    
    
    fclose(fd);
    return 0;
}