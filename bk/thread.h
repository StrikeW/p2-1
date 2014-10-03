/** @file  thread.h
 *  @brief  thread  function prototypes
 *
 *
 *  @author  Yuhang Jiang (yuhangj)
 *  @author  Subramanian Natarajan (snatara1)
 *
 *
 *  @bugs
 *
 */

#include "thr_internals.h"




int thr_init( unsigned int size );
int thr_create( void *(*func)(void *), void *args );
void fork_child_handler(void *(*func) (void *), void *arg, tcb_t*);
void thr_exit( void *status );
int thr_yield( int tid );
int thr_getid(void);
void page_fault_handler(void *arg, ureg_t *ureg);
