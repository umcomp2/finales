#ifndef _contar_h_
#define _contar_h_

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <semaphore.h>
#include <sys/mman.h>

int contar ( char*,int);
char * codigo_articulo(char*);
char * cantidad_articulo(char*);
char * procesamiento(char*,char*);
char *  insertar (char*);


#endif 
