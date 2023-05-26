/***********************************************************************
@File Name: sfx_ssd_srv.cpp
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
#include <signal.h>
#include "sfx_util.h"

#define SERVER_PATH  "/tmp/sfx_ssd_unix_sock.server"
#define CLIENT_PATH "/tmp/sfx_ssd_unix_sock.client"
#define DATA "Hello from server"

#define MAXSOCKETFD 20
#define MAXMSGSIZE  256

pthread_t g_srv_thread;

static int64_t sfx_ssd_proc(void *req_msg, int len, void *resp_msg)
{
    static int f = 0;
    hexdump("Srv Recv", req_msg, len);
    memset(resp_msg, 0, MAXMSGSIZE);
    sprintf((char *)resp_msg, "%s: %d", DATA, f++);

    hexdump("Srv Resp", resp_msg, strlen((char*)resp_msg));

    return strlen((char *)resp_msg);
}

static void *sfx_ssd_srv(void *args) 
{
    int server_sock, client_sock, connected_fd[MAXSOCKETFD] = { 0 }, rc;
    socklen_t len;
    int bytes_rec = 0;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;     
    fd_set readfds;
    int backlog = 10;
    char recv_buf[MAXMSGSIZE];
    char resp_buf[MAXMSGSIZE];


    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));

    pr_dbg_srv("%s thread start\n", __func__);
    
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
    pr_dbg_srv("socket %d listening...\n", server_sock);
    
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

        switch (select(max_fd + 1, &readfds, NULL, NULL, NULL)) {
            case 0:
                pr_dbg("timeout");
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
                                pr_info("warning: no free connections \n");
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
                            pr_dbg_srv("accept fd %d\n", client_sock);

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
                                pr_dbg_srv("Client socket filepath: %s\n", client_sockaddr.sun_path);
                            }

                            connected_fd[j] = client_sock;
                        } else {
                            /************************************/
                            /* Read and print the data          */
                            /* incoming on the connected socket */
                            /************************************/
                            pr_dbg_srv("waiting to read...\n");
                            memset(recv_buf, 0, MAXMSGSIZE);                
                            bytes_rec = recv(fd, recv_buf, sizeof(recv_buf), 0);
                            if (bytes_rec == -1){
                                perror("RECV");
                                close(client_sock);

                                connected_fd[i] = 0;
                                continue;
                            } else {
                                /*proc rcv msg*/
                                rc = sfx_ssd_proc(recv_buf, bytes_rec, resp_buf);


                               /******************************************/
                                /* Send data back to the connected socket */
                                /******************************************/
                                              
                                pr_dbg_srv("Sending data...\n");
                                rc = send(fd, resp_buf, rc, 0);
                                if (rc == -1) {
                                    perror("SEND");
                                } else {
                                    pr_dbg_srv("Data sent!\n");
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

int64_t sfx_ssd_send_req(void *req_msg, int req_len, void *resp_msg)
{
    int client_sock, rc, len;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;
    char buf[MAXMSGSIZE];
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));

    pr_dbg_srv("client start\n");
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
    pr_dbg_srv("Sending data...\n");
    hexdump("ClientSend", req_msg, req_len);
    rc = send(client_sock, req_msg, req_len, 0);
    if (rc == -1) {
        perror("send");
        close(client_sock);
        return -1;
    }
    else {
        pr_dbg_srv("Data sent!\n");
    }

    /**************************************/
    /* Read the data sent from the server */
    /* and print it.                      */
    /**************************************/
    pr_dbg_srv("Waiting to recieve data...\n");
    memset(buf, 0, sizeof(buf));
    rc = recv(client_sock, buf, sizeof(buf), 0);
    if (rc == -1) {
        perror("recv");
        close(client_sock);
        return -2;
    }
    else {
        hexdump("ClintRecv", buf, rc);
        memcpy(resp_msg, buf, rc);
    }

    /******************************/
    /* Close the socket and exit. */
    /******************************/
    close(client_sock);

    return rc;
}

int start_sfx_ssd_srv(void)
{
    int res = 0;

    res = pthread_create(&g_srv_thread, NULL, sfx_ssd_srv, NULL);
    if (res != 0) {
        perror("thread_create");
        return -1;
    }
    return 0;
}

int sfx_ssd_srv_alive(void)
{
    int rc = 0;
    if (g_srv_thread == 0) {
        return 0;
    }
    rc = pthread_kill(g_srv_thread, 0);
    switch (rc) {
        case EINVAL: pr_info("Warning: invalid sig to check thread alive\n");
        case ESRCH: return 0;
        default: return 1;
    }
}

#if 0
int main()
{
    int rc = 0;
    char data[] = "Hello from Client";
    char buf[256];
    start_sfx_ssd_srv();

    rc = sfx_ssd_srv_alive();
    pr_info("sfx_srv thread %s\n", rc ? "alive":"dead");

    sleep(2);

    sfx_ssd_send_req(data, strlen(data), buf);

    while(1)
        sleep(10);
    return 0;
}
#endif
