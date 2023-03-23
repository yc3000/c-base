#ifndef __C_BASE_HEAP_H
#define __C_BASE_HEAP_H

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

struct heap_node {
  struct heap_node *parent, *left, *right;
};

typedef bool (*compare_fn)(const struct heap_node *l,
                           const struct heap_node *r);

struct heap_root {
  struct heap_node *root;
  int nums;
  compare_fn compare;
};

static inline void heap_root_init(struct heap_root *r, compare_fn compare) {
  r->root = NULL;
  r->nums = 0;
  r->compare = compare;
}

static inline void heap_node_init(struct heap_node *n) {
  n->parent = NULL;
  n->left = NULL;
  n->right = NULL;
}

static inline void __heap_swap(struct heap_root *hroot,
                               struct heap_node *parent,
                               struct heap_node *child) {
  struct heap_node *pparent = parent->parent;
  struct heap_node *lchild = child->left;
  struct heap_node *rchild = child->right;
  struct heap_node *brother = NULL;

  int is_child_left = 0;

  if (child == parent->left) {
    is_child_left = 1;
    brother = parent->right;
  } else {
    is_child_left = 0;
    brother = parent->left;
  }

  parent->left = lchild;
  parent->right = rchild;
  parent->parent = child;

  child->parent = pparent;
  if (is_child_left) {
    child->left = parent;
    child->right = brother;
  } else {
    child->left = brother;
    child->right = parent;
  }

  if (lchild) lchild->parent = parent;
  if (rchild) rchild->parent = parent;

  brother->parent = child;

  if (pparent) {
    if (pparent->left == parent)
      pparent->left = child;
    else
      pparent->right = child;
  } else
    hroot->root = child;
}

static inline void __heap_replace(struct heap_root *hroot,
                                  struct heap_node *olde,
                                  struct heap_node *last) {
  assert(last->parent);
  assert(last->left == NULL);
  assert(last->right == NULL);

  if (last->parent->left == last)
    last->parent->left = NULL;
  else
    last->parent->right = NULL;

  last->parent = olde->parent;
  last->left = olde->left;
  last->right = olde->right;

  olde->parent = olde->left = olde->right = NULL;

  if (last->left) last->left->parent = last;
  if (last->right) last->right->parent = last;
  if (last->parent)
    if (last->parent->left == olde)
      last->parent->left = last;
    else
      last->parent->right = last;
  else
    hroot->root = last;
}

static inline void heap_add(struct heap_root *hroot, struct heap_node *node) {
  ++hroot->nums;

  int nums = hroot->nums;

  if (nums == 1) {
    hroot->root = node;
    return;
  }

  int levels = 0;
  int sums = 1;

  while (nums > 1) {
    if (nums % 2 == 0) {
      sums *= 2;
    } else {
      sums *= 2;
      sums += 1;
    }
    nums /= 2;
    ++levels;
  }

  struct heap_node *parent;
  for (int i = 0; i < levels - 1; ++i) {
    parent = (sums % 2 == 0) ? parent->left : parent->right;
    sums /= 2;
  }

  node->parent = parent;
  if (sums % 2 == 0)
    parent->left = node;
  else
    parent->right = node;

  sums /= 2;
  assert(sums == 1);
  assert(hroot->compare);

  while (parent && hroot->compare(parent, node)) {
    __heap_swap(hroot, parent, node);
    parent = node->parent;
  }
}

static inline void heap_pop(struct heap_root *hroot) {
  if (hroot->nums == 0) return;

  if (hroot->nums == 1) {
    --hroot->nums;
    heap_node_init(hroot->root);
    hroot->root = NULL;
    return;
  }

  int nums = hroot->nums;
  int levels = 0;
  int sums = 1;

  --hroot->nums;

  while (nums > 1) {
    if (nums % 2 == 0)
      sums *= 2;
    else {
      sums *= 2;
      sums += 1;
    }
    nums /= 2;
  }

  struct heap_node *last = hroot->root;

  for (int i = 0; i < levels; ++i) {
    last = (sums % 2 == 0) ? last->left : last->right;
  }
  assert(sums == 1);

  __heap_replace(hroot, hroot->root, last);

  struct heap_node *v = last;
  struct heap_node *est = NULL;

  while (1) {
    est = v;

    if (v->left) est = hroot->compare(est, v->left) ? est : v->left;
    if (v->right) est = hroot->compare(est, v->right) ? est : v->right;

    if (est == v) break;

    heap_swap(hroot, v, est);
  }
}

#endif