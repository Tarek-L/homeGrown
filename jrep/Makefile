CC = gcc
CFLAGS = -Wall -Wextra -g

jrep: jrep.o
	$(CC) $(CFLAGS) -o jrep jrep.o

jrep.o: jrep.c
	$(CC) $(CFLAGS) -c jrep.c

clean:
	rm -fr jrep.o jrep

run:
	./jrep

