/* bui.h */

#include "list.h"

typedef enum {
  KEYBOARD,
  GRAPHS,
} items;

typedef enum {
  MEETING = 1<<0,
  ETHICS  = 1<<1,
  SYSTEMS = 1<<2,
  LUNCH   = 1<<3,
  SITTER  = 1<<4,
} Todos;

/* utilities */
void string_strip(char *);
char *string_translate(char *, char *, char *);

/* handlers */
void handle_look(char *);
void handle_quit(int);
char *handle_command(struct list*, char *);
