/** @file   thread.h
 *  @brief  thread function prototypes
 *
 *  @author  Yuhang Jiang (yuhangj)
 *  @author  Subramanian Natarajan (snatara1)
 *
 *  @bugs No known bugs
 */

#include "thr_internals.h"

#ifndef THREAD_H
#define THREAD_H

int thr_init( unsigned int size );
int thr_create( void *(*func)(void *), void *args );
int thr_yield( int tid );
int thr_getid( void );
void thr_exit( void *status );
void page_fault_handler( void *arg, ureg_t *ureg );
void fork_child_handler( void *(*func) (void *), void *arg, tcb_t* );

#endif
