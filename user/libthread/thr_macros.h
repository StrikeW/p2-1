

#define PAGE_ALIGN(addr) ((addr) & ~ (PAGE_SIZE -1))

#define THR_SIZE_ALIGN(size) \
    ( ( (unsigned int)(size) + (PAGE_SIZE-1) ) & ~(PAGE_SIZE-1))

#define THR_PTR_ALIGN(ptr) (((uintptr_t) (ptr)) & ~(PAGE_SIZE-1) )

#define SET_STACK_CURR_BASE(addr) (cur_stack_base_addr = (uintptr_t) (addr) )

#define GET_STACK_CURR_BASE(addr) (cur_stack_base_addr)

#define MULTI_THREADING_ENABLE()  ( multi_threading = 1 ;)  
#define MULTI_THREADING_DISABLE()  ( multi_threading = 0 ;)  

#define IS_MULTI_THREADING_ENABLED() ( multi_threading == 1;)
