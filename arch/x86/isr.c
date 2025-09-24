#include <stdint.h>
#include "isr.h"
#include "idt.h"
#include "tty.h"
#include "pic.h"
#include "keyboard.h"
#include "pit.h"
#include "sched.h"
void isr_install_stubs(void);
void irq_install_stubs(void);
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void isr_handler(struct regs *r){ tty_write("[EXC] int="); tty_write_hex(r->int_no); tty_write(" err="); tty_write_hex(r->err_code); tty_write("\n"); }
void irq_handler(struct regs *r){
    int irq = r->int_no - 32;
    if(r->int_no == 32){ schedule_from_timer(); }
    else if(r->int_no == 33){ kbd_isr(); }
    pic_send_eoi(irq);
}
void isr_or_irq(struct regs* r){ if(r->int_no>=32 && r->int_no<=47) irq_handler(r); else isr_handler(r); }
void isr_install(void){ idt_install(); isr_install_stubs(); irq_install_stubs(); }
