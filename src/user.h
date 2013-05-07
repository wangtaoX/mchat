#ifndef __USER_H
#define __USER_H

#include <netinet/in.h>
#include <stdbool.h>
#include "msg.h"
#include "socket.h"
#include "../lib/list.h"
#include "../lib/hash.h"
#include "../lib/string.h"

#ifndef DEFAULT_NAME_SIZE
#define DEFAULT_NAME_SIZE 20
#endif

/* basic information  */
char user_name[DEFAULT_NAME_SIZE];
struct sockaddr_in b_addr, m_addr;
struct user myself;

/* all online friends should be in an hash table */
struct hash user_friends;

struct user
{
  char user_name[DEFAULT_NAME_SIZE];
  struct sockaddr_storage user_ss;
  struct hash_elem user_hash_e;
  bool in_chatting;
};

void initialization_myself();
int add_friends(struct name_msg *nm);
bool is_friends(char *name);
int delete_friends(char *name);
struct user *search_friends(char *name);
size_t friends_cnt();
bool is_inchatting(char *name);
bool is_myself(char *ip);

#endif
