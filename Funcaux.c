#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Analex.h"
#include "Funcaux.h"
#include "Tabidentific.h"

char *msgs_erro[] = {
"Caracter inválido!",//0
"Erro de sintaxe!",//1
"Arquivo de entrada do código nao encontrado!", //2
"Estouro na tabela de identificadores!",//3
"Bloco não declarado",//4
"Esperado declaração de variável ou PR block",//5
"Redeclaração de identificador",//6
"Esperado PR block",//7
"PRs esperadas: char, int, real, bool",//8
"Identificador esperado",//9
"Bloco já definido",//10
"Número inteiro ou identificador inteiro constante (const int id) esperado",//11
"Identificador inteiro constante (const int id) não declarado",//12
"Identificador inteiro constante (const int id) esperado",//13
"Fecha colchetes esperado",//14
"Limite de dimensões de argumento excedido (Max: 3)",//15
"Parâmetro com tipo diferente da declaração",//16
"Parâmetro com dimensões diferentes da declaração",//17
"Identificador já presente na lista de parâmetros formais",//18
"Quantidade de parâmetros diferente da declaração",//19
"Esperado PR endblock",//20
"Limite de dimensões de identificador excedido (Max: 3)",//21
"Constante não inicializada",//22
"Número inteiro, real, char ou string esperado",//23
"Fecha chaves esperado",//24
"Abre chaves, número inteiro, real, char ou string esperado",//25
"Quantidade máxima de parâmetros excedida",//26
"Esperado PR main",//27
"Identificador não declarado",//28
"Identificador esperado ou PRs esperadas: do, if, while, goback, getint, getreal, getchar, putint, putreal, putchar",//29
"Esperado PR from",//30
"Esperado PR to ou PR downto",//31
"Esperado abre parenteses",//32
"Esperado fecha parenteses",//33
"Esperado PR times",//34
"Esperado PR endif",//35
"Esperado PR endwhile",//36
"Atribuição em variável constante",//37
"Sinal de atribuição (=) esperado",//38
"Esperado: Identificador, número inteiro, real ou char, abre parenteses ou !",//39
"Operador relacional esperado (=, !=, <, <=, >, >=)",//40
"Não foi possível criar o arquivo de saída!"//41
};

void erro(int i) {
    printf("%s\n", msgs_erro[i]);
    exit(1);
}

void erroanasint(int i){
  printf("ERRO DE SINTAXE NA LINHA %d: %s\n", contLinha, msgs_erro[i]);
  exit(1);
}

void errosemantico(int i){
  printf("ERRO SEMÂNTICO NA LINHA %d: %s\n", contLinha, msgs_erro[i]);
  exit(1);
}

void gera_label(){
  char buffer[TAM_MAX_LABEL];
  static int cont = 1;
  strcpy(label, "L");
  sprintf(buffer, "%d", cont);
  strcat(label, buffer);
  cont++;
}