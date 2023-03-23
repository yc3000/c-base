#ifndef _C_BASE_TIMER_H
#define _C_BASE_TIMER_H

#include <stdint.h>

#include "list.h"

/**
 * @file - 模仿 linux kernel 实现时间轮算法，用来做服务器的定时器，*
 * 可以同时调度 10W+ 个任务。
 */

#define TIMER_VEC_NODE_BITS 6
#define TIMER_VEC_ROOT_BITS 8
#define TIMER_VEC_NODE_SIZE (1 << TIMER_VEC_NODE_BITS)
#define TIMER_VEC_ROOT_SIZE (1 << TIMER_VEC_ROOT_BITS)
#define TIMER_VEC_NODE_MARK (TIMER_VEC_NODE_SIZE - 1)
#define TIMER_VEC_ROOT_MARK (TIMER_VEC_ROOT_SIZE - 1)

struct timer_vec {
  struct list_head vec[TIMER_VEC_NODE_SIZE];
};

struct timer_vec_root {
  struct list_head vec[TIMER_VEC_ROOT_SIZE];
};

struct timer_core {
  uint32_t timer_jiffies;
  struct timer_vec *tves[6];
  struct timer_vec_root tv1;
  struct timer_vec tv2;
  struct timer_vec tv3;
  struct timer_vec tv4;
  struct timer_vec tv5;
};

struct timer_node {
  struct list_head head;
  uint32_t expires;
  uint32_t state;
  void *data;
  void (*callback)(void *data);
  struct timer_core *core;
};

/**
 * @timer_core_init - 初始化 timer_core
 */
void timer_core_init(struct timer_core *core, uint32_t jiffies);

/**
 * @timer_core_destory - 破坏 timer_core
 */
void timer_core_destory(struct timer_core *core);

/**
 * @timer_core_run -
 */
void timer_core_run(struct timer_core *core, uint32_t jiffies);

void timer_node_init(struct timer_node *node, void (*fn)(void *), void *data);

void timer_node_destory(struct timer_node *node);

void timer_node_add(struct timer_core *core, struct timer_node *node,
                    uint32_t expires);

int timer_node_del(struct timer_core *core, struct timer_node *node);

int timer_node_mod(struct timer_core *core, struct timer_node *node,
                    uint32_t expires);

#endif