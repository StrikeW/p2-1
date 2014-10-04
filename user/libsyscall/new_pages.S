/** @file 
 *  @brief 
 *  @author 
 *  @author 
 *
 *  @bugs
 *
 */



#include <syscall_int.h>
.global new_pages

new_pages:
    push %esi
    leal 8(%esp), %esi
    int $NEW_PAGES_INT 
    pop %esi
    ret


