.intel_syntax noprefix
.section .text
.globl tss_flush
tss_flush:
    mov ax, 5*8    # GDT entry 5 selector
    ltr ax
    ret
