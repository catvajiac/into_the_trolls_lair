#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "trolls_layer.h"
#include "list.h"

Todos todos;

struct list * parse_rules(FILE *input) {
  struct list *commands = list_create();
  char buffer[BUFSIZ];

  while (fgets(buffer, BUFSIZ, input)) {
    char *source   = strtok(buffer, ":");
    char *target   = strtok(0, ":");
    char *string   = strtok(0, ":");
    char *unlock_s = strtok(0, ":");
    if (!source || !target) {
      return commands;
    }
    int unlock;
    string_strip(unlock_s);
    if (!unlock_s) {
      unlock = 0;
    } else {
      unlock = atoi(unlock_s);
    }
    struct node* entry = node_create(source, target, string, unlock, NULL);
    list_push_back(commands, entry);
    memset(buffer, 0, BUFSIZ);
  }

  fclose(input);
  return commands;
}

struct list *read_room(char * room) {

  printf("\n");
  char * path = "rooms/";

  char roompath[strlen(path) + strlen(room) + 1];
  strcpy(roompath, path);
  strcat(roompath, room);

  char buffer[BUFSIZ];
  FILE *input;
  
  if (!(input = fopen(roompath, "r"))) {
    return NULL;
  }

  while (fgets(buffer, BUFSIZ, input) && strcmp(buffer, "\n") != 0) {
    printf("%s", buffer);
    memset(buffer, 0, BUFSIZ);
  }

  return parse_rules(input);
}

int main() {
  signal(SIGINT, handle_quit);
  signal(SIGHUP, handle_quit);

  read_room("welcome");
  read_room("premise");

  char *current_room = "bui_chair";
  struct list *commands = read_room("bui_chair");
  printf("What would you like to do?\n> ");

  char buffer[BUFSIZ];
  while(fgets(buffer, BUFSIZ, stdin)) {
    string_strip(buffer);

    char *transition = handle_command(commands, buffer);
    if (transition) {
      current_room = strdup(transition);
    }

    string_translate(current_room, " ", "_");
    struct list* new_commands = read_room(current_room);
    if (new_commands) {
      commands = new_commands;
    }
    printf("What would you like to do?\n> ");
  }

  return EXIT_SUCCESS;
}
