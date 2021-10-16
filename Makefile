CC=g++
CFLAGS=-I
DEPS=connection.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

make: p1.o p2.o 
	$(CC) -o p1 p1.o
	$(CC) -o p2 p2.o 

.PHONY:clean

clean:
	rm p1
	rm p2
	rm *.o