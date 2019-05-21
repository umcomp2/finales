#include "contar.h"

char * insertar (char * usuario_id){
	
	int fd;
       	char  archivo[10] = "-";
	char * archivo_id;
	char * id;
	char name_archivo[50]="";
	char buffer[2048];
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char *server = "localhost";
	char *user = "nfaraz";
	char *password = "1424/4";
	char *database = "computacion2";
	char blob[2048];
	char query[1024] = "INSERT INTO `archivo_pedido` (`id`, `usuario_id`, `contenido`) VALUES ('NULL','";
	char select[1024] = "SELECT MAX(id) FROM archivo_pedido WHERE usuario_id = ";

	strcat(name_archivo,usuario_id);
        strcat(name_archivo,".txt");

	fd = open(name_archivo,O_RDWR,NULL);
	if(fd == -1){
		write(1,"Error\n",7);
	}
	read(fd,buffer,sizeof buffer);

	id = strtok(usuario_id,"usuario_id:");

	conn = mysql_init(NULL);
	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
		write(1,"Error",5);
		return 0;
	}

	mysql_real_escape_string(conn, blob, buffer, strlen(buffer));

	strcat(query,id);
	strcat(query,"', '");
	strcat(query,blob);
	strcat(query,"');");
	
	
	if (mysql_query(conn,query)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		return 0; 
	}
 
	strcat(select,id);

	if (mysql_query(conn,select)) {
                fprintf(stderr, "%s\n", mysql_error(conn));
                return 0;
        }

	res = mysql_use_result(conn);
	while ((row = mysql_fetch_row(res)) != NULL)	
	strcat(archivo, row[0]);
	archivo_id = strtok(archivo,"-");

	close(fd);
	mysql_close(conn);


	return archivo_id ; 
}
