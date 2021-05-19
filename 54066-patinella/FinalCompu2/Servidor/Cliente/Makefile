CC = gcc
CFLAGS = -lrt -g -Wall
LDLIBS = -pthread
TARGETS = cliente
all: $(TARGETS)
#tp2: utils.c gestion_argumentos.c gestion_partes.c gestion_hijos.c
cliente	: Cliente.c menuComprador.c authenticate.c SockEmp.c SockCmp.c#socketEmpleado.c#socketEmpleado.c archivoDao.c comunicar.c menuCliente.c
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $^ -lrt
clean:
	$(RM) $(TARGETS) *.o *~ *.txt
