#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "list.h"

void handle_quit(int signal) {
  printf("\nBork, thanks for playing!\n");
  exit(signal);
}

void handle_help(struct list *commands) {
  printf("Possible commands:\n");
  struct node *curr = commands->head;
  while (curr) {
    printf("    %s %s\n", curr->source, curr->target);
    curr = curr->next;
  }
}

char *handle_command(struct list *commands, char *command, char *arg) {
  
  if (!arg) {
    printf("I can't %s there, try a different argument\n", command);
    return "";
  }

  if (strcmp(command, "help") == 0) {
    handle_help(commands);
    return "";
  } else if (strcmp(command, "quit") == 0) {
    handle_quit(0);
  }

  struct node *curr = commands->head;
  while (curr) {
    if (strcmp(curr->source, command) == 0 && strcmp(curr->target, arg) == 0) {
      return curr->target;
    }
    curr = curr->next;
  }

  printf("That's not a valid command, you dummy! ");
  printf("Type 'help' to see what you can do here\n");
  return "";
}
