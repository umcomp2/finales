GETS= proceso
FLAGS= -Wall -g
CC= gcc
TARGETS= proceso

proceso: main.c	contar.c codigo_articulo.c cantidad_articulo.c procesamiento.c insertar.c
	$(CC) $(FLAGS) -o $@ $^ `mysql_config --cflags --libs`

clean:
	rm -fv $(TARGETS) *.o *~
