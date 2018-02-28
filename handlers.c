#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#include "list.h"
#include "trolls_lair.h"

Todos todos;

void handle_quit(int signal) {
  tcsetattr(STDIN_FILENO, TCSANOW, &TERM_SETS);
  printf("\nBork, thanks for playing!\n");
  exit(signal);
}


void handle_help(struct list *commands) {
  printf("Possible commands:\n");
  struct node *curr = commands->head;
  while (curr) {
    printf("  %s\n", curr->source);
    curr = curr->next;
  }
  fflush(stdout);
  sleep(1);
}


void handle_todos() {
  if ((todos & MEETING) == MEETING) {
    printf("meeting");
  } else if ((todos & ETHICS) == ETHICS) {
    printf("ethics");
  } else if ((todos & SYSTEMS) == SYSTEMS) {
    printf("systems");
  } else if ((todos & LUNCH) == LUNCH) {
    printf("lunch");
  } else if ((todos & SITTER) == SITTER) {
    printf("sitter");
  }
  fflush(stdout);
  sleep(1);
}


char *handle_command(struct list *commands, char *command) {
  fflush(stdout);
  if (strcmp(command, "") == 0) {
    return NULL;
  } else if (strcmp(command, "help") == 0) {
    handle_help(commands);
    return NULL;
  } else if (strcmp(command, "todo") == 0) {
    handle_todos();
    return NULL;
  } else if (strcmp(command, "bork") == 0) {
    printf("bork bork\n");
    fflush(stdout);
    sleep(1);
    return NULL;
  } else if (strcmp(command, "quit") == 0) {
    handle_quit(0);
  }

  struct node *curr = commands->head;
  while (curr) {
    if (strcmp(curr->source, command) == 0) {
      printf("%s\n", curr->string);
      fflush(stdout);
      sleep(1);
      return curr->target;
    }
    curr = curr->next;
  }

  printf("That's not a valid command, you dummy! ");
  printf("Type 'help' to see what you can do here\n");
  return NULL;
}


Status handle_key(struct State *state) {
  int key = getch(stdin);
  Status return_value = REGULAR;

  switch (key) {
    case '\033': // arrow key
      return_value = decide_arrow();
      break;
    case 10: // enter
      return_value = ENTER;
      break;
    case 127: // backspace
      handle_backspace(state);
      break;
    default:
      putc(key, stdout);
      state->buffer[state->index++] = key;
      break;
  }
  return return_value;
}

Status decide_arrow() {
  Status return_status;
  int ch = getch(stdout);
  switch (ch) {
    case 'A':
      return_status = UP;
      break;
    case 'B':
      return_status = DOWN;
      break;
  }
  
  return return_status;
}


void handle_backspace(struct State *state) {
  getch(stdout);
  if (state->index) {
    printf("\b\033[K");
    state->buffer[state->index--] = 0;
  }
}


void handle_special_key(Status status, struct State *state) {
  if (status == UP || status == DOWN) {
    handle_arrow(status, state);
  } else if (status == ENTER) {
    handle_enter(status, state);
  }
}


void handle_arrow(Status status, struct State *state) {
  if (status == UP && state->curr_history->next) {
    state->curr_history = state->curr_history->next;
  } else if (status == DOWN && state->curr_history->prev) {
    state->curr_history = state->curr_history->prev;
  }

  // handle replacing previous command
  for (int i = 0; i < strlen(state->buffer); i++) {
    getch(stdout);
    printf("\n033[K");
  }
  sprintf(state->buffer, state->curr_history->string);
  printf(state->buffer);
  state->index = strlen(state->buffer);
}

void handle_enter(Status status, struct State *state) {
  printf("\n\n");
  //struct list *commands = read_room(state->current_room);
  //printf("COMMANDS ARE\n");
  //list_dump(commands, stdout);
  //fflush(stdout);
  char *transition = handle_command(state->commands, state->buffer);

  if (transition) {
    state->current_room = strdup(transition);
    state->commands = read_room(state->current_room);
  }
  list_push_back(state->history, "", "", state->buffer, 0);

  // left command logic out for now?

  memset(state->buffer, 0, strlen(state->buffer));
  state->index = 0;

  printf("What would you like to do?\n> ");
}
