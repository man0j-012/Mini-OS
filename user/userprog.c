#include <stdint.h>
#include "syscall.h"
void user_echo_task(void* arg){
    (void)arg;
    const char* msg = "Hello from 'user' via int 0x80!\n";
    for(;;){
        sys_write(msg, 34);
        sys_sleep_ms(250);
    }
}
