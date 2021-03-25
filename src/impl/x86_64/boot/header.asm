section .multiboot_header
header_start:
	; magic number
	dd 0x1BADB002 ; multiboot 1
	dd 0x00000007 ; flags for multiboot 1: Give video info (4) + Give mem info (2) + Align kernel to 4kb (1)
	; checksum
	dd 0x100000000 - (0x1BADB002 + 0x7)
	dd 0
	dd 0
	dd 0
	dd 0
	dd 0
	dd 1
	dd 80
	dd 25
	dd 0
header_end:
