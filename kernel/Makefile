CFLAGS += -I include

OBJS = \
	src/boot/boot64.o \
	src/boot/boot.o \
	src/boot/header.o \
	src/boot/multiboot2.o \
	src/check/panic.o \
	src/check/ssp.o \
	src/check/ubsan.o \
	src/cpu/gdt.o \
	src/cpu/idt.o \
	src/cpu/io.o \
	src/cpu/irq.o \
	src/cpu/irq_stub.o \
	src/cpu/pic.o \
	src/cpu/tss.o \
	src/devices/disc.o \
	src/devices/keyboard.o \
	src/devices/serial.o \
	src/devices/timer.o \
	src/fs/ext2.o \
	src/libk/list.o \
	src/libk/math.o \
	src/libk/serial_stdio.o \
	src/libk/stdio.o \
	src/libk/string.o \
	src/main.o \
	src/mem/heap.o \
	src/mem/paging_asm.o \
	src/mem/paging.o \
	src/misc/graphics.o \
	src/misc/stdbuff.o \
	src/sys/syscall_asm.o \
	src/sys/syscall.o \
	src/sys/userspace_asm.o \
	src/sys/userspace.o \
	src/scheduler/scheduler.o \
	src/scheduler/switch_process.o \
	src/scheduler/process.o \

all: kernel.bin

kernel.bin: $(OBJS) src/kernel.ld ../Makefile Makefile
	$(info [kernel] $@)
	@$(LD) $(LDFLAGS) -T src/kernel.ld $(OBJS) -o kernel.bin


%.o: %.c
	$(info [kernel] $@)
	@$(CC) $(CFLAGS) -c $< -o $@


%.o: %.S
	$(info [kernel] $@)
	@$(CC) $(CFLAGS) -c $< -o $@


.PHONY: all

-include $(OBJS:.o=.d)
