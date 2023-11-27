#ifndef _TAB_IDENTIFIC_
#define _TAB_IDENTIFIC_

#include <stdlib.h>
#include <stdio.h>
#include "Analex.h"

#define TAM_MAX_TAB_IDENTIF 200

enum IDENTIF_CAT {VG = 0, VL, BLK, ARG};

enum ESCOPO {GLOBAL = 0, LOCAL};

enum TIPO {INTEIRO = 0, REAL_TIPO, CHARACTER, BOOLEAN};

typedef
  struct {
      char nomeId[TAM_MAX_LEXEMA];
      int ender;
      int catId; // ENUM VG, VL, BLK, ARG
      int escopo; // GLOBAL, LOCAL
      int tipo; // INTEIRO, REAL_TIPO, CHARACTER, BOOLEAN
      bool isConst;
      bool zombie;
      bool ref;
      int dimensoes; // 0, 1, 2...
      int parametros; // 0, 1, 2...
  } REG_IDENTIF;                      // Uma entrada na Tabela de Identificadores


typedef
  struct {
      int tamTab;
      REG_IDENTIF identificador[TAM_MAX_TAB_IDENTIF];
  } TAB_IDENTIF;                      // Tabela de identificadores


/* Variaveis globais */
extern TOKEN t;
extern TOKEN lookahead;
extern TAB_IDENTIF tabIdentif;

/* Assinaturas de funcoes */
void IniciaTabIdentif();
int BuscaTabIdetif(char []);
int BuscaTabIdetifBlk(char []);
int InsereTabIdentif(char [], int, int, int, bool, bool, bool, int, int);
void EditaQtdParams(char [], int);
void EditaNomeArg(char [], int);
void ExcluiVL(char []);


#endif // _TAB_IDENTIFIC_