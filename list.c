#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

/* node definitions */
struct node * node_create(char *source, char *target, char *string, int unlock, struct node *prev, struct node *next) {
  struct node *n = malloc(sizeof(*next));
  n->source = strdup(source);
  n->target = strdup(target);
  n->string = strdup(string);
  n->unlock = unlock;
  n->next   = next;
  n->prev   = prev;
  return n;
}


struct node * node_delete(struct node *n, bool recursive) {
  struct node *n_ptr = n;
  struct node *n_next = n->next;
  free(n->string);
  free(n);
  while (recursive && n_next) {
    n_ptr = n_next;
    n_next = n_ptr->next;
    free(n_ptr->source);
    free(n_ptr->target);
    free(n_ptr->string);
    free(n_ptr);
  }
  return NULL;
}


void node_dump(struct node *n, FILE *stream) {
  if (n) {
    printf("node:\n");
    fprintf(stream, "  source: %s\n", n->source);
    fprintf(stream, "  target: %s\n", n->target);
    fprintf(stream, "  string: %s\n", n->string);
    fprintf(stream, "  unlock: %d\n", n->unlock);
  } else {
    fprintf(stream, "<null>\n");
  }
}


/* list definitions */
struct list * list_create() {
  struct list *l = malloc(sizeof(struct list));
  l->head = 0;
  l->tail = 0;
  l->size = 0;
  return l;
}


struct list * list_delete(struct list *l) {
  node_delete(l->head, 1); 
  free(l);
  return NULL;
}


struct node *list_push_front(struct list *l, char *s, char *t, char *str, int unlock) {
  struct node *n = node_create(s, t, str, unlock, NULL, l->head);
  l->head = n;
  if (l->size == 0) {
    l->tail = l->head;
  }
  if (n->next) {
    n->next->prev = n;
  }

  l->size++;
  return n;
}


void list_push_back(struct list *l, char *s, char *t, char *str, int unlock) {
  struct node *n = node_create(s, t, str, unlock, l->tail, NULL);
  l->head = n;
  if (l->size >= 1) {
    l->tail->next = n;
  } else {
    l->head = n;
  }
  if (n->prev) {
    n->prev->next = n;
  }
  l->tail = n;
  l->size++;
}

void list_dump(struct list *l, FILE *stream) {
  struct node *n_ptr = l->head;
  while (n_ptr) {
    node_dump(n_ptr, stdout);
    n_ptr = n_ptr->next;
  }
}
