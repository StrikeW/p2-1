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
	void *ptr = NULL;
	
	//mutex_lock;
	ptr = _malloc(__size);
	//mutex_unlock;

  	return ptr;
}

void *calloc(size_t __nelt, size_t __eltsize)
{
	void *ptr = NULL;
	
	//mutex_lock;
	ptr = _calloc(__nelt, __eltsize);
	//mutex_unlock;

  	return ptr;
}

void *realloc(void *__buf, size_t __new_size)
{
	void *ptr = NULL;
	
	//mutex_lock;
	ptr = _realloc(__buf, __new_size);
	//mutex_unlock;

  	return ptr;
}

void free(void *__buf)
{
	//mutex_lock;
	_free(__buf);
	//mutex_unlock;

	return;
}
