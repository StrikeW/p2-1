/** @file  autostack.c
 *  @brief Page fault handler implementation for the main and other 
 *         threads
 *
 *  @author  Yuhang Jiang (yuhangj)
 *  @author  Subramanian Natarajan (snatara1)
 *
 *  @bugs No known bugs
 */

#include <thread.h>
#include <ureg.h>
#include <syscall.h>
#include <stddef.h>
#include <simics.h>

#define ALIGN_ADDR(addr) ((addr) & 0xfffff000)

char exception_handler_stack[PAGE_SIZE];

/** @brief Function that implements the page fault
 *         handler
 *
 *  The page fault handler for the main thread obtains the faulting 
 *  address and alligns the address to a page and extends stack to 
 *  the faulting address.   
 *   
 *  If a non-main thread encounters a page fault due to stack overflow, 
 *  we terminate the thread.
 * 
 *  @param arg - arguments to the handler
 *  @param ureg - register state containing the cause for exception
 * 
 *  @return void
 */
void page_fault_handler(void *arg, ureg_t *ureg)
{
    if(ureg->cause == SWEXN_CAUSE_PAGEFAULT)
    {
        if(!IS_MULTI_THREADING_ENABLED()) 
        {
            unsigned int addr = (unsigned int)ureg->cr2;
            unsigned int stack_start = ALIGN_ADDR(addr);
            if(new_pages((void*)stack_start,
                cur_stack_base_addr - stack_start) < 0)
            {
                printf("Access Invalid memory!\n");
                exit(-2);
            }

            cur_stack_base_addr = stack_start;
            if (swexn((void *)(exception_handler_stack + PAGE_SIZE),
            page_fault_handler, NULL, ureg) < 0) {
            printf("page fault re-registration failed for main thread\n");
                exit(-1);
            }
        
        } else {
            lprintf("stack overflow for thread = %d", gettid());
            task_vanish(-1);
        }
    }
}

/** @brief Function that initialises auto stack
 *         
 *  Get the stack high and stack low for the main thread
 *
 *  @param stack_high - stack high addess of the main thread
 *  @param stack_low  - stack low address of the main thread
 *  @return void
 */
void
install_autostack(void *stack_high, void *stack_low)
{
    main_thr_high_addr =(uintptr_t) stack_high;
    main_thr_low_addr = (uintptr_t) stack_low;
    SET_STACK_CURR_BASE(THR_PTR_ALIGN(stack_low));
    multi_threading = 0;

    if (swexn((void *)(exception_handler_stack + PAGE_SIZE), 
        page_fault_handler, NULL, NULL) < 0) {
        printf("re-register exception handler for main failed.\n");
        exit(-2);
    }
}

