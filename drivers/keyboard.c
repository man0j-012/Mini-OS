#include <stdint.h>
#include "ports.h"
#include "tty.h"
static const char keymap[128] = {
    0,27,'1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
    'z','x','c','v','b','n','m',',','.','/',0,0,0,' ',
};
void kbd_isr(){ uint8_t sc=port_inb(0x60); if(sc&0x80) return; char c=keymap[sc]; if(c) tty_putc(c); }
