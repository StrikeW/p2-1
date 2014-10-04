/** @file  thread_fork.S
 *  @brief Asm for thread_fork, jump according to eax
 *
 *  @author  Yuhang Jiang (yuhangj)
 *  @author  Subramanian Natarajan (snatara1)
 *
 *  @bugs No known bugs
 */

#include <syscall_int.h>

.global thread_fork
thread_fork:
    # Setup the stack
    pushl   %ebp    
    movl    %esp, %ebp

    # Copy all arguements
    movl    8(%ebp), %edx       #stackbase       
    subl    $12, %edx           
    movl    12(%ebp), %eax      #function address
    movl    %eax, (%edx)   
    movl    16(%ebp), %eax      #arguement for the above function
    movl    %eax, 4(%edx)
    movl    20(%ebp), %eax      #tcb data structure
    movl    %eax, 8(%edx)

    #Set up stack such that EBP = 0
    movl    $0, %eax
    movl    %eax, 12(%edx)
    
    pushl   %esi             
    movl    8(%ebp), %esi       # set up ESI as child stack base   
    int     $THREAD_FORK_INT    # fork thread	  
    cmp     $0, %eax 
    je      child                  
    popl    %esi               
    movl    %ebp, %esp
    popl    %ebp
    ret

child:                      # Setup child stack
    movl    %esi, %ebp      
    movl    %esi, %esp
    subl    $12, %esp       # Three arguments to the child function     
    call fork_child_handler 
    ret
