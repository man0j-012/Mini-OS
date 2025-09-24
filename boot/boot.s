.section .multiboot
    .align 4
    .long 0x1BADB002
    .long 0x00010003
    .long -(0x1BADB002 + 0x00010003)
    .section .text
    .globl _start
_start:
    mov $stack_top, %esp
    push $0
    popf
    call kmain
1:  hlt
    jmp 1b
    .comm stack, 16384, 16
    .globl stack_top
stack_top = stack + 16384
