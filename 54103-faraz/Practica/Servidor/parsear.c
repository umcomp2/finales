#include "servidor.h"

char * parsear( char * respuesta ){
	
	char * token;	
	char * ruta = "/home/nicolas/Escritorio/Proyecto Computacion 2/";
	token = strtok(respuesta,"/");
	token = strtok(NULL,"/");
	token = strtok(token,"HTTP");
	if (!strcmp(token," ")){
		strcat(ruta,"inicio.php");
	}else{
		strcat(ruta,token);
	}
	return ruta;	
}
