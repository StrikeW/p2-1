/* If you want to use assembly language instead of C,
 * delete this autostack.c and provide an autostack.S
 * instead.
 */
#include <thread.h>



void
install_autostack(void *stack_high, void *stack_low)
{
    lprintf ("DEBUG install_autostack start");
    main_thr_high_addr =(uintptr_t) stack_high;
    main_thr_low_addr = (uintptr_t) stack_low;
    SET_STACK_CURR_BASE(THR_PTR_ALIGN(stack_low));
    multi_threading = 0;
    //investigate how to register a handler
    lprintf ("DEBUG install_autostack end");
}





/* write handler here
 *
 *
 *1. Check whether it is a page fault due to stack overgrowth
 *2.
*/
 

void page_fault_handler(int *ret_addr, void *arg, ureg_t *ureg)
{
   #if 0
    uintptr_t allocate_page;
    lprintf("IN STACKGROWTH HANDLER");
    if(ureg->cause == SWEXN_CAUSE_PAGEFAULT){
   #endif
  lprintf ("page fault handler");
}

void end_reached( uintptr_t *add)
{
  #if 0
 if ( addr < main_thr_low_addr && main_thr_high_addr < addr + 
  # endif

  lprintf ("end reached");
}
