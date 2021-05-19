#include "menu.h"
#include "cargarEmpleado.h"
#include "BuscarEmpleado.h"
void* menu(void *connfd, char ipStr[] ,char *fd_empleados_on, sem_t *sem_empleados_on) {
	int fdClient=0;
	fdClient= (int) (intptr_t) connfd;

	char buffSend[100];//buffer de escritura;
	char buffRecived[100];//buffer de recepcion;
	bzero(buffSend,100); //limpiamos
	bzero(buffRecived, 100); //limpiamos

	while(1){
		printf("Esperando comando...\n");

		read(fdClient, buffRecived, sizeof(buffRecived)); // Esperamos por alguna peticion
		printf("comando recibido-->-%s- tam->%ld\n",buffRecived,strlen(buffRecived));

		if(!strcmp(buffRecived,"") || !strcmp(buffRecived,"\n")){
			return 0;
		}else if(!strcmp(buffRecived,"1\n")){ //El cliente es un Empleado
			//printf("Llamamos a la funcion para pedirle los datos al empleado\n");
			char *retorno=authenticate(connfd); //si la autentucacion falla retorno="" sino retorno=fd/id/usurio/tipo
			if(!strcmp(retorno,"")){
				printf("Erro al loguearse\n");
				write(fdClient,"0\n",2);

				break; //salimos del segundo if
				break; //salimos del primer if para que no retorne 0
			}else{
				printf("Se autentico correctamente %s , deberiamos enviarle el ip\n",retorno);
				write(fdClient,ipStr,strlen(ipStr)); //ENVIAMOS EL IP AL EMPLEADO
				//-----------------REARMAMOS UN POCO LOS DATOS-----------------------
				strncat(retorno,"/",2);
				strncat(retorno,ipStr,strlen(ipStr)); //ahora retorno=fd/id/usuario/tipo/ipStr... nos faltaria el puerto para completar todos los datos
				strncat(retorno,"/",2);
				//--------------------------------------------------------------------
				printf("Esperando puerto...\n");
				//----------------ESPERAMOS POR EL DATO "PUERTO"----------------------
				bzero(buffRecived,100);
				read(fdClient,buffRecived,sizeof(buffRecived));
				printf("El puerto que nos masnda el empleado es -> -%s-\n",buffRecived);
				strncat(retorno,buffRecived,strlen(buffRecived));
				bzero(buffRecived,100);
				printf("Datos que vamos a pegar en el archivo -> -%s- \n",retorno);
				//--------------------------------------------------------------------
				//-----------------CARGAMOS AL EMPLEADO-------------------------------
				cargarEmpleado(retorno,fd_empleados_on,sem_empleados_on);
				//--------------------------------------------------------------------
				//DEBERIAMOS LLAMAR A LA FC QUE CARGUE LOS DATOS DEL EMPLEADO
			}
		}else if(!strcmp(buffRecived,"2\n")){ // El cliente es un comprador
			printf("Llamamos a la funcion que le envie los datos al comprador\n");
			printf("Esperamos por el tipo de empleado que demanda el cliente\n");
			bzero(buffRecived,100);
			read(fdClient,buffRecived,sizeof(buffRecived));//esperamos recibir el tipo de empleado que quiere el cliente
			printf("El cliente quiere un empleado del tipo->-%s-\n",buffRecived);

			char *tipo=(char*)malloc(15*sizeof(char));
			strncpy(tipo,buffRecived,strlen(buffRecived));
			bzero(buffRecived,100);


			char *datos_empleado=buscarEmpleado(fd_empleados_on,sem_empleados_on,tipo); //BUSCAMOS AL EMPLEADO
			printf("\nLos datos que tenemos que enviarle al cliente -> -%s-\n",datos_empleado);
			//printf("\nLos datos que tenemos que enviarle al cliente -> %s\n",datos_empleado);
			//printf("\nLos datos que tenemos que enviarle al cliente -> %s\n",datos_empleado);

			if(!strcmp(datos_empleado,"")){
				bzero(buffSend,100);
				strncpy(buffSend,"\n",2);
				printf("Vamos a enviar -> buffSend->-%s-",buffSend);
				write(fdClient,buffSend,strlen(buffSend)); //Enviamos los datos del empleado o \n en su defecto
				bzero(buffSend,100); //vaciamos el buffer
			} else {
				printf("Datos del empleado que enviamos al cliente -> -%s-",datos_empleado);
				bzero(buffSend,100);
				strncpy(buffSend,datos_empleado,strlen(datos_empleado)); //
				printf("Vamos a enviar -> buffSend->-%s-",buffSend);
				write(fdClient,buffSend,strlen(buffSend)); //Enviamos los datos del empleado o \n en su defecto
				bzero(buffSend,100); //vaciamos el buffer
			}

		}


		bzero(buffRecived, 100); //vaciamos buffer

	}
	return 0;
}
