#include "msg.h"

extern char user_name[DEFAULT_NAME_SIZE];
extern struct sockaddr_in m_addr;
/* the first time a user online should construct
 * this kind of msg */
static struct msg construct_name_msg()
{
  struct msg msg;
  struct name_msg nm;

  strcpy(nm.name, user_name);
  strcpy(nm.ip, inet_ntoa(m_addr.sin_addr));
  msg.msg_type = NAME_MSG;
  msg.msg.nm = nm;
  
  return msg;
}

/* construct a message */
struct msg construct_msg(int msg_type, char *str)
{
  struct msg msg;

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
