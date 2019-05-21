#include "multiproceso.h"

#define sms(a,b)(a == 1) ? b : (void)0

int master (int numbytes, int a, char * buffer, char * servidor, char * user, char * password, char * database){

	int ard;
	char buff[2048];
	int  cantidad;
	char * cod_articulo;
	char * cant_articulo;
	char * token;
	char * resultado;
	char * usuario_id;
	char * archivo_id;
	int enviado;
	char copy[128];
	char name_archivo[50]="";
	int tuberia_hijos[2];
	int tuberia_padre[2];
	int leido;
	char mensaje[20];
	//sem_t * semaforo;
	memset(buff,0,sizeof buff);
	memset(mensaje,0,sizeof mensaje);

	usuario_id = strtok(buffer,"--> ");
	strcat(name_archivo,usuario_id);
	strcat(name_archivo,".txt");
	cantidad = contar(buffer,numbytes) + 1;
	pipe(tuberia_padre);
	pipe(tuberia_hijos);
	token = strtok(NULL," ");
	token = strtok(NULL,";");
	for(int i = 0; i<cantidad; i++){
		switch(fork()){
			case 0:
				if(i != cantidad-1){// hijos procesadores
					close(tuberia_hijos[0]);
					strcpy(copy,token);
					cod_articulo = codigo_articulo(token);
					cant_articulo = cantidad_articulo(copy);
					sms(a,printf("Hijo %d --> codigo: %s cantidad:  %s\n",getpid(),cod_articulo,cant_articulo));
					resultado = procesamiento(cod_articulo,cant_articulo,servidor,user,password,database);
					write(tuberia_hijos[1],resultado,strlen(resultado));
					close(tuberia_hijos[1]);
					//sem_post(semaforo);
					return 0;
				}else{// hijo monitor
					sms(a,printf("Hijo monitor %d \n",getpid()));
					close(tuberia_padre[0]);
					close(tuberia_hijos[1]);
					remove(name_archivo);
					ard = open(name_archivo, O_CREAT| O_RDWR, 0644);
						if(ard == -1){
                                                	write(tuberia_padre[1],"ERROR!\n",7);
							perror("Error archivo\n");
							return -1;
                                        	}
						while((leido = read(tuberia_hijos[0],buff,sizeof buff))>0){
							write(ard,buff,leido);
							sms(a,write(1,buff,leido));
						}
						archivo_id = insertar(usuario_id,servidor,user,password,database);
						write(tuberia_padre[1],archivo_id,sizeof archivo_id);
						sms(a,printf("Archivo_id --> %s\n",archivo_id));
						//sem_post(semaforo);
						close(ard);
                                                close(tuberia_hijos[0]);
                                                close(tuberia_padre[1]);
						return 0 ;
			}
			break;
		}
                              	//sem_wait(semaforo);
	token = strtok(NULL,";");
	}
	close(tuberia_padre[1]);
	close(tuberia_hijos[1]);
	close(tuberia_hijos[0]);
	read(tuberia_padre[0],mensaje,sizeof mensaje);
	close(tuberia_padre[0]);
	enviado = enviar(mensaje);
	if(enviado > 0){
		sms(a,printf("enviado\n"));
	}else{
		sms(a,printf("error\n"));
	}
	
	return 1;
}
