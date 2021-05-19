#ifndef SOCKCMP
#define SOCKCMP
#include "AllIncludes.h"
int conectar(void *fd, char *datos_empleado);

char *salvar(char *datos_empleado, int op); //op==1 devuelve fd  //op==2 id //op==3 usuario
                                             //op==4 tipo //op==5 ip //op==6 puerto
void porobarSalvar();
#endif
