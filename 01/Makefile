cc = gcc
CFLAGS = -g

output: main.o
	$(cc) $(CFLAGS) main.o -o main

main: main.c
	$(cc) $(CFLAGS) -c main.c

clean:
	rm -f *.o main
