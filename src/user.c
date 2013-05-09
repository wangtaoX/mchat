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
/* hash function of user friends */
static unsigned hash_user_func(const struct hash_elem *e, void *aux)
{
  struct user *u = hash_entry(e, struct user, user_hash_e);

  return hash_bytes(u->user_name, strlen(u->user_name));
}

/* convert a name message to an user information */
static inline struct user *name_msg_to_user(message *msg)
{
  struct user *new_user = malloc(sizeof(struct user));
  struct sockaddr_in *si = (struct sockaddr_in *)&(new_user->user_ss);

  initialization_new_user(new_user);
  strlcpy(new_user->user_name, msg->header.name, DEFAULT_NAME_SIZE);
  inet_aton(msg->header.ip, &(si->sin_addr));
  new_user->in_chatting = false;

  return new_user;
}

/* here, should be rewrite */
void initialization_myself()
{
  strlcpy(myself.user_name, user_name, DEFAULT_NAME_SIZE);
  myself.user_ss = *((struct sockaddr_storage *)&m_addr);

  hash_init(&user_friends, hash_user_func, user_less_func, NULL);
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

  strlcpy(res.user_name, name, DEFAULT_NAME_SIZE);
  e = hash_find(&user_friends, &res.user_hash_e);

  if (e != NULL)
    return hash_entry(e, struct user, user_hash_e);
  else
    return NULL;
}

/* add an user into hash table */
int add_friends(message *mm)
{
  struct user *new_user;

  /* ourself should not be in hash table */
  if (is_myself(mm->header.ip))
    return -1;
  new_user = name_msg_to_user(mm);

  if (!is_friends(mm->header.name))
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

bool is_inchatting(char *name)
{
  struct user *user = search_friends(name);
  
  return user->in_chatting;
}

/* return true if ip string is equal to sin_addr */
bool is_myself(char *ip)
{
  if (ip == NULL)
    return false;

  if (strcmp(ip, inet_ntoa(m_addr.sin_addr)) == 0)
  {
//    printf("got myself, aha\n");
    return true;
  }

  return false;
}

/* print all online user */
void dump_all_online_friends()
{
  struct hash_iterator i;
  struct user *u;
  struct sockaddr_in *si;

  if (hash_empty(&user_friends))
  {
    fprintf(stdout, "No friends online ;(\n");
    return;
  }
  fprintf(stdout, "User Name\tIP\n");
  /* iterate all friends in hash table */
  hash_first(&i, &user_friends);
  while(hash_next(&i))
  {
    u = hash_entry(hash_cur(&i), struct user, user_hash_e);
    si = (struct sockaddr_in *)(&u->user_ss);
    fprintf(stdout, "%s\t%s\n", u->user_name, inet_ntoa(si->sin_addr));
  }
  
  return;
}

void free_user_info()
{
  hash_destroy(&user_friends, NULL);
}
