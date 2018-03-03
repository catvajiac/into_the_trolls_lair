#include "trolls_lair.h"

/* strips newline from string */
void string_strip(char *s) {
  if (!s) {
    return;
  }
  char *c = s + strlen(s) - 1;
  if (*c == '\n') {
    *c = 0;
  }
}

/* helper function for string_translate */
int find_char_in_array(char * array, char c) {
  int count = 0;
  char *array_ptr = array;
  while (*array_ptr) {
    if (*array_ptr == c) {
      return count;
    }   
    count++, array_ptr++;
  }
  return -1; 
}

/* replaces chars in s */
char *string_translate(char *s, char *from, char *to) {
  char *s_ptr = s;
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

/* get character from stdin with changing terminal settings */
int getch(FILE *stream) {
  struct termios new_t;
  tcgetattr(STDIN_FILENO, &new_t);
  new_t.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_t); // apply new settings
  int ch = getc(stream);
  tcsetattr(STDIN_FILENO, TCSANOW, &TERM_SETS); // apply old settings
  return ch;
}

/* print function that respects a maximum width */
void print(char *s, bool newline, char color[]) {
  if (!s) {
    return;
  }
  s = strdup(s);
  string_strip(s);
  int curr_width = 0;
  char *word = strtok(s, " ");

  printf(color);
  while (word && strlen(word)) {
    while (word && curr_width + strlen(word) < WIDTH) {
      printf("%s ", word);
      curr_width += strlen(word) + 1;
      word = strtok(0, " ");
    }
    if (newline) {
      printf("\n");
    }
    curr_width = 0;
  }
  printf(NOCOLOR);
  free(s);
}

/* read room file */
struct list *read_room(struct State *state) {
  printf("\n");
  char * path = "rooms/";
  char *room = string_translate(state->current_room, " ", "_");

  char roompath[strlen(path) + strlen(room) + 1]; 
  strcpy(roompath, path);
  strcat(roompath, room);

  char buffer[BUFSIZ];
  FILE *input;

  if (!(input = fopen(roompath, "r"))) {
    return NULL;
  }

  /* read room string */
  while (fgets(buffer, BUFSIZ, input) && strcmp(buffer, "\n") != 0) {
    if (strcmp(room, "welcome") == 0) { // special, want spaces preserved
      printf("%s%s%s", GREEN, buffer, NOCOLOR);
    } else {
      print(buffer, true, GREEN);
    }
    memset(buffer, 0, BUFSIZ);
  }

  return parse_rules(input, state);
}

/* rules for moving to the next room */
struct list * parse_rules(FILE *input, struct State *state) {
  struct list *commands = list_create();
  char buffer[BUFSIZ];

  while (fgets(buffer, BUFSIZ, input)) {
    char *source   = strtok(buffer, ":");
    char *target   = strtok(0, ":");
    char *string   = strtok(0, ":");
    char *unlock_s = strtok(0, ":");
    char *item_s   = strtok(0, ":");
    if (!source || !target) {
      return commands;
    }

    int unlock;
    int item;
    string_strip(unlock_s);
    string_strip(item_s);
    if (item_s) {
      item = atoi(item_s);
    } else {
      item = -1;
    }

    if (unlock_s) {
      unlock = atoi(unlock_s);
    } else {
      unlock = -1;
    }

    if (!(state->items & 1<<item) && !(state->items & 1<<unlock)) {
      list_push_front(commands, source, target, string, unlock, item);
    }
    memset(buffer, 0, BUFSIZ);
  }

  fclose(input);
  return commands;
}
