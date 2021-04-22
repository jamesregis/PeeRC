#ifndef _PEERC_H
#define _PEERC_H


/* includes */
#include <stdio.h>
#include <stdlib.h> 
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/* constant definition */
/* #define MSGSIZE 500 */
/* #define NUM_CONNECTION_MAX 30 */
#define DEFAULT_PORT_NUMBER 30000
#define LOGIN_SIZE 32
#define DEFAULT_LOGIN "james"
#define MSG_SIZE 512

/* message size*/
#define HEADER_SIZE  1

/* message header */
#define LOGIN_MESSAGE "A"
#define LOGOUT_MESSAGE "B"
#define PUBLIC_MESSAGE "C"
#define PRIVATE_MESSAGE "D"
#define R_PEER_LIST "E"
#define A_PEER_LIST "F"


/* macro */
#undef max
#define max(x,y) ((x) > (y) ? (x) : (y))


/* first server (or client) to connect to  */
#define DEFAULT_SERVER "127.0.0.1"

/* utils/utils.c*/


/* struct definition */

struct peer {
    int sockfd;
    char * htoname;
    u_short portnum;
    struct sockaddr_in *to;
    struct hostent *toinfo;
    char login[LOGIN_SIZE];
    u_long toaddr;
    int addr_in_size;
    struct peer * prev;
};

struct server {
    int sockfd;
    struct sockaddr_in *my_addr, *from;
    u_short portnum;
    u_long fromaddr;
};

#endif
