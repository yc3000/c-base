
#include "array.h"

#include <stdlib.h>
#include <string.h>

void array_init(struct array *a) {
  a->data = NULL;
  a->size = 0;
  a->capacity = 0;
}

void array_destory(struct array *a) {
  if (a->capacity > 0) {
    free(a->data);
  }
  a->data = NULL;
  a->size = 0;
  a->capacity = 0;
}

static int array_resize(struct array *a, size_t newsize);
static int array_capacity(struct array *a, size_t newcap);

int array_push(struct array *a, const void *data, size_t sz) {
  size_t current = a->size;
  if (array_resize(a, sz + current) == -1) {
    return -1;
  }
  memcpy(a->data + current, data, sz);
  return 0;
}

size_t array_pop(struct array *a, void *data, size_t sz) {
  size_t minsize = (sz > a->size) ? a->size : sz;
  memcpy(data, a->data + a->size - minsize, minsize);
  a->size -= minsize;

  return minsize;
}

static int array_capacity(struct array *a, size_t newcap) {
  if (newcap == a->capacity) {
    return 0;
  }

  if (newcap == 0) {
    if (a->capacity > 0) {
      free(a->data);
    }
    a->data = NULL;
    a->size = 0;
    a->capacity = 0;
    return 0;
  }

  unsigned char *ptr = (unsigned char *)malloc(newcap);
  if (ptr == NULL) {
    return -1;
  }
  /* 这里要考虑第一次分配内存, 所以 a->data 是空的, 无法对其进行 memcpy 和 free
   * 操作 */
  if (a->data) {
    size_t minsize = (newcap < a->size) ? newcap : a->size;
    if (minsize > 0 && a->data) {
      memcpy(ptr, a->data, minsize);
    }
    free(a->data);
  }
  a->data = ptr;
  a->capacity = newcap;
  if (a->size > a->capacity) {
    a->size = a->capacity;
  }

  return 0;
}

static int array_resize(struct array *a, size_t newsize) {
  if (newsize > a->capacity) {
    size_t capacity = a->capacity * 2;
    if (capacity < newsize) {
      capacity = sizeof(char *);
      while (capacity < newsize) {
        capacity *= 2;
      }
    }
    if (array_capacity(a, capacity) == -1) {
      return -1;
    }
  }
  a->size = newsize;
  return 0;
}

int array_reverse(struct array *a, size_t sz) {
  if (sz > a->size) {
    return array_capacity(a, sz);
  } else {
    return array_capacity(a, a->size);
  }
}