#include "SockEmp.h"



void handl(int nro);
int fds;
int levantarSocket(void *fd, char *ipStr){
  signal(SIGINT, handl);
  int fdSrv=0;
  fdSrv= (int) (intptr_t) fd;
  fds=fdSrv;
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock < 0) {
    printf("socket error\n");
    return 0;
  }
  printf("Opened %d\n", sock);

  struct sockaddr_in serv_addr;
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = 0;
  if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    if(errno == EADDRINUSE) {
      printf("the port is not available. already to other process\n");
      return 0;
    } else {
      printf("could not bind to process (%d) %s\n", errno, strerror(errno));
      return 0;
    }
  }

  socklen_t len = sizeof(serv_addr);
  if (getsockname(sock, (struct sockaddr *)&serv_addr, &len) == -1) {
    perror("getsockname");
    return 0;
  }

  printf("port number %d\n", ntohs(serv_addr.sin_port));

  int iport=ntohs(serv_addr.sin_port);
  char *sport=(char*)malloc(15*sizeof(char));
  sprintf(sport,"%d", iport);

  //printf("Puerto string -> %s\n",sport);
  //unsigned int leng;     /* length of client address */
  //struct sockaddr_in client;
  listen(sock, 1);

  int fdCmp;
  //-----------------------ENVIAMOS LOS DATOS DEL SOCKET AL SERVIDOR-------------
  char buffSend[100];
  char buffRecived[100];
  bzero(buffRecived,100);
  bzero(buffSend,100);
  strncpy(buffSend,sport,strlen(sport));
  strncat(buffSend,"\n",2);
  write(fdSrv,buffSend,strlen(buffSend));
  bzero(buffSend,100);
  //-----------------------------------------------------------------------------
  printf("esperando que alguien se conecte...\n");
  fdCmp = accept(sock,NULL, 0);
  if (fdCmp < 0)
  {
    fprintf(stderr, "[SERVER-error]: connection not accepted. %d: %s \n", errno, strerror( errno ));
    return -1;
  }else{
    printf("Conexion exitosa, fd de comunicacion-> fdCmp->%d\n",fdCmp);
    read(fdCmp,buffRecived,sizeof(buffRecived));// espramos a recibir el primer saludo
    printf("-> %s \n",buffRecived);
    bzero(buffRecived,100);

    while(!strstr(buffRecived, "adios") && !strstr(buffSend, "adios")){
      bzero(buffSend,100);
      read(1,buffSend,sizeof(buffSend)); //leemos desde la terminal del empleado
      write(fdCmp,buffSend,strlen(buffSend)); //enviamos la respuesta al comprador
      if(!strstr(buffSend, "adios")){
        bzero(buffRecived,100);
        read(fdCmp,buffRecived,sizeof(buffRecived)); //Esperamos respuesta del comprador
        write(1,buffRecived,strlen(buffRecived)); //Mostramos el mensaje al empleado
      }
    }
  }
printf("--------------Chat finalizado--------------\n");



if (close (sock) < 0 ) {
    printf("did not close: %s\n", strerror(errno));
    return 0;
}
  return 0;
}


void handl(int nro){
  char buffSend[100];//buffer de escritura;
  bzero(buffSend,100);
  printf("handle ejecutado le escribiremos a fd->%d ",fds);
  char *comando="";
  strncpy(buffSend,comando,strlen(comando));
  read(1,buffSend,sizeof(buffSend)); //leemos lo que vamos a enviar
  write(fds,buffSend,strlen(buffSend));
  exit(-1);
}
