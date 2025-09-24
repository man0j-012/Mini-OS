#include <stdint.h>
#include "tss.h"
#include "gdt.h"

extern void tss_flush(void); /* asm ltr */

static struct tss_entry tss;

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

extern struct gdt_entry gdt[]; /* from gdt.c */
extern void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

/* We define gdt_set_gate here as extern, but it's static in gdt.c. Provide a local helper. */
static void set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
static void set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran){
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;
    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[num].access      = access;
}

void tss_write_descriptor(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran){
    set_gate(num, base, limit, access, gran);
}

void tss_install(uint32_t kernel_ss, uint32_t kernel_esp){
    /* zero TSS */
    for (unsigned i=0;i<sizeof(tss)/4;i++) ((uint32_t*)&tss)[i]=0;
    tss.ss0 = kernel_ss;
    tss.esp0 = kernel_esp;
    tss.cs = gdt_sel_user_code();
    tss.ss = tss.ds = tss.es = tss.fs = tss.gs = gdt_sel_user_data();
    /* Install TSS descriptor at GDT index 5: type 0x89 (present, exec=0, accessed=1, type=9) */
    uint32_t base = (uint32_t)&tss;
    tss_write_descriptor(5, base, sizeof(tss), 0x89, 0x00);
    tss_flush();
}

void tss_set_kernel_stack(uint32_t esp0){
    tss.esp0 = esp0;
}
