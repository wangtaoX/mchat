#include "wfun.h"

/* Wrap the function [getaddrinfo] */
int w_getaddrinfo(char *node, char *service, 
    struct addrinfo *hints, struct addrinfo **res)
{
  int rv;

  if ((rv = getaddrinfo(node, service, hints, res)) != 0)
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    exit(1);
  }

  return rv;
}

/* Wrap the function [socket] */
int w_socket(int domain, int type, int protocol)
{
  int fd;

  if ((fd = socket(domain, type, protocol)) == -1)
  {
    perror("socket error");
    exit(1);
  }

  return fd;
}

/* Wrap the bind function */
int w_bind(int fd, struct sockaddr *my_addr, int addrlen)
{
  int status;

  if ((status = bind(fd, my_addr, addrlen)) == -1)
  {
    /* here we close the socket */
    close(fd);
    perror("bind error");
    exit(1);
  }

  return status;
}

/* Wrap function [listen] */
void w_listen(int fd, int backlog)
{
  if (listen(fd, backlog) == -1)
  {
    perror("listen error");
    exit(1);
  }
}

/* Wrap function [connect]*/
void w_connect(int fd, struct sockaddr *serv_addr, int addrlen)
{
  if (connect(fd, serv_addr, addrlen) == -1)
  {
    close(fd);
    perror("connect error");
    exit(1);
  }
}

/* Wrap function [setsockopt] */
void w_setsockopt(int s, int level, int optname, const void *optval,
    socklen_t optlen)
{
  if (setsockopt(s, level, optname, optval, optlen) == -1)
  {
    perror("setsockopt error");
    exit(1);
  }
}

/* Wrap function [sigaction] */
void w_sigaction(int signum, struct sigaction *act, 
    struct sigaction *oldact)
{
  if (sigaction(signum, act, oldact) == -1)
  {
    perror("sigaction error");
    exit(1);
  }
}

/* general function */
int create_and_bind(char *port)
{
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int s, sfd;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  s = getaddrinfo(NULL, port, &hints, &result);
  if (s != 0)
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    return -1;
  }

  for (rp = result; rp != NULL; rp = rp->ai_next)
  {
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sfd == -1)
      continue;

    s = bind(sfd, rp->ai_addr, rp->ai_addrlen);
    if (s == 0)
      break;
    close(sfd);
  }

  if (rp == NULL)
  {
    fprintf(stderr, "Could not bind\n");
    return -1;
  }
  freeaddrinfo(result);

  return sfd;
}
