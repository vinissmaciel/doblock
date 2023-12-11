#ifndef _FUNC_AUX_
#define _FUNC_AUX_

#include <stdbool.h>

extern int contLinha;
extern char *msgs_erro[];
extern char label[];

void erro(int i);
void erroanasint(int i);
void errosemantico(int i);
void gera_label();

#endif