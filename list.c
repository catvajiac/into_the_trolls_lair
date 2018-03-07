#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

/* node definitions */
Node * node_create(char *s, char *t, char *str, int unlock, int item, int todo, Node *prev, Node *next) {
  Node *n = malloc(sizeof(*next));
  n->source = strdup(s);
  n->target = strdup(t);
  n->string = strdup(str);
  n->unlock = unlock;
  n->item   = item;
  n->todo   = todo;
  n->next   = next;
  n->prev   = prev;
  return n;
}


Node * node_delete(Node *n, bool recursive) {
  Node *n_ptr = n;
  Node *n_next = n->next;
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


void node_dump(Node *n, FILE *stream) {
  if (n) {
    printf("node:\n");
    fprintf(stream, "  source: %s\n", n->source);
    fprintf(stream, "  target: %s\n", n->target);
    fprintf(stream, "  string: %s\n", n->string);
    fprintf(stream, "  unlock: %d\n", n->unlock);
    fprintf(stream, "  item  : %d\n", n->item);
  } else {
    fprintf(stream, "<null>\n");
  }
}


/* list definitions */
List * list_create() {
  List *l = malloc(sizeof(List));
  l->head = 0;
  l->tail = 0;
  l->size = 0;
  return l;
}


List * list_delete(List *l) {
  node_delete(l->head, 1); 
  free(l);
  return NULL;
}


Node *list_push_front(List *l, char *s, char *t, char *str, int unlock, int item, int todo) {
  Node *n = node_create(s, t, str, unlock, item, todo, NULL, l->head);
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


void list_push_back(List *l, char *s, char *t, char *str, int unlock, int item, int todo) {
  Node *n = node_create(s, t, str, unlock, item, todo, l->tail, NULL);
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

void list_insert(List *l, char *s, char *t, char *str, int unlock, int
item, int todo, int position) {
  if (position > l->size) {
    printf("no\n");
    return;
  }

  Node *n = node_create(s, t, str, unlock, item, todo, NULL, NULL);
  Node *prev = 0;
  Node *curr = l->head;
  int count = 0;

  while (curr && count < position) {
    count++;
    prev = curr;
    curr = curr->next;
  }

  if (prev) {
    prev->next = n;
  }
  n->prev = prev;
  if (curr) {
    curr->prev = n;
  }
  n->next = curr;
}

void list_dump(List *l, FILE *stream) {
  Node *n_ptr = l->head;
  while (n_ptr) {
    node_dump(n_ptr, stdout);
    n_ptr = n_ptr->next;
  }
}
