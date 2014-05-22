CC=gcc
OBJ = client-psk-nonblocking.o
CFLAG=-Wall

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

client-psk-nonblocking: client-psk-nonblocking.c
	$(CC) -Wall -o client-psk-nonblocking client-psk-nonblocking.c -lm -lcyassl	

.PHONY: clean

clean:
	rm -f *.o client-psk-nonblocking