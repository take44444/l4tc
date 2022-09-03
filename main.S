.intel_syntax noprefix
.text
.global main
main:
push rbp
mov rbp, rsp
sub rsp, 120
mov r10, 10
push r10
lea r10, [rbp - 120]
push r10
mov r10, 3
push r10
pop r11
imul r11, 24
pop r10
add r10, r11
push r10
mov r10, 2
push r10
pop r11
imul r11, 8
pop r10
add r10, r11
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
.global printf
mov r10, [rip + printf@GOTPCREL]
push r10
lea r10, [rip + L0]
push r10
lea r10, [rbp - 120]
push r10
mov r10, 3
push r10
pop r11
imul r11, 24
pop r10
add r10, r11
push r10
mov r10, 2
push r10
pop r11
imul r11, 8
pop r10
add r10, r11
push r10
pop rsi
mov rsi, [rsi]
pop rdi
pop rax
sub rsp, 8
call rax
add rsp, 8
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
