#include "trolls_lair.h"
#include "list.h"


int main(int argc, char *argv[]) {
  /* parse command line args */
  WIDTH = 80;
  HEIGHT = 45;
  int arg = 1;
  while (arg < argc) {
    if (strlen(argv[arg]) != 2 || argv[arg][0] != '-') {
      printf("gimme good args bruh\n");
      help(1);
    }
    switch (argv[arg][1]) {
      case 'W':
        WIDTH = atoi(argv[++arg]);
        break; case 'H':
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
  State *state = malloc(sizeof(State));
  global_state = state; // used for cleanup only
  state->history = list_create();
  state->commands = list_create();
  state->buffer = malloc(BUFSIZ);
  list_push_front(state->history, "", "", "", 0, 0, 0);
  state->curr_history = state->history->head;

  state->current_room = strdup("welcome");
  read_room(state);
  state->current_room = strdup("premise");
  read_room(state);
  cowsay("eyes", "good luck...");
  state->current_room = strdup("bui chair");
  state->commands = read_room(state);
  print("What would you like to do?\n> ", false, YELLOW);

  while (status != END && status != WIN) {
    while ((status = handle_key(state)) == REGULAR) {}
    handle_special_key(status, state);
  }

  if (status == WIN) {
    printf("Congratulations!\n");
  }

  handle_quit(0);
  return EXIT_SUCCESS;
}
