#include <stdint.h>
#include <stddef.h>
#include "sched.h"
#include "heap.h"
#include "tty.h"
#include "printf.h"
#define MAX_THREADS 16
#define STACK_SIZE (8*1024)
typedef enum { T_READY, T_RUNNING, T_BLOCKED, T_ZOMBIE } tstate_t;
typedef struct thread{ uint32_t* sp; uint8_t* kstack; tstate_t state; const char* name; } thread_t;
static thread_t thrs[MAX_THREADS]; static int current=-1; static int thr_count=0;
extern void context_switch(uint32_t** old_sp_out, uint32_t* new_sp);
static uint32_t* setup_stack(void (*entry)(void*), void* arg, uint8_t* base){
    uint32_t* sp=(uint32_t*)(base+STACK_SIZE);
    *(--sp)=0;              /* ret addr */
    *(--sp)=(uint32_t)arg;  /* arg */
    *(--sp)=(uint32_t)entry;/* eip */
    *(--sp)=0; *(--sp)=0; *(--sp)=0; *(--sp)=0; /* edi,esi,ebx,ebp */
    return sp;
}
int thread_create(thread_fn fn, void* arg, const char* name){
    if(thr_count>=MAX_THREADS) return -1;
    int id=thr_count++;
    thrs[id].kstack=(uint8_t*)kmalloc(STACK_SIZE); if(!thrs[id].kstack) return -1;
    thrs[id].sp=setup_stack(fn,arg,thrs[id].kstack);
    thrs[id].state=T_READY; thrs[id].name=name;
    return id;
}
void yield(void){
    if(thr_count<2) return;
    int next=(current+1)%thr_count; if(next==current) return;
    int prev=current; current=next;
    thrs[prev].state=T_READY; thrs[current].state=T_RUNNING;
    context_switch(&thrs[prev].sp, thrs[current].sp);
}
void schedule_from_timer(void){
    if(thr_count<2) return;
    int next=(current+1)%thr_count; if(next==current) return;
    int prev=current; current=next;
    thrs[prev].state=T_READY; thrs[current].state=T_RUNNING;
    context_switch(&thrs[prev].sp, thrs[current].sp);
}
void sched_init(void){
    current=0; thrs[0].kstack=(uint8_t*)kmalloc(STACK_SIZE);
    thrs[0].sp=setup_stack((thread_fn)0,0,thrs[0].kstack);
    thrs[0].state=T_RUNNING; thrs[0].name="idle";
}
void scheduler_dump(void){
    kprintf("Threads: %d current=%d\n",(unsigned)thr_count,(unsigned)current);
    for(int i=0;i<thr_count;i++){ kprintf("  [%d] %s sp=%x state=%d\n",(unsigned)i, thrs[i].name?thrs[i].name:"(nil)", (unsigned)thrs[i].sp, (unsigned)thrs[i].state); }
}
