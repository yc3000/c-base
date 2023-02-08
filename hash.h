#ifndef __INTRUSIVE_C_HASH_H
#define __INTRUSIVE_C_HASH_H

#include <assert.h>
/**
   @section: 哈希计算. 通过外部传入的 key 和 数组大小计算具体的桶下标
*/

/* 用于和 key 结合生成哈希值 */
#define GOLDEN_RATIO_32 0x61C88647
#define GOLDEN_RATIO_64 0x61C8864680B583EBull

/* 计算哈希数组的元素个数 */
#define hash_size(hashtable) (sizeof((hashtable)) / sizeof((hashtable)[0]))

/* 根据 32位 key 和 bits 生成哈希数组的下标 */
static inline unsigned int hash_32(unsigned int key, unsigned int bits) {
    return (key * GOLDEN_RATIO_32) >> (32 - bits);
}

/* 根据用户传入的 64位的 key 和 Bit 生成哈希下标 */
static inline unsigned int hash_64(unsigned long key, unsigned int bits) {
    return (key * GOLDEN_RATIO_64) >> (64 - bits);
}

/* 获取哈希表的桶下标 */
#define hash_min(key, bits) (sizeof((key)) <= 4 ? hash_32(key, bits) : hash_64(key, bits))

/*  根据哈希数组求 bits */
#define hash_bits(hashtable) ilog2(hash_size(hashtable))

/**
   @section fls 系列, 取 last bits
 */

static inline int fls(unsigned int x) {
    int r = 32;

    if (!x) return 0;
    if (!(x & 0xffff0000u)) {
        x <<= 16;
        r -= 16;
    }
    if (!(x & 0xff000000u)) {
        x <<= 8;
        r -= 8;
    }
    if (!(x & 0xf0000000u)) {
        x <<= 4;
        r -= 4;
    }
    if (!(x & 0xc0000000u)) {
        x <<= 2;
        r -= 2;
    }
    if (!(x & 0x80000000u)) {
        x <<= 1;
        r -= 1;
    }
    return r;
}

#define BITS_PER_LONG 64
static inline unsigned long __fls(unsigned long word) {
    int num = BITS_PER_LONG - 1;

#if BITS_PER_LONG == 64
    if (!(word & (~0ul << 32))) {
        num -= 32;
        word <<= 32;
    }
#endif
    if (!(word & (~0ul << (BITS_PER_LONG - 16)))) {
        num -= 16;
        word <<= 16;
    }
    if (!(word & (~0ul << (BITS_PER_LONG - 8)))) {
        num -= 8;
        word <<= 8;
    }
    if (!(word & (~0ul << (BITS_PER_LONG - 4)))) {
        num -= 4;
        word <<= 4;
    }
    if (!(word & (~0ul << (BITS_PER_LONG - 2)))) {
        num -= 2;
        word <<= 2;
    }
    if (!(word & (~0ul << (BITS_PER_LONG - 1)))) num -= 1;
    return num;
}

static inline int fls64(unsigned long x) {
    if (x == 0) return 0;
    return __fls(x) + 1;
}

/**
   @section: ilog2 系列函数, 将哈希表的 size 转化为 bits
 */

static inline int __ilog2_u32(unsigned int u) {
    return fls(u) - 1;
}
static inline int __ilog2_u64(unsigned long u) {
    return fls64(u) - 1;
}

/* 根据传入的 size 返回 bits */
#define ilog2(n) (__builtin_constant_p(n) ? ((n) < 2 ? 0 : 63 - __builtin_clzll(n)) : (sizeof(n) <= 4) ? __ilog2_u32(n) : __ilog2_u64(n))

#endif