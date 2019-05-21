#include "multiproceso.h"


int contar(char * buffer,int num){

	int cantidad=0;

	for (int i=0; i<num; i++){
		if (buffer[i] == ';'){
			cantidad ++;
		}
	}

	return cantidad;
}
