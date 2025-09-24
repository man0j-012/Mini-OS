#include <stdint.h>
#include "ports.h"
#include "pic.h"
#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2+1)
#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01
void pic_remap(int o1,int o2){
    uint8_t a1=port_inb(PIC1_DATA), a2=port_inb(PIC2_DATA);
    port_outb(PIC1_COMMAND, ICW1_INIT|ICW1_ICW4); port_wait();
    port_outb(PIC2_COMMAND, ICW1_INIT|ICW1_ICW4); port_wait();
    port_outb(PIC1_DATA, o1); port_wait();
    port_outb(PIC2_DATA, o2); port_wait();
    port_outb(PIC1_DATA, 4);  port_wait();
    port_outb(PIC2_DATA, 2);  port_wait();
    port_outb(PIC1_DATA, ICW4_8086); port_wait();
    port_outb(PIC2_DATA, ICW4_8086); port_wait();
    port_outb(PIC1_DATA, a1);
    port_outb(PIC2_DATA, a2);
}
void pic_send_eoi(int irq){ if(irq>=8) port_outb(PIC2_COMMAND, 0x20); port_outb(PIC1_COMMAND, 0x20); }
