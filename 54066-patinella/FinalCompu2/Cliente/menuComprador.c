#include "menuComprador.h"

char *menuComprador(){
  char *tipo=(char*)malloc(15*sizeof(char*));
  printf("1)Ventas\n2)Consultas\n3)Reclamos\n4)exit\n");
  scanf("%s",tipo);
  return tipo;
}
