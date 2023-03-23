
#include "ringbuf.h"

#include <stddef.h>
#include <string.h>

void ringbuf_init(struct ringbuf *r, void *buf, size_t sz) {
  r->data = (unsigned char *)buf;
  r->size = sz;
  r->head = 0;
  r->tail = 0;
}

void ringbuf_destory(struct ringbuf *r) {
  r->data = 0;
  r->size = 0;
  r->head = 0;
  r->tail = 0;
}

size_t ringbuf_drop(struct ringbuf *r, size_t sz) {
  size_t used_size = ringbuf_used_size(r);
  sz = (used_size < sz) ? used_size : sz;
  r->tail += sz;
  if (r->tail > r->size) {
    r->tail -= r->size;
  }
  return sz;
}

size_t ringbuf_write(struct ringbuf *r, const void *data, size_t sz) {
  size_t free_sz = ringbuf_free_size(r);
  if (free_sz == 0) {
    return 0;
  }

  unsigned char *ptr = (unsigned char *)data;
  size_t half = r->size - r->head;
  sz = (free_sz < sz) ? free_sz : sz;

  if (half > sz) {
    memcpy(r->data + r->head, ptr, sz);
  } else {
    memcpy(r->data + r->head, ptr, half);
    memcpy(r->data, ptr + half, sz - half);
  }

  r->head += sz;
  if (r->head > r->size) {
    r->head -= r->size;
  }

  return sz;
}

size_t ringbuf_peek(struct ringbuf *r, void *data, size_t sz) {
  size_t used_sz = ringbuf_used_size(r);
  if (used_sz == 0) {
    return 0;
  }

  unsigned char *ptr = (unsigned char *)data;
  size_t half = r->size - r->tail;
  sz = (used_sz < sz) ? used_sz : sz;

  if (half > sz) {
    memcpy(ptr, r->data + r->tail, sz);
  } else {
    memcpy(ptr, r->data + r->tail, half);
    memcpy(ptr + half, r->data, sz - half);
  }

  return sz;
}

size_t ringbuf_read(struct ringbuf *r, void *data, size_t sz) {
  size_t ret = ringbuf_peek(r, data, sz);
  if (ret == 0) {
    return 0;
  }

  r->tail += ret;
  if (r->tail >= r->size) {
    r->tail -= r->size;
  }

  return sz;
}
