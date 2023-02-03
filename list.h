#ifndef __INTRUSIVE_C_LIST_H
#define __INTRUSIVE_C_LIST_H

#include <stdbool.h>

#include "container_of.h"

struct list_head {
    struct list_head *prev, *next;
};

#define list_entry(ptr, type, member) container_of(ptr, type, member)

/**
   初始化链表节点
   @head: 链表节点

   使节点的前后指针指向自己, 形成一个环形链表.
 */
static inline void list_head_init(struct list_head *head) {
    head->prev = head;
    head->next = head;
}

/**
   判断该链表为空
   @head: 链表头

   如果链表闭环, 则为空
*/
static inline bool list_empty(struct list_head *head) { return head->next == head; }

static inline void __list_add(struct list_head *e, struct list_head *p, struct list_head *n) {
    e->prev = p;
    e->next = n;
    p->next = e;
    n->prev = e;
}

static inline void list_add_tail(struct list_head *entry, struct list_head *head) {
    __list_add(entry, head->prev, head);
}

static inline void list_add_head(struct list_head *entry, struct list_head *head) {
    __list_add(entry, head, head->next);
}

static inline void __list_del(struct list_head *prev, struct list_head *next) {
    prev->next = next;
    next->prev = prev;
}

static inline void list_del_init(struct list_head *entry) {
    if (!list_empty(entry)) {
        __list_del(entry->prev, entry->next);
        list_head_init(entry);
    }
}

#define list_foreach_entry(pos, head, member)                           \
    for ((pos) = list_entry((head)->next, typeof(*(pos)), member);      \
         &(pos)->member != (head);                                      \
         (pos) = list_entry((pos)->member.next, typeof(*(pos)), member))

#endif