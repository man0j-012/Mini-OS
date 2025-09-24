#include <stdint.h>
#include <stddef.h>
#include "heap.h"
#include "string.h"
static uint8_t* heap_start; static uint8_t* heap_end; static uint8_t* heap_cur;
void heap_init(void* start, void* end){ heap_start=start; heap_end=end; heap_cur=heap_start; }
void* kmalloc(size_t size){ if(size==0) return 0; size=(size+15)&~((size_t)15); if(heap_cur+size>heap_end) return 0; void* p=heap_cur; heap_cur+=size; return p; }
void kfree(void* p){ (void)p; }
