CC=gcc
OBJ = client-tcp.o
CFLAG=-Wall

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


client-tcp: client-tcp.c
	$(CC) -Wall -o client-tcp client-tcp.c
	
.PHONY: clean

clean:
	rm -f *.o client-tcp