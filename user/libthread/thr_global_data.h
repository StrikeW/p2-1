#include <stdint.h>
#include <stdio.h>
#include <simics.h>
#include <thr_macros.h>
#include <syscall.h>
#include <_list.h>
#include <stdlib.h>

//NEED to remove this, only for debugging
int main_thread_id;
uintptr_t cur_stack_base_addr;
list_head *head_thr_list;
uintptr_t main_thr_high_addr;
uintptr_t main_thr_low_addr;
int multi_threading;
int thread_stack_size;


typedef enum
{
   INIT = 0,
   RUNNING,
   RUNNABLE,
   SLEEP,
   ZOMBIE
} exec_state_t;


typedef struct _tcb 
{
    list_head *list;
    void *exit_status;
    int tid;
    int parentid;
    uintptr_t stack_start_addr;
    exec_state_t state;
    int main_thread;   
}tcb_t;


typedef enum
{
   THR_ERROR = -1,
   THR_SUCCESS,
   THR_ENOMEM,
   THR_FORK_ERROR
} thr_err_t;


tcb_t *head;





