export

ARCH = x86_64-elf-
CC = $(ARCH)gcc
AS = $(ARCH)as
LD = $(ARCH)ld
OBJDUMP = $(ARCH)objcopy
OBJCOPY = $(ARCH)objdump

W := -Wall -Werror -Wextra -Wshadow -Wcast-align
# W:= -Wpointer-arith -pedantic -Wmissing-prototypes -Wmissing-declarations
W += -Wwrite-strings -Wredundant-decls -Wnested-externs -Winline -Wno-long-long
W += -Wconversion -Wstrict-prototypes
WNO := -Wno-error=unused-parameter -Wno-error=unused-variable
WNO += -Wno-error=unused-but-set-variable -Wno-error=unused-but-set-parameter
WNO += -Wno-error=infinite-recursion

CFLAGS = $(W) $(WNO) -fno-omit-frame-pointer -mcmodel=large
CFLAGS += -mgeneral-regs-only # disables SIMD instructions
CFLAGS += -MD -O3 -ffreestanding -nostdlib -std=gnu89
CFLAGS += -fno-common -fno-stack-protector
CFLAGS += -fno-pie -no-pie -fno-pic
CFLAGS += -g -fsanitize=undefined
#CFLAGS += -fstack-protector-all
LDFLAGS = -z max-page-size=4096

MAKE:=$(MAKE) -s

K = kernel

all: kernel.iso

FORCE:

$K/kernel.bin: FORCE
	@$(MAKE) -C $(@D)

isodir:
	$(info [all] $@)
	@mkdir -p isodir/boot/grub
	@cp $K/src/grub.cfg isodir/boot/grub/grub.cfg
	@mkdir -p isodir/modules
	@dd if=/dev/zero of=isodir/modules/ext2.img bs=4M count=2 > /dev/null 2>&1
	@mkfs.ext2 isodir/modules/ext2.img > /dev/null 2>&1

kernel.iso: $K/kernel.bin $K/src/grub.cfg Makefile isodir FORCE
	$(info [all] $@)
	@cp $K/kernel.bin isodir/boot/kernel.bin
	@grub-mkrescue -o kernel.iso isodir > /dev/null 2>&1


.PHONY: all build qemu bochs mount umount clean

QEMU = qemu-system-x86_64
BOCHS = bochs -q

qemu: kernel.iso
	@$(QEMU) -machine q35 -cdrom kernel.iso -serial file:serial.log -smp 4
	@cat serial.log

bochs: kernel.iso
	@$(BOCHS) -qf .bochsrc
	@cat serial.log

mount: isodir
ifeq ("$(wildcard /mnt/ext2/lost+found/)","")
	$(info [all] $@)
	@doas mount isodir/modules/ext2.img /mnt/ext2 -o loop
	@doas chown -R aleksa:aleksa /mnt/ext2
endif

umount:
ifneq ("$(wildcard /mnt/ext2/lost+found/)","")
	$(info [all] $@)
	@doas umount /mnt/ext2
endif

clean:
	@find -name "*.o" -exec rm {} \;
	@find -name "*.d" -exec rm {} \;
	@rm -f kernel.iso $K/kernel.bin xbochs.log bx_enh_dbg.ini log serial.log
	@rm -rf isodir
	@$(MAKE) umount
