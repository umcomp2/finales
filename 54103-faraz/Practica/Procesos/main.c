#include "contar.h"

int main (int argc , char** argv){
	
	int fd, ard, leido, numbytes, cantidad;
	pid_t pid;  	
	char buffer[2048];
	char * cod_articulo; 
	char * cant_articulo;
	char * token;
	char * resultado;
	char * usuario_id;
	char * archivo_id;
	char copy[128];
	int tuberia_hijos[2];
	int tuberia_padre[2];
	char buff[2048];
	char mensaje[20];
	char name_archivo[50]="";
	//sem_t * semaforo;
	//sem_t * semaforo2;
	memset(buff,0,sizeof buff);
	memset(mensaje,0,sizeof mensaje);

	/*semaforo = mmap(NULL, sizeof (sem_t),PROT_READ | PROT_WRITE, MAP_SHARED| MAP_ANONYMOUS, -1,0);
	sem_init(semaforo,1,0);
	semaforo2 = mmap(NULL, sizeof (sem_t),PROT_READ | PROT_WRITE, MAP_SHARED| MAP_ANONYMOUS, -1,0);
        sem_init(semaforo2,1,0);*/

	fd=open(argv[1],O_RDWR, NULL);
	if( fd == -1 ){
		perror("error al abrir el archivo");
	}
 	
	numbytes = read(fd, &buffer, sizeof(buffer));
	usuario_id = strtok(buffer,"--> ");
	strcat(name_archivo,usuario_id);
	strcat(name_archivo,".txt");

	cantidad = contar(buffer,numbytes) + 1;
	pipe(tuberia_padre);
	pipe(tuberia_hijos);

	token = strtok(NULL," ");
	token = strtok(NULL,";");
	for(int i = 0; i<cantidad; i++){
		switch(pid = fork()){
			case 0:
				if(i != cantidad-1){// hijos procesadores
					//printf("hijo_procesador\n");
					close(tuberia_hijos[0]);
					strcpy(copy,token);
					cod_articulo = codigo_articulo(token);
					cant_articulo = cantidad_articulo(copy);
					resultado = procesamiento(cod_articulo,cant_articulo);
					write(tuberia_hijos[1],resultado,strlen(resultado));
					close(tuberia_hijos[1]);
					return 0;
				}
				if(i == cantidad-1){// hijo monitor
					printf("inicio->hijo_monitor\n");
					remove(name_archivo);
					close(tuberia_padre[0]);
					close(tuberia_hijos[1]);

					ard = open(name_archivo, O_RDWR | O_CREAT, 0644);
					/*if(ard == -1){
                                                write(tuberia_padre[1],"ERROR!\n",7);
                                        }else{
                                                write(tuberia_padre[1],"OK!\n",4);
                                        }*/

					while((leido = read(tuberia_hijos[0],buff,sizeof buff))>0){
                                                write(ard,buff,leido);

      					}

					archivo_id = insertar(usuario_id);
					printf("%s\n",archivo_id);
					write(tuberia_padre[1],archivo_id,strlen(archivo_id));
				
					close(ard);
					close(tuberia_hijos[0]);
					close(tuberia_padre[1]);
					printf("fin->hijo_monitor\n");
					return 0 ;
				}
			break;
			
		}

		token = strtok(NULL,";");
	
	}
	sleep(10);
	printf("fin-> padre\n");
	close(tuberia_padre[1]);
	read(tuberia_padre[0],mensaje,sizeof mensaje);
	//write(1,mensaje,sizeof mensaje);
	printf("%s\n",mensaje);

	close(tuberia_padre[1]);
	close(fd);	 	
	return 0;
}
