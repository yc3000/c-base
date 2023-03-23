#ifndef _C_BASE_ARRAY_H
#define _C_BASE_ARRAY_H

#include <stddef.h>

/**
 * @file: 自动扩容的动态数组
 */

struct array {
  unsigned char *data;
  size_t size;
  size_t capacity;
};

/**
 *  @mod: Memory Function
 */

#define array_capacity(v) ((v)->capacity)
#define array_size(v) ((v)->size)
#define array_data(v) ((v)->data)

/**
 * @mod: 用户接口
 */

/**
 *  @array_init - 初始化结构体
 *
 *  因为作为对象的内置 POD，所以没有动态分配该对象的接口
 */
void array_init(struct array *a);

/**
 *  @array_destory - Destory
 */
void array_destory(struct array *a);

/**
 *  @array_reverse - 给数组分配内存
 *
 *  可以提前分配内存或者, 把过多的内存释放掉, 但是不能使容量低于 size,
 *  这可能丢失数据.
 */
int array_reverse(struct array *a, size_t sz);

/**
 *  @array_push - 把数据从数组末尾插入
 */
int array_push(struct array *a, const void *data, size_t sz);

/**
 *  @array_pop - 把数组末尾的数据取出
 *
 *  返回取的数据字节大小
 */
size_t array_pop(struct array *a, void *data, size_t sz);

/**
 * @array_insert - 在任意位置插入数据
 */
int array_insert(struct array *a, size_t pos, void *data, size_t sz);

/**
 * @array_erase - 在任意位置删除数据
 */
int array_erase(struct array *a, size_t pos, size_t sz);

#endif
