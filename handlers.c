#include "list.h"
#include "trolls_lair.h"

Todos todos;

/* clean up */
void handle_quit(int signal) {
  tcsetattr(STDIN_FILENO, TCSANOW, &TERM_SETS);
  print("\nBork, thanks for playing!", true, CYAN);
  exit(signal);
  list_delete(global_state->history);
  list_delete(global_state->commands);
  free(global_state->current_room);
  free(global_state->buffer);
  free(global_state);
}

/* handle help message */
void help(int status) {
  print("Usage: trolls_layer [-H height] [-W width] [-h]", true, GREEN);
  exit(status);
}


/* handle 'help' command */
void handle_help(struct list *commands) {
  print("Possible commands:", true, MAGENTA);
  struct node *curr = commands->head;
  while (curr) {
    print(curr->source, true, MAGENTA);
    curr = curr->next;
  }
  fflush(stdout);
  sleep(1);
}


/* handle 'todo' command */
void handle_todos() {
  char color[] = YELLOW;
  if ((todos & MEETING) == MEETING) {
    print("meeting", true, color);
  } else if ((todos & ETHICS) == ETHICS) {
    print("ethics", true, color);
  } else if ((todos & SYSTEMS) == SYSTEMS) {
    print("systems", true, color);
  } else if ((todos & LUNCH) == LUNCH) {
    print("lunch", true, color);
  } else if ((todos & SITTER) == SITTER) {
    print("sitter", true, color);
  }
  fflush(stdout);
  sleep(1);
}


void handle_items(struct State *state) {
  char color[] = BLUE;
    print("Inventory:", true, color);
    printf("%d\n", state->items);
   if (state->items &= KEYBOARD == KEYBOARD) {
      print("  keyboard", true, color);
   }
   if (state->items &= GRAPHS == GRAPHS) {
      print("  graphs", true, color);
   }
}


/* handle general command */
struct node *handle_command(struct State *state) {
  if (strcmp(state->buffer, "") == 0) {
    return NULL;
  } else if (strcmp(state->buffer, "help") == 0) {
    handle_help(state->commands);
    return NULL;
  } else if (strcmp(state->buffer, "todo") == 0) {
    handle_todos();
    return NULL;
  } else if (strcmp(state->buffer, "bork") == 0) {
    printf("bork bork\n");
    fflush(stdout);
    sleep(1);
    return NULL;
  } else if (strcmp(state->buffer, "quit") == 0) {
    handle_quit(0);
  } else if (strcmp(state->buffer, "room") == 0) {
    printf("ROOM IS %s\n", state->current_room);
  } else if (strcmp(state->buffer, "items") == 0) {
    handle_items(state);
    return NULL;
  }

  struct node *curr = state->commands->head;
  while (curr) {
    if (strcmp(curr->source, state->buffer) == 0) {
      //node_dump(curr, stdout); // debug
      print(curr->string, true, CYAN);

      if (curr->item > 0 && (state->items & 1<<curr->item) == 0) {
        state->items |= 1<<curr->item;
      }
      fflush(stdout);
      sleep(1);
      return curr;
    }
    curr = curr->next;
  }

  print("That's not a valid command, you dummy! ", false, RED);
  print("Type 'help' to see what you can do here", true, GREEN);
  return NULL;
}


/* handle arrow keypress */
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

/* determine if up or down arrow */
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


/* handle backspace keypress */
void handle_backspace(struct State *state) {
  getch(stdout);
  if (state->index) {
    printf("\b\033[K");
    state->buffer[state->index--] = 0;
  }
}


/* handle enter or arrow keypress */
void handle_special_key(Status status, struct State *state) {
  if (status == UP || status == DOWN) {
    handle_arrow(status, state);
  } else if (status == ENTER) {
    handle_enter(status, state);
  }
}


/* handle arrow keypress */
void handle_arrow(Status status, struct State *state) {
  if (status == UP && state->curr_history->next) {
    state->curr_history = state->curr_history->next;
  } else if (status == DOWN && state->curr_history->prev) {
    state->curr_history = state->curr_history->prev;
  }

  // handle replacing previous command
  for (int i = 0; i < strlen(state->buffer); i++) {
    getch(stdout);
    print("\n033[K", false, NOCOLOR);
  }
  sprintf(state->buffer, state->curr_history->string);
  printf(state->buffer);
  state->index = strlen(state->buffer);
}

/* handle enter keypress */
void handle_enter(Status status, struct State *state) {
  printf("\n\n");
  struct node *transition = handle_command(state);

  if (transition) {
    //printf("state->items is %d\n", state->items);
    state->current_room = strdup(transition->target);
    state->commands = read_room(state);
  }

  list_push_back(state->history, "", "", state->buffer, 0, 0);

  memset(state->buffer, 0, strlen(state->buffer));
  state->index = 0;

  print("What would you like to do?\n> ", false, YELLOW);
}
