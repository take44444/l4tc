.intel_syntax noprefix
.text
.global main
main:
push rbp
mov rbp, rsp
mov r10, [rip + printf@GOTPCREL]
push r10
lea r10, [rip + L0]
push r10
mov r10, 4
push r10
mov r10, 10
push r10
mov r10, 3
push r10
xor rdx, rdx
pop r10
pop rax
imul r10
mov r10, rax
push r10
pop r11
pop r10
add r10, r11
push r10
pop rsi
pop rdi
pop rax
call rax
push rax
pop r10
mov r10, 0
push r10
pop rax
mov rsp, rbp
pop rbp
ret
lea rsp, [rbp - 0]
mov rsp, rbp
pop rbp
ret
.data
.align 8
L0: .asciz "%d\n"
