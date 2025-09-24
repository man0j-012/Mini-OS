.intel_syntax noprefix
.section .text
.globl gdt_flush
gdt_flush:
    mov eax, [esp+4]   # &gdt_ptr
    lgdt [eax]
    mov ax, 0x10       # kernel data selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    # far jump to reload CS
    ljmp $0x08, $flush2
flush2:
    ret
