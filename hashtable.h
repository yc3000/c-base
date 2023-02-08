#ifndef __INTRUSIVE_C_HASHTABLE_H
#define __INTRUSIVE_C_HASHTABLE_H

#include "hlist.h"
#include "hash.h"

/**
   声明一个哈希表
   @name: 哈希表的名字
   @bits: 用于设置哈希数组的大小

   本质是封装一个长度固定的静态数组作为桶数组, 数组元素是链表.
*/
#define DECLARE_HASHTABLE(name, bits) struct hlist_head name[1 << (bits)]

static inline void __hash_init(struct hlist_head *hashtable, unsigned int sz) {
    for (unsigned int i = 0; i < sz; ++i) hlist_head_init(&hashtable[i]);
}

/**
  初始化哈希表
  @hashtable: 哈希数组

  哈希数组必须是静态数组, see @DECLARE_HASHTABLE
 */
#define hash_init(hashtable) __hash_init(hashtable, hash_size(hashtable))

/**
   插入节点到哈希表
   @hashtable: 哈希数组
   @node:节点
   @key: 关键词
 */
#define hash_add(hashtable, node, key) hlist_add_head((node), &((hashtable)[hash_min(key, hash_bits(hashtable))]))

/**
   删除节点, 从哈希表中
   @node: &struct hlist_node 哈希节点
 */
#define hash_del(node) hlist_del_init(node)

/**
   该对象是否处在任何哈希表中
   @node: &struct hlist_node 哈希节点
*/
#define hash_hashed(node) (!hlist_unhash(node))

#endif