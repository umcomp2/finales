#ifndef _multiproceso_h_
#define _multiproceso_h_

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <assert.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <netdb.h>
#include <sys/stat.h>
#include <mysql/mysql.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>

void *  hilo(void *);
int enviar(char*);
int contar ( char*,int);
char * codigo_articulo(char*);
char * cantidad_articulo(char*);
char * procesamiento(char*, char*, char*, char*, char*, char*);
char *  insertar (char*, char*, char*, char*, char*);
int  enviar(char*);
int master(int, int, char*,char*, char*, char*, char*);

#endif      
