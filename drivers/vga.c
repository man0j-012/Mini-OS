#include <stdint.h>
#include <stddef.h>
#include "tty.h"
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
static uint16_t* const VGA = (uint16_t*)0xB8000;
static uint8_t term_color = 0x07;
static size_t cx=0, cy=0;
static void putentryat(char c, uint8_t color, size_t x, size_t y){ VGA[y*VGA_WIDTH + x] = (uint16_t)color<<8 | (uint8_t)c; }
static void newline(){ cx=0; if(++cy>=VGA_HEIGHT){ for(size_t y=1;y<VGA_HEIGHT;y++){ for(size_t x=0;x<VGA_WIDTH;x++){ VGA[(y-1)*VGA_WIDTH+x]=VGA[y*VGA_WIDTH+x]; } } cy=VGA_HEIGHT-1; for(size_t x=0;x<VGA_WIDTH;x++) putentryat(' ', term_color, x, cy);} }
void tty_clear(){ for(size_t y=0;y<VGA_HEIGHT;y++) for(size_t x=0;x<VGA_WIDTH;x++) putentryat(' ', term_color, x, y); cx=cy=0; }
void tty_putc(char c){ if(c=='\n'){ newline(); return; } putentryat(c, term_color, cx, cy); if(++cx>=VGA_WIDTH){ newline(); } }
void tty_write(const char* s){ for(size_t i=0;s[i];++i) tty_putc(s[i]); }
void tty_write_hex(uint32_t v){ const char* hex="0123456789ABCDEF"; tty_write("0x"); for(int i=28;i>=0;i-=4){ tty_putc(hex[(v>>i)&0xF]); } }
void tty_write_dec(uint32_t v){ char b[12]; int i=0; if(v==0){ tty_putc('0'); return; } while(v){ b[i++]='0'+(v%10); v/=10; } while(i--) tty_putc(b[i]); }
