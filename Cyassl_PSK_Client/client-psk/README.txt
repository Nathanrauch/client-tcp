examples-client-tcp-psk
========

TCP client using PSK (pre-shared keys) example for wolfSSL

When configuring Cyassl

./configure --enable-psk

make

make install

Now in the directory of Cyassl_PSK_Client
To compile the c code run

make

The clients default port is 11111.

To start the program, on the terminal while in the same directory that 
client-tcp.c is in type ./client-tcp 127.0.0.1 or the ip address you want 
to connect to. After starting the client and connecting,there will be a 
prompt for a string to send to the server. 