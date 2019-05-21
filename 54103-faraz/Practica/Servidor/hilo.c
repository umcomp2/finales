#include "servidor.h"

pthread_mutex_t sem;

void *hilo (void* sd_conn){
	
	pthread_mutex_lock(&sem);
	int sdc, leido, file;
	char respuesta[2048];
	char * token;
	char ruta[100]="/home/nicolas/Escritorio/Proyecto Computacion 2/Servidor/";
	char buffer[2048];
	memset(buffer,0,sizeof buffer);

        sdc = *((int *)sd_conn);
        free(sd_conn);
	
	read(sdc,respuesta,sizeof respuesta);
        token = strtok(respuesta,"/");
        token = strtok(NULL,"/");
        token = strtok(token,"HTTP");
	if (!strcmp(token," ")){
                strcat(ruta,"inicio.php");
        }else if(!strcmp(token,"favicon.ico ")){
		close(sdc);
        	pthread_mutex_unlock(&sem);
        	pthread_exit(NULL);
		return 0;
	}else{
		token = strtok(token," ");
                strcat(ruta,token);
        }

	write(sdc,"HTTP/1.1 200 ok\n",16);
	write(sdc,"Content-Type: text/html; charset=UTF-8\n\n",40);
	file = open(ruta,O_RDONLY,NULL);
	 if (file < 0){
                write (sdc,"Error, todo mal\n",16);
                close(file);
        	close(sdc);
        	pthread_mutex_unlock(&sem);
        	pthread_exit(NULL);
		return 0;
        }
	while ((leido=read(file,buffer,sizeof buffer))>0){
		write (sdc,buffer,leido);
	}
	close(file);
	close(sdc); 
	pthread_mutex_unlock(&sem);
	pthread_exit(NULL);
}

