
/* client-tcp.c
 *
 * Copyright (C) 2006-2014 wolfSSL Inc.
 *
 * This file is part of CyaSSL.
 *
 * CyaSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * CyaSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
 * USA */

#include    <sys/socket.h>	/* basic socket definitions */
#include    <netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <errno.h>
#include    <arpa/inet.h>
#include    <signal.h>
#include    <unistd.h>

#define    MAXLINE     256    /* max text line length */
#define    SERV_PORT   11111
#define    SA  struct sockaddr

/*
 * this function will send the inputted string to the server and then 
 * recieve the string from the server outputing it to the termial
 */ 
void SendReceive(FILE *fp, int sockfd){
    char sendline[MAXLINE]; /* string to send to the server */
    char recvline[MAXLINE]; /* string received from the server */

    while (fgets(sendline, MAXLINE, fp) != NULL) {
       
        /* write string to the server */
        write(sockfd, sendline, strlen(sendline));
       
        /* flags if the server stopped before the client could end */     
        if (read(sockfd, recvline, MAXLINE) == 0) {
            printf("Client: Server Terminated Prematurely!\n");
            exit(0);
        }

        /* writes the string supplied to the indicated output stream */
        fputs(recvline, stdout);
    }
}

int main(int argc, char **argv){
    
    struct sockaddr_in servaddr;;

    /* must include an ip address or this will flag */
    if (argc != 2) {
        printf("Usage: tcpClient <IPaddress>\n");
        exit(0);
    }

    /* create a stream socket using tcp,internet protocal IPv4,
     * full-duplex stream */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); 

    /* places n zero-valued bytes in the address servaddr */
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT); 

    /* converts IPv4 addresses from text to binary form */
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    /* attempts to make a connection on a socket */
    connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

    /* takes inputting string and outputs it to the server */
    SendReceive(stdin, sockfd);

    /* end client */
    exit(0);
}
