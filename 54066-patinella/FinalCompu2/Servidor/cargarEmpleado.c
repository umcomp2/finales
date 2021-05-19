#include "cargarEmpleado.h"


int cargarEmpleado(char *empleado, char *fd_empleados_on, sem_t *sem_empleados_on) {
	//empleado nos llega con los siguientes datos fd/id/usuario/tipo/ip/puerto

	//---VAMOS A TRABAJAR CON LA MEMORIA COMPARTIDA---
	sem_wait(sem_empleados_on);

	//printf("+++Esperando...\n");
	//sleep(1000);
	//printf("+++Fin-espera...\n");

	printf("FC cargarEmpleado: DATOS DEL EMPLEADO CON IP Y PUERTO -> -%s-\n",empleado);
	strncat(fd_empleados_on,empleado,strlen(empleado));

	printf("++++++++++++++PRUEBA++++++++++++++++++++\n");
	printf("imprimimos el archivo, como una matriz de caracteres\n");
	for(int i=0; i<strlen(fd_empleados_on); i++){
		printf("%c",fd_empleados_on[i]);
	}
	sem_post(sem_empleados_on);

	return 1;
}

//op==1 devuelve fd  //op==2 id //op==3 usuario//op==4 tipo //op==5 ip //op==6 puerto
char *salvar(char *datos_empleado, int op){
	printf("fc:salvar, datos_empleado->%s\n",datos_empleado);
	char *basura; //NOSE POQUE MOTIVO LLEGA BASURA A LA FUNCION
	char *fd;
	char *id;
	char *usuario;
	char *tipo;
	char *ip;
	char *port;
	char letra='\0';
	int posicion=1; //1->id 2->fd 3->usuario ...
	basura=(char*)malloc(30*sizeof(char)); // GUARDAMOS LA BASURA
	fd=(char*)malloc(30*sizeof(char));
	id=(char*)malloc(30*sizeof(char));
	usuario=(char*)malloc(30*sizeof(char));
	tipo=(char*)malloc(30*sizeof(char));
	ip=(char*)malloc(30*sizeof(char));
	port=(char*)malloc(30*sizeof(char));
	free(basura); // LIBERAMOS LA BASURA

	while(*datos_empleado!='\0'){
		if(*datos_empleado=='/' || *datos_empleado=='\n'){ //viene otra palabra
			if(op==1 && posicion==1){
				return fd;
			}else if(op==2 && posicion==2){
				return id;
			}else if(op==3 && posicion==3){
				return usuario;
			}else if(op==4 && posicion==4){
				return tipo;
			}else if(op==5 && posicion==5){
				return ip;
			}else if(op==6 && posicion==6){
				return port;
			}
			datos_empleado++;
			posicion++; //pasamos a la posicion de la siguiente palabra

		}else{
			if(posicion==1){
				letra=*datos_empleado;
				strcat(fd,&letra);
				datos_empleado++;
			} else if(posicion==2){
				letra=*datos_empleado;
				strncat(id,&letra,1);
				datos_empleado++;
			} else if(posicion==3){
				letra=*datos_empleado;
				strncat(usuario,&letra,1);
				datos_empleado++;
			} else if(posicion==4){
				letra=*datos_empleado;
				strncat(tipo,&letra,1);
				datos_empleado++;
			} else if(posicion==5){
				letra=*datos_empleado;
				strncat(ip,&letra,1);
				datos_empleado++;
			} else if(posicion==6){
				letra=*datos_empleado;
				strncat(port,&letra,1);
				datos_empleado++;
			}
		}
	}
	//printf("Escogio la op %d\n",op);
	return NULL; // ocurrio un problema
}

