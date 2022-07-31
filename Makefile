ARCH = x86_64-elf-
CC = $(ARCH)gcc
AS = $(ARCH)as
LD = $(ARCH)ld
OBJDUMP = $(ARCH)objcopy
OBJCOPY = $(ARCH)objdump


CFLAGS = -Wall -Werror -Wno-error=infinite-recursion -O -fno-omit-frame-pointer -ggdb
CFLAGS += -MD
CFLAGS += -ffreestanding -fno-common -nostdlib
CFLAGS += -I.
CFLAGS += $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)
CFLAGS += -fno-pie -no-pie -fno-pic
LDFLAGS = -z max-page-size=4096

K=kernel

OBJS = \
	$K/header.o \
	$K/boot.o \
	$K/boot64.o \
	$K/main.o \

kernel.iso: kernel.bin
	mkdir -p isodir/boot/grub
	cp kernel.bin isodir/boot/kernel.bin
	cp $K/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o kernel.iso isodir

kernel.bin: $(OBJS) $K/kernel.ld Makefile
	$(LD) $(LDFLAGS) -T $K/kernel.ld $(OBJS) -o kernel.bin

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: qemu clean run

QEMU = qemu-system-x86_64
QEMUFLAGS =

qemu: kernel.iso
	$(QEMU) $(QEMUFLAGS) -cdrom kernel.iso

clean:
	rm -f kernel.bin kernel.iso $K/*.o $K/*.d xbochs.log bx_enh_dbg.ini
	rm -rf isodir

run: qemu
