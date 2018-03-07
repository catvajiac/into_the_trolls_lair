#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdio.h>

typedef struct Node {
  char *source;
  char *target;
  char *string;
  int unlock;
  int item;
  int todo;
  struct Node *next;
  struct Node *prev;
} Node;

typedef struct {
  Node *head;
  Node *tail;
  size_t size;
} List;

/* node prototypes */
Node *node_create(char *, char *, char *, int, int, int, Node *, Node*);
Node * node_delete(Node *, bool);
void node_dump(Node *, FILE *);

/* list prototypes */
List *list_create();
List *list_delete(List *);
Node *list_push_front(List *, char *, char *, char *, int, int, int);
void list_push_back(List *, char *, char *, char *, int, int, int);
void list_insert(List *, char *, char *, char *, int, int, int, int);
void list_dump(List *, FILE *);

#endif
