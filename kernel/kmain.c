#include <stdint.h>
#include "tty.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"
#include "keyboard.h"
#include "heap.h"
#include "sched.h"
#include "paging.h"
#include "syscall.h"
#include "printf.h"
#include "gdt.h"
#include "tss.h"
#include "usermode.h"
static inline void sti(){ __asm__ __volatile__("sti"); }
extern void spawn_demo_threads(void);
void kmain(){
    tty_clear(); tty_write("MiniOS: booting (paging+syscalls+ring3)...\n");
    idt_install(); isr_install();
    void* heap_start=(void*)0x01000000; void* heap_end=(void*)0x01400000; paging_init_identity_32mb();
    heap_init(heap_start, heap_end);
    syscall_init();
    pic_remap(32,40); pit_init();
    tty_write("Interrupts ON, timer=100Hz. Keyboard active.\nSpawning threads...\n");
    sched_init(); spawn_demo_threads(); scheduler_dump();
    sti();
    /* Install GDT with user segments and TSS so IRQs from ring3 switch to kernel stack */
    gdt_install();
    /* Set initial kernel stack for TSS (use a static area near heap_end) */
    tss_install(gdt_sel_kernel_data(), (uint32_t)heap_end);

    /* Prepare a ring3 stack (just a region in our identity-mapped RAM) */
    uint32_t user_stack_top = 0x01800000; /* 24MB; arbitrary high address inside mapped window */

    extern void user_ring3_main(void);
    uint16_t ucs = gdt_sel_user_code();
    uint16_t uds = gdt_sel_user_data();
    tty_write("Entering ring3 user mode...\n");
    enter_user_mode((uint32_t)user_ring3_main, user_stack_top, uds, ucs);
 for(;;){ __asm__ __volatile__("hlt"); }
}
