#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Analex.h"
#include "Funcaux.h"

void erro(char msg[]) {
    printf("%s\n", msg);
    exit(1);
}

void erroanasint(char msg[]){
  printf("ERRO DE SINTAXE NA LINHA %d: %s\n", contLinha, msg);
  exit(1);
}