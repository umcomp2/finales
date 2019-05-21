#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>        
/* netbd.h es necesitada por la estructura hostent ;-) */
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#define PORT 5000         
/* El Puerto Abierto del nodo remoto */

#define MAXDATASIZE 100   

int main(int argc, char** argv)
{
   int fd, cd ,numbytes;       
   /* ficheros descriptores */
 
   struct hostent *he;         
   /* estructura que recibirá información sobre el nodo remoto */

   struct sockaddr_in server;  
   /* información sobre la dirección del servidor */

   he = gethostbyname("localhost");
   if ( he == NULL){       
      /* llamada a gethostbyname() */
      printf("gethostbyname() error\n");
      return 0;
   }

   fd = socket(AF_INET,SOCK_STREAM,0);
   if ( fd < 0 ){  
      /* llamada a socket() */
      printf("socket() error\n");
      return -1;
   }

   server.sin_family = AF_INET;
   server.sin_port = htons(PORT); 
   server.sin_addr = *((struct in_addr *)he->h_addr);  
   bzero(&(server.sin_zero),8);

   cd=connect(fd, (struct sockaddr *)&server,sizeof(struct sockaddr));

   if( cd < 0 ){ 
      /* llamada a connect() */
      printf("connect() error\n");
      return -1;
   }

  numbytes = send(fd,"aca te mando los datos\n",23,0);
   if ( numbytes < 0 ){  
      /* llamada a recv() */
      printf("Error en send() \n");
      return -1;
   }
 
   close(fd);

   return 0;
}
