#include <stdint.h>
void port_outb(uint16_t p, uint8_t v){ __asm__ __volatile__("outb %0,%1"::"a"(v),"Nd"(p)); }
uint8_t port_inb(uint16_t p){ uint8_t r; __asm__ __volatile__("inb %1,%0":"=a"(r):"Nd"(p)); return r; }
void port_wait(void){ __asm__ __volatile__("outb %%al,$0x80"::"a"(0)); }
