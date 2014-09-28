/*
 * these functions should be thread safe.
 * It is up to you to rewrite them
 * to make them thread safe.
 *
 */

#include <stdlib.h>
#include <types.h>
#include <stddef.h>

void *malloc(size_t __size)
{
    void *ret = _malloc(__size);
    return ret;
}

void *calloc(size_t __nelt, size_t __eltsize)
{
	void *ret = _calloc(__nelt, __eltsize);
    return ret;
}

void *realloc(void *__buf, size_t __new_size)
{
  return NULL;
}

void free(void *__buf)
{
  return;
}
