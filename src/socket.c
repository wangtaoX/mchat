#include "socket.h"

extern struct sockaddr_in b_addr;
/* make a socket with option SO_BROADCAST */
int make_socket()
{
 int socket_fd;
 int broadcast = True;
 struct sockaddr_in server;

 socket_fd = w_socket(AF_INET, SOCK_DGRAM, 0);
 if (setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &broadcast,
       sizeof(broadcast)) == -1)
 {
    perror("setsockopt error");
    exit(1);
 }

 /* bind a port to socket socket_fd */
 bzero(&server, sizeof(server));
 server.sin_family = AF_INET;
 server.sin_addr.s_addr = htonl(INADDR_ANY);
 server.sin_port = htons(PORT_NUMBER);
 w_bind(socket_fd, (struct sockaddr *)&server, sizeof(server));

 return socket_fd;
}

/* get the broadcast address, use ioctl */
struct sockaddr_in get_broadcast_addr(int socket_fd)
{
  struct sockaddr_in b_addr;
  struct ifreq ifr;

  bzero(&ifr, sizeof(ifr));
  strlcpy(ifr.ifr_name, "eth0", 5);
  if (ioctl(socket_fd, SIOCGIFBRDADDR, &ifr) < 0)
  {
    perror("ioctl");
    exit(1);
  }
  b_addr.sin_addr = ((struct sockaddr_in *)&(ifr.ifr_addr))->sin_addr;
  b_addr.sin_family = AF_INET;
  b_addr.sin_port = htons(PORT_NUMBER);
  memset(b_addr.sin_zero, '\0', sizeof(b_addr.sin_zero));

  return b_addr;
}

struct sockaddr_in get_myself_addr(int socket_fd)
{
  struct sockaddr_in m_addr;
  struct ifreq ifr;

  bzero(&ifr, sizeof(ifr));
  strlcpy(ifr.ifr_name, "eth0", 5);
  if (ioctl(socket_fd, SIOCGIFADDR, &ifr) < 0)
  {
    perror("ioctl");
    exit(1);
  }
  m_addr.sin_addr = ((struct sockaddr_in *)&(ifr.ifr_addr))->sin_addr;
  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons(PORT_NUMBER);
  memset(m_addr.sin_zero, '\0', sizeof(m_addr.sin_zero));

  return m_addr;
}

/* use broadcast address to notify any other
 * online users*/
int broadcast_myself(int socket_fd)
{
  union tran_message tm;
  message *msg;
  int numbytes;
  int length;

  msg = construct_msg(NAME_MSG, NULL);
  tm.msg = *msg;
  length = sizeof(message);
  if ((numbytes = send_message(socket_fd, &tm, &length, 
          (struct sockaddr *)&b_addr) == -1))
  {
    perror("sendto");
    exit(1);
  }
  //printf("broadcast: user name[%s] ip[%s]\n", msg->header.name, msg->header.ip);
  //printf("numbytes %d size_of transport_msg %d\n", numbytes, 
  //    sizeof(union transport_msg));
  destory_message(msg); 
  
  return 1;
}
