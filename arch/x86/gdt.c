#include <stdint.h>
#include "gdt.h"

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

extern void gdt_flush(uint32_t); /* asm to load GDTR and update segment registers */

static struct gdt_entry gdt[6];
static struct gdt_ptr gp;

static void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;

    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access      = access;
}

void gdt_install(void)
{
    gp.limit = (sizeof(struct gdt_entry) * 6) - 1;
    gp.base  = (uint32_t)&gdt;

    /* Null */
    gdt_set_gate(0, 0, 0, 0, 0);
    /* Kernel code: base=0, limit=4GB, access=0x9A, gran=0xCF */
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    /* Kernel data: 0x92 */
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    /* User code: DPL=3: 0xFA */
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    /* User data: DPL=3: 0xF2 */
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
    /* TSS placeholder, filled by tss_install in its own file at index 5 */

    gdt_flush((uint32_t)&gp);
}

uint16_t gdt_sel_kernel_code(void){ return 0x08; }
uint16_t gdt_sel_kernel_data(void){ return 0x10; }
uint16_t gdt_sel_user_code(void){   return 0x18 | 0x03; } /* RPL 3 */
uint16_t gdt_sel_user_data(void){   return 0x20 | 0x03; } /* RPL 3 */
