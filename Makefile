export

ARCH = x86_64-elf-
CC = $(ARCH)gcc
AS = $(ARCH)as
LD = $(ARCH)ld
OBJDUMP = $(ARCH)objcopy
OBJCOPY = $(ARCH)objdump

W := -Wall -Werror -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align
W += -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations
W += -Wredundant-decls -Wnested-externs -Winline -Wno-long-long -Wconversion
W += -Wstrict-prototypes
WNO := -Wno-error=unused-parameter -Wno-error=unused-variable
WNO += -Wno-error=infinite-recursion

CFLAGS = $(W) $(WNO) -fno-omit-frame-pointer -mcmodel=large
CFLAGS += -mgeneral-regs-only # disables SIMD instructions
CFLAGS += -MD -O3 -ffreestanding -nostdlib
CFLAGS += -fno-common -fno-stack-protector
CFLAGS += -fno-pie -no-pie -fno-pic
LDFLAGS = -z max-page-size=4096

MAKE:=$(MAKE) -s

K = kernel

all: kernel.iso

$K/kernel.bin: FORCE
	@$(MAKE) -C $(@D)
FORCE:

kernel.iso: $K/kernel.bin $K/src/grub.cfg Makefile
	$(info [all] $@)
	@mkdir -p isodir/boot/grub
	@cp $K/kernel.bin isodir/boot/kernel.bin
	@cp $K/src/grub.cfg isodir/boot/grub/grub.cfg
	@grub-mkrescue -o kernel.iso isodir 2> /dev/null


.PHONY: all build qemu bochs clean

QEMU = qemu-system-x86_64
BOCHS = bochs -q

qemu: kernel.iso
	$(QEMU) -cdrom kernel.iso

bochs: kernel.iso
	$(BOCHS) -qf .bochsrc

clean:
	@find -name "*.o" -exec rm {} \;
	@find -name "*.d" -exec rm {} \;
	@rm -f kernel.iso $K/kernel.bin xbochs.log bx_enh_dbg.ini
	@rm -rf isodir
