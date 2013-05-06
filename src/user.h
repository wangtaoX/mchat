#ifndef __USER_H
#define __USER_H

#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include "msg.h"
#include "socket.h"
#include "../lib/list.h"
#include "../lib/hash.h"

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
};

void initialization_myself();
int add_friends(struct name_msg nm);
bool is_friends(char *name);
int delete_friends(char *name);
struct user *search_friends(char *name);
size_t friends_cnt();
#endif
