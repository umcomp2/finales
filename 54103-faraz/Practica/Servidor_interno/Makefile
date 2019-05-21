GETS=servidor_interno
FLAGS= -Wall -g -pthread -lrt
CC= gcc
TARGETS=servidor_interno

servidor_interno: main.c 
	$(CC) $(FLAGS) -o $@ $^

clean:
	rm -fv $(TARGETS) *.o *~
