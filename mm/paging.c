#include <stdint.h>
#include "paging.h"

/* Simple identity map for first 32MB using 8 page tables */
__attribute__((aligned(4096))) static uint32_t page_directory[1024];
__attribute__((aligned(4096))) static uint32_t page_tables[8][1024];

static inline void load_cr3(uint32_t val){ __asm__ __volatile__("mov %0, %%cr3"::"r"(val)); }
static inline uint32_t read_cr0(void){ uint32_t v; __asm__ __volatile__("mov %%cr0,%0":"=r"(v)); return v; }
static inline void write_cr0(uint32_t v){ __asm__ __volatile__("mov %0, %%cr0"::"r"(v)); }
static inline uint32_t read_cr4(void){ uint32_t v; __asm__ __volatile__("mov %%cr4,%0":"=r"(v)); return v; }
static inline void write_cr4(uint32_t v){ __asm__ __volatile__("mov %0, %%cr4"::"r"(v)); }

void paging_init_identity_32mb(void){
    /* clear PD */
    for(int i=0;i<1024;i++) page_directory[i]=0;
    for(int t=0;t<8;t++){
        for(int i=0;i<1024;i++){
            uint32_t phys = (t*1024 + i) * 0x1000; /* 4KB * index */
            page_tables[t][i] = phys | 0x3; /* present + rw */
        }
        page_directory[t] = ((uint32_t)page_tables[t]) | 0x3; /* present + rw */
    }
    load_cr3((uint32_t)page_directory);
    /* Enable paging bit in CR0 */
    uint32_t cr0 = read_cr0();
    cr0 |= 0x80000000u; /* PG */
    write_cr0(cr0);
}
