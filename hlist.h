#ifndef __INTRUSIVE_C_HLIST_H
#define __INTRUSIVE_C_HLIST_H

#include <stdbool.h>

#include "container_of.h"

struct hlist_node {
    struct hlist_node **pprev, *next;
};

struct hlist_head {
    struct hlist_node *first;
};

#define hlist_entry(ptr, type, member) container_of(ptr, type, member)

#define hlist_head_init(h) ((h)->first = 0)
#define hlist_node_init(n) ((n)->pprev = 0, (n)->next = 0)

#define hlist_empty(h) ((h)->first == 0)
#define hlist_unhash(n) ((n)->pprev == 0)

static inline void hlist_add_head(struct hlist_node *entry, struct hlist_head *head) {
    entry->next = head->first;
    entry->pprev = &head->first;
    if (head->first != 0)
        head->first->pprev = &entry->next;
    head->first = entry;
}

static inline void __hlist_del(struct hlist_node *entry) {
    *entry->pprev = entry->next;
    if (entry->next)
        entry->next->pprev = entry->pprev;
}

static inline void hlist_del_init(struct hlist_node *entry) {
    if (!hlist_unhash(entry)) {
        __hlist_del(entry);
        hlist_node_init(entry);
    }
}

#endif