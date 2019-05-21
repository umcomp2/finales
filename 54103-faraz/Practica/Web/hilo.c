#include "servidor.h"

pthread_mutex_t sem;

void *hilo (void* sd_conn){
	
	pthread_mutex_lock(&sem);
	int sdc, leido;
	char buffer[2048];
	memset(buffer,0,sizeof buffer);

        sdc = *((int *)sd_conn);
        free(sd_conn);
	
	while ((leido=read(sdc,buffer,sizeof buffer))>0){
		write (1,buffer,leido);
	}

	close(sdc); 
	pthread_mutex_unlock(&sem);
	pthread_exit(NULL);
}

