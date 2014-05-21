CC=gcc
OBJ = client-psk-resume.o
CFLAG=-Wall

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

client-psk-resume: client-psk-resume.c
	$(CC) -Wall -o client-psk-resume client-psk-resume.c -lm -lcyassl	

.PHONY: clean

clean:
	rm -f *.o client-psk-resume
