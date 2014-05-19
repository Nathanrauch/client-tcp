/*client-psk.c
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

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<errno.h>
#include	<arpa/inet.h>
#include	<signal.h>
#include    <unistd.h>
#include    <cyassl/ssl.h>
#define     MAXLINE     256    /* max text line length */
#define     SERV_PORT   11111  /*This is the port number, default used in the tutorial*/
#define     SA  struct sockaddr
#define     INLINE inline

/*PSK client set up.*/
static 
INLINE unsigned int my_psk_client_cb(CYASSL* ssl, const char* hint,
        char* identity, unsigned int id_max_len, unsigned char* key, 
        unsigned int key_max_len){
    (void)ssl;
    (void)hint;
    (void)key_max_len;

    /*identity is OpenSSL testing default for openssl s_client, keep same*/
    strncpy(identity, "Client_identity", id_max_len);

    /* test key n hex is 0x1a2b3c4d , in decimal 439,041,101, we're using
     * unsigned binary */
    key[0] = 26;
    key[1] = 43;
    key[2] = 60;
    key[3] = 77;

    return 4;
}


void
str_Client(FILE *fp, CYASSL* ssl){
    char sendline[MAXLINE]; /*String to send to the server*/
    char recvline[MAXLINE]; /*String received from the server*/
    
    while(fgets(sendline, MAXLINE, fp) != NULL){
        
        /*write string to the server*/
        CyaSSL_write(ssl, sendline, strlen(sendline));
        
        /*Flags if the Server stopped before the client could end*/
        if(CyaSSL_read(ssl, recvline, MAXLINE) == 0){
            printf("Client: Server Terminated Prematurely!\n");
            exit(0);
        }
        /*Writes the string supplied to the indicated output stream*/
       fputs(recvline, stdout);
       printf("\n");
        exit(0);
    }
}

int
main(int argc, char **argv){
    struct sockaddr_in servaddr;;
    
    /* define a signal handler for when the user closes the program 
              with Ctrl-C */
     

    /*Must include an IP address of this will flag*/
    if(argc != 2){
        printf("Usage: tcpClient <IPaddress>\n");
        exit(0);
    }
    
    CyaSSL_Init();  // Initialize CyaSSL
    CYASSL_CTX* ctx;
            
    /* Create and initialize CYASSL_CTX structure */
    if ( (ctx = CyaSSL_CTX_new(CyaTLSv1_2_client_method())) == NULL){
        fprintf(stderr, "SSL_CTX_new error.\n");
        exit(EXIT_FAILURE);
       }
                
   /* Load CA certificates into CYASSL_CTX.
    * These will be used to verify the server we connect to */
     if (CyaSSL_CTX_load_verify_locations(ctx,"../certs/ca-cert.pem",0) != 
            SSL_SUCCESS) {
         fprintf(stderr, "Error loading ../certs/ca-cert.pem, "
                 "please check the file.\n");
         exit(EXIT_FAILURE);
      }

    /*Create a stream socket using TCP,Internet Protocal IPv4,
     *full-duplex stream */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    
    CyaSSL_CTX_set_psk_client_callback(ctx,my_psk_client_cb);
    
    connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
    
    CYASSL* ssl;

    if( (ssl= CyaSSL_new(ctx)) == NULL){
        fprintf(stderr, "CyaSSL_new error.\n");
        exit(EXIT_FAILURE);
    }

    CyaSSL_set_fd(ssl, sockfd);

    str_Client(stdin, ssl);
    CyaSSL_free(ssl);
    CyaSSL_CTX_free(ctx);
    CyaSSL_Cleanup();
    exit(0);
}
