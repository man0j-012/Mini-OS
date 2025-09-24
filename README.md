## MiniOS – A Minimal x86 Teaching Kernel

MiniOS is a from-scratch x86 kernel written in C and x86 assembly.
It depicts how modern operating systems work at the lowest levels: from boot, to interrupts, to threading, memory management, and running user programs in ring3.

The codebase walks through the fundamental mechanisms behind Unix-like kernels: bootstrapping, interrupts, memory management, scheduling, system calls, and user mode transitions.

---

## Features Implemented

### Bootloader & Protected Mode

Multiboot-compliant, GRUB loads the kernel ELF

Assembly entry point sets up the stack and jumps to the C kernel

### Console & Drivers

VGA text console (direct write to 0xB8000)

Keyboard driver (IRQ1, scancode translation)

### Interrupts & Exceptions

GDT with kernel and user segments

IDT setup with ISR and IRQ stubs

PIC remap to avoid conflicts with CPU exceptions

PIT timer at 100Hz for preemption

### Threading & Scheduling

Cooperative + preemptive round-robin scheduler

Context switch in assembly

Demo threads printing interleaved output

### Memory Management

Paging enabled (identity map of first 32MB)

Simple kernel heap allocator

### Syscalls (int 0x80)

SYS_write → print string via VGA

SYS_yield → yield CPU to scheduler

SYS_sleep_ms → sleep via PIT ticks

DPL=3 syscall gate for user processes

User Mode (Ring 3)

TSS setup so interrupts from ring3 switch to kernel stack

Entry to user mode with iret

Demo ring3 program that calls sys_write and sys_sleep via int 0x80

---

## Project Structure

boot/         # Bootloader entry (multiboot, stack setup)
arch/x86/     # GDT, IDT, ISR, IRQ, PIC, PIT, syscalls, TSS, usermode
kernel/       # Kernel entry and initialization
drivers/      # VGA console, keyboard driver
lib/          # printf, string, port I/O
mm/           # Paging, heap allocator
sched/        # Threading, context switch, scheduler
user/         # Demo ring3 user program
include/      # Shared headers
grub.cfg      # GRUB config
linker.ld     # Linker script
Makefile      # Build system

---

## Building and Running
### Toolchain
Cross-compiler recommended: i686-elf-gcc, i686-elf-ld
Requires: qemu-system-x86, grub-pc-bin, xorriso
### Build
make
### Run under QEMU
make run

### Expected Output
MiniOS: booting (paging+syscalls+ring3)...
[A][B][ring3] Hello from user mode via syscalls!
[A], [B] → kernel threads scheduled round-robin

[ring3] ... → user-mode process executing syscalls through int 0x80

Keyboard input echoed via IRQ1 handler

## Future Extensions

ELF loader for standalone user binaries

Per-process page tables and memory protection

Filesystem driver (e.g., FAT12 or EXT2)

Networking stack (basic NIC + TCP/IP)

Multiprocessor (SMP) bring-up and advanced scheduling

## Author
Manoj Dattatreya Myneni (MMD)

License

MIT License — free to use for learning and educational purposes.
