/** @file 
 *  @brief 
 *  @author 
 *  @author 
 *
 *  @bugs
 *
 */

#include <syscall_int.h>
.global print

print:
    push %esi
    leal 8(%esp), %esi
    int $PRINT_INT
    pop %esi
    ret

