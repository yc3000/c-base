#ifndef _C_BASE_RINGBUF_H
#define _C_BASE_RINGBUF_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @file - 实现环形缓冲区
 */

struct ringbuf {
  unsigned char *data;
  size_t size;
  size_t head;
  size_t tail;
};

/**
 * @ringbuf_size - 返回缓冲区的字节
 */
#define ringbuf_size(r) ((r)->size)

/**
 * @ringbuf_used_size - 返回缓冲区的已经使用的字节
 */
#define ringbuf_used_size(r)                          \
  (((r)->head >= (r)->tail) ? ((r)->head - (r)->tail) \
                            : (((r)->size - (r)->tail) + (r)->head))

/**
 * @ringbuf_free_size - 返回缓冲区的未使用空闲空间的字节
 */
#define ringbuf_free_size(r) ((r)->size - ringbuf_used_size(r) - 1)

/**
 * @ringbuf_init - 初始化节点
 *
 * 内部不分配缓存区, 而是由外部提供进来.
 */
void ringbuf_init(struct ringbuf *r, void *buf, size_t sz);

/**
 * @ringbuf_destory - Destory
 */
void ringbuf_destory(struct ringbuf *r);

/**
 * @ringbuf_write - 把外部数据写入 ringbuf
 *
 * 返回写入的字节
 */
size_t ringbuf_write(struct ringbuf *r, const void *data, size_t sz);

/**
 * @ringbuf_read -  读取 ringbuf
 *
 * 返回读取的字节
 */
size_t ringbuf_read(struct ringbuf *r, void *data, size_t sz);

/**
 * @ringbuf_peek - 从缓冲区读取数据, 但是不清除数据
 */
size_t ringbuf_peek(struct ringbuf *r, void *data, size_t sz);

/**
 * @ringbuf_drop - 释放缓存区的数据
 */
size_t ringbuf_drop(struct ringbuf *r, size_t sz);

/**
 * @ringbuf_clear - 清空缓存区的数据
 */
void ringbuf_clear(struct ringbuf *r);

size_t ringbuf_flat(struct ringbuf *r, void **pointer);

#endif
