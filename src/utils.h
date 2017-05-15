#ifndef UTILS_H
#define UTILS_H

#include "type.h"
ptr_t mem_alloc(uval_t size);
null_t mem_free(ptr_t ptr);

#ifdef DEBUG
#ifdef __KERNEL__
#define DEBUG_PRINT printk
#else
#define DEBUG_PRINT printf
#endif
#else
#define DEBUG_PRINT(...)
#endif

#ifndef __KERNEL__
#include "stdio.h"
#include "stdlib.h"
#endif

#endif /* UTILS_H */
