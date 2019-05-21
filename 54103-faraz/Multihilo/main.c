#include "multihilo.h"

#define STDOUT 1

int main (int argc, char ** argv){
	
	int port = 7000;	
	int val=1;
	int sd, bd,ad;
	int *sdc;
	struct sockaddr_in server;

	sd=socket(AF_INET,SOCK_STREAM,0);
	if (sd < 0){
		perror("fallo el socket");
		return -1;
	}
	pthread_t tid1;
	pthread_attr_t atributo;
	pthread_attr_init(&atributo);
	pthread_attr_setdetachstate(&atributo, PTHREAD_CREATE_DETACHED);

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr= INADDR_ANY;
	bzero(&(server.sin_zero),8);

	setsockopt(sd,SOL_SOCKET, SO_REUSEADDR,(void *)&val,sizeof(val));
	bd=bind (sd,(struct sockaddr*)&server, sizeof (server));
	if (bd < 0 ){
		perror("fallo el bind");
		return -1;	
	}

	listen (sd, 2000);
	while ((ad=accept(sd,NULL,NULL)) > 0){
		if (ad < 0 ){
			perror("fallo el accept");
			return -1;
		}	
		sdc=malloc(sizeof (int));
		*sdc=ad;
		if ((pthread_create(&tid1,&atributo,&hilo,(void *)sdc))!=0){
			perror("pthread");
			return -1;
		}
		//pthread_join(tid1, NULL);		
	}	
	return 0;
}
