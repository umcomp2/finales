
#include "menu.h"
#include "Servidor.h"
#include "Hilo.h"
char *fd_empleados_on;
sem_t *sem_empleados_on;
int main(int argc, char * argv[]){
	signal(SIGCHLD, SIG_IGN);
	int fd, connfd;
	char *ipSrvStr=serchIpPort(1);
	char *portSrvStr=serchIpPort(2);
	printf("Datos de conexion: %s/%s\n",ipSrvStr,portSrvStr);

	struct sockaddr_in procrem={};
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if  (fd < 0 ){
		perror ("socket");
		return -1;
	}

	procrem.sin_family = AF_INET;
	procrem.sin_port = htons(atoi(portSrvStr));
	procrem.sin_addr.s_addr = inet_addr(ipSrvStr);

	int a=1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,&a, sizeof a);
	if (bind(fd,(struct sockaddr *)&procrem, sizeof procrem) < 0 ) {
		perror ("bind");
		return -1;
	}

	listen(fd, 50);
	//INICIALIZAMOS VARIABLES QUE COMPARTIRAN LOS PROCESOS
	sem_empleados_on=mmap(NULL, sizeof(sem_empleados_on), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	fd_empleados_on=mmap(NULL,256,PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_SHARED,-1,0);
	sem_init(sem_empleados_on,1,1); //PARA PROTEGER EL ARCHIVO Y QUE SOLO UN PROCESO ENTRE A EDITARLO A LA VEZ
	//FIN INICIALIZACION

	int pid=0; //para guardar el id del proceso

	struct sockaddr_in direccionCliente; //new
  unsigned int tam_direccion; //new

	/*
  int cliente = accept(servidor,(struct sockaddr *)&direccionCliente,&tam_direccion);
  struct in_addr clientIp = direccionCliente.sin_addr;
  char ipStr[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &clientIp, ipStr, INET_ADDRSTRLEN);
	*/
	//new, antes connfd = accept(fd,NULL, 0))
	while ((connfd = accept(fd,(struct sockaddr *)&direccionCliente,&tam_direccion)) > 0 ){

		struct in_addr clientIp = direccionCliente.sin_addr; //new
		char ipStr[100]; //new
		bzero(ipStr,100);
	  inet_ntop(AF_INET, &clientIp, ipStr, INET_ADDRSTRLEN);
		printf("---> confd->%d ip->%s\n",connfd,ipStr); //connfd es el fd de la conexion, por este fd tiene que
		//CREAMOS EL HILO
		int retorno;//printf("hola mundo, soy el hilo main \n");
	  pthread_t tid;
		pthread_attr_t atributo;
		pthread_attr_init(&atributo);
		pthread_attr_setdetachstate(&atributo,PTHREAD_CREATE_DETACHED);

		retorno = pthread_create(&tid, &atributo, hilo, (void *) ipStr ); // CREANDO EL HILO

		if (retorno == 0){
	    printf("guardando datos de conxion\n");
	  }
			//CREAMOS UN PROCESOS PARA CADA CLIENTE QUE SE CONECTA AL SERVIDOR
		pid=fork();//responder el cliente
		if(pid==0){
			if(menu((void*) (intptr_t) connfd, ipStr,fd_empleados_on, sem_empleados_on)==0){
				printf("Cerramos conexion\n");
				close(connfd); //cerramos el fd de la conexion
				return 0;
			}
		}

	}
	kill(pid,SIGTERM); // matamos el proceso terminado
	pthread_exit(NULL); //TERMINAMOS EL HILO
	return 0;
}

char *serchIpPort(int i){

    int fda=0, leido=0;
    char *buff=malloc(sizeof(char));
    char *ipSrvStr=malloc(20*sizeof(char));
    char *portSrvStr=malloc(20*sizeof(char));
    char *aux_archivo=malloc(256*sizeof(char));
    char *palabra=malloc(32*sizeof(char));
    char letra='\0';
    int acceso=0;//acceso=1 -> recolectamos dato del ip. acceso=2 -> recolectamos datos del puerto

    strcpy(buff,"");
    strcpy(aux_archivo,"");

    fda = open("datosConexion/Server.conf", O_RDONLY, 0666); //abro archivo con los datos del servidor
    if (fda == -1){
      char* mensaje="Error al abrrir el archivo server.conf";
      write(1,mensaje,strlen(mensaje));
    }

    while ((leido = read(fda, buff, sizeof(char))) > 0) {
          strncat(aux_archivo,buff,1);
    }

    //printf("\nEl archivo contiene: \n%s \n",aux_archivo);
    free(buff);


    while(*aux_archivo!='\0'){
      if((!strcmp(palabra,"ip: ") || acceso==1) && *aux_archivo!='\n'){ //este va a entrar hasta que el primer if le saque el acceso
        letra=*aux_archivo;
        acceso=1;
        //printf("dato_ip->-%c-\n",letra);
        strncat(ipSrvStr,&letra,1);
      }else if((!strcmp(palabra,"puerto: ") || acceso==2) && *aux_archivo!='\n'){ //
        letra=*aux_archivo;
        acceso=2;
        //printf("dato_puerto->-%c-\n",letra);
        strncat(portSrvStr,&letra,1);
      }
      if(*aux_archivo!='\n'){
        strncat(palabra,&(*aux_archivo),1);
      }else{
        strcpy(palabra,"");
        acceso=0;
      }
    	*aux_archivo++;
    }

    //printf("ip-> -%s-\n puerto-> -%s-\n",ipSrvStr,portSrvStr);
    if(i==1){
      return ipSrvStr;
    }else if(i==2){
      return portSrvStr;
    }
    return "";
}
