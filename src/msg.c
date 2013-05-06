#include "msg.h"

extern char user_name[DEFAULT_NAME_SIZE];
extern struct sockaddr_in m_addr;

/* private function of msg module */
static struct msg *construct_msg_msg(char *str);
static struct msg_msg *__msg_msg(char *msg);
static struct msg *construct_name_msg();
static int message_type(union transport_msg tm);

/* the first time a user online should construct
 * this kind of msg */
static struct msg *construct_name_msg()
{
  struct msg *msg = malloc(sizeof(struct msg));
  
  msg->msg.nm = malloc(sizeof(struct name_msg));
  strcpy(msg->msg.nm->name, user_name);
  strcpy(msg->msg.nm->ip, inet_ntoa(m_addr.sin_addr));
  msg->msg_type = NAME_MSG;
  
  return msg;
}

/* chatting message */
static struct msg_msg *__msg_msg(char *msg)
{
  int length = strlen(msg);
  struct msg_msg *mm = (struct msg_msg *)malloc(sizeof(struct msg_msg) + length);

  mm->length = length;
  strcpy(mm->name, user_name);
  strcpy(mm->msg, msg);

  return mm;
}

static struct msg *construct_msg_msg(char *str)
{
  struct msg *msg;

  if (str == NULL)
    return NULL;
  msg = malloc(sizeof(struct msg));
  msg->msg_type = MSG_MSG;
  msg->msg.mm = __msg_msg(str);

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
    case MSG_MSG:
      msg = construct_msg_msg(str);
      break;
//    case GROUP_MSG:
//      msg = construct_group_msg();
//      break;
    default:
      break;
  }

   return msg;
}

static void destory_name_msg(struct msg *msg)
{
  free(msg->msg.nm);
  free(msg);
}

static void destory_msg_msg(struct msg *msg)
{
  free(msg->msg.mm);
  free(msg);
}

/* destory a message */
void destory_msg(struct msg *msg)
{
  if (msg == NULL)
    return ;

  switch(msg->msg_type)
  {
    case NAME_MSG:
      destory_name_msg(msg);
      break;
    case MSG_MSG:
      destory_msg_msg(msg);
      break;
    default:
      break;
  }
}

struct msg resolve_message(union transport_msg tm)
{
  struct msg message = tm.msg; 

  return message;
}

