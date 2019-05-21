GETS= servidor
FLAGS= -Wall -g -pthread -lrt
CC= gcc
TARGETS= web

web: main.c hilo.c	
	$(CC) $(FLAGS) -o $@ $^

clean:
	rm -fv $(TARGETS) *.o *~
