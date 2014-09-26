/** @file xchg.S
 *
 *  @brief Implementation of atomic xchg in assembly 
 *  
 *  @bug No known bug
 */

 .global atom_xchg

 atom_xchg:
 	movl 8(%esp),%eax
 	movl 4(%esp),%ecx
 	XCHG (%ecx), %eax
 	ret
