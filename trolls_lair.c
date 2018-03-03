#include "trolls_lair.h"
#include "list.h"

Todos todos;

int main(int argc, char *argv[]) {
  /* parse command line args */
  WIDTH = 80;
  HEIGHT = 100;
  int arg = 1;
  while (arg < argc) {
    printf("%s\n", argv[arg]);
    if (strlen(argv[arg]) != 2 || argv[arg][0] != '-') {
      printf("gimme good args bruh\n");
      help(1);
    }
    switch (argv[arg][1]) {
      case 'W':
        WIDTH = atoi(argv[++arg]);
        break;
      case 'H':
        HEIGHT = atoi(argv[++arg]); 
        break;
      case 'h':
        help(0);
        break;
    }
    arg++;
  }

  tcgetattr(STDIN_FILENO, &TERM_SETS); // terminal settings
  signal(SIGINT, handle_quit);
  signal(SIGHUP, handle_quit);

  /* intialize status: contains game state info */
  Status status;
  struct State *state = malloc(sizeof(struct State));
  global_state = state; // used for cleanup only
  state->history = list_create();
  state->commands = list_create();
  state->buffer = malloc(BUFSIZ);
  list_push_front(state->history, "", "", "", 0, 0);

  state->current_room = strdup("welcome");
  read_room(state);
  state->current_room = strdup("premise");
  read_room(state);
  state->current_room = strdup("bui chair");
  state->commands = read_room(state);
  print("What would you like to do?\n> ", false, YELLOW);
  sleep(1);

  while (status != END) {
    while ((status = handle_key(state)) == REGULAR) {}
    handle_special_key(status, state);
  }
}
