CC=gcc
OBJ = client-psk.o
CFLAG=-Wall

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

client-psk: client-psk.c
	$(CC) -Wall -o client-psk client-psk.c -lm -lcyassl	

.PHONY: clean

clean:
	rm -f *.o client-psk