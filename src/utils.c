#include "utils.h"
#include <string.h>

ptr_t mem_alloc(uval_t size)
{
  ptr_t p = nullptr;
#ifdef __KERNEL__
  p = vmalloc(size);
  if (p) {
    memset(p, 0, size)
  }

#else
  p = calloc(1, size);
#endif
  return p;
}

null_t mem_free(ptr_t ptr)
{
#ifdef __KERNEL__
  vfree(ptr);
#else
  free(ptr);
#endif
}
