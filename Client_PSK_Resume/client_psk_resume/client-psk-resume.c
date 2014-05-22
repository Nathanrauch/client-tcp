
/* client-psk-resume.c
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

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <errno.h>
#include    <arpa/inet.h>
#include    <signal.h>
#include    <unistd.h>
#include    <cyassl/ssl.h>  /* must include this to use cyassl security */

#define    MAXLINE 256      /* max text line length */
#define    SERV_PORT 11111  /* default port*/
#define    SA  struct sockaddr

/*
 *psk client set up.
 */
static inline unsigned int My_Psk_Client_Cb(CYASSL* ssl, const char* hint,
        char* identity, unsigned int id_max_len, unsigned char* key, 
        unsigned int key_max_len){
    (void)ssl;
    (void)hint;
    (void)key_max_len;

    /* identity is OpenSSL testing default for openssl s_client, keep same*/
    strncpy(identity, "Client_identity", id_max_len);

    /* test key n hex is 0x1a2b3c4d , in decimal 439,041,101, we're using
     * unsigned binary */
    key[0] = 26;
    key[1] = 43;
    key[2] = 60;
    key[3] = 77;

    return 4;
}

/*
 * this function will send the inputted string to the server
 */ 
void SendReceive(FILE *fp, CYASSL* ssl){
    char sendline[MAXLINE]; /* string to send to the server */
    
        fgets(sendline, MAXLINE, fp);
        /* write string to the server */
        CyaSSL_write(ssl, sendline, strlen(sendline));
}

int main(int argc, char **argv){
    
    CYASSL* ssl;
    CYASSL*         sslResume = 0;
    CYASSL_SESSION* session   = 0;
    struct sockaddr_in servaddr;;

    /* must include an ip address of this will flag */
    if (argc != 2) {
        printf("Usage: tcpClient <IPaddress>\n");
        exit(0);
    }
    
    CyaSSL_Init();  /* initialize cyaSSL */
    CYASSL_CTX* ctx;
            
    /* create and initialize CYASSL_CTX structure */
    if ((ctx = CyaSSL_CTX_new(CyaTLSv1_2_client_method())) == NULL) {
        fprintf(stderr, "SSL_CTX_new error.\n");
        exit(EXIT_FAILURE);
       }
                
     /* load ca certificates into CYASSL_CTX.
      * these will be used to verify the server we connect to */
     if (CyaSSL_CTX_load_verify_locations(ctx,"../certs/ca-cert.pem",0) != 
            SSL_SUCCESS) {
         fprintf(stderr, "Error loading ../certs/ca-cert.pem, "
                 "please check the file.\n");
         exit(EXIT_FAILURE);
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
    
    /* set up pre shared keys */
    CyaSSL_CTX_set_psk_client_callback(ctx,My_Psk_Client_Cb);

    /* attempts to make a connection on a socket */
    connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
    
    /* creat cyassl object after each tcp connct */
    if ( (ssl = CyaSSL_new(ctx)) == NULL) {
        fprintf(stderr, "CyaSSL_new error.\n");
        exit(EXIT_FAILURE);
    }

    /* associate the file descriptor with the session */
    CyaSSL_set_fd(ssl, sockfd);

     /* takes inputting string and outputs it to the server */
    SendReceive(stdin, ssl);

    /* Save the session ID to reuse */
    session   = CyaSSL_get_session(ssl);
    sslResume = CyaSSL_new(ctx);

    /* shut down Cyassl SSL */
    CyaSSL_shutdown(ssl);

    /* close connection */
    close(sockfd);

    /* cleanup */
    CyaSSL_free(ssl);
    CyaSSL_CTX_free(ctx);
    CyaSSL_Cleanup();

    /*
     * resume session, start new connection and socket   
     */

    /* start a new socket connection */
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    
    /* connect to the socket */
    connect(sock, (SA *) &servaddr, sizeof(servaddr));

    /* set the session ID to connect to the server */
    CyaSSL_set_fd(sslResume, sock);
    CyaSSL_set_session(sslResume, session);

    /* check has connect successfully */
    if (CyaSSL_connect(sslResume) != SSL_SUCCESS) {
        printf("SSL resume failed\n");
        exit(0);
    }

    /* takes inputting string and outputs it to the server */
    SendReceive(stdin, sslResume);

    /* check to see if the session id is being reused */
    if (CyaSSL_session_reused(sslResume))
        printf("reused session id\n");
    else
        printf("didn't reuse session id!!!\n");

    /* shut down cyassl ssl */
    CyaSSL_shutdown(sslResume);

    /* shut down socket */
    close(sock);

    /* clean up */
    CyaSSL_free(sslResume);   
    CyaSSL_CTX_free(ctx);
    CyaSSL_Cleanup();
    
    /* exit client */
    exit(0);
}
