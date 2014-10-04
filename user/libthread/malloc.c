/** @file  malloc.c
 *  @brief Implement thread safe malloc variants and free function.
 *
 *  @author  Yuhang Jiang (yuhangj)
 *  @author  Subramanian Natarajan (snatara1)
 *
 *  @bugs No known bugs
 */

#include <stdlib.h>
#include <types.h>
#include <stddef.h>
#include "spin_lock.h"

static int __malloc_lock_init = 0;
static int __calloc_lock_init = 0;
static int __realloc_lock_init = 0;
static int __free_lock_init = 0;

spinlock_t __malloc_lock;
spinlock_t __calloc_lock;
spinlock_t __realloc_lock;
spinlock_t __free_lock;	

/** @brief Function to check whether malloc lock needs to be 
 *  	   initialised. If it is not initialised, lock and 
 *  	   initialise. 
 *
 *  @param  none
 *  @return void 
 */
inline void __check_malloc_lock_init(){
	if(__malloc_lock_init == 0){
		spin_lock_init(&__malloc_lock);
		__malloc_lock_init = 1;
		return;
	}
	else 
		return;
}

/** @brief Function to check whether calloc lock needs to be 
 *  	   initialised. If it is not initialised, lock and 
 *         initialise.
 *
 *  @param  none
 *  @return void 
 */
inline void __check_calloc_lock_init(){
	if(__calloc_lock_init == 0){
		spin_lock_init(&__calloc_lock);
		__calloc_lock_init = 1;
		return;
	}
	else 
		return;
}

/** @brief Function to check whether realloc lock needs to be initialised.
 *         If it is not initialised, lock and initialise.
 *
 *  @param  none
 *  @return void 
 */
inline void __check_realloc_lock_init(){
	if(__realloc_lock_init == 0){
		spin_lock_init(&__realloc_lock);
		__realloc_lock_init = 1;
		return;
	}
	else 
		return;
}

/** @brief Function to check whether free lock needs to be initialised.
 *         If it is not initialised, lock and initialise.
 *
 *  @param  none
 *  @return void 
 */
inline void __check_free_lock_init(){
	if(__free_lock_init == 0){
		spin_lock_init(&__free_lock);
		__free_lock_init = 1;
		return;
	}
	else 
		return;
}

/** @brief thread free user malloc 
 *  
 *  @param  none
 *  @return void 
 */
void *malloc(size_t __size)
{
	__check_malloc_lock_init();

	spin_lock_request(&__malloc_lock);
    void *ret = _malloc(__size);
    spin_lock_release(&__malloc_lock);
    
    return ret;
}

/** @brief thread free user calloc 
 *
 *  @param  none
 *  @return void 
 */
void *calloc(size_t __nelt, size_t __eltsize)
{
	__check_calloc_lock_init();

	spin_lock_request(&__calloc_lock);
	void *ret = _calloc(__nelt, __eltsize);
	spin_lock_release(&__calloc_lock);

	return ret;
}

/** @brief thread free user realloc 
 *  
 *  @param  none
 *  @return void 
 */
void *realloc(void *__buf, size_t __new_size)
{
	__check_realloc_lock_init();

	spin_lock_request(&__realloc_lock);
	void *ret = _realloc(__buf, __new_size);
	spin_lock_release(&__realloc_lock);

	return ret;
}

/** @brief thread free user  free function
 *  
 *  @param  none
 *  @return void 
 */
void free(void *__buf)
{
	__check_free_lock_init();
	
	spin_lock_request(&__free_lock);
	_free(__buf);
	spin_lock_release(&__free_lock);

}
