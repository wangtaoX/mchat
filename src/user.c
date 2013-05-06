#include "user.h"
#include <stdbool.h>
#include <string.h>

/* when a new user is added into the hash table, we always 
 * initialize it */
static void inline initialization_new_user(struct user *new_user)
{
  struct sockaddr_in *si = (struct sockaddr_in *)&(new_user->user_ss);
  si->sin_family = AF_INET;
  si->sin_port = htons(PORT_NUMBER);
  memset(si->sin_zero, '\0', sizeof(si->sin_zero));
}
/* compare function when find an user in hash table */
static bool user_less_func(const struct hash_elem *u1,
                           const struct hash_elem *u2,
                           void *aux)
{
  struct user *u1_ = hash_entry(u1, struct user, user_hash_e);
  struct user *u2_ = hash_entry(u2, struct user, user_hash_e);

  return strcmp(u1_->user_name, u2_->user_name) < 0 ? 1 : 0;
}

/* convert a name message to an user information */
static inline struct user *name_msg_to_user(struct name_msg nm)
{
  struct user *new_user = malloc(sizeof(struct user));
  struct sockaddr_in *si = (struct sockaddr_in *)&(new_user->user_ss);

  initialization_new_user(new_user);
  strcpy(new_user->user_name, nm.name);
  inet_aton(nm.ip, &(si->sin_addr));

  return new_user;
}

/* here, should be rewrite */
void initialization_myself()
{
  strcpy(myself.user_name, user_name);
  myself.user_ss = *((struct sockaddr_storage *)&m_addr);

  hash_init(&user_friends, NULL, NULL, NULL);
}

bool is_friends(char *name)
{
  struct user *u = search_friends(name);

 return u != NULL; 
}

/* search for an user in hash table, return NULL if 
 * there is no such a friend, otherwise return the 
 * pointer to the USER */
struct user *search_friends(char *name)
{
  struct user res;
  struct hash_elem *e;

  strcpy(res.user_name, name);
  e = hash_find(&user_friends, &res.user_hash_e);

  if (e != NULL)
    return hash_entry(e, struct user, user_hash_e);
  else
    return NULL;
}

/* add an user into hash table */
int add_friends(struct name_msg nm)
{
  struct user *new_user;

  new_user = name_msg_to_user(nm);

  if (!is_friends(nm.name))
    hash_insert(&user_friends, &new_user->user_hash_e);
  else
    return -1;

  return 1;
}

/* delete an user from the hash table */
int delete_friends(char *name)
{
  struct user *exist_user;
  if (name == NULL)
    return -1;
  
  exist_user = search_friends(name);
  if (exist_user == NULL)
    return -1;
  else
    hash_delete(&user_friends, &exist_user->user_hash_e);
  /* here, we free the memory an user obtained */
  free(exist_user);

  return 1;
}

size_t friends_cnt()
{
  return hash_size(&user_friends);
}
