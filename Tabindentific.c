#include <stdlib.h>
#include <string.h>
#include "Funcaux.h"
#include "Tabidentific.h"

void IniciaTabIdentif() {
  tabIdentif.tamTab = 0;  
}


int BuscaTabIdetif(char nomeId[]) {

  int i;

  for (i=tabIdentif.tamTab-1; i>=0; i--) {
      if (!(strcmp(nomeId, tabIdentif.identificador[i].nomeId))) return i;
  }
  return -1;
}

int BuscaTabIdetifPorEscopo(char nomeId[], int escopo) {

  int i;

  for (i=tabIdentif.tamTab-1; i>=0; i--) {
      if (!(strcmp(nomeId, tabIdentif.identificador[i].nomeId)) && tabIdentif.identificador[i].escopo == escopo) return i;
  }
  return -1;
}

int BuscaTabIdetifBlk(char nomeId[]) {

  int i;

  for (i=tabIdentif.tamTab-1; i>=0; i--) {
      if (!(strcmp(nomeId, tabIdentif.identificador[i].nomeId)) && tabIdentif.identificador[i].catId == BLK) return i;
  }
  return -1;
}

int BuscaTabIdetifVAR(char nomeId[]) {

  int i;

  for (i=tabIdentif.tamTab-1; i>=0; i--) {
      if (!(strcmp(nomeId, tabIdentif.identificador[i].nomeId)) && (tabIdentif.identificador[i].catId == VL || tabIdentif.identificador[i].catId == VG || (tabIdentif.identificador[i].catId == ARG && tabIdentif.identificador[i].zombie == 0))) return i;
  }
  return -1;
}


int InsereTabIdentif(char nomeId[], int catId, int escopo, int tipo, bool isConst, bool zombie, bool ref, int dimensoes, int parametros) {

  int i;
  if (tabIdentif.tamTab == TAM_MAX_TAB_IDENTIF) erro(3);
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
  if(catId == BLK) tabIdentif.identificador[i].def = 0;
  strcpy(tabIdentif.identificador[i].label, "");

  tabIdentif.tamTab++;
  return i;
}

void EditaQtdParams(char nomeId[], int parametros){
  int i;
  i = BuscaTabIdetifBlk(nomeId);

  if(i == -1) erro(4);

  if(tabIdentif.identificador[i].catId != BLK) erro(4);

  tabIdentif.identificador[i].parametros = parametros;
}

void EditaNomeArg(char nomeId[], int i){
  strcpy(tabIdentif.identificador[i].nomeId, nomeId);
}

void ExcluiVL(){
  for(int i=tabIdentif.tamTab-1; i>=0; i--){
    if(tabIdentif.identificador[i].catId == VL){
      strcpy(tabIdentif.identificador[i].nomeId, "");
      tabIdentif.identificador[i].isConst = 0;
      tabIdentif.identificador[i].zombie = 0;
      tabIdentif.identificador[i].ref = 0;
      tabIdentif.identificador[i].dimensoes = 0;
      tabIdentif.identificador[i].parametros = 0;
      tabIdentif.identificador[i].def = 0;
      tabIdentif.identificador[i].valorInt = 0;
      tabIdentif.identificador[i].valorReal = 0;
      tabIdentif.identificador[i].valorChar = 0;
      tabIdentif.identificador[i].tam_dim1 = 0;
      tabIdentif.identificador[i].tam_dim2 = 0;
      tabIdentif.identificador[i].tam_dim3 = 0;
      strcpy(tabIdentif.identificador[i].valorString, "");
      strcpy(tabIdentif.identificador[i].label, "");

      tabIdentif.tamTab--;
    }
  }
}

void ZombieARG(char nomeId[]){

  int aux, lim;
  aux = BuscaTabIdetifBlk(nomeId);
  lim = tabIdentif.identificador[aux].parametros + aux;
  
  for(int i=aux+1; i<=lim; i++){
    if(tabIdentif.identificador[i].catId == ARG){
      tabIdentif.identificador[i].zombie = 1;
    }
  }
}