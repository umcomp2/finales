#include "multiproceso.h"

char * insertar (char * usuario_id, char * server, char * user, char * password, char * database){
	
	int fd;
       	char  archivo[10] = "-";
	char * archivo_id;
	char * id;
	char name_archivo[50]="";
	char buffer[2048];
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	/*char *server = "localhost";
	char *user = "nfaraz";
	char *password = "1424/4";
	char *database = "computacion2";*/
	char blob[2048];
	char id2[200] = "SELECT MAX(id)+1 FROM archivo_pedido;";
	char query[1000] = "INSERT INTO `archivo_pedido` (`id`, `usuario_id`, `contenido`) VALUES ('";
	char query2[1000] = "','";
	//char query[1024] = "INSERT INTO `archivo_pedido` (`id`, `usuario_id`, `contenido`) VALUES ('NULL','";
	char select[1024] = "SELECT MAX(id) FROM archivo_pedido WHERE usuario_id = ";

	strcat(name_archivo,usuario_id);
        strcat(name_archivo,".txt");

	fd = open(name_archivo,O_RDWR,NULL);
	if(fd == -1){
		write(1,"Error\n",7);
		return 0;
	}
	read(fd,buffer,sizeof buffer);

	id = strtok(usuario_id,"usuario_id:");

	conn = mysql_init(NULL);
	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
		write(1,"Error",5);
		return 0;
	}
	
	if (mysql_query(conn,id2)) {
                fprintf(stderr, "%s\n", mysql_error(conn));
                return 0;
        }
        res = mysql_use_result(conn);
        while ((row = mysql_fetch_row(res)) != NULL)
        strcat(query,row[0]);
        strcat(query,query2);



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
//	printf("%s\n",archivo_id);

	close(fd);
	mysql_close(conn);
	
	return archivo_id ; 
}
