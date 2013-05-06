#ifndef __MSG_H
#define __MSG_H

#include <sys/types.h>
#include "user.h"
#include "../lib/wfun.h"

/* the length of user name */
#define DEFAULT_NAME_SIZE 20
#define MAX_MSG_SIZE 150

/* type of msg */
#define GROUP_MSG 0x0
#define NAME_MSG 0x1
#define MSG_MSG 0x2

/* new user should broadcast this kind of msg */
struct name_msg
{
  char name[DEFAULT_NAME_SIZE];
  char ip[INET_ADDRSTRLEN];
};

/* message between users */
struct msg_msg
{
  int length;
  char name[DEFAULT_NAME_SIZE];
  char msg[0];
};

/* group control msg */
struct group_crl_msg
{
  char flags;
  char g_name[DEFAULT_NAME_SIZE];
};


/* all kinds of msg shuold be in one */
struct msg
{
  int msg_type; /*the type of this msg
                 eg : name message...*/
  union
  {
    struct name_msg nm;
    struct msg_msg mm;
    struct group_crl_msg gcm;
  }msg;
};

union transport_msg
{
  struct msg msg;
  char string[MAX_MSG_SIZE];
};

/* construct a message */
struct msg construct_msg(int msg_type, char *msg);
#endif
