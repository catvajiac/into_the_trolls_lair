#include "trolls_lair.h"
#include "list.h"

Todos todos;

int main() {
  tcgetattr(STDIN_FILENO, &TERM_SETS);
  signal(SIGINT, handle_quit);
  signal(SIGHUP, handle_quit);

  Status status;
  struct State * state = malloc(sizeof(struct State));
  state->history = list_create();
  state->commands = list_create();
  state->current_room = strdup("bui chair");
  state->buffer = malloc(BUFSIZ);
  list_push_front(state->history, "", "", "", 0); // push dummy node

  read_room("welcome");
  read_room("premise");
  state->commands = read_room(state->current_room);
  printf("What would you like to do?\n> ");

  while (status != END) {
    while ((status = handle_key(state)) == REGULAR) {}
    handle_special_key(status, state);
  }
}
