#ifndef _C_BASE_HASHTABLE_H
#define _C_BASE_HASHTABLE_H

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "container_of.h"

/**
   @mod: 哈希链表
*/

struct hlist_node {
  struct hlist_node **pprev;
  struct hlist_node *next;
};

struct hlist_head {
  struct hlist_node *first;
};

#define HLIST_HEAD_INITIALIZE(h) \
  { .first = NULL }
#define HLIST_HEAD(h) struct hlist_head h = HLIST_HEAD_INITIALIZE(h)

#define hlist_head_init(h) ((h)->first = NULL)

#define hlist_entry(ptr, type, member) containter_of(ptr, type, member)

static inline void hlist_node_init(struct hlist_node *node) {
  node->pprev = NULL;
  node->next = NULL;
}

static inline bool hlist_unhashed(struct hlist_node *node) {
  return !node->pprev;
}

static inline bool hlist_head_empty(struct hlist_head *head) {
  return !head->first;
}

static inline void hlist_add_head(struct hlist_node *node,
                                  struct hlist_head *head) {
  struct hlist_node *first = head->first;
  node->next = first;
  if (first) {
    first->pprev = &(node->next);
  }
  node->pprev = &head->first;
  head->first = node;
}

static inline void list_add_before(struct hlist_node *node,
                                   struct hlist_node *next) {
  // node next
  *(next->pprev) = node;
  node->pprev = next->pprev;
  node->next = next;
  next->pprev = &(node->next);
}

static inline void hlist_add_after(struct hlist_node *node,
                                   struct hlist_node *prev) {
  /* prev node */
  prev->next->pprev = &(node->next);
  node->next = prev->next;
  prev->next = node;
  node->pprev = &(prev->next);
}

static inline void __hlist_del(struct hlist_node *node) {
  *(node->pprev) = node->next;
  if (node->next) {
    node->next->pprev = node->pprev;
  }
}

static inline void hlist_del(struct hlist_node *node) {
  __hlist_del(node);
  node->pprev = NULL;
  node->next = NULL;
}

static inline void hlist_del_init(struct hlist_node *node) {
  if (!hlist_unhashed(node)) {
    __hlist_del(node);
    hlist_node_init(node);
  }
}

#define hlist_for_each(pos, head) \
  for (pos = (head)->first; pos; pos = pos->next)

/**
   @mod: 哈希表
*/

#define DECLARE_HASHTABLE(ht, bit) struct hlist_head ht[1 << (bit)]

#define __array_size(array) (sizeof(array) / sizeof(array[0]))

#define hash_size __array_size

#define __hashtable_init(ht, sz)      \
  do {                                \
    for (size_t i = 0; i < sz; ++i) { \
      hlist_head_init(&ht[i]);        \
    }                                 \
  } while (0)

#define hashtable_init(ht) __hashtable_init(ht, hash_size(ht))

static inline bool __hashtable_empty(struct hlist_head *head, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    if (hlist_head_empty(&head[i])) {
      return false;
    }
  }
  return true;
}

#define hashtable_empty(ht) __hashtable_empty(ht, hash_size(ht))

static inline bool hashtable_hashed(struct hlist_node *node) {
  return !hlist_unhashed(node);
}

static inline void hashtable_del(struct hlist_node *node) { hlist_del(node); }

/**
 *@mod: 插入节点与计算哈希因子
 */

static inline int __ilog2_u32(uint32_t n) { return fls(n) - 1; }

static inline int __ilog2_u64(uint64_t n) { return fls64(n) - 1; }

#define ilog2(n) (sizeof(n) <= 4 ? __ilog2_u32(n) : __ilog2_u64(n))

#define hash_bits(ht) ilog2(hash_size(ht))

#define GOLDEN_RATIO_32 0x61c88647
#define GOLDEN_RATIO_64 0x61C8864680B583EBull

#define __hash_32 __hash_32_generic
static inline uint32_t __hash_32_generic(uint32_t u) {
  return u * GOLDEN_RATIO_32;
}

#define hash_32 hash_32_generic
static inline uint32_t hash_32_generic(uint32_t val, unsigned int bits) {
  return __hash_32(val) >> (32 - bits);
}

#define hash_64 hash_64_generic
static inline uint32_t hash_64_generic(uint64_t val, unsigned int bits) {
  return val * GOLDEN_RATIO_64 >> (64 - bits);
}

#define hash_min(val, bits) \
  (sizeof(val) <= 4 ? hash_32(val, bits) : hash_64(val, bits))

#define hashtable_add(ht, node, key) \
  hlist_add_head(&ht[hash_min(key, hash_bits(ht)], node)

#endif
