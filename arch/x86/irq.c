#include <stdint_t.h>
#include <stdint.h>
#include "isr.h"
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
#define DECL_ISR(n) extern void isr##n(void);
#define DECL_IRQ(n) extern void irq##n(void);
#define SET_GATE(i, fn) idt_set_gate((i), (uint32_t)(fn), 0x08, 0x8E);
DECL_ISR(0) DECL_ISR(1) DECL_ISR(2) DECL_ISR(3)
DECL_ISR(4) DECL_ISR(5) DECL_ISR(6) DECL_ISR(7)
DECL_ISR(8) DECL_ISR(9) DECL_ISR(10) DECL_ISR(11)
DECL_ISR(12) DECL_ISR(13) DECL_ISR(14) DECL_ISR(15)
DECL_ISR(16) DECL_ISR(17) DECL_ISR(18) DECL_ISR(19)
DECL_ISR(20) DECL_ISR(21) DECL_ISR(22) DECL_ISR(23)
DECL_ISR(24) DECL_ISR(25) DECL_ISR(26) DECL_ISR(27)
DECL_ISR(28) DECL_ISR(29) DECL_ISR(30) DECL_ISR(31)
DECL_IRQ(0) DECL_IRQ(1) DECL_IRQ(2) DECL_IRQ(3)
DECL_IRQ(4) DECL_IRQ(5) DECL_IRQ(6) DECL_IRQ(7)
DECL_IRQ(8) DECL_IRQ(9) DECL_IRQ(10) DECL_IRQ(11)
DECL_IRQ(12) DECL_IRQ(13) DECL_IRQ(14) DECL_IRQ(15)
void isr_install_stubs(){
    SET_GATE(0, isr0)  SET_GATE(1, isr1)  SET_GATE(2, isr2)  SET_GATE(3, isr3)
    SET_GATE(4, isr4)  SET_GATE(5, isr5)  SET_GATE(6, isr6)  SET_GATE(7, isr7)
    SET_GATE(8, isr8)  SET_GATE(9, isr9)  SET_GATE(10, isr10) SET_GATE(11, isr11)
    SET_GATE(12, isr12) SET_GATE(13, isr13) SET_GATE(14, isr14) SET_GATE(15, isr15)
    SET_GATE(16, isr16) SET_GATE(17, isr17) SET_GATE(18, isr18) SET_GATE(19, isr19)
    SET_GATE(20, isr20) SET_GATE(21, isr21) SET_GATE(22, isr22) SET_GATE(23, isr23)
    SET_GATE(24, isr24) SET_GATE(25, isr25) SET_GATE(26, isr26) SET_GATE(27, isr27)
    SET_GATE(28, isr28) SET_GATE(29, isr29) SET_GATE(30, isr30) SET_GATE(31, isr31)
}
void irq_install_stubs(){
    SET_GATE(32, irq0)  SET_GATE(33, irq1)  SET_GATE(34, irq2)  SET_GATE(35, irq3)
    SET_GATE(36, irq4)  SET_GATE(37, irq5)  SET_GATE(38, irq6)  SET_GATE(39, irq7)
    SET_GATE(40, irq8)  SET_GATE(41, irq9)  SET_GATE(42, irq10) SET_GATE(43, irq11)
    SET_GATE(44, irq12) SET_GATE(45, irq13) SET_GATE(46, irq14) SET_GATE(47, irq15)
}
