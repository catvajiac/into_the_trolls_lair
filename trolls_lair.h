#ifndef TROLLH
#define TROLLH

/* macros */
#define length(x)  (sizeof(x) / sizeof((x)[0]))
#define streq(s0, s1) (strcmp((s0), (s1)) == 0)

#include <wait.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "list.h"

typedef struct termios Termios;

typedef enum {
  KEYBOARD  = 1<<1,
  GRAPHS    = 1<<2, 
  STICKY    = 1<<3,
} Items;


typedef enum {
  REGULAR,
  UP,
  DOWN,
  ENTER,
  END,
  WIN,
} Status;

typedef enum {
  MEETING = 1<<1,
  ETHICS  = 1<<2,
  SYSTEMS = 1<<3,
  LUNCH   = 1<<4,
  SITTER  = 1<<5,
} Todos;

typedef struct State {
  List *history;
  List *commands;
  Node *curr_history;
  Items items;
  Todos todos;
  char *buffer;
  char *current_room;
  int index;
} State;


/* utilities */
void cowsay(char *, char *);
int getch(FILE *);
List *parse_rules(FILE *, State *);
void print(char *, bool, char[]);
void string_strip(char *);
char *string_translate(char *, char *, char *);
List *read_room(State *);

/* handlers */
void handle_arrow(Status, State *);
void handle_backspace(State *);
void handle_clear(State *);
void handle_enter(Status, State *);
void handle_items(State *);
void handle_look(char *);
void handle_quit(int);
void handle_special_key(Status, State *);
void help(int);

Status decide_arrow();
Node *handle_command(State *);
Status handle_key(State *);

/* globals */
Termios TERM_SETS; // old terminal settings
State *global_state;
int WIDTH, HEIGHT;


/* terminal color */
#define NOCOLOR "\x1B[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"
#define WHITE   "\x1B[37m"

#endif
