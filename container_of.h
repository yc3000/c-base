#ifndef _C_BASE_CONTAINER_OF_H
#define _C_BASE_CONTAINER_OF_H

#include <stddef.h>

#define container_of(ptr, type, member) \
  ((type*)(((char*)((type*)(ptr))) - offsetof(type, member)))

#endif
