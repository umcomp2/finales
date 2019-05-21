#ifndef _multihilo_h_
#define _multihilo_h_

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


void *  hilo(void *);
int enviar(char*);
int responder(char*);
int verificar(char*);
int responder_error();

#endif
