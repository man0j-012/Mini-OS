#include <stdint.h>
#include "sched.h"
#include "tty.h"
#include "userprog.h"
#include "syscall.h"
static void worker1(void* a){ (void)a; for(;;){ tty_write("[A] "); for(volatile int i=0;i<100000;i++); } }
static void worker2(void* a){ (void)a; for(;;){ tty_write("[B] "); for(volatile int i=0;i<100000;i++); } }
void spawn_demo_threads(void){ thread_create(worker1,0,"workerA"); thread_create(worker2,0,"workerB"); thread_create((thread_fn)user_echo_task,0,"user_echo"); }
