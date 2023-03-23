
#include "array.h"

#include <assert.h>
#include <stdio.h>

#define SIZE 10

struct val {
  int nums[10];
};

void test_push_and_pop() {
  struct array a;
  array_init(&a);

  struct val v;

  for (int i = 0; i < 10; ++i) {
    v.nums[i] = i + 1;
  }

  int ret = array_push(&a, &v, sizeof(struct val));
  assert(ret == 0);

  while (array_size(&a)) {
    struct val popv;
    /*  即使设置成取 100, 也不会超过 */
    size_t sz = array_pop(&a, &popv, 100);
    assert(sz == sizeof(popv));

    for (int i = 0; i < SIZE; ++i) {
      printf("%d ", popv.nums[i]);
    }
  }

  array_destory(&a);
}

/* 调整动态数组的容量，例如扩容或者是把容量减少到 size */
void test_reverse() {
  struct array a;
  array_init(&a);

  /* 提前分配 10000 * sizeof(int) 空间 */
  array_reverse(&a, sizeof(int) * 10000);

  printf("cap: is %ld\n", a.capacity / sizeof(int));

  int nums[10000];
  for (int i = 0; i < 10000; ++i) {
    nums[i] = i + 1;
    int r = array_push(&a, &nums[i], sizeof(int));
    assert(r == 0);
  }

  for (int i = 0; i < 5000; ++i) {
    int val;
    size_t t = array_pop(&a, &val, sizeof(val));
    assert(t == sizeof(val));
  }

  printf("cap: is %ld\n", a.capacity / sizeof(int));

  array_reverse(&a, array_size(&a));

  printf("cap: is %ld\n", a.capacity / sizeof(int));

  array_destory(&a);
}

int main() {
  /* for (int i = 0; i < 2; ++i) { */
  /*   test_push_and_pop(); */
  /*   printf("\n"); */
  /* } */

  test_reverse();

  return 0;
}