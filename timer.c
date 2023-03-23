#include "timer.h"

#include <assert.h>

#include "container_of.h"
#include "list.h"

#define TIMER_NODE_STATE_OK 0x1981
#define TIMER_NODE_STATE_BAD 0x2014

static void timer_internal_add(struct timer_core *core,
                               struct timer_node *node) {
  uint32_t expires = node->expires;
  uint32_t idx = expires - core->timer_jiffies;
  struct list_head *vec = NULL;

  if (idx < TIMER_VEC_ROOT_SIZE) {
    int i = expires & TIMER_VEC_ROOT_MARK;
    vec = core->tv1.vec + i;
  } else if () {
  }
  list_add_head(&node->head, vec);
  node->core = core;
}

static inline timer_internal_update(struct timer_core *core,
                                    uint32_t jiffies) {

}

void timer_core_init(struct timer_core *core, uint32_t jiffies) {
  core->timer_jiffies = jiffies;

  union {
    struct timer_vec_root *vr;
    struct timer_vec *vn;
  } rn;

  rn.vr = &core->tv1;
  core->tves[0] = rn.vn;

  core->tves[1] = &core->tv2;
  core->tves[2] = &core->tv3;
  core->tves[3] = &core->tv4;
  core->tves[4] = &core->tv5;

  for (int i = 0; i < TIMER_VEC_ROOT_SIZE; ++i) {
    list_head_init(&core->tv1.vec[i]);
  }
  for (int i = 0; i < TIMER_VEC_NODE_SIZE; ++i) {
    list_head_init(&core->tv2.vec[i]);
    list_head_init(&core->tv3.vec[i]);
    list_head_init(&core->tv4.vec[i]);
    list_head_init(&core->tv5.vec[i]);
  }
}

void timer_core_destory(struct timer_core *core) {
  for (int i = 0; i < 5; ++i) {
    int count = (i == 0) ? TIMER_VEC_ROOT_SIZE : TIMER_VEC_NODE_SIZE;
    for (int j = 0; j < count; ++j) {
      struct list_head *root = &core->tves[i]->vec[j];

      while (!list_is_empty(root)) {
        struct timer_node *node = (struct timer_node *)list_entry(
            root->next, struct timer_node, head);

        if (!list_is_empty(&node->head)) {
          list_del_init(&node->head);
        }
        node->core = NULL;
      }
    }
  }
}

void timer_core_run(struct timer_core *core, uint32_t jiffies) {
  timer_internal_update(core, jiffies);
}

void timer_node_init(struct timer_node *node, void (*fn)(void *), void *data) {
  list_head_init(&node->head);
  node->expires = 0;
  node->state = TIMER_NODE_STATE_OK;
  node->data = data;
  node->callback = fn;
  node->core = NULL;
}

void timer_node_destory(struct timer_node *node) {
  if (node->state != TIMER_NODE_STATE_OK) {
    assert(node->state == TIMER_NODE_STATE_OK);
    return;
  }

  if (!list_is_empty(&node->head)) {
    list_del_init(&node->head);
    node->core = NULL;
  }

  node->expires = 0;
  node->state = TIMER_NODE_STATE_BAD;
  node->data = NULL;
  node->callback = NULL;
  node->core = 0;
}

void timer_node_add(struct timer_core *core, struct timer_node *node,
                    uint32_t expires) {
  assert(node->state == TIMER_NODE_STATE_OK);

  if (!list_is_empty(&node->head)) {
    list_del_init(&node->head);
    node->core = NULL;
  }

  node->expires = expires;

  timer_internal_add(core, node);
}

int timer_node_del(struct timer_core *core, struct timer_node *node) {
  assert(node->state == TIMER_NODE_STATE_OK);

  if (!list_is_empty(&node->head)) {
    assert(node->core != NULL);
    list_del_init(&node->head);
    node->core = NULL;
    return 1;
  }
  return 0;
}

int timer_node_mod(struct timer_core *core, struct timer_node *node,
                   uint32_t expires) {
  int ret = timer_node_del(core, node);
  timer_node_add(core, node, expires);
  return ret;
}
