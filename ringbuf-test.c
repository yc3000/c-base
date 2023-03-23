
#include "ringbuf.c"

#include <assert.h>
#include <stdio.h>

void print_size_and_cap(struct ringbuf *r) {
  int free_sz = ringbuf_free_size(r) / sizeof(int);
  int used_sz = ringbuf_used_size(r) / sizeof(int);
  printf("剩余容量 ： %d, 使用的容量： %d\n", free_sz, used_sz);
}

void test_read_and_write() {
  int data[1024];

  struct ringbuf r;
  ringbuf_init(&r, data, sizeof(int) * 1024);

  for (int i = 0; i < 1000; ++i) {
    size_t a = ringbuf_write(&r, &i, sizeof(int));
    if (a == 0) {
      printf("buffer used size is nothing\n");
      break;
    }
  }

  print_size_and_cap(&r);

  for (int i = 0; i < 5000; ++i) {
    int val;
    size_t ret = ringbuf_read(&r, &val, sizeof(val));
    if (ret == 0) break;
    assert(ret == sizeof(int));
    assert(val == i);
  }

  print_size_and_cap(&r);

  for (int i = 0; i < 1000; ++i) {
    size_t a = ringbuf_write(&r, &i, sizeof(int));
    if (a == 0) {
      printf("buffer used size is nothing\n");
      break;
    }
  }

  print_size_and_cap(&r);

  ringbuf_destory(&r);
}

int main() { test_read_and_write(); }
