#include "AllIncludes.h"
#include "authenticate.h"
#include "menuComprador.h"
#include "SockEmp.h"
#include "SockCmp.h"

int fd;
void handler(int nro);

int main(int argc, char * argv[]){
  if(argc<2){
    printf("Error al ingresar argumentos");
    return 0;
  }

  char *ipSrvStr=malloc(32*sizeof(char));
  strncpy(ipSrvStr,argv[1],strlen(argv[1]));
  char *portSrvStr=malloc(32*sizeof(char));
  strncpy(portSrvStr,argv[2],strlen(argv[2]));

  printf("Intenta conectarse a-> IP(%s) PUERTO(%s) \n",ipSrvStr,portSrvStr);

  //porobarSalvar();
  struct sockaddr_in direccionServidor;

  int fdSrv = socket(AF_INET, SOCK_STREAM, 0);

  memset(&direccionServidor,0,sizeof(direccionServidor));

  direccionServidor.sin_family = AF_INET;
  direccionServidor.sin_addr.s_addr = inet_addr(ipSrvStr); // direccion a la que se va a conectar
  direccionServidor.sin_port = htons(atoi(portSrvStr));

  char buffSend[100];//buffer de escritura;
  char buffRecived[100];//buffer de recepcion;
  bzero(buffSend,100);
  bzero(buffRecived,100);


  if(connect(fdSrv, (void*)&direccionServidor, sizeof(direccionServidor)) != 0){
    perror("No se pudo conectar");
    return 1;
  }
  fd=fdSrv;

  while(1){
    printf("1)Empleado\n2)Cliente\n");

    read(1,buffSend,sizeof(buffSend)); //leemos lo que vamos a enviar
    write(fdSrv,buffSend,strlen(buffSend));
    printf("La peticion enviada fue-> %s",buffSend);

    if(!strcmp(buffSend,"1\n")){
      char *ipStr=login((void*) (intptr_t) fdSrv);

      if(strcmp(ipStr,"")){
        printf("El ip que nos devolvio el Servidor es ->%s\n",ipStr);
        //printf("Deriamos enviarle al Servidor el puerto a utilizar\n");
        int res_sock=levantarSocket((void*) (intptr_t) fdSrv,ipStr);
        printf("res_sock->%d\n",res_sock);
        //si el empleado ser desconecta en esta fucnion con ctrl+c deberiamos llamar al handler
      }else{
        printf("Error al loguearse.\n");
        break; //salimos del segundo if
        break; //salimos del primer if para que no retorne 0
      }
    } else if(!strcmp(buffSend,"2\n")){
      printf("Deberia conectar con el empleado\n");
      char *tipo=menuComprador((void*) (intptr_t) fdSrv);
      printf("Escogio un empleado del tipo->-%s-",tipo);

      bzero(buffSend,100);
      strncpy(buffSend,tipo,strlen(tipo));
      strncat(buffSend,"\n",2);
      printf("buffSend->%s\n",buffSend);
      write(fdSrv,buffSend,strlen(buffSend)); //le enviamos al servidor el tipo de empleado que estamos buscando
      //---------------ESPERAMOS AL SERVUDOR QUE ENVIE DATOS DEL EMPLEADO--------------------
      bzero(buffRecived,100);
      read(fdSrv,buffRecived,sizeof(buffRecived));
      printf("buffRecived->-%s-\n",buffRecived);
      char *datos_empleado=(char*)malloc(128*sizeof(char));
      strncpy(datos_empleado,buffRecived,strlen(buffRecived)); // Guardamos los datos del empleado en datos_empleado
      bzero(buffRecived,100);
      printf("datos_empleado->-%s-\n",datos_empleado);

      if(!strcmp(datos_empleado,"\n")){
          printf("Lo sentimos... no hay ningun empleado en este momento intente mas tarde\n");
          return 0;
      }
      else{
          printf("FC Cliente.c -> los datos son datos_empleado->-%s-\n",datos_empleado);
          printf("Llamamos a la fc SockEmp para establecer el chat\n");
          int retorno = conectar((void*) (intptr_t) fdSrv,datos_empleado);
          printf("Menu.c -> retorno %d",retorno);
      }


      return 0;
    }else{
      printf("Opcion incorrecta\n");
    }

    bzero(buffSend,100);
  }

  signal(SIGINT, handler);

  printf("FIN\n");
  return 0;
}

void handler(int nro){
  char buffSend[100];//buffer de escritura;
  bzero(buffSend,100);
  printf("handle ejecutado le escribiremos a fd->%d ",fd);
  char *comando="exit\n\0";
  strncpy(buffSend,comando,strlen(comando));
  read(1,buffSend,sizeof(buffSend)); //leemos lo que vamos a enviar
  write(fd,buffSend,strlen(buffSend));

}
