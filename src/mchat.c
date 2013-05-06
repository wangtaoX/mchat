#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "user.h"
#include "msg.h"
#include "socket.h"
#include "pstring.h"


extern char user_name[DEFAULT_NAME_SIZE];
extern struct sockaddr_in b_addr, m_addr;

static void parse_user_info()
{
  gethostname(user_name, DEFAULT_NAME_SIZE);
}

/* user info initialization */
int initialization()
{
  int sfd;

  sfd = make_socket();
  b_addr = get_broadcast_addr(sfd);
  m_addr = get_myself_addr(sfd);
  parse_user_info();
  initialization_myself();
  //printf("broadcast addresss: %s\n", inet_ntoa(b_addr.sin_addr));
  //printf("myself    addresss: %s\n", inet_ntoa(m_addr.sin_addr));
  
  return sfd;
}

void *socket_handler(void *arg)
{
  int socket_fd = *((int *)arg);
  //fd_set read_fds;
  //int i = 0;
  union transport_msg tm;
  int numbytes;
  socklen_t addr_len;
  struct sockaddr_in their_addr;
  struct msg message;
  
  /* listener thread main loop */
  for (;;)
  {
  //  FD_SET(socket_fd, &read_fds);
  //  if (select(socket_fd + 1, &read_fds, NULL, NULL, NULL) == -1)
  //  {
  //    perror("select");
  //    exit(0);
  //  }
  //  for (; i<=socket_fd; i++)
  //  {
  //    if (FD_ISSET(i, &read_fds)) {
        // handle data
        addr_len = sizeof(their_addr);
        if ((numbytes = recvfrom(socket_fd, tm.string, sizeof(union transport_msg),
              0, (struct sockaddr *)&their_addr, &addr_len)) == -1)
        {
          perror("recvfrom");
          exit(0);
        }
        message = resolve_message(tm);
        switch(message.msg_type)
        {
          case NAME_MSG:
            add_friends(message.msg.nm);
            break;
          case GROUP_MSG:
            break;
          case MSG_MSG:
            break;
          default:
            break;
        }
   //     printf("user_name : %s, Ip : %s\n", tm.msg.msg.nm->name, tm.msg.msg.nm->ip);
   //   }
   // }/* end of socket readable */
  }/* end of main loop */
}

int main(int argc, char *argv[])
{
  int sfd;
  struct sockaddr_in b_addr, m_addr;
  pthread_t t;

  system("reset");
  print_bannar();
  sfd = initialization(); 
  broadcast_myself(sfd);
  pthread_create(&t, NULL, socket_handler, (void *)&sfd);
  
  pthread_join(t, NULL);
//
//  while(True)
//  {
//    prase_option();
//
//    switch opt:
//    
//  }
}

