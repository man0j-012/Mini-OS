#include <stdint.h>
#include "syscall.h"

void user_ring3_main(void){
    const char* msg = "[ring3] Hello from user mode via syscalls!\n";
    for(;;){
        sys_write(msg, 41);
        sys_sleep_ms(500);
    }
}
