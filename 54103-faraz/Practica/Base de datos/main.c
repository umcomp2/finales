#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char** argv){
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char *server = "localhost";
	char *user = "nfaraz";
	char *password = "1424/4";
	char *database = "computacion2";
	conn = mysql_init(NULL);
	/* Conectarse a la base de datos */
	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
		write(1,"Error",5);
		return 0;
	}
	/* enviar la consulta SQL */
	if (mysql_query(conn, "SELECT * FROM usuario")) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		return 0; 
	}
 
	res = mysql_use_result(conn);
	/* imprimir los nombres de las tablas */
	while ((row = mysql_fetch_row(res)) != NULL)
		printf("%s\t%s\t%s\t%s\t%s \n", row[0],row[1],row[2],row[3],row[4]);
	
	/* liberar los recursos y cerrar la conexion */
	mysql_free_result(res);
	mysql_close(conn);
	return 0;
}
