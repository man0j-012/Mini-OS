#include <stdint.h>
#include "isr.h"
#include "idt.h"
#include "tty.h"
#include "printf.h"
#include "sched.h"
#include "pit.h"
#include "syscall.h"

/* from idt.c */
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
extern void isr128(void);

/* DPL=3 interrupt gate so (later) user mode can invoke it */
#define IDT_FLAG_INTGATE | 0x8E
void syscall_init(void){
    /* 0x8E with DPL=3 => 0xEE */
    idt_set_gate(128, (uint32_t)isr128, 0x08, 0xEE);
}

/* Very small syscall table using eax=nr, ebx, ecx, edx args */
static int ksys_write(const char* s, uint32_t len){
    for(uint32_t i=0;i<len && s[i]; ++i) tty_putc(s[i]);
    return (int)len;
}
static void ksys_yield(void){ yield(); }
extern volatile unsigned pit_ticks;
static void ksys_sleep_ms(uint32_t ms){
    unsigned start = pit_ticks;
    /* PIT_HZ is 100 -> 10ms per tick; busy-wait for simplicity */
    unsigned target_ticks = start + (ms/10);
    while ((int)(pit_ticks - target_ticks) < 0) { /* spin */ }
}

void isr_handler(struct regs* r); /* already defined elsewhere */
void irq_handler(struct regs* r); /* already defined elsewhere */

void isr_or_irq(struct regs* r); /* forward */

void isr_handler_sys(struct regs* r){
    uint32_t nr = r->eax;
    uint32_t a0 = r->ebx, a1 = r->ecx, a2 = r->edx;
    int ret = -1;
    switch(nr){
        case SYS_write:     ret = ksys_write((const char*)a0, a1); break;
        case SYS_yield:     ksys_yield(); ret = 0; break;
        case SYS_sleep_ms:  ksys_sleep_ms(a0); ret = 0; break;
        default:            tty_write("[sys] unknown nr\n"); break;
    }
    r->eax = (uint32_t)ret;
}

extern void pic_send_eoi(int irq);

void isr_or_irq(struct regs* r){
    if (r->int_no == 128) { isr_handler_sys(r); return; }
    if (r->int_no >= 32 && r->int_no <= 47) irq_handler(r);
    else isr_handler(r);
}

/* minimal user-ish wrappers usable from kernel threads for now */
int sys_write(const char* s, uint32_t len){
    int ret;
    __asm__ __volatile__(
        "int $0x80"
        : "=a"(ret)
        : "a"(SYS_write), "b"(s), "c"(len), "d"(0)
        : "memory"
    );
    return ret;
}
void sys_yield(void){
    __asm__ __volatile__("int $0x80" :: "a"(SYS_yield), "b"(0), "c"(0), "d"(0) : "memory");
}
void sys_sleep_ms(uint32_t ms){
    __asm__ __volatile__("int $0x80" :: "a"(SYS_sleep_ms), "b"(ms), "c"(0), "d"(0) : "memory");
}
