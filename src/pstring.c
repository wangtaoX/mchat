#include "pstring.h"
#include <stdio.h>

const char *welcome_str =  "Welcome to the use mchat server,\
 Use -h to get the help documentation\n";

const char *help = "-l list all online friends\n \
-s [[name][ip]] [msg] send a message to a friend\n ";

void print_help()
{
 fprintf(stdout, help);
}

void print_bannar()
{
  fprintf(stdout, welcome_str);
}
