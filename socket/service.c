/***********************************************************************
@File Name: service.c
@Desc: 
@Author: Andy-wei.hou
@Mail: wei.hou@scaleflux.com 
@Created Time: 2023年05月26日 星期五 20时21分29秒
@Log: 
************************************************************************/
/************************************************************/
/* This is a stream socket server sample program for UNIX   */
/* domain sockets. This program listens for a connection    */
/* from a client program, accepts it, reads data from the   */
/* client, then sends data back to connected UNIX socket.   */
/************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>

#define SERVER_PATH  "tpf_unix_sock.server"
#define CLIENT_PATH "tpf_unix_sock.client"
#define DATA "Hello from server"

#define MAXSOCKETFD 20
#define MAXMSGSIZE  256


static int64_t sfx_ssd_proc(void *req_msg, int len, void *resp_msg)
{
    static int f = 0;
    memset(resp_msg, 0, MAXMSGSIZE);
    sprintf(resp_msg, "%s: %d", DATA, f++);

    return strlen(resp_msg);
}

void *sfx_ssd_srv(void *args) {
    int server_sock, client_sock, connected_fd[MAXSOCKETFD] = { 0 }, len, rc;
    int bytes_rec = 0;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;     
    fd_set readfds;
    int backlog = 10;
    char recv_buf[MAXMSGSIZE];
    char resp_buf[MAXMSGSIZE];


    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));

    printf("%s thread start\n", __func__);
    
    /**************************************/
    /* Create a UNIX domain stream socket */
    /**************************************/
    server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sock == -1){
        perror("socket");
        exit(1);
    }
    
    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* by using AF_UNIX for the family and */
    /* giving it a filepath to bind to.    */
    /*                                     */
    /* Unlink the file so the bind will    */
    /* succeed, then bind to that file.    */
    /***************************************/
    server_sockaddr.sun_family = AF_UNIX;   
    strcpy(server_sockaddr.sun_path, SERVER_PATH); 
    len = sizeof(server_sockaddr);
    
    unlink(SERVER_PATH);
    rc = bind(server_sock, (struct sockaddr *) &server_sockaddr, len);
    if (rc == -1){
        perror("bind");
        close(server_sock);
        exit(1);
    }
    
    /*********************************/
    /* Listen for any client sockets */
    /*********************************/
    rc = listen(server_sock, backlog);
    if (rc == -1){ 
        perror("listen");
        close(server_sock);
        exit(1);
    }
    printf("socket %d listening...\n", server_sock);
    
    /***********************************/
    /*   select to polling recv requst */ 
    /***********************************/
    while (1) {
        int i = 0;
        int max_fd = 0;
        FD_ZERO(&readfds);
        connected_fd[0] = server_sock;

        /*polling the connected fds too*/
        for (i = 0; i < MAXSOCKETFD; i++) {
            if (connected_fd[i]) {
                FD_SET(connected_fd[i], &readfds);
                if (max_fd < connected_fd[i]) {
                    max_fd = connected_fd[i];
                }
            }
        }

        printf("select\n");
        switch (select(max_fd + 1, &readfds, NULL, NULL, NULL)) {
            case 0:
                printf("timeout");
                break;
            case -1:
                perror("select");
                break;
            default: {
                for (i = 0; i < MAXSOCKETFD; i++) {
                    int fd = connected_fd[i];
                    if (fd == 0)
                        continue;

                    if (FD_ISSET(fd, &readfds)) {
                        if (server_sock == fd) {
                            int j = 0;
                            for (j = 0; j < MAXSOCKETFD; j++) {
                                if (connected_fd[j] == 0) {
                                    break;
                                }
                            }
                            if (j >= MAXSOCKETFD) {
                                printf("warning: no free connections \n");
                                continue;
                            }
                            /*********************************/
                            /* Accept an incoming connection */
                            /*********************************/
                            client_sock = accept(server_sock, (struct sockaddr *) &client_sockaddr, &len);
                            if (client_sock == -1){
                                perror("accept");
                                close(client_sock);
                                continue;
                            }
                            printf("accept fd %d\n", client_sock);

                            /****************************************/
                            /* Get the name of the connected socket */
                            /****************************************/
                            len = sizeof(client_sockaddr);
                            rc = getpeername(client_sock, (struct sockaddr *) &client_sockaddr, &len);
                            if (rc == -1){
                                perror("getpeername");
                                close(client_sock);
                                continue;
                            }
                            else {
                                printf("Client socket filepath: %s\n", client_sockaddr.sun_path);
                            }

                            connected_fd[j] = client_sock;
                        } else {
                            /************************************/
                            /* Read and print the data          */
                            /* incoming on the connected socket */
                            /************************************/
                            printf("waiting to read...\n");
                            memset(recv_buf, 0, MAXMSGSIZE);                
                            bytes_rec = recv(fd, recv_buf, sizeof(recv_buf), 0);
                            if (bytes_rec == -1){
                                perror("RECV");
                                close(client_sock);

                                connected_fd[i] = 0;
                                continue;
                            } else {
                                /*proc rcv msg*/
                                printf("DATA RECEIVED = %s\n", recv_buf);
                                rc = sfx_ssd_proc(recv_buf, bytes_rec, resp_buf);


                               /******************************************/
                                /* Send data back to the connected socket */
                                /******************************************/
                                              
                                printf("Sending data...\n");
                                rc = send(fd, resp_buf, rc, 0);
                                if (rc == -1) {
                                    perror("SEND");
                                } else {
                                    printf("Data sent!\n");
                                }
                                close(fd);
                                connected_fd[i] = 0;
                            }
                        }
                    }
                } 
            }
        }
    }

    /******************************/
    /* Close the sockets and exit */
    /******************************/
    close(server_sock);
    
    pthread_exit((void *)__func__);
}




int start_sfx_ssd_srv(void)
{
    int res = 0;
    pthread_t a_thread;
    void * thread_reasult;

    res = pthread_create(&a_thread, NULL, sfx_ssd_srv, NULL);
    if (res != 0) {
        perror("thread_create");
        return -1;
    }

}

int main()
{
    start_sfx_ssd_srv();
    while (1)
        sleep(10);
    return 0;
}
