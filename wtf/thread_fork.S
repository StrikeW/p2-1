#include <syscall_int.h>

.global thread_fork
thread_fork:
    pushl   %ebp    
    movl    %esp, %ebp
    movl    8(%ebp), %edx       
    subl    $12, %edx           
    movl    12(%ebp), %eax
    movl    %eax, (%edx)   
    movl    16(%ebp), %eax
    movl    %eax, 4(%edx)
    movl    20(%ebp), %eax
    movl    %eax, 8(%edx)
    movl    $0, %eax
    movl    %eax, 12(%edx)
    pushl   %esi
    movl    8(%ebp), %esi       
    int     $THREAD_FORK_INT   
    cmp     $0, %eax
    je      child         
    popl    %esi               
    movl    %ebp, %esp
    popl    %ebp
    ret

child:
    movl    %esi, %ebp
    movl    %esi, %esp
    subl    $12, %esp            
    call fork_handler
    ret
