CC = gcc
CFLAGS = -lrt -g -Wall
LDLIBS = -pthread
TARGETS = servidor
all: $(TARGETS)
#tp2: utils.c gestion_argumentos.c gestion_partes.c gestion_hijos.c
servidor	: menu.c servidor.c authenticate.c cargarEmpleado.c BuscarEmpleado.c Hilo.c# BuscarEmpleado.c   auxiliares.c #archivoDao.c comunicar.c menuCliente.c 
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $^ -lrt
clean:
	$(RM) $(TARGETS) *.o *~ *.txt
