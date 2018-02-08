#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "list.h"
#include "trolls_layer.h"

Todos todos;

void handle_quit(int signal) {
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
}

char *handle_command(struct list *commands, char *command) {
  if (strcmp(command, "help") == 0) {
    handle_help(commands);
    return NULL;
  } else if (strcmp(command, "todo") == 0) {
    handle_todos();
    return NULL;
  } else if (strcmp(command, "bork") == 0) {
    printf("bork bork");
    return NULL;
  } else if (strcmp(command, "quit") == 0) {
    handle_quit(0);
  }

  struct node *curr = commands->head;
  while (curr) {
    if (strcmp(curr->source, command) == 0) {
      printf(curr->string);
      sleep(1);
      return curr->target;
    }
    curr = curr->next;
  }

  printf("That's not a valid command, you dummy! ");
  printf("Type 'help' to see what you can do here\n");
  return NULL;
}
