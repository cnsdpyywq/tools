#ifndef TYPE_H
#define TYPE_H

#ifdef __KERNEL__
#ifdef __FREEBSD__
#include <sys/types.h>
#else
#include <linux/types.h>
#endif
#else
#include <stdint.h>
#endif

typedef void null_t;
typedef null_t* ptr_t;
typedef int32_t val_t;
typedef uint32_t uval_t;

#define nullptr     (0)

#define SUCCESS     (1)
#define FAILED      (0)
#define ERR_PARAMS  (-1)
#define ERR_NO_MEM  (-2)

#endif /* TYPE_H */
