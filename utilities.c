#include <string.h>

void string_strip(char *s) {
  if (!s) {
    return;
  }

  char *c = s;
  while (c && *c != '\n') {
    c++;
  }
  *c = 0;
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
