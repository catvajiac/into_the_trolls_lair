#ifndef TROLLH
#define TROLLH

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "list.h"

typedef enum {
  KEYBOARD,
  GRAPHS,
} items;

typedef enum {
  REGULAR,
  UP,
  DOWN,
  ENTER,
  END,
} Status;

typedef enum {
  MEETING = 1<<0,
  ETHICS  = 1<<1,
  SYSTEMS = 1<<2,
  LUNCH   = 1<<3,
  SITTER  = 1<<4,
} Todos;

struct State {
  struct list *history;
  struct list *commands;
  struct node *curr_history;
  char *buffer;
  char *current_room;
  int index;
};


/* utilities */
void string_strip(char *);
char *string_translate(char *, char *, char *);
int getch(FILE *);
struct list *parse_rules(FILE *);
struct list *read_room(char *);

/* handlers */
void handle_look(char *);
void handle_quit(int);
char *handle_command(struct list*, char *);
Status decide_arrow();
void handle_arrow(Status, struct State *);
void handle_enter(Status, struct State *);
void handle_special_key(Status, struct State *);
Status handle_key(struct State *);
void handle_backspace(struct State *);

/* globals */
struct termios TERM_SETS; // old terminal settings
int WIDTH;

#endif
