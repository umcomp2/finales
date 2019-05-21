#include "multiproceso.h"

char * procesamiento (char * codigo, char* cantidad, char * server, char*user,char*password,char*database){


	char result[128]="-";
	char * resultado;
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	/*char *server = "localhost"; // agregar archivo de configuracion 
	char *user = "nfaraz";
	char *password = "1424/4";
	char *database = "computacion2";*/
	char query[2000]="SELECT laboratorio.nombre, producto.descripcion, MIN(producto_inventario.precio) AS precio FROM laboratorio JOIN inventario ON inventario.id_laboratorio = laboratorio.id JOIN producto_inventario ON producto_inventario.id_inventario = inventario.id JOIN producto ON producto.id = producto_inventario.id_producto WHERE producto.codigo = ";
	char base[100]=" AND producto_inventario.cantidad >=";
	memset(result,0,sizeof result);

	strcat(query,codigo);
	strcat(query,base);
	strcat(query,cantidad);

	conn = mysql_init(NULL);
	
	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
		write(1,"Error",5);
		return 0;
	}
	
	if (mysql_query(conn,query)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		return 0; 
	}
 
	res = mysql_use_result(conn);
	while ((row = mysql_fetch_row(res)) != NULL)	

	for(int i=0;i<3;i++){
		strcat(result,row[i]);
        	strcat(result," ");
	}
	strcat(result,"\n");
        
	resultado = strtok(result,"-");

	mysql_free_result(res);
	mysql_close(conn);

	return resultado;
}
