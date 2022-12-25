bits 16
extern kernel_main

start:
	mov ax, cs
	mov ds, ax

	call load_gdt
	call init_video_mode
	call enter_protected_mode
	call setup_interrupts

	call 08h:start_kernel

load_gdt:
	cli
	lgdt [gdtr - starter]

	ret

enter_protected_mode:
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	ret

setup_interrupts:
	ret

