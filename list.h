#ifndef _C_BASE_LIST_H
#define _C_BASE_LIST_H

#include <stdbool.h>
#include <stddef.h>

#include "container_of.h"

/**
   @mod - 内核侵入式链表

   双向循环链表，可以实现任意节点 O(1) 插入和删除
*/
struct list_head {
  struct list_head *prev, *next;
};

#define list_entry(ptr, type, member) container_of(ptr, type, member)

static inline void list_head_init(struct list_head *head) {
  head->prev = head;
  head->next = head;
}

static inline void __list_add(struct list_head *node, struct list_head *prev,
                              struct list_head *next) {
  node->prev = prev;
  node->next = next;
  next->prev = node;
  prev->next = node;
}

static inline void __list_del(struct list_head *prev, struct list_head *next) {
  prev->next = next;
  next->prev = prev;
}

static inline void __list_del_entry(struct list_head *entry) {
  __list_del(entry->prev, entry->next);
}

static inline void list_add_head(struct list_head *entry,
                                 struct list_head *head) {
  __list_add(entry, head, head->next);
}

static inline void list_add_tail(struct list_head *entry,
                                 struct list_head *head) {
  __list_add(entry, head->prev, head);
}

static inline void list_del_init(struct list_head *entry) {
  __list_del_entry(entry);
  list_head_init(entry);
}

#define list_for_each(pos, head) \
  for ((pos) = (head)->next; (pos) != (head); (pos) = (pos)->next)

#define list_for_each_entry(iter, head, member)                  \
  for ((iter) = list_entry((head)->next, typeof(*iter), member); \
       &(iter)->member != (head);                                \
       (iter) = list_entry(iter->member.next, typeof(*iter), member))

static inline void list_replace(struct list_head *olde,
                                struct list_head *newe) {
  newe->prev = olde->prev;
  newe->next = olde->next;
  newe->prev->next = newe;
  newe->next->prev = newe;
}

/**
   @list_is_empty - 判断链表为空
   @head: 链表
*/
static inline bool list_is_empty(struct list_head *head) {
  return head->next == head;
}

static inline void __list_splice(struct list_head *list,
                                 struct list_head *head) {
  struct list_head *first = list->next;
  struct list_head *last = list->prev;
  struct list_head *head_next = head->next;

  first->prev = head;
  head->next = first;

  last->next = head_next;
  head_next->prev = last;
}

/**
   @list_splice - 合并链表
*/
static inline void list_splice(struct list_head *list, struct list_head *head) {
  if (!list_is_empty(list)) __list_splice(list, head);
}

/**
   @list_splice_init - 合并链表并且初始化
*/
static inline void list_splice_init(struct list_head *list,
                                    struct list_head *head) {
  list_splice(list, head);
  list_head_init(list);
}

/**
   @mod: 内核哈希链表
*/
struct hlist_node {
  struct hlist_node **pprev, *next;
};

struct hlist_head {
  struct hlist_node *first;
};

#define hlist_node_entry(ptr, type, member) container_of(ptr, type, member)

static inline void hlist_node_init(struct hlist_node *node) {
  node->pprev = NULL;
  node->next = NULL;
}

static inline void hlist_add_head(struct hlist_head *head,
                                  struct hlist_node *node) {
  node->next = head->first;
  node->pprev = &(head->first);
  head->first->pprev = &node->next;
  head->first = node;
}

static inline void hlist_del_init(struct hlist_node *node) {
  *(node->pprev) = node->next;
  if (node->next) {
    node->next->pprev = node->pprev;
  }
  hlist_node_init(node);
}

#endif
