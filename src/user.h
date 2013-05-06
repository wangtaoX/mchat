#ifndef __USER_H
#define __USER_H

#include <netinet/in.h>
#include "msg.h"
#include "../lib/list.h"
#include "../lib/hash.h"

#ifndef DEFAULT_NAME_SIZE
#define DEFAULT_NAME_SIZE 20
#endif

/* basic information  */
char user_name[DEFAULT_NAME_SIZE];
struct sockaddr_in b_addr, m_addr;
struct user myself;

struct user
{
  char user_name[DEFAULT_NAME_SIZE];
  struct sockaddr_storage user_ss;
  struct hash_elem user_hash_e;

  /* all online friends should be in an hash table */
  struct hash user_friends;
};

void initialization_myself();
#endif
