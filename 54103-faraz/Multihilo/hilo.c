#include "multihilo.h"

pthread_mutex_t sem;

void *hilo (void* sd_conn){
	
	pthread_mutex_lock(&sem);
	int sdc, leido;
	char buffer[2048];
	char * pedido;
	char pedido_verificado[2048] = " ";
	char * respuesta;
	char * control;
	int check;
	int sid;
	memset(buffer,0,sizeof buffer);

        sdc = *((int *)sd_conn);
        free(sd_conn);
	
	leido = read(sdc,buffer,sizeof buffer);
	write (1,buffer,leido);
	write (1,"\n",1);
	if (!strcmp(buffer," ")){
		responder_error();
                close(sdc);
                pthread_mutex_unlock(&sem);
                pthread_exit(NULL);
	}
	
	control = strtok(buffer,":/ ");	

	if(!strcmp(control,"0")){
		pedido = strtok(NULL,"/");
		if(!pedido){
			responder_error();
                        close(sdc);
                        pthread_mutex_unlock(&sem);
                        pthread_exit(NULL);
		}	
		strcpy(pedido_verificado,pedido);
		check = verificar(pedido);
		if(check < 1){
			responder_error();
			close(sdc);
                	pthread_mutex_unlock(&sem);
                	pthread_exit(NULL);
		}
        	sid = enviar(pedido_verificado);
        	if(sid < 0 ){
			responder_error();
                	close(sdc);
                	pthread_mutex_unlock(&sem);
                	pthread_exit(NULL);
        	}
	}else if(!strcmp(control,"1")){
	       	respuesta = strtok(NULL,"/");
		if(!strcmp(respuesta," ")){
			responder_error();
                        close(sdc);
                        pthread_mutex_unlock(&sem);
                        pthread_exit(NULL);
		}
	       	sid = responder(respuesta);
                if(sid < 0 ){
			responder_error();
                        close(sdc);
                        pthread_mutex_unlock(&sem);
                        pthread_exit(NULL);
                }
	
	}else{		
		responder_error();
		printf("Error\n");
		close(sdc);
        	pthread_mutex_unlock(&sem);
        	pthread_exit(NULL);
	}
	
	close(sdc); 
	pthread_mutex_unlock(&sem);
	pthread_exit(NULL);
}

