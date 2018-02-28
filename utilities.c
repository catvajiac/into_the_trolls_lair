#include "trolls_lair.h"

void string_strip(char *s) {
  if (!s) {
    return;
  }
  char *c = s + strlen(s) - 1;
  if (*c == '\n') {
    *c = 0;
  }
}

int find_char_in_array(char * array, char c) {
  int count = 0;
  char * array_ptr = array;
  while (*array_ptr) {
    if (*array_ptr == c) {
      return count;
    }   
    count++, array_ptr++;
  }
  return -1; 
}

char * string_translate(char *s, char *from, char *to) {
  char * s_ptr = s;
  int index;
  if (strlen(to) == 0) {
    return s;
  }
  while (*s_ptr) {
    index = find_char_in_array(from, *s_ptr);
    if (index >= 0) {
      *s_ptr = *(to + index);
    }   
    s_ptr++;
  }
  return s;
}

int getch(FILE *stream) {
  struct termios new_t;
  tcgetattr(STDIN_FILENO, &new_t);
  new_t.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_t); // apply new settings
  int ch = getc(stream);
  tcsetattr(STDIN_FILENO, TCSANOW, &TERM_SETS); // apply old settings
  return ch;
}

void print(char *s) {
  string_strip(s);
  int count = 0;
  while (strlen(s)) {
    printf("%.*s\n", WIDTH, s);
    count += WIDTH;
    s += WIDTH;
  }
}

struct list *read_room(char * room) {
  printf("\n");
  char * path = "rooms/";
  room = string_translate(room, " ", "_");

  char roompath[strlen(path) + strlen(room) + 1]; 
  strcpy(roompath, path);
  strcat(roompath, room);

  char buffer[BUFSIZ];
  FILE *input;

  if (!(input = fopen(roompath, "r"))) {
    return NULL;
  }

  while (fgets(buffer, BUFSIZ, input) && strcmp(buffer, "\n") != 0) {
    print(buffer);
    memset(buffer, 0, BUFSIZ);
  }

  return parse_rules(input);
}

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
    list_push_front(commands, source, target, string, unlock);
    memset(buffer, 0, BUFSIZ);
  }

  fclose(input);
  return commands;
}
