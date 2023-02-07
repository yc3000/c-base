#ifndef __INTRUSIVE_C_HEAP_H
#define __INTRUSIVE_C_HEAP_H

#include <assert.h>

/**
   @file: 侵入式的最小堆

   其实最小堆是教科书上用顺序表 (数组) 实现的代表.

   @todo: 未完
*/

/* 结构 */
struct heap_node {
	struct heap_node *parent, *left, *right;
};
struct heap_head {
	struct heap_node *root;
};

/* 初始化*/
#define heap_head_init(h) ((h)->root = 0)
#define heap_node_init(n) ((n)->parent = (n)->left = (n)->right = 0)

/**
   取最小堆的头节点
   @head: 头

   最小堆的核心功能, 取头节点(最高优先级)
*/
#define heap_first(head) ((head)->root)

/* 用新节点取代旧节点 */
static inline void __heap_replace(struct heap_head *head,
				  struct heap_node *oldn,
				  struct heap_node *newn)
{
	struct heap_node *parent, *left, *right;
	parent = oldn->parent;
	left = oldn->left;
	right = oldn->right;

	if (parent) {
		if (parent->left == oldn)
			parent->left = newn;
		else
			parent->right = newn;
	} else {
		head->root = newn;
	}
	if (left)
		left->parent = newn;
	if (right)
		right->parent = newn;

	newn->parent = parent;
	newn->left = left;
	newn->right = right;

	oldn->left = oldn->right = oldn->parent = 0;
}

/* 父子节点进行交换 */
static inline void __heap_swap(struct heap_head *head, struct heap_node *parent,
			       struct heap_node *child)
{
	assert(parent);
	struct heap_node *pparent = parent->parent;
	struct heap_node *lchild = child->left;
	struct heap_node *rchild = child->right;
	struct heap_node *other_child;
	if (parent->left == child)
		other_child = parent->right;
	else
		other_child = parent->left;

	child->parent = pparent;
	if (child->parent == 0) {
		head->root = child;
	}
	parent->left = lchild;
	parent->right = rchild;

	if (child == parent->left) {
		child->left = parent;
		child->right = other_child;
	} else {
		child->right = parent;
		child->left = other_child;
	}
	parent->parent = child;

	other_child->parent = child;
	if (lchild)
		lchild->parent = parent;
	if (rchild)
		rchild->parent = parent;
	if (pparent) {
		if (pparent->left == parent)
			pparent->left = child;
		else
			pparent->right = child;
	}
}

/**
   将节点插入到最小堆中
   @head: 头
   @node: 节点
*/
static inline void heap_add(struct heap_head *head, struct heap_node *node)
{
}

/**
   将节点移除最小堆并初始化
   @head: 头
   @node: 节点
*/
static inline void heap_del_init(struct heap_node *node)
{
}

/**
   移除最小堆的头节点并初始化
   @node: 节点

   对 @heap_del_init 的封装，最常用的接口, 也是最小堆的核心功能,
   把优先级高的集中在头部处理.
*/
static inline void heap_del_head_init(struct heap_node *node)
{
	heap_del_init(node);
}

#endif
