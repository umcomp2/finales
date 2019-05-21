#include "multiproceso.h"


char * codigo_articulo (char* token){

	char * codigo;
	codigo = strtok(token," ");
	return codigo;
}
