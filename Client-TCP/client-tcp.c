/*client-tcp.c
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

#include	<sys/socket.h>	/* basic socket definitions */
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<errno.h>
#include	<arpa/inet.h>
#include	<signal.h>
#include    <unistd.h>
#define     MAXLINE     4096    /* max text line length */
#define     SERV_PORT   9877
#define SA  struct sockaddr

void 
str_Client(FILE *fp, int sockfd){
    char sendline[MAXLINE]; /*String to send to the server*/
    char recvline[MAXLINE]; /*String received from the server*/

    while(fgets(sendline, MAXLINE, fp) != NULL){
       
        /*write string to the server*/
        write(sockfd, sendline, strlen(sendline));
       
        /*Flags if the Server stopped before the client could end*/     
        if(read(sockfd, recvline, MAXLINE) == 0){
            printf("Client: Server Terminated Prematurely!\n");
            exit(0);
        }

        /*Writes the string supplied to the indicated output stream*/
        fputs(recvline, stdout);
    }
}

int main(int argc, char **argv)
{
    struct sockaddr_in servaddr;;

    /*Must include an IP address of this will flag*/
    if(argc != 2){
        printf("Usage: tcpClient <IPaddress>\n");
        exit(0);
    }

    /*Create a stream socket using TCP,Internet Protocal IPv4,
     *full-duplex stream */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); 

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

    str_Client(stdin, sockfd);

    exit(0);
}
