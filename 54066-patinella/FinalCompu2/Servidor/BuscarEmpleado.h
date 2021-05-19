#ifndef BUSCAREMPLEADOH
#define BUSCAREMPLEADOH
#include "AllIncludes.h"
#include "cargarEmpleado.h"
char *buscarEmpleado(char *fd_empleados_on, sem_t *sem_empleados_on, char *tipo);

void conectarEmpleado(void * connfd,char *datos_empleado);

char *salvarEmpleado(char *fd_empleados_on,char *tipo);

#endif
