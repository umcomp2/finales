#include "multiproceso.h"

char * cantidad_articulo(char * token){

	char * cant;

	cant = strtok(token," ");
	cant = strtok(NULL," ");
	cant = strtok(NULL," ");
	return cant;
}
