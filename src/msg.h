#ifndef __MSG_H
#define __MSG_H

#include <sys/types.h>
#include "user.h"
#include "args.h"
#include "../lib/wfun.h"
#include "../lib/list.h"
#include "../lib/string.h"

/* the length of user name */
#define DEFAULT_NAME_SIZE 20
#define MAX_MSG_SIZE 150

/* type of msg */
#define NAME_REPLY_MSG 0x0
#define NAME_MSG 0x1
#define MSG_MSG 0x2
#define OFFLINE_MSG 0x4
#define GROUP_MSG 0x8

/* any unreaded message should be in a list */
struct list unreaded_msg_list;

struct msg_header 
{
  char name[DEFAULT_NAME_SIZE];
  int  type;
  char ip[INET_ADDRSTRLEN];
  time_t time;
  int length;
};

typedef struct msg
{
  struct msg_header header;
  char msg[0];
}message;

union tran_message
{
  message msg;
  char str[MAX_MSG_SIZE];
};

/* construct a message */
message *construct_msg(int type, char *msg);
void destory_message(message *msg);
int send_message(int socket, union tran_message *buffer, 
    int *length, struct sockaddr *addr);
message *receive_message(int socket);
void reply_name_message(int socket_fd, char *name);
void reply_offline_message(int sfd);
void dump_message(message *msg);
#endif
