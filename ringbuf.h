#ifndef __INTRUSIVE_C_RINGBUF_H
#define __INTRUSIVE_C_RINGBUF_H

#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

struct ringbuf {
    unsigned char *data;
    size_t size; // 缓存分配的大小
    size_t head; //
    size_t tail;
};

/* 大小 */
static inline size_t __ringbuf_size(struct ringbuf *r) {
    return r->size;
}

/* ringbuf 使用的容量 */
static inline size_t __ringbuf_used(struct ringbuf *r) {
    if (r->tail >= r->head)
        return r->tail - r->head;
    else
        return r->size - (r->head - r->tail);
}

/* ringbuf 剩余容量 */
static inline size_t __ringbuf_capacity(struct ringbuf *r) {
    size_t used = __ringbuf_used(r);
    return r->size - used;
}

/* 当 ringbuf 左右两端有容量时, 计算左边端的容量 */
static inline size_t __ringbuf_left_capacity(struct ringbuf *r) {
    assert(r->tail > r->head);
    return r->head + 1 - 0; // [)  r->head + 1 是右开闭, 减去 0 下标
}

/* 当 ringbuf 左右两端有容量时, 计算右边端的容量 */
static inline size_t __ringbuf_right_capacity(struct ringbuf *r) {
    assert(r->tail > r->head);
    return r->size - r->head;
}

/**
   把数据写入到环型缓存
   @r:    环形缓存
   @data: 外部缓存
   @sz:   外部缓存大小

   会发生数据的拷贝, 会把外部 buf 拷贝进来. @data 不能为空, 因为写入空字节没有意义.
 */
static inline size_t ringbuf_write(struct ringbuf *r, const void *data, size_t sz) {
    assert(data);

    const char *lptr = (const char *)data;
    size_t capacity = __ringbuf_capacity(r);
    size_t right_capacity = __ringbuf_right_capacity(r);

    size_t write_sz = (sz > capacity) ? capacity : sz;

    if (r->head >= r->tail) {
        memcpy(r->data + r->tail, lptr, write_sz);
    }
    else {
        memcpy(r->data + r->tail, lptr, right_capacity);
        memcpy(r->data + r->head, lptr + right_capacity, write_sz - right_capacity);
    }

    r->tail += write_sz;
    r->tail %= r->size;

    return write_sz;
}

/**
   从环型缓存读取数据
   @r:    环形缓存
   @data: 外部缓存
   @sz:   外部缓存大小

   会发生数据的拷贝. @data 不能为空, 因为函数尤其职责, 就是从 ringbuf 读取数据
 */
static inline size_t ringbuf_read(struct ringbuf *r, void *data, size_t sz) {
    assert(data);
    size_t read_sz = sz;
    size_t used_sz = __ringbuf_used(r);

    read_sz = (sz > used_sz) ? used_sz : sz;

    return read_sz;
}

/**
   从环形缓存预览数据, 而不改变 ringbuf 的 tail 指针
   @r:    环形缓存
   @data: 外部缓存
   @sz:   外部缓存大小

   会发生数据的拷贝, 会把外部 buf 拷贝进来. @data 不能为空, 因为写入空字节没有意义.
 */
static inline size_t ringbuf_preview(struct ringbuf *r, const void *data, size_t sz) {}

#endif
