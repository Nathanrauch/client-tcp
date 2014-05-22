examples-client-tcp-psk-nonblocking
===================================

TCP client PSK (pre-shared keys) nonblocking example for wolfSSL

When configuring Cyassl

./configure --enable-psk

make

make install

Now in the directory of Client_PSK_NONBLOCKING/client-nonblocking
To compile the c code run

make

The clients default port is 11111.

To start the program, on the terminal while in the same directory that 
client-tcp.c is in type ./client-tcp 127.0.0.1 or the ip address you want 
to connect to. After starting the client and connecting,there will be a 
prompt for a string to send to the server. This program will connect, send 
a string to the server and then discount but save the session id. It will
then go and connect again using the session id so it doesn't have to go
through the handshake with the server again. 

What is nonblocking?
=====================================================================================
Applications wishing to communicate securely to one another may establish 
a secure connection. Each application opens a socket and attempts to establish 
an SSL connection. After an SSL connection has been established, the applications 
may now use the socket to exchange data securely. The default (blocking) mode of a 
socket requires an application attempting to read or write to the socket to block 
until all expected data has been received. This blocking may not be desirable since 
no other processing may occur while the application is waiting for a read or write 
to complete. One solution to this problem is the use of non-blocking sockets.

When a socket is setup as non-blocking, reads and writes to the socket do not cause
the application to block and wait. Instead the read or write function will read/write 
only the data currently available (if any). If the entire read/write is not completed, 
a status indicator is returned. The application may retry the read/write later.
======================================================================================
