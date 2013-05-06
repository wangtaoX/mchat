#include "msg.h"

extern char user_name[DEFAULT_NAME_SIZE];
extern struct sockaddr_in m_addr;
/* the first time a user online should construct
 * this kind of msg */
static struct msg *construct_name_msg()
{
  struct msg *msg = malloc(sizeof(struct msg));
  struct name_msg nm;

  strcpy(msg->msg.nm.name, user_name);
  strcpy(msg->msg.nm.ip, inet_ntoa(m_addr.sin_addr));
  msg->msg_type = NAME_MSG;
  
  return msg;
}

/* return the type of this message */
static int message_type(union transport_msg tm)
{
  return tm.msg.msg_type;
}

/* construct a message */
struct msg *construct_msg(int msg_type, char *str)
{
  struct msg *msg;

  switch(msg_type) {
    case NAME_MSG:
      msg = construct_name_msg();
      break;
//    case MSG_MSG:
//      msg = construct_msg_msg(msg);
//      break;
//    case GROUP_MSG:
//      msg = construct_group_msg();
//      break;
    default:
      break;
  }

   return msg;
}

struct msg resolve_message(union transport_msg tm)
{
  struct msg message = tm.msg; 

  return message;
}

