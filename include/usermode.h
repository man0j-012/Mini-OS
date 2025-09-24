#pragma once
#include <stdint.h>
void enter_user_mode(uint32_t entry, uint32_t user_stack_top, uint16_t user_ds, uint16_t user_cs);
