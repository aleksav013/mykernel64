// multiboot tags
.set TAG_END,		0
.set TAG_FRAMEBUFFER,	5

// multiboot flags
.set TAG_REQUIRED,	0
.set TAG_OPTIONAL,	1

# multiboot2 header constants
.set MAGIC,		0xe85250d6
.set ARCH,		0
.set HEADER_LENGTH,	(header_end - header_start)
.set CHECKSUM,		0x100000000 - (MAGIC + ARCH + HEADER_LENGTH)

.section .multiboot_header, "a"
.align 4
header_start:
	# magic
	.align 8
	.long MAGIC
	.long ARCH
	.long HEADER_LENGTH
	.long CHECKSUM

	# framebuffer
	.align 8
	.word TAG_FRAMEBUFFER
	.word TAG_REQUIRED
	.long 20
	.long 1024
	.long 768
	.long 32

	# end tag
	.align 8
	.word TAG_END
	.word TAG_REQUIRED
	.long 8

header_end:
