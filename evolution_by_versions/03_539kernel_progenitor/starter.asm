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

setup_interrupts:
	ret


