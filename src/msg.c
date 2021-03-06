#include "msg.h"

extern char user_name[DEFAULT_NAME_SIZE];
extern struct sockaddr_in m_addr;

/* private function of msg module */
static message *construct_msg_msg(char *str);
static message *__msg_msg(char *msg);
static message *construct_name_msg(int type);
static int message_type(message *msg);

/* the first time a user online should construct
 * this kind of msg */
static message *construct_name_msg(int type)
{
  message *msg = malloc(sizeof(message));
  
  strlcpy(msg->header.name, user_name, DEFAULT_NAME_SIZE);
  strlcpy(msg->header.ip, inet_ntoa(m_addr.sin_addr), INET_ADDRSTRLEN);
  msg->header.type = type;
  msg->header.length = 0;
  
  return msg;
}

/* chatting message */
static message *__msg_msg(char *msg)
{
  int length = strlen(msg) + 1;
  message *mm = (message *)malloc(sizeof(message) + length);

  mm->header.length = length;
  strlcpy(mm->header.name, user_name, DEFAULT_NAME_SIZE);
  strlcpy(mm->msg, msg, length);
  strlcpy(mm->header.ip, inet_ntoa(m_addr.sin_addr), INET_ADDRSTRLEN);
  mm->header.type = MSG_MSG;

  return mm;
}

static message *construct_msg_msg(char *str)
{
  message *msg;

  if (str == NULL)
    return NULL;
  msg = __msg_msg(str);

  return msg;
}

/* return the type of this message */
static int message_type(message *msg)
{
  return msg->header.type;
}

/* construct a message */
message *construct_msg(int type, char *msg)
{
  message *m;

  switch(type) {
    case NAME_MSG:
      m = construct_name_msg(NAME_MSG);
      break;
    case NAME_REPLY_MSG:
      m = construct_name_msg(NAME_REPLY_MSG);
      break;
    case MSG_MSG:
      m = construct_msg_msg(msg);
      break;
    case OFFLINE_MSG:
      m = construct_name_msg(OFFLINE_MSG);
      break;
    default:
      break;
  }

   return m;
}

/* destory a message */
void destory_message(message *msg)
{
  if (msg == NULL)
    return ;
  free(msg);

  return ;
}

int send_message(int socket, union tran_message *buffer, 
    int *length, struct sockaddr *addr)
{
  int total = 0;             //how many bytes we have sent
  int bytesleft = *length;   //how many we have left to sent
  int n;

  while(total < *length)
  {
    n = sendto(socket, buffer->str + total, bytesleft, 0,
        addr, sizeof(*addr));
    if (n == -1) 
      break;
    total += n;
    bytesleft -= n;
  }

  *length = total;

  return n == -1 ? -1 : 0;
}

message *receive_message(int socket)
{
  socklen_t addrlen;
  struct sockaddr_in their_addr;
  int numbytes;
  union tran_message buffer;
  message *msg;
  int length;

  /* use MSG_PEEK to get the length of the message, but it is 
   * not read the packet */
  addrlen = sizeof(their_addr);
  if ((numbytes = recvfrom(socket, buffer.str, sizeof(message), MSG_PEEK,
          (struct sockaddr *)&their_addr, &addrlen)) == -1)
  {
    perror("recvfrom");
    exit(0);
  }
  length = buffer.msg.header.length;

  /* we just read the real message */
  numbytes = recvfrom(socket, buffer.str, sizeof(message) + length,
      0, (struct sockaddr *)&their_addr, &addrlen);
  if (numbytes == -1)
  {
    perror("recvfrom");
    exit(0);
  }

  msg = malloc(sizeof(message) + buffer.msg.header.length);
  memcpy((void *)msg, (void *)&buffer, sizeof(message) + length);
  msg->msg[length - 1] = '\0';

  return msg;
}

/* when receive a name message, we should *
 * reply a name message                   */
void reply_name_message(int socket_fd, char *name)
{
  struct user *receiver;
  message *nm;
  int numbytes, length;
  union tran_message buffer;
  
  receiver = search_friends(name);
  if (receiver == NULL)
    return;

  nm = construct_msg(NAME_REPLY_MSG, NULL);
  length = sizeof(message);
  memcpy((void *)&buffer, (void *)nm, length);
  if ((numbytes = send_message(socket_fd, &buffer, &length, 
        (struct sockaddr *)&(receiver->user_ss))) == -1)
  {
    perror("sendto");
    exit(0);
  }
  destory_message(nm);
}

/* offline message */
void reply_offline_message(int socket_fd)
{
  struct user *receiver;
  struct hash_iterator i;
  union tran_message buffer;
  message *msg;
  int length = sizeof(message);
  int numbytes;

  /* no friends here */
  if (hash_empty(&user_friends))
    return;

  msg = construct_msg(OFFLINE_MSG, NULL);
  memcpy((void *)&buffer, (void *)msg, length);
  hash_first(&i, &user_friends);
  while(hash_next(&i))
  {
    receiver = hash_entry(hash_cur(&i), struct user, user_hash_e);
    if ((numbytes = send_message(socket_fd, &buffer, &length, 
          (struct sockaddr *)&(receiver->user_ss))) == -1)
    {
      perror("sendto");
      exit(0);
    }
  }
  destory_message(msg);
}

void dump_message(message *msg)
{
  if (msg == NULL)
    return;

  printw("\nMessage from %s: %s\n", msg->header.name, msg->msg);
  return;
}
