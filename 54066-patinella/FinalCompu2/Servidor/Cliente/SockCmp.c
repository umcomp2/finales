#include "SockCmp.h"



int conectar(void *fd, char *datos_empleado){
  //int fdSrv=0;
  //fdSrv = (int) (intptr_t) fd;
  printf("FC conectarEmpleado -> -%s-",datos_empleado);
  char *ipStr=salvar(datos_empleado,5);
  char *portStr=salvar(datos_empleado,6);
  char *usuario=salvar(datos_empleado,3);


  printf("Datos de conexion ipStr->%s portStr->%s",ipStr,portStr);

  struct sockaddr_in direccionServidor;

  int fdEmp = socket(AF_INET, SOCK_STREAM, 0);

  memset(&direccionServidor,0,sizeof(direccionServidor));

  direccionServidor.sin_family = AF_INET;
  direccionServidor.sin_addr.s_addr = inet_addr(ipStr); // direccion a la que se va a conectar
  direccionServidor.sin_port = htons(atoi(portStr));

  char buffSend[100];//buffer de escritura;
  char buffRecived[100];//buffer de recepcion;
  bzero(buffSend,100);
  bzero(buffRecived,100);

  if(connect(fdEmp, (void*)&direccionServidor, sizeof(direccionServidor)) != 0){
    perror("No se pudo conectar");
    return 1;
  }
  strncpy(buffSend,"Hola ",6);
  strncat(buffSend,usuario,strlen(usuario));
  strncat(buffSend," ya estoy conectado...\n",24);
  write(fdEmp,buffSend,strlen(buffSend)); //enviamos el primer saludo
  bzero(buffSend,100);
  while(!strstr(buffRecived, "adios") && !strstr(buffSend, "adios")){
    bzero(buffRecived,100);
    read(fdEmp,buffRecived,sizeof(buffRecived)); //leemos el mensaje que envia el empleado
    write(1,buffRecived,strlen(buffRecived)); //mostramos en la terminal del cliente el mensaje

    if(!strstr(buffRecived, "adios")){
      bzero(buffSend,100);
      read(1,buffSend,sizeof(buffSend));// leemos la respuesta del cliente
      write(fdEmp,buffSend,strlen(buffSend)); //enviamos la respuesta al empleado
    }
  }
  printf("--------------Chat finalizado--------------\n");

  close(fdEmp);
  return 0;
}

char *salvar(char *datos_empleado, int op){
	//printf("fc:salvar, datos_empleado->%s\n",datos_empleado);
	//char *basura; //NOSE POQUE MOTIVO LLEGA BASURA A LA FUNCION
	char *fd;
	char *id;
	char *usuario;
	char *tipo;
	char *ip;
	char *port;
	char letra='\0';
	int posicion=1; //1->id 2->fd 3->usuario ...
	//basura=(char*)malloc(30*sizeof(char)); // GUARDAMOS LA BASURA
	fd=(char*)malloc(30*sizeof(char));
	id=(char*)malloc(30*sizeof(char));
	usuario=(char*)malloc(30*sizeof(char));
	tipo=(char*)malloc(30*sizeof(char));
	ip=(char*)malloc(30*sizeof(char));
	port=(char*)malloc(30*sizeof(char));
	//free(basura); // LIBERAMOS LA BASURA
  bool salir=true;
	while(salir){
    //printf("->%s\n",datos_empleado);
		if(*datos_empleado=='/' || *datos_empleado=='\n' || *datos_empleado=='\0'){ //viene otra palabra
      //printf("op=%d posicion=%d\n",op,posicion);
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
        if(*datos_empleado=='\0'){
          salir=false;
        }
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
	return NULL; //a ocurriedo un problema
}

void porobarSalvar(){
  char *empleadoPreuba="4/5/manuel/ventas/127.0.0.1/22000\n";
  char *fdStr=salvar(empleadoPreuba,1);
  char *idStr=salvar(empleadoPreuba,2);
  char *usuarioStr=salvar(empleadoPreuba,3);
  char *tipoStr=salvar(empleadoPreuba,4);
  char *ipStr=salvar(empleadoPreuba,5);
  char *portStr=salvar(empleadoPreuba,6);
  printf("Datos de empleado: \n fd=-%s- \n id=-%s- \n usuario=-%s- \n tipo=-%s- \n ip=-%s- \n port=-%s- \n",fdStr,idStr,usuarioStr,tipoStr,ipStr,portStr);
}
