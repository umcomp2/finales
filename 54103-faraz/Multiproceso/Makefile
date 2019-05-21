GETS=multiproceso
FLAGS= -Wall -g -pthread -lrt
CC= gcc
TARGETS=multiproceso

multiproceso: main.c master.c contar.c codigo_articulo.c cantidad_articulo.c procesamiento.c insertar.c enviar.c
	$(CC) $(FLAGS) -o $@ $^ `mysql_config --cflags --libs`

clean:
	rm -fv $(TARGETS) *.o *~
