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
  KEYBOARD  = 1<<1,
  GRAPHS    = 1<<2, 
} Items;


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
  Items items;
  char *buffer;
  char *current_room;
  int index;
};


/* utilities */
void string_strip(char *);
char *string_translate(char *, char *, char *);
int getch(FILE *);
struct list *parse_rules(FILE *, struct State *);
struct list *read_room(struct State *);
void print(char *, bool, char[]);

/* handlers */
void handle_look(char *);
void handle_quit(int);
void help(int);
struct node *handle_command(struct State *);
Status decide_arrow();
void handle_arrow(Status, struct State *);
void handle_enter(Status, struct State *);
void handle_special_key(Status, struct State *);
Status handle_key(struct State *);
void handle_backspace(struct State *);
void handle_items(struct State *);

/* globals */
struct termios TERM_SETS; // old terminal settings
struct State *global_state;
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
