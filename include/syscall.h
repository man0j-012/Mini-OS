#pragma once
#include <stdint.h>

enum {
    SYS_write = 0,
    SYS_yield = 1,
    SYS_sleep_ms = 2,
};

void syscall_init(void);
/* User-facing wrappers (work in ring0 for now) */
int sys_write(const char* s, uint32_t len);
void sys_yield(void);
void sys_sleep_ms(uint32_t ms);