//tener en cuenta que hay que enviar datos_empleado sin el salto de linea
char *quitarEmpleado(char *fd_empleados_on, char *datos_empleado){
	//printf("quitarEmpleado: datos_empleado->%s\n",datos_empleado);
	char *aux_fd_empleados_on=NULL;
	char *empleado=NULL;
	empleado=(char*)malloc(128*sizeof(char));
	aux_fd_empleados_on=(char*)malloc(512*sizeof(char));
	datos_empleado = strtok( datos_empleado, "\n\0" );
	while(*fd_empleados_on!='\0'){
		if(*fd_empleados_on=='\n'){ //se encontro un salto de linea por ende tenemos armados losdatos de un empleado
			//printf("++viene otro empleado\n");
			//printf("Datos a comparar %s <<-->> %s \n",empleado,datos_empleado);
			if(strcmp(empleado,datos_empleado)){
				strncat(aux_fd_empleados_on,empleado,strlen(empleado));
				strncat(aux_fd_empleados_on,"\n",2); //le agregamos el salto de linea
				strncpy(empleado,"",1); //vaciamos la variable empleado
			} else {
				//printf("Empleado encontrado y retirado de la lista\n");
				strncpy(empleado,"",1); //vaciamos la variable empleado
			}
			fd_empleados_on++;
		} else {
			//printf("++Guardamos la letra en la variable empleado\n");
			strncat(empleado,&(*fd_empleados_on),1);
			fd_empleados_on++;
		}
	}
	free(empleado);
	return aux_fd_empleados_on;
}

