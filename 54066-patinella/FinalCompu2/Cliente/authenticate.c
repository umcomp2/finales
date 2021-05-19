#include "authenticate.h"


char *login(void *connfd){
  printf("Hola login\n");
  char *usuario=(char*)malloc(30*sizeof(char));
  char *clave=(char*)malloc(30*sizeof(char));
  char buffSend[100];
  char buffRecived[100];
  bzero(buffSend,100);
  bzero(buffRecived,100);

  //printf("sizeof(usuario)=%ld",30*sizeof(char));

  int fdSrv=0;
  fdSrv = (int) (intptr_t) connfd;
  printf("Usuario: \n");
  read(1,usuario,30*sizeof(char));
  printf("Clave: \n");
  read(1,clave,30*sizeof(char));

  usuario=strtok(usuario,"\n");//quitamos el salto de linea
  clave=strtok(clave,"\n");//quitamos el salto de linea


  printf("-> usuario:-%s- clave: -%s- ",usuario,clave);

  printf("Datos usr->-%s- clave->-%s-\n",usuario,clave);

  strncpy(buffSend,usuario,strlen(usuario));
  strncat(buffSend,"/",2);
  strncat(buffSend,clave,strlen(clave));
  strncat(buffSend,"\n",2);

  printf("buffSend->%s tam->%ld \n",buffSend,strlen(buffSend));

  write(fdSrv,buffSend,strlen(buffSend)); //enviamos los datos de log al Server
  bzero(buffSend,100);

  //esperamos la respuesta del server <<<<<-------------
  read(fdSrv,buffRecived,sizeof(buffRecived)); // En caso de que el logueo no sea excitoso el server manda '0\n'
                                                // En caso contrario manda el ip a utilizar
  if(!strcmp(buffRecived,"0\n")){
    return "";
  } else {
    char *retorno=(char*)malloc(30*sizeof(char));
    strncpy(retorno,buffRecived,strlen(buffRecived));
    return retorno;
  }
}
