#
! $Source$
! $State$
! $Revision$

#include "libsysasm.h"

! Get the current system time from MS-DOS.

.define __sys_gettime
__sys_gettime:
	movb ah, 0x2c
	int 0x21
	mov ebx, esp
	mov ebx, 4(ebx)
	o16 mov 0(ebx), cx
	o16 mov 2(ebx), dx
	ret
