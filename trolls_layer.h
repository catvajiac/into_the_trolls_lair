/* bui.h */

#include "list.h"

typedef enum {
  KEYBOARD,
  GRAPHS,
} items;

/* utilities */
void string_strip(char *);
char *string_translate(char *, char *, char *);

/* handlers */
void handle_look(char *);
void handle_quit(int);
char *handle_command(struct list*, char *, char *);
