#include <stdint.h>
#include "ports.h"
#include "pit.h"
#define PIT_CH0 0x40
#define PIT_CMD 0x43
#define PIT_HZ  100
volatile unsigned pit_ticks=0;
void pit_init(){ uint32_t div=1193180/PIT_HZ; port_outb(PIT_CMD,0x36); port_outb(PIT_CH0,div&0xFF); port_outb(PIT_CH0,(div>>8)&0xFF); }
