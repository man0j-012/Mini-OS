#pragma once
#include <stdint.h>

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void gdt_install(void);
uint16_t gdt_sel_kernel_code(void);
uint16_t gdt_sel_kernel_data(void);
uint16_t gdt_sel_user_code(void);
uint16_t gdt_sel_user_data(void);
