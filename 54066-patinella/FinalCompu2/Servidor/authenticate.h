#ifndef AUTHENTICATEH
#define AUTHENTICATEH
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "menu.h"


//#include "auxiliares.h"

char *authenticate(void * connfd);

char *authenticate_ok(char *usr, char *pass, void * connfd);

char *comparar(char *usr, char *pass);

char *login(void *connfd);
//si existe una coicidencia entre el usuario y claves enviados con alguno
//de los usuarios y claves del archivo, la funcion retornara una cadena
// de caracteres con la siguiente forma id/usuario/tipo

#endif
