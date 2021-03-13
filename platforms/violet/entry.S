.section .text.entry

.globl _start
_start:
  la sp, _STACK_END
  call main

_end:
  j _end

.section .data
.align 8
_STACK:
.space 16384
_STACK_END:
