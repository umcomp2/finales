#include "multiproceso.h"

#define sms(a,b)(a == 1) ? b : (void)0

#define BACKLONG 2000


int main (int argc , char** argv){

	int port=5000;
	int val=1,a;
	int sd, bd,ad,pid,opcion,config;
	char buffer[2048];
	char param[2048];
	int numbytes;
	struct sockaddr_in server;
	char * servidor;
	char * user;
       	char * password;
       	char * database;
	//sem_t * semaforo;
	memset(param,0,sizeof param);

	/*semaforo= mmap(NULL, sizeof (sem_t),PROT_READ | PROT_WRITE, MAP_SHARED| MAP_ANONYMOUS, -1,0);
	sem_init(semaforo,1,0);*/

	while (1){
		opcion = getopt (argc, argv, "f:pv");
		switch(opcion){
			case 'f':
				config = open(argv[2],O_RDWR,0644);
				if (config < 0){
					perror("Error al abrir archivo de base de datos.");
                			return -1;
				}
				read(config,param,sizeof param);	
				servidor = strtok(param,"; ");
				user = strtok(NULL,"; ");
				password = strtok(NULL,"; ");
				database = strtok(NULL,"; ");
				a=1;
			break;
			case 'p':
				a = 1;
                                servidor = "localhost";
                                user = "nfaraz";
                                password = "1424/4";
                                database = "computacion2";
				pid = fork();
				if(pid == 0){
					if (execv("/home/nicolas/Escritorio/Proyecto/Proyecto/Multihilo/./multihilo",argv)<0){
						perror("Fallo execv");
						return -1;
					}	
					return 0;
				}
			break;
			case 'v':
				a = 1;
				servidor = "localhost";
                                user = "nfaraz";
                                password = "1424/4";
                                database = "computacion2";
	
			break;
			default:
				a = 0;
				servidor = "localhost";
                                user = "nfaraz";
                                password = "1424/4";
                                database = "computacion2";
			break;
		}
		break;
	}

	sms(a,printf("server:%s \n",servidor));
	sms(a,printf("usuario:%s \n",user));
	sms(a,printf("database:%s \n",database));

	sd=socket(AF_INET,SOCK_STREAM,0);
	if (sd < 0){
		perror("fallo el socket");
		return 0;
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr= INADDR_ANY;
	bzero(&(server.sin_zero),8);

	setsockopt(sd,SOL_SOCKET, SO_REUSEADDR,(void *)&val,sizeof(val));
	bd=bind (sd,(struct sockaddr*)&server, sizeof (server));
	if (bd < 0 ){
		perror("fallo el bind");
		return 0;	
	}

	listen (sd, BACKLONG);
	signal(SIGCHLD,SIG_IGN);

	while ((ad=accept(sd,NULL,NULL)) > 0){
		if (ad < 0 ){
			perror("fallo el accept");
			return 0;
		}	
		pid = fork ();
		if ( pid == 0 ) {
			sms(a,printf("Conectado:\n"));
			sms(a,printf("Padre %d\n",getpid()));
			numbytes = read(ad, &buffer, sizeof(buffer));
			sms(a,printf("%s\n",buffer));
			if(master(numbytes,a,buffer,servidor,user,password,database)<0){
				printf("Error master\n");
			}
			return 0;		
		}
		close (ad);
	}
	return 0;
}
