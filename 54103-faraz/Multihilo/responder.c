#include "multihilo.h"

int responder (char * buffer){

	int fd, cd ,numbytes;
 	int port = 9000;	
	struct hostent *he;         
  	struct sockaddr_in server;

	he = gethostbyname("localhost");
   	if ( he == NULL){       
      		printf("gethostbyname() error\n");
      		return -1;
   	}

   	fd = socket(AF_INET,SOCK_STREAM,0);
   	if ( fd < 0 ){  
      		printf("socket() error\n");
      		return -1;
   	}	

  	server.sin_family = AF_INET;
  	server.sin_port = htons(port); 
   	server.sin_addr = *((struct in_addr *)he->h_addr);  
   	bzero(&(server.sin_zero),8);

   	cd=connect(fd, (struct sockaddr *)&server,sizeof(struct sockaddr));
   	if( cd < 0 ){ 
       		printf("connect() error\n");
      		return -1;
   	}

	printf("%s\n",buffer);  
	numbytes = send(fd, buffer,strlen(buffer), 0);
   	if ( numbytes < 0 ){  
      		printf("Error en send() \n");
      		return -1;
	}
	
	close(fd);
	return 1;
}
