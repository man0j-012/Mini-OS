.intel_syntax noprefix
.section .text
.globl enter_user_mode
# void enter_user_mode(uint32_t entry, uint32_t user_stack_top, uint16_t user_ds, uint16_t user_cs);
enter_user_mode:
    mov eax, [esp+4]   # entry
    mov ebx, [esp+8]   # user_stack_top
    mov cx,  [esp+12]  # user_ds
    mov dx,  [esp+14]  # user_cs

    # Set data segments to user data selector
    mov ds, cx
    mov es, cx
    mov fs, cx
    mov gs, cx

    # Push iret frame: SS, ESP, EFLAGS, CS, EIP
    push cx            # SS (user data)
    push ebx           # ESP
    pushf
    or dword ptr [esp], 0x200  # ensure IF=1
    push dx            # CS (user code)
    push eax           # EIP (entry)
    iretd
