GETS= multihilo
FLAGS= -Wall -g -pthread -lrt
CC= gcc
TARGETS= multihilo

multihilo: main.c hilo.c enviar.c responder.c verificar.c responder_error.c	
	$(CC) $(FLAGS) -o $@ $^

clean:
	rm -fv $(TARGETS) *.o *~
