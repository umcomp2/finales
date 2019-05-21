GETS= bd
FLAGS= -Wall -g 
CC= gcc
TARGETS= bd

bd: main.c	
	$(CC) $(FLAGS) -o $@ $^ `mysql_config --cflags --libs`

clean:
	rm -fv $(TARGETS) *.o *~
