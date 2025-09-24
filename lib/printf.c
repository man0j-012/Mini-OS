#include "printf.h"
#include "tty.h"
#include <stdarg.h>
#include <stdint.h>
static void kputd(unsigned v){ char b[12]; int i=0; if(v==0){ tty_putc('0'); return; } while(v){ b[i++]='0'+(v%10); v/=10; } while(i--) tty_putc(b[i]); }
static void kputx(uint32_t v){ const char* h="0123456789ABCDEF"; tty_write("0x"); for(int i=28;i>=0;i-=4) tty_putc(h[(v>>i)&0xF]); }
void kprintf(const char* fmt, ...){ va_list ap; va_start(ap, fmt); for(const char* p=fmt; *p; ++p){ if(*p!='%'){ tty_putc(*p); continue; } ++p; if(*p=='s'){ const char* s=va_arg(ap,const char*); tty_write(s);} else if(*p=='c'){ char c=(char)va_arg(ap,int); tty_putc(c);} else if(*p=='d'){ unsigned v=va_arg(ap,unsigned); kputd(v);} else if(*p=='x'){ uint32_t v=va_arg(ap,uint32_t); kputx(v);} else { tty_putc('%'); tty_putc(*p);} } va_end(ap); }
