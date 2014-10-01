// // #include <stdint.h>
// // #include <stdio.h>
// // #include <simics.h>
// // #include <thr_macros.h>
// // #include <syscall.h>
// // #include <_list.h>
// // #include <stdlib.h>
// // #include "mutex_type.h"

// //NEED to remove this, only for debugging
// int main_thread_id;
// ///NEED to remove this, only for debugging


// uintptr_t cur_stack_base_addr;
// list_head *head_thr_list;
// uintptr_t main_thr_high_addr;
// uintptr_t main_thr_low_addr;
// int multi_threading;
// int thread_stack_size;
// int parent;
// mutex_t thread_list;
// list_head *join_queue;

// #define DEFAULT_TID -1

// typedef enum
// {
//    INIT = 0,
//    RUNNABLE,
//    BLOCKED,
//    ZOMBIE
// } exec_state_t;


// typedef struct _tcb 
// {
//     list_head *list;
//     void *exit_status;
//     int tid;
//     int parentid;
//     int join_tid;
//     uintptr_t stack_start_addr;
//     exec_state_t state;
// }tcb_t;


// typedef enum
// {
//    THR_ERROR = -1,
//    THR_SUCCESS,
//    THR_ENOMEM,
//    THR_FORK_ERROR
// } thr_err_t;




// tcb_t *head;






