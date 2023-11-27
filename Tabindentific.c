#include <stdlib.h>
#include <string.h>
#include "Funcaux.h"
#include "Tabidentific.h"

void IniciaTabIdentif() {
  tabIdentif.tamTab = 0;  
}


int BuscaTabIdetif(char nomeId[]) {

  int i;

  for (i=0; i<tabIdentif.tamTab; i++) {
      if (!(strcmp(nomeId, tabIdentif.identificador[i].nomeId))) return i;
  }
  return -1;
}

int BuscaTabIdetifBlk(char nomeId[]) {

  int i;

  for (i=0; i<tabIdentif.tamTab; i++) {
      if (!(strcmp(nomeId, tabIdentif.identificador[i].nomeId)) && tabIdentif.identificador[i].catId == BLK) return i;
  }
  return -1;
}


int InsereTabIdentif(char nomeId[], int catId, int escopo, int tipo, bool isConst, bool zombie, bool ref, int dimensoes, int parametros) {

  int i;
  if (tabIdentif.tamTab == TAM_MAX_TAB_IDENTIF) erro("Estouro na tabela de identificadores!");
  i = tabIdentif.tamTab;
  strcpy(tabIdentif.identificador[i].nomeId, nomeId);
  tabIdentif.identificador[i].ender = i;
  tabIdentif.identificador[i].catId = catId;
  tabIdentif.identificador[i].escopo = escopo;
  if(catId != BLK) tabIdentif.identificador[i].tipo = tipo;
  if(catId == VG || catId == VL) tabIdentif.identificador[i].isConst = isConst;
  if(catId == ARG) tabIdentif.identificador[i].zombie = zombie;
  if(catId == ARG) tabIdentif.identificador[i].ref = ref;
  if(catId != BLK) tabIdentif.identificador[i].dimensoes = dimensoes;
  if(catId == BLK) tabIdentif.identificador[i].parametros = parametros;

  tabIdentif.tamTab++;
  return i;
}

void EditaQtdParams(char nomeId[], int parametros){
  int i;
  i = BuscaTabIdetifBlk(nomeId);

  if(i == -1) erro("Identificador nao encontrado!");

  if(tabIdentif.identificador[i].catId != BLK) erro("Identificador não é um bloco");

  tabIdentif.identificador[i].parametros = parametros;
}

void EditaNomeArg(char nomeId[], int i){
  strcpy(tabIdentif.identificador[i].nomeId, nomeId);
}

void ExcluiVL(char nomeId[]){
  int blk;
  blk = BuscaTabIdetifBlk(nomeId);

  for(int i=tabIdentif.tamTab-1; i>blk; i--){
    if(tabIdentif.identificador[i].catId == VL){
      strcpy(tabIdentif.identificador[i].nomeId, "");
      tabIdentif.identificador[i].isConst = 0;
      tabIdentif.identificador[i].zombie = 0;
      tabIdentif.identificador[i].ref = 0;
      tabIdentif.identificador[i].dimensoes = 0;
      tabIdentif.identificador[i].parametros = 0;

      tabIdentif.tamTab--;
    }
  }
}

void ZombieARG(char nomeId[]){
  int blk;
  blk = BuscaTabIdetifBlk(nomeId);

  for(int i=tabIdentif.tamTab-1; i>blk; i--){
    if(tabIdentif.identificador[i].catId == ARG){
      tabIdentif.identificador[i].zombie = 1;
    }
  }
}