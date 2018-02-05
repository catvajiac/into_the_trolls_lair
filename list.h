#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

struct node {
  char *source;
  char *target;
  char *string;
  int unlock;
  struct node *next;
};

struct list {
  struct node *head;
  struct node *tail;
  size_t       size;
};

/* node prototypes */
struct node * node_create(char *, char *, char *, int, struct node *);
struct node * node_delete(struct node *, bool);
void node_dump(struct node *, FILE *);

/* list prototypes */
struct list * list_create();
struct list * list_delete(struct list *);
void list_push_front(struct list *, struct node *);
void list_push_back(struct list *, struct node *);
void list_dump(struct list *, FILE *);

#endif