/*int fdSrvEmp = 0;
void levantarSocket(void *fd, char *ip, char *puerto){
	fdSrvEmp = (int) (intptr_t) fd;
	printf("hola levantarSocket fd->%d ip->%s puerto->%s\n",fdSrvEmp,ip,puerto);

	int fdEmpCmp = 0; //esta variable contendra el socket del servidor
  struct sockaddr_in servaddr;
	fdEmpCmp = socket(AF_INET, SOCK_STREAM, 0); //si no entendi mal con este fd comunico al empleado con el comprador
  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(atoi(puerto));

  inet_pton(AF_INET, ip, &(servaddr.sin_addr)); //tenemos que tener el ip con el cual nos vamos a comunicar, en este caso con nuestra propia compu
  connect(fdEmpCmp,(struct sockaddr*)&servaddr, sizeof(servaddr)); //Nos conectamos al servidor que el empleado a crado, esto deberiamos hacerlo con netcat

	char *mensaje_a_recibir=(char*)malloc(100*(sizeof(char)));//char cadena[100]; //servira para guardar los mensajes que recivamos
  char *mensaje_a_enviar=(char*)malloc(100*(sizeof(char)));//char sendline[100] = "Usando el puerto 22000 \n";

  char *msjBienvenida="\n\n------Sr Cliente: se ah establecido una comunicacion-----\n\n";
  char *msjDespedida="\n\n------Finalizo la comunicacion-----\n\n";

  write(fdSrvCmp,msjBienvenida,strlen(msjBienvenida)); //Mensaje Del servidor hacia el comprador

  while(!strstr(cadena, "adios") && !strstr(sendline, "adios")){
		strcpy(mensaje_a_recibir,"");
		strcpy(mensaje_a_enviar,"");
		//bzero(cadena, 100);   //mismo codigo que en el servidor
    mensaje_a_enviar=pedirString((void*) (intptr_t) fdSrvEmp);//read(fdSrvEmp,sendline,strlen(sendline));//ACA DEBERIAMOS LEER EL MENSAJE QUE QUIERE ESCRIBIR EL COMPRADOR //
		write(fdEmpCmp, mensaje_a_enviar, strlen(mensaje_a_enviar)); //mensaje del empleado hacia el comprador
    if(!strstr(mensaje_a_enviar, "adios")){ //si el mensaje del empleado es adios finaliza el chat
      mensaje_a_recibir=pedirString((void*) (intptr_t) fdEmpCmp) // Leemos lo que el empleado envia al comprador
      write(fdSrvCmp,mensaje_a_recibir,strlen(mensaje_a_recibir)); // Imprimimos lo que el empleado envia al comprador
    }
  }
    write(fdSrvCmp,msjDespedida,strlen(msjDespedida));//Mensaje Del servidor hacia el comprador
}
*/
/*
char *fabricarDatos(char *fd_empleados_on){
	char *vacio="";
	//char *datos_empleado=NULL; //estos datos los usamos para usarla de retorno en la fc generar datos
	//datos_empleado=(char*)malloc(128*sizeof(char));

	if(!strcmp(fd_empleados_on,vacio)){ //si el archivo esta vacio es porque no hay ningun empleado on
		char *datos_empleado="/127.0.0.2/22001";
		//strncpy(datos_empleado,"/127.0.0.2/22001",17);//datos_empleado="/127.0.0.2/22001";
		//printf("Nigun empleado usando puertos\n");
		//printf("Deberiamos usar el puerto -> 22001\n");
		return datos_empleado;
	} else {
		char *datos_empleado = (char*)malloc(128*sizeof(char));
		char *puerto_string = (char*)malloc(64*sizeof(char));
		printf("Archivo no vacio, hay puertos ocupados\n");
		//printf("Deberiamos analizar la memoria y ver que puerto podemos utilizar\n");
		//printf("Imprimimos contenido de fd_empleados_on: \n+++\n%s \n+++\n",fd_empleados_on);

		//---------------------BUSCAMOS UN PUERTO-------------------------------
		int puerto=22001;
		while(asignarPuerto(fd_empleados_on,puerto)==1){
			puerto++;
		}
		sprintf(puerto_string,"%d",puerto); //lo transformamos en string

		//---------------------------ARMAMOS EL IP------------------
		char *ip_string="/127.0.0.";
		int aux=0;
		char *letra=NULL;
		char *ip=NULL; //Defino ip, porque no puedo concatenarle la letra a ip_string ya que tiene un tamaño fijo
		ip=(char*)malloc(30*sizeof(char));
		letra=(char*)malloc(3*sizeof(char));
		aux=(puerto-22000)+1; //si cambiamos el puerto del servidor debemos cambiar esta linea
		//printf("int aux=%d\n",aux);
		sprintf(letra,"%d",aux);
		//printf("string letra=%s\n",letra );

		strncpy(ip,ip_string,strlen(ip_string));
		strncat(ip,letra,1);
		free(letra);
		//-----------------------------------------------------------
		//---------Armamos un solos string con el ip/puerto-----------
		//strncpy(datos_empleado,"/",2);
		strncat(datos_empleado,ip,strlen(ip));
		strncat(datos_empleado,"/",2);
		strncat(datos_empleado,puerto_string,strlen(puerto_string));
		//----------------------------------------------------
		free(puerto_string);
		free(ip);

		return datos_empleado;
	}
}
*/
/*
int asignarPuerto(char *fd_empleados_on, int puerto){
  char letra;
  char *aux_puerto=NULL;
	char *datos_empleado;
	aux_puerto=(char*)malloc(30*sizeof(char));
	datos_empleado=(char*)malloc(128*sizeof(char));

	while (*fd_empleados_on != '\0') {
		if(*fd_empleados_on=='\n'){
			letra=*fd_empleados_on; //
			strncat(datos_empleado,&letra,1); //agregamos el salto de linea a los datos porque la fc salvar espera los datos con el salto de linea al final
			aux_puerto=salvar(datos_empleado,6);
			printf("FC asginarPuerto -> Datos a comparar puerto %d - %s aux_puerto\n",puerto,aux_puerto);
			if(puerto==atoi(aux_puerto)){
				printf("FC asginarPuerto: Puerto %d ocupado\n",puerto);
				return 1;
			}
		}else{
			letra=*fd_empleados_on;
			strncat(datos_empleado,&letra,1);
		}
	fd_empleados_on++;
	}
  //----------------------------------------------------
  printf("Archivo-> cargarEmpleado fc->identificarPuerto msj-> El puerto %d no esta ocupado\n",puerto);
  free(aux_puerto);
  return 0;
}
*/
