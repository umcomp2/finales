#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <assert.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define BACKLONG 2000

int main (int argc , char** argv){

	int port=5000;
	int val=1;
	int sd, bd,ad,pid;
	char buff[2048];
	struct sockaddr_in server;
	//struct sockaddr_in client;	
	memset(buff,0,sizeof buff);

	sd=socket(AF_INET,SOCK_STREAM,0);
	if (sd < 0){
		perror("fallo el socket");
		return -1;
	}
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

	listen (sd, BACKLONG);

	signal(SIGCHLD,SIG_IGN);
	while ((ad=accept(sd,NULL,NULL)) > 0){
		if (ad < 0 ){
			perror("fallo el accept");
			return -1;
		}	
		pid = fork ();
		if ( pid == 0 ) {
			printf("Conectado:\n");
			read(ad,buff,sizeof buff);
			write(1,buff,sizeof buff);
			close(ad);		
			return 0;		
		}
		close (ad);
	}
	return 0;

}
