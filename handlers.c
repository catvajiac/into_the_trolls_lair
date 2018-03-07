#include "list.h"
#include "trolls_lair.h"

bool check_todos(State *state) {
  Todos flags[] = {MEETING, ETHICS, SYSTEMS, LUNCH, SITTER};
  char *todos[] = {"meeting", "ethics", "systems", "lunch", "sitter"};
  for (int i = 0; i < length(todos); i++) {
    if (!(state->todos & flags[i])) {
      return false;
    }
  }
  return true;
}

/* clean up */
void handle_quit(int signal) {
  tcsetattr(STDIN_FILENO, TCSANOW, &TERM_SETS);
  print("\nBork, thanks for playing!", true, CYAN);
  list_delete(global_state->history);
  list_delete(global_state->commands);
  free(global_state->current_room);
  free(global_state->buffer);
  free(global_state);
  exit(signal);
}

/* handle help message */
void help(int status) {
  print("Usage: trolls_layer [-H height] [-W width] [-h]", true, GREEN);
  exit(status);
}


/* handle 'help' command */
void handle_help(List *commands) {
  print("Possible commands:", true, MAGENTA);
  Node *curr = commands->head;
  while (curr) {
    print(curr->source, true, MAGENTA);
    curr = curr->next;
  }
  fflush(stdout);
}


/* handle 'todo' command */
void handle_todos(State *state) {
  char *color;
  Todos flags[] = {MEETING, ETHICS, SYSTEMS, LUNCH, SITTER};
  char *todos[] = {"meeting", "ethics", "systems", "lunch", "sitter"};
  for (int i = 0; i < length(todos); i++) {
    color = state->todos & flags[i] ? GREEN : RED;
    print(todos[i], true, color);
  }
  fflush(stdout);
}


/* handle items */
void handle_items(State *state) {
  char *color = GREEN;
  print("Inventory:", true, color);
  if (state->items & KEYBOARD) {
    print("  keyboard", true, color);
  }
  if (state->items & GRAPHS) {
    print("  graphs", true, color);
  }
  if (state->items & STICKY) {
    print("  sticky notes", true, color);
  }
}


/* handle general command */
Node *handle_command(State *state) {
  if (streq(state->buffer, "")) {
    return NULL;
  } else if (streq(state->buffer, "help")) {
    handle_help(state->commands);
    return NULL;
  } else if (streq(state->buffer, "todo")) {
    handle_todos(state);
    return NULL;
  } else if (streq(state->buffer, "bork")) {
    printf("bork bork\n");
    fflush(stdout);
    return NULL;
  } else if (streq(state->buffer, "quit")) {
    handle_quit(0);
  } else if (streq(state->buffer, "room")) {
    printf("you are currently in the room %s\n", state->current_room);
    return NULL;
  } else if (streq(state->buffer, "items")) {
    handle_items(state);
    return NULL;
  }

  Node *curr = state->commands->head;
  while (curr) {
    if (streq(curr->source, state->buffer)) {
      print(curr->string, true, CYAN);

      if (curr->item > 0 && (state->items & 1<<curr->item) == 0) {
        state->items |= 1<<curr->item;
      }

      if (curr->todo > 0 && (state->todos & 1<<curr->todo) == 0) {
        state->todos |= 1<<curr->todo;
      }
      fflush(stdout);
      return curr;
    }
    curr = curr->next;
  }

  print("That's not a valid command, you dummy! ", false, RED);
  print("Type 'help' to see what you can do here", true, GREEN);
  return NULL;
}


/* handle arrow keypress */
Status handle_key(State *state) {
  Status return_value = REGULAR;
  int key = getch(stdin);

  switch (key) {
    case '\033': // arrow key
      return_value = decide_arrow();
      break;
    case 10: // enter
      return_value = ENTER;
      break;
    case 12: // ctrl-L
      handle_clear(state);
      break;
    case 127: // backspace
      handle_backspace(state);
      break;
    case -1: // EOF
      return_value = END;
      break;
    default:
      putc(key, stdout);
      state->buffer[state->index++] = key;
      break;
  }

  if (check_todos(state)) {
    return WIN;
  }
  return return_value;
}


/* handle ctrl-L */
void handle_clear(State *state) {
  printf("%d\n", HEIGHT);
  for (int i = 0; i < HEIGHT; i++) {
    printf("\n");
  }
}

/* determine if up or down arrow */
Status decide_arrow() {
  Status return_status;
  int ch = getch(stdin);
  ch = getch(stdin);
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
void handle_backspace(State *state) {
  if (state->index) {
    printf("\b\033[K");
    state->buffer[state->index--] = 0;
  } else {
    state->curr_history = state->history->head;
  }
}


/* handle enter or arrow keypress */
void handle_special_key(Status status, State *state) {
  if (status == UP || status == DOWN) {
    handle_arrow(status, state);
  } else if (status == ENTER) {
    handle_enter(status, state);
  }
}


/* handle arrow keypress */
void handle_arrow(Status status, State *state) {
  if (status == UP && state->curr_history->next) {
    state->curr_history = state->curr_history->next;
  } else if (status == DOWN && state->curr_history->prev) {
    state->curr_history = state->curr_history->prev;
  } else {
    return;
  }

  // handle replacing previous command
  for (int i = 0; i < strlen(state->buffer); i++) {
    printf("\b\033[K");
  }
  sprintf(state->buffer, state->curr_history->string);
  printf(state->buffer);
  state->index = strlen(state->buffer) - 1;
}


/* handle enter keypress */
void handle_enter(Status status, State *state) {
  printf("\n\n");
  Node *transition = handle_command(state);

  if (transition) {
    state->current_room = strdup(transition->target);
    state->commands = read_room(state);
  }

  list_insert(state->history, "", "", state->buffer, 0, 0, 1, 0);

  memset(state->buffer, 0, strlen(state->buffer));
  state->index = 0;

  print("What would you like to do?\n> ", false, YELLOW);
}
