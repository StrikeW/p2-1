/** @file thread.c
 *  
 *  @brief This file declares the function of thead library
 *
 *  @bug no bug found yet
 */
#include <thread.h>
#include <syscall.h>
#include <assert.h>
#include <thr_internals.h>

#include <stdlib.h>
#include <stdio.h>
#include "simics.h"

/* Use one page as thread stack as defualt which is easy to
 * manage */
#define DEFAULT_STACK_SIZE 4096

tcb_t* construct_tcb()
{ 
   lprintf( "DEBUG: construct_tcb start");
   tcb_t *new_thr = (tcb_t*) malloc( sizeof (tcb_t));
   new_thr->exit_status = NULL;
   new_thr->stack_start_addr = cur_stack_base_addr;
   LIST_HEAD(new_thr.list);
   lprintf( "DEBUG: construct_tcb end");
   return new_thr;
}

int thr_init( unsigned int size )
{
    void *stack_low;
    thread_stack_size = THR_SIZE_ALIGN(size);
    lprintf ("DEBUG: thr_init start: thread_stack_size:%d", 
    	thread_stack_size);
    //Construct tcb
    // pcb_t *curr_pcb = construct_pcb();  We will look into pcb later 
    tcb_t *curr_tcb = construct_tcb(); 
    curr_tcb->main_thread = 1;
    curr_tcb->stack_start_addr = main_thr_high_addr;
    curr_tcb->state = RUNNING;
    head_thr_list = &(curr_tcb->list_head);
    main_thread_id = curr_tcb->tid = gettid();
    MULTI_THREADING_ENABLE();
    lprintf ("DEBUG:  thr_init end");
}

int thr_create( void *(*func)(void *), void *args )
{
    int status;
    lprintf ("DEBUG:  thr_create start");
    if (new_pages((void*)cur_stack_base_addr - PAGE_SIZE, PAGE_SIZE) < 0)
    {
        lprintf("PAGE ALLOCATION FAILED");
        return NULL;
    } 
  
    if (new_pages((void*)cur_stack_base_addr - 2*PAGE_SIZE, PAGE_SIZE) < 0)
    {
        lprintf("PAGE ALLOCATION for Exception handler FAILED");
        return NULL;
    }
   
    tid = thread_fork(cur_stack_base, func, args);
    if (tid < 0) {
        return tid;
    }
    lprintf ("In Parent");	    
    // int ret = deschedule(&status);
    // if ( ret < 0) {
    //    return ret;
    // }
    cur_stack_base_addr = cur_stack_base_addr - 2*PAGE_SIZE;
    lprintf (" cur_stack_base_addr is %x", (int) cur_stack_base_addr);
    lprintf ("DEBUG:  thr_create stop");
    return tid;
}


void fork_handler(void *(*func) (void *), void *arg)
{
 	int status;
        lprintf ("In thread, need to register handler");
        while(1) 
        {
	    if (thr_yield(main_thread_id) != 0) {
		break;
        }
       

        tcb_t *curr_tcb = construct_tcb();
        curr_tcb->tid = gettid();
        curr_tcb->main_thread = 0;

        //Need to implement Push

        curr_tcb->list_head.next = head_thr_list;
        curr_tcb->list_head.prev = head_thr_list;
        head_thr_list->next = curr_tcb;
        head_thr_list->prev = curr_tcb;
        status = (func)(arg);
        swexn((void *)(curr_tcb->stack_start_addr - thread_stack_size),
              page_fault_handler, NULL, NULL);
        make_runnable(main_thread_id);
        thr_exit((void*)status);
}