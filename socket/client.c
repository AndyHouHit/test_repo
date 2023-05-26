/*************************************************************************
@File Name: client.c
@Desc: 
@Author: Andy-wei.hou
@Mail: wei.hou@scaleflux.com 
@Created Time: 2023年05月26日 星期五 20时21分44秒
@Log: 
************************************************************************/
/************************************************************/
/* This is a datagram socket server sample program for UNIX */
/* domain sockets. This program creates a socket and        */
/* receives data from a client.                             */
/************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_PATH "tpf_unix_sock.server"
#define CLIENT_PATH "tpf_unix_sock.client"

#define MAXMSGSIZE  256

int64_t sfx_ssd_req(void *req_msg, int req_len, void *resp_msg)
{
    int client_sock, rc, len;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;
    char buf[MAXMSGSIZE];
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));

    printf("client start\n");
    /**************************************/
    /* Create a UNIX domain stream socket */
    /**************************************/
    client_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_sock == -1) {
        perror("socket");
        return -1;
    }

    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* by using AF_UNIX for the family and */
    /* giving it a filepath to bind to.    */
    /*                                     */
    /* Unlink the file so the bind will    */
    /* succeed, then bind to that file.    */
    /***************************************/
    client_sockaddr.sun_family = AF_UNIX;
    strcpy(client_sockaddr.sun_path, CLIENT_PATH);
    len = sizeof(client_sockaddr);

    unlink(CLIENT_PATH);
    rc = bind(client_sock, (struct sockaddr *) &client_sockaddr, len);
    if (rc == -1){
        perror("bind");
        close(client_sock);
        return -1;
    }

    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* for the server socket and connect   */
    /* to it.                              */
    /***************************************/
    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, SERVER_PATH);
    rc = connect(client_sock, (struct sockaddr *) &server_sockaddr, len);
    if(rc == -1){
        perror("connet");
        close(client_sock);
        return -1;
    }

    /************************************/
    /* Copy the data to the buffer and  */
    /* send it to the server socket.    */
    /************************************/
    printf("Sending data...\n");
    rc = send(client_sock, req_msg, req_len, 0);
    if (rc == -1) {
        perror("send");
        close(client_sock);
        return -1;
    }
    else {
        printf("Data sent!\n");
    }

    /**************************************/
    /* Read the data sent from the server */
    /* and print it.                      */
    /**************************************/
    printf("Waiting to recieve data...\n");
    memset(buf, 0, sizeof(buf));
    rc = recv(client_sock, buf, sizeof(buf), 0);
    if (rc == -1) {
        perror("recv");
        close(client_sock);
        return -2;
    }
    else {
        printf("DATA RECEIVED = %s\n", buf);
        memcpy(resp_msg, buf, rc);
    }

    /******************************/
    /* Close the socket and exit. */
    /******************************/
    close(client_sock);

    return rc;
}

int main()
{
    char data[] = "Hello from Client";
    char buf[256];
    int i = 0;
    for ( i = 0; i < 10; i++)
        sfx_ssd_req(data, strlen(data), buf); 
}
