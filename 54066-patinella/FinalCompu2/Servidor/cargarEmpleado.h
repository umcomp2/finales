#ifndef CARGAREMPLEADOH
#define CARGAREMPLEADOH
#include "AllIncludes.h"
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/mman.h>
#include <semaphore.h>

int cargarEmpleado(char *empleado, char *fd_empleados_on, sem_t *sem_empleados_on);

char *fabricarDatos(char *fd_empleados_on);

char *quitarEmpleado(char *fd_empleados_on, char *datos_empleado);

int asignarPuerto(char *fd_empleados_on, int puerto);

char *salvar(char *datos_empleado, int op); //op==1 devuelve fd  //op==2 id //op==3 usuario
                                             //op==4 tipo //op==5 ip //op==6 puerto
void levantarSocket(void *fd, char *ip, char *puerto);
#endif
