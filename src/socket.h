#ifndef __SOCKET_H
#define __SOCKET_H

#include "../lib/wfun.h"
#include "user.h"
#include "msg.h"
#include <pthread.h>

#define True 1
#define False 0

/* port number for mchat */
#define PORT_STRING "7777"
#define PORT_NUMBER 7777


int make_socket();
struct sockaddr_in get_broadcast_addr(int socket_fd);
struct sockaddr_in get_myself_addr(int socket_fd);

/* broadcast a message */
int broadcast_myself(int socket_fd);
int send_to_all(int s, char *buf, int *len);
#endif
