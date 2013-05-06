#include "user.c"

void initialization_myself()
{
  strcpy(myself.user_name, user_name);
  myself.user_ss = *((struct sockaddr_storage *)&m_addr);

  hash_init(&user_friends, hash_string, NULL, NULL);
}

