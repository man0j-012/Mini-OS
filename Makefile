CC      ?= i686-elf-gcc
LD      ?= i686-elf-ld
AS      ?= i686-elf-as

CFLAGS  = -ffreestanding -fno-pic -fno-builtin -fno-stack-protector -nostdlib -m32 -Wall -Wextra -O2 -ggdb -fno-omit-frame-pointer
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib

INCLUDES = -I. -Iinclude

SRC_C = \
  kernel/kmain.c \
  lib/ports.c \
  lib/printf.c \
  lib/string.c \
  drivers/vga.c \
  drivers/keyboard.c \
  arch/x86/idt.c \
  arch/x86/isr.c \
  arch/x86/irq.c \
  arch/x86/pic.c \
  arch/x86/pit.c \
  mm/paging.c \
  arch/x86/syscall.c \
  arch/x86/gdt.c \
  arch/x86/tss.c \
  arch/x86/gdt_flush.s \
  arch/x86/tss_flush.s \
  arch/x86/usermode.s \
  user/ring3_entry.c \
  mm/heap.c \
  sched/thread.c \
  sched/sched.c

SRC_S = \
  boot/boot.s \
  arch/x86/isr_stub.s \
  sched/context.s

OBJ   = $(SRC_C:.c=.o) $(SRC_S:.s=.o)

all: build/os.iso

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.s
	$(AS) --32 $< -o $@

build/kernel.elf: $(OBJ) linker.ld
	mkdir -p build
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

build/os.iso: build/kernel.elf grub.cfg
	mkdir -p build/isofiles/boot/grub
	cp build/kernel.elf build/isofiles/boot/kernel.elf
	cp grub.cfg build/isofiles/boot/grub/grub.cfg
	grub-mkrescue -o build/os.iso build/isofiles

run: build/os.iso
	qemu-system-i386 -cdrom build/os.iso -serial stdio

debug: build/os.iso
	qemu-system-i386 -cdrom build/os.iso -s -S -serial stdio

clean:
	rm -rf build
	find . -name '*.o' -delete

.PHONY: all run debug clean
