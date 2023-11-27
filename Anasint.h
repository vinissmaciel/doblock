#ifndef _ANASINT_
#define _ANASINT_

#include <stdio.h>
#include <stdbool.h>
#include "Analex.h"
#include "Tabidentific.h"


/* Variaveis globais */
extern TOKEN t;
extern TOKEN lookahead;
extern TAB_IDENTIF tabIdentif;
extern FILE *fd;
extern int contLinha;

/* Assinaturas das funcoes */
void Prog();
void Decl_list_var();
void Decl_block_prot();
void Block_main();
void Decl_var();
void Tipo();
void Block_def();
void Cmd();
void Atrib ();
void Expr();
void Expr_simp();
void Termo();
void Fator();
void Op_rel();

#endif // ANASINT