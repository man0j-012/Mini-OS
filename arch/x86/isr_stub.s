.intel_syntax noprefix
.section .text
.globl idt_load
idt_load: mov eax,[esp+4]; lidt [eax]; ret
.extern isr_handler
.extern irq_handler
.extern isr_or_irq
.macro ISR_NOERR n
.globl isr\n
isr\n: cli; push dword 0; push dword \n; jmp isr_common
.endm
.macro ISR_ERR n
.globl isr\n
isr\n: cli; push dword \n; jmp isr_common_errpushed
.endm
.macro IRQ n
.globl irq\n
irq\n: cli; push dword 0; push dword (32+\n); jmp isr_common
.endm
ISR_NOERR 0  ISR_NOERR 1  ISR_NOERR 2  ISR_NOERR 3
ISR_NOERR 4  ISR_NOERR 5  ISR_NOERR 6  ISR_NOERR 7
ISR_ERR 8    ISR_NOERR 9  ISR_ERR 10  ISR_ERR 11
ISR_ERR 12   ISR_ERR 13   ISR_ERR 14  ISR_NOERR 15
ISR_NOERR 16 ISR_NOERR 17 ISR_NOERR 18 ISR_NOERR 19
ISR_NOERR 20 ISR_NOERR 21 ISR_NOERR 22 ISR_NOERR 23
ISR_NOERR 24 ISR_NOERR 25 ISR_NOERR 26 ISR_NOERR 27
ISR_NOERR 28 ISR_NOERR 29 ISR_NOERR 30 ISR_NOERR 31
IRQ 0 IRQ 1 IRQ 2 IRQ 3 IRQ 4 IRQ 5 IRQ 6 IRQ 7
IRQ 8 IRQ 9 IRQ 10 IRQ 11 IRQ 12 IRQ 13 IRQ 14 IRQ 15
isr_common_errpushed:
    pusha; push ds; push es; push fs; push gs
    mov ax,0x10; mov ds,ax; mov es,ax; mov fs,ax; mov gs,ax
    push esp; call isr_handler; add esp,4
    pop gs; pop fs; pop es; pop ds; popa
    add esp,8; sti; iretd
isr_common:
    pusha; push ds; push es; push fs; push gs
    mov ax,0x10; mov ds,ax; mov es,ax; mov fs,ax; mov gs,ax
    push esp; call isr_or_irq; add esp,4
    pop gs; pop fs; pop es; pop ds; popa
    add esp,8; sti; iretd
# append isr128 at the end
.intel_syntax noprefix
.section .text
.globl isr128
isr128:
    cli
    push dword 0
    push dword 128
    jmp isr_common
