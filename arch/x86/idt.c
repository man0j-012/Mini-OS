#include <stdint.h>
#include "idt.h"
struct idt_entry{ uint16_t base_lo, sel; uint8_t always0, flags; uint16_t base_hi; } __attribute__((packed));
struct idt_ptr{ uint16_t limit; uint32_t base; } __attribute__((packed));
extern void idt_load(uint32_t);
static struct idt_entry idt[256]; static struct idt_ptr idtp;
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags){ idt[num].base_lo=base&0xFFFF; idt[num].sel=sel; idt[num].always0=0; idt[num].flags=flags; idt[num].base_hi=(base>>16)&0xFFFF; }
void idt_install(){ idtp.limit=sizeof(idt)-1; idtp.base=(uint32_t)&idt; for(int i=0;i<256;i++) idt_set_gate(i,0,0x08,0x8E); idt_load((uint32_t)&idtp); }
