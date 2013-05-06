#ifndef __WFUN_H
#define __WFUN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <net/if.h>
#include <sys/ioctl.h>

#define DEFAULT_BACKLOG 10
/* Wrap the basic operation on server programming */

int w_getaddrinfo(char *node, char *service, struct addrinfo *hints, struct addrinfo **res);
int w_socket(int domain, int type, int protocol);
int w_bind(int fd, struct sockaddr *my_addr, int addrlen);
void w_listen(int fd, int backlog);
void w_connect(int fd, struct sockaddr *serv_addr, int addrlen);
void w_setsockopt(int s, int level, int optname, const void *optval,  socklen_t optlen);
void w_sigaction(int signum, struct sigaction *act, struct sigaction *oldact);


/* general function for Network Programming */
int create_and_bind(char *port);
#endif
