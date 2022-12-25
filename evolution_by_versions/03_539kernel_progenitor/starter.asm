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

init_video_mode:
	mov ah, 0h
	mov al, 03h
	int 10h

	mov ah, 01h
	mov cx, 2000h
	int 10h

	ret

setup_interrupts:
	ret

