#include "multihilo.h"


int verificar(char * pedido){

	char * usuario_id;
	char * contenido;
	char duplicado[2048]=" ";
	strcpy(duplicado,pedido);
	usuario_id = strtok(pedido,":");
	usuario_id = strtok(NULL," ");
	if(!usuario_id){
		return -1;
	}
	contenido = strtok(duplicado,"-->");
	contenido = strtok(NULL,"->");
	if(!contenido){
		return -1;
	}
	return 1;
}
