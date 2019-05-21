GETS= servidor
FLAGS= -Wall -g -pthread -lrt
CC= gcc
TARGETS= servidor

servidor: main.c hilo.c parsear.c	
	$(CC) $(FLAGS) -o $@ $^

clean:
	rm -fv $(TARGETS) *.o *~
