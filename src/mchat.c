#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "user.h"
#include "msg.h"
#include "socket.h"
#include "pstring.h"
#include "../lib/string.h"

/* arguments when user input */
typedef struct args
{
  char command;
  char arg1[DEFAULT_NAME_SIZE];
  int length;
  char arg2[0];
}args;

/* information about myself */
extern char user_name[DEFAULT_NAME_SIZE];
extern struct sockaddr_in b_addr, m_addr;

static void parse_user_info();
static inline void get_user_input(char *s, int length);

static inline void parse_user_info()
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
  
  return sfd;
}

void *socket_handler(void *arg)
{
  int socket_fd = *((int *)arg);
  int numbytes;
  message *msg;
  char buf[MAX_MSG_SIZE];
  union tran_message buffer;
  
  /* listener thread main loop */
  for (;;)
  {
    msg = receive_message(socket_fd);
    switch(msg->header.type)
    {
      case NAME_REPLY_MSG:
      case NAME_MSG:
        add_friends(msg);
        if (msg->header.type & NAME_MSG)
          reply_name_message(socket_fd, msg->header.name);
        break;
      case GROUP_MSG:
        break;
      case MSG_MSG:
        break;
      case OFFLINE_MSG:
        delete_friends(msg->header.name);
        break;
      default:
        break;
    }
    //printf("receive: user name[%s] ip[%s]\n", msg->header.name, msg->header.ip);
    destory_message(msg);
  }/* end of main loop */
}

/* we should rewrite this function */
struct args *parse_option(char *s)
{
  char *token, *save_ptr;
  int length;
  struct args *args;
  char c, str[DEFAULT_NAME_SIZE];

  token = strtok_r(s, " ", &save_ptr);
  if (*token == '\n')
    return NULL;

  c = *token;
  token = strtok_r(NULL, " ", &save_ptr);
  if (token != NULL)
  {
    strlcpy(str, token, DEFAULT_NAME_SIZE);
  } else
    goto one_arg;

  length = strlen(save_ptr);
  goto three_arg;

one_arg:
  args = malloc(sizeof(struct args));
  args->command = c;
  return args;

three_arg:
  args = malloc(sizeof(struct args) + length);
  args->command = c;
  strlcpy(args->arg1, str, DEFAULT_NAME_SIZE);
  strlcpy(args->arg2, save_ptr, length);
  args->length = length;

  return args;
}

static inline void get_user_input(char *s, int length)
{
  fgets(s, length, stdin);
}

int main(int argc, char *argv[])
{
  int sfd;
  struct sockaddr_in b_addr, m_addr;
  pthread_t t;
  char args[MAX_MSG_SIZE + 32];
  bool quitting = false;
  struct args *opt;
  union tran_message tm;
  struct user *reciver;
  message *msg;
  int length;

  /* do some initialization */
  system("reset");
  print_bannar();
  sfd = initialization(); 
  broadcast_myself(sfd);
  pthread_create(&t, NULL, socket_handler, (void *)&sfd);

  while(true)
  {
    fprintf(stderr, ">");
    fflush(stdout);
    get_user_input(args, MAX_MSG_SIZE + 32);
    opt = parse_option(args);
    if (opt == NULL)
      continue;
    switch(opt->command) 
    {
      case 'l':
        dump_all_online_friends();
        break;
      case 's':
        msg = construct_msg(MSG_MSG, opt->arg2);
        length = sizeof(message) + strlen(opt->arg2);
        memcpy((void *)&tm, (void *)msg, length);

        reciver = search_friends(opt->arg1);
        if (reciver == NULL)
        {
          fprintf(stdout, "No such friends, Please check ;)\n");
          break;
        }
        send_message(sfd, &tm, &length, 
            (struct sockaddr *)&reciver->user_ss);
        destory_message(msg);
        break;
      case 'q':
        print_goodbye();
        quitting = true;
        reply_offline_message(sfd);
        free_user_info();
        break;
      case 'h':
        print_help();
        break;
      default:
        fprintf(stdout, "Unknown option, Please use h(help) to get help ;)\n");
        break;
    }
    if (quitting)
    {
      free(opt);
      break;
    }
  }
  pthread_join(t, NULL);
}

