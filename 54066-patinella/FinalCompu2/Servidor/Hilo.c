#include "Hilo.h"

char *pedirDatos(){
  char *datos_conexion=NULL;
  datos_conexion=(char * )malloc(32*sizeof(char));
  time_t tiempo = time(0);
  struct tm *tlocal = localtime(&tiempo);
  char output[128];
  strftime(output,128,"%d/%m/%y %H:%M:%S",tlocal);
  //printf("%s\n tam-> %ld\n",output,strlen(output));
  strcpy(datos_conexion,output);
  //printf("datos con -> %s\n",datos_conexion);
  return datos_conexion;
}
void *hilo(void * ipStr){
  //pthread_detach(pthread_self());
  /*
  if (pthread_detach(pthread_self()) != 0) {
    perror("!!!!pthread_detach() error");
    exit(4);
  }
  */
  char *ipCliStr=(char *)ipStr;
  //CREAMSO EL HILO PARA GUARDAR LOS DATOS DE CONEXION
  char *datos_conexion=pedirDatos();
  //printf("Hilo.c fc:hilo -> datos_conexion: -%s-\n",datos_conexion);
  strncat(datos_conexion,"/",2);
  strncat(datos_conexion,ipCliStr,strlen(ipCliStr));
  strncat(datos_conexion,"\n",2);
  saveConn(datos_conexion);
  printf("!!!!\n");
  pthread_exit(NULL); // ESTO ES PARA TERMINAR EL HILO PRINCIPAL Y QUE LOS DEMAS HIlOS SE SIGAN EJECUTANDO
}

void saveConn(char * datos_conexion){
	int fda=0;
	//printf("\nsaveConn: datos->-%s-\n",datos);
	fda = open("datosConexion/conexiones.txt", O_RDWR | O_APPEND, 0666); //abro archivo con los datos del servidor
  if (fda == -1){
	    char* mensaje="Error al abrrir el archivo que guarda las conexiones";
	    write(1,mensaje,strlen(mensaje));
	 }
  //strncat(datos_conexion,"\n",1);
	write(fda,datos_conexion,strlen(datos_conexion));
  close(fda);
}
