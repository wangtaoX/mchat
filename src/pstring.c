#include "pstring.h"
#include <ncurses.h>

const char *welcome_str =  "Welcome to the use mchat server,\
 Use -h to get the help documentation\n";

const char *help = "-l list all online friends\n\
-s [[name][ip]] [msg] send a message to a friend\n";

void print_help()
{
  printw("%s", help);
}

void print_bannar()
{
  printw("%s", welcome_str);
}
void print_goodbye()
{
  printw("Goodbye ;)\n");
}
