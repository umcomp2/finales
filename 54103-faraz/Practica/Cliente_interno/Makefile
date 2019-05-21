GETS=cliente_interno
FLAGS= -Wall -g -pthread -lrt
CC= gcc
TARGETS=cliente_interno

cliente_interno: main.c 
	$(CC) $(FLAGS) -o $@ $^

clean:
	rm -fv $(TARGETS) *.o *~
