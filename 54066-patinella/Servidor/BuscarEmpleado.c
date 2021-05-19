#include "BuscarEmpleado.h"

char *buscarEmpleado(char *fd_empleados_on, sem_t *sem_empleados_on, char *tipo){
  if(!strcmp(tipo,"1\n")){
    strncpy(tipo,"ventas",7);
  }else if(!strcmp(tipo,"2\n")){
    strncpy(tipo,"consultas",10);
  }else if(!strcmp(tipo,"3\n")){
    strncpy(tipo,"reclamos",9);
  }else{
    return "";
  }
  char *datos_empleado=NULL;
  char *vacio="";

  sem_wait(sem_empleados_on); // Editamos fd_empleados_on

  printf("Los datos con los que nos encontraremos son +++\n%s+++\n",fd_empleados_on);

  if(!strcmp(fd_empleados_on,vacio)){
    printf("NO HAY NINGUN EMPLEADO ON\n");
    sem_post(sem_empleados_on);
    return "";
    //aca deberia desconectar al cliente.
  } else {
    printf("Llamar funcion para obetener IP y PUERTO del empleado\n");
    printf("Los datos que cargaremos son tipo->%s y fd_empleados_on->%s \n",tipo,fd_empleados_on);
    datos_empleado=salvarEmpleado(fd_empleados_on,tipo);
    printf("Buscar empleado Los datos salvados del empleado %s\n",datos_empleado);
    if(datos_empleado!=NULL){
      //printf("Datos empleados no es nulo datos_empleado-> %s\n",datos_empleado);
      //printf("FC(buscarEmpleado): aux->\n>>>%s<<<\n",aux); //MOSTRAMOS DATOS NUEVOS
      //printf("FC(buscarEmpleado): (ANTES DE EDITAR) fd_empleados_on->\n>>>%s<<<\n",fd_empleados_on); //MOSTRAMOS DATOS VIEJOS
      char *aux=quitarEmpleado(fd_empleados_on,datos_empleado); //NO ME GUSTO COMO QUEDO
      strcpy(fd_empleados_on,aux); // REESCRIVIMOS LA MEMORIA SIN EL EMPLEADO DEMANDADO
      //printf("FC(buscarEmpleado):(DESPUES DE EDITAR) fd_empleados_on->\n>>>%s<<<\n",fd_empleados_on); //MOSTRAMOS fd_empleados_on CON LOS DATOS MODIFICADOS
      sem_post(sem_empleados_on); // Liberamos fd_empleados_on
      return datos_empleado;
    } else {
      printf("Error: datos_empleados es nulo\n");
      sem_post(sem_empleados_on);
      return "";
    }
  }
}


char *salvarEmpleado(char *fd_empleados_on, char *tipo){
  printf("hola salvar empleado");
  printf("FC salvarEmpleado llega-> tipo->-%s- y la memoria tiene fd_empleados_on->-%s-",tipo,fd_empleados_on);

  char *datos_empleado=NULL;
  datos_empleado=malloc(64);

  while(*fd_empleados_on!='\0'){ //Si no se termino el archivo entra
    if(*fd_empleados_on=='\n'){ // si encontramos un salto de linea es porque tenemos armados los datos del empleado
      if(!strcmp(tipo,salvar(datos_empleado,4))){ //si tipo es igual al tipo a tipo del empleado de la memoria entra
        return datos_empleado; //retornamos los datos del empleado encontrado
      } else { //en el caso de que no sea igual pasamos al siguiente caracter
        strcpy(datos_empleado,"");//vaciamos datos_empleado porque no coincide
        fd_empleados_on++; //siguiente caracter
      }
    } else {
      strncat(datos_empleado,&(*fd_empleados_on),1); //guardamos caracter en datos_empleado
      fd_empleados_on++;
    }
  }
  return NULL;
}

void conectarEmpleado(void * connfd,char *datos_empleado){ //CORREGIR ALGO ESTA ANDANDO MAL
  int fdSrvCmp = (int) (intptr_t) connfd; //fd para cominicar el servidor con el comprador
  printf ("hola conectarEmpleado, datos de conexion: %s\n",datos_empleado);
  char *port=NULL;
  port=malloc(64);
  char *ip=NULL;
  ip=malloc(64);
  ip=salvar(datos_empleado,1);
  printf("FC(conectarEmpleado): ip->%s\n",ip);
  port=salvar(datos_empleado,2);
  printf("FC(conectarEmpleado): puerto->%s\n",port);

  char cadena[100]; //no servira para guardar los mensajes que recivamos
  int fdEmpCmp; //esta variable contendra el socket del servidor
  struct sockaddr_in servaddr;
  char sendline[100] = "Usando el puerto 22000 \n";

  fdEmpCmp = socket(AF_INET, SOCK_STREAM, 0); //si no entendi mal con este fd comunico al empleado con el comprador
  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(atoi(port));

  inet_pton(AF_INET, ip, &(servaddr.sin_addr)); //tenemos que tener el ip con el cual nos vamos a comunicar, en este caso con nuestra propia compu
  connect(fdEmpCmp,(struct sockaddr*)&servaddr, sizeof(servaddr)); //Nos conectamos al servidor que el empleado a crado, esto deberiamos hacerlo con netcat

  char *msjBienvenida="\n\n------Sr Cliente: se ah establecido una comunicacion-----\n\n";
  char *msjDespedida="\n\n------Finalizo la comunicacion-----\n\n";

  write(fdSrvCmp,msjBienvenida,strlen(msjBienvenida)); //Mensaje Del servidor hacia el comprador

  while(!strstr(cadena, "adios") && !strstr(sendline, "adios")){
    bzero(cadena, 100);   //mismo codigo que en el servidor
    read(fdSrvCmp,sendline,strlen(sendline));//ACA DEBERIAMOS LEER EL MENSAJE QUE QUIERE ESCRIBIR EL COMPRADOR //
    write(fdEmpCmp, sendline, strlen(sendline)); //mensaje del comprador hacia el empleado
    if(!strstr(cadena, "adios")){ //si el mensaje del empleado es adios finaliza el chat
      read(fdEmpCmp, cadena, 100); // Leemos lo que el empleado envia al comprador
      write(fdSrvCmp,cadena,strlen(cadena)); // Imprimimos lo que el empleado envia al comprador
    }
  }
    write(fdSrvCmp,msjDespedida,strlen(msjDespedida));//Mensaje Del servidor hacia el comprador
}
