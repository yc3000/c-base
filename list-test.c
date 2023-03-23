
#include "list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct val {
  int v;
  struct list_head head;
  void *ptr;
};

void test_list_add_and_del() {
  struct list_head head;
  list_head_init(&head);

  int ptr = 3;
  struct val *v;
  for (int i = 0; i < 1000; ++i) {
    v = malloc(sizeof(struct val));
    v->v = i;
    v->ptr = &ptr;
    list_head_init(&v->head);
    list_add_head(&v->head, &head);
  }

  struct list_head *p = head.next;
  struct list_head *next = p;
  while (p != &head) {
    p = next;
    next = p->next;

    struct val *v = list_entry(p, struct val, head);
    v->ptr = NULL;

    if (v->v % 2 == 0) {
      list_del_init(&v->head);
    }
  }

  list_for_each_entry(v, &head, head) {
    assert(v->v % 2 != 0);
    assert(v->ptr == NULL);
  }
  printf("没有异常就是最好的结果");
}

int main() { test_list_add_and_del(); }
