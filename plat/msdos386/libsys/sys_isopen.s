#
! $Source$
! $State$
! $Revision$

#include "libsysasm.h"

! Say whether a given file descriptor number refers to a valid open file
! descriptor.

.define __sys_isopen
__sys_isopen:
	mov ebx, 4(esp)
	mov eax, 0x4400
	int 0x21
	sbb eax, eax
	inc eax
	ret
