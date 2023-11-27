#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "Analex.h" 
#include "Funcaux.h"

#define TAM_LEXEMA 50 
#define TAM_NUM 20

char *palavras_reservadas[] = {"const", "block", "main", "endblock", 
                                "char", "int", "real", "bool", "with", "do", "varying", "from", "to", "downto", "while", "for", 
                                "times", "if", "elseif", "else", "endif", "endwhile", "goback", "geint", "getreal",
                                 "getchar", "putint", "putreal", "putchar"};

TOKEN AnaLex(FILE *fd) {
  static bool primVez = true;
  TOKEN t;
  if (primVez) {
      primVez = false;
      t = AnaLexTLA(fd);
      if (t.cat == FIM_ARQ) lookahead = t;
      else lookahead = AnaLexTLA(fd);
      return t;
  }
  else if (lookahead.cat == FIM_ARQ) {
      return(lookahead);
  }
  else {
      t = lookahead;
      lookahead = AnaLexTLA(fd);

      return t;
  }
}

TOKEN AnaLexTLA(FILE *fd){

    int estado = 0;
    char lexema[TAM_LEXEMA] = "";
    int tamL = 0;
    char digitos[TAM_NUM] = "";
    char string[TAM_MAX_STRING] = "";
    int tamD = 0;
    int pr = 0, indexpr;
    char caracter;

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
                        //printf("\nLINHA %d: ", contLinha);
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
                    else if(c == '!'){
                        estado = 22;
                    }
                    else if(c == '<'){
                        estado = 25;
                    }
                    else if(c == '>'){
                        estado = 27;
                    }
                    else if(c == '='){
                        estado = 33;
                    }
                    else if(c == '&'){
                        estado = 36;
                    }
                    else if(c == '|'){
                        estado = 38;
                    }
                    else{
                        erro("Caracter inválido!");
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
                            t.codigo = indexpr; // SE FOR PR, ATRIBUI O CODIGO DE ACORDO COM INDEX
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
                        erro("Caracter inválido!");
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
                        erro("Caracter inválido!");
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
                        erro("Caracter inválido!");
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
                        erro("Caracter inválido!");
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
                        erro("Caracter inválido!");
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
                        erro("Caracter inválido!");
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
                    if(c == '\n'){              // ACABOU COMENTÁRIO, VOLTA PRO ESTADO 0
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
                        erro("Caracter inválido!");
                    }
                    break;
            case 22:
                    if(c == '='){
                        estado = 32;
                        t.cat = SN;
                        t.codigo = DIFERENTE;
                        return t;
                    }
                    else{
                        estado = 31;
                        ungetc(c, fd);
                        t.cat = SN;
                        t.codigo = NOT;
                        return t;
                    }
                    break;
            case 25:
                    if(c == '='){
                        estado = 28;
                        t.cat = SN;
                        t.codigo = MENOR_IGUAL;
                        return t;
                    }
                    else{
                        estado = 26;
                        ungetc(c, fd);
                        t.cat = SN;
                        t.codigo = MENOR_Q;
                        return t;
                    }
                    break;
            case 27:
                    if(c == '='){
                        estado = 30;
                        t.cat = SN;
                        t.codigo = MAIOR_IGUAL;
                        return t;
                    }
                    else{
                        estado = 29;
                        ungetc(c, fd);
                        t.cat = SN;
                        t.codigo = MAIOR_Q;
                        return t;
                    }
                    break;
            case 33:
                    if(c == '='){
                        estado = 35;
                        t.cat = SN;
                        t.codigo = IGUAL;
                        return t;
                    }
                    else{
                        estado = 34;
                        ungetc(c, fd);
                        t.cat = SN;
                        t.codigo = ATRIB;
                        return t;
                    }
                    break;
            case 36:
                    if(c == '&'){
                        estado = 37;
                        t.cat = SN;
                        t.codigo = AND;
                        return t;
                    }
                    else{
                        estado = 45;
                        ungetc(c, fd);
                        t.cat = SN;
                        t.codigo = E_COMERCIAL;
                        return t;
                    }
                    break;
            case 38:
                    if(c == '|'){
                        estado = 39;
                        t.cat = SN;
                        t.codigo = OR;
                        return t;
                    }
                    else{
                        erro("Caracter inválido!");
                    }
                    break;
        }
    }
}