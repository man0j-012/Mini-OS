# MiniOS – A Minimal x86 Teaching Kernel

*A pedagogical kernel that implements the skeleton of a Unix-like OS: from boot to user mode.*  

MiniOS is a compact operating system kernel written in **C and x86 assembly**.  
It demonstrates the **core building blocks** of modern kernels:  
bootstrapping, interrupts, device drivers, memory management, scheduling, syscalls, and user mode transitions.

---

## Table of Contents
- [Features](#features)  
- [Project Structure](#project-structure)  
- [Prerequisites](#prerequisites)  
- [Quick Start](#quick-start)  
- [Architecture Overview](#architecture-overview)  
- [Testing](#testing)  
- [Documentation](#documentation)  
- [Future Work](#future-work)  
- [Authors](#authors)  
- [License](#license)  

---

## Features

### Core Kernel ✅
- **Bootloader & Protected Mode**
  - Multiboot-compliant (GRUB)
  - GDT setup, protected mode entry  

- **Interrupts & Exceptions**
  - IDT with ISR + IRQ stubs
  - PIC remap, PIT timer at 100Hz
  - Fault handlers for CPU exceptions  

- **Drivers**
  - VGA text console (`0xB8000`)
  - PS/2 keyboard (IRQ1, scancodes)  

- **Threading & Scheduling**
  - Cooperative + preemptive round-robin scheduler
  - Context switching in assembly
  - Demo kernel threads interleaving output  

- **Memory Management**
  - Paging enabled (identity map, 32MB)
  - Kernel heap allocator  

- **Syscalls (`int 0x80`)**
  - `SYS_write` → print string
  - `SYS_yield` → yield CPU
  - `SYS_sleep_ms` → sleep with PIT ticks
  - DPL=3 syscall gate  

- **User Mode (Ring 3)**
  - TSS stack switching
  - `iret` transition to user space
  - Demo ring3 process using syscalls  

---

## Project Structure

```text
minios/
 ├── boot/           # Multiboot entry, early assembly
 ├── arch/x86/       # GDT, IDT, ISR, PIC, PIT, syscalls, TSS, usermode
 ├── kernel/         # Kernel entry and initialization
 ├── drivers/        # VGA console, keyboard
 ├── lib/            # printf, string, port I/O helpers
 ├── mm/             # Paging, heap allocator
 ├── sched/          # Threading and scheduler
 ├── user/           # Ring3 demo program
 ├── include/        # Shared headers
 ├── grub.cfg        # GRUB config
 ├── linker.ld       # Linker script
 └── Makefile        # Build system

```

---

## Prerequisites

| Tool                          | Version        | Purpose                |
| ----------------------------- | -------------- | ---------------------- |
| `i686-elf-gcc`, `i686-elf-ld` | Cross-compiler | Build kernel           |
| `qemu-system-x86`             | Latest         | Run kernel in emulator |
| `grub-pc-bin`, `xorriso`      | Latest         | Build bootable ISO     |


## Quick Start
git clone https://github.com/<your-username>/minios.git
cd minios

## Build the kernel + ISO:

make

## Run in QEMU:

make run

## Architecture Overview

### System Components

Bootloader → Multiboot + entry to C kernel

Initialization → GDT, IDT, paging, heap

Core Services → VGA console, keyboard, PIT

Scheduler → Preemptive round-robin

Syscall ABI → int 0x80 with basic system calls

User Mode → Ring 3 via TSS + iret

## Testing
Run the kernel in QEMU and validate:

Timer interrupts trigger preemption (threads A and B interleave)

Keyboard input is echoed via IRQ1

Syscalls (write, yield, sleep) function correctly from ring3

## Documentation
| Document             | Purpose                                   |
| -------------------- | ----------------------------------------- |
| README.md            | Project overview, setup, quick start      |
| docs/ARCHITECTURE.md | System architecture, diagrams             |
| docs/DESIGN.md       | GDT/IDT setup, paging, syscall ABI        |
| docs/FUTUREWORK.md   | Roadmap (ELF loader, SMP, FS, networking) |

## Future Work
ELF loader for standalone user binaries

Per-process page tables and memory isolation

Filesystem driver (FAT/EXT)

Networking stack (basic NIC + TCP/IP)

Multiprocessor (SMP) scheduler

## Author

Manoj Dattatreya Myneni (MMD) – University of Illinois Chicago

## License
This project is licensed under the [MIT License](LICENSE).
