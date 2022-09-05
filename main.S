.intel_syntax noprefix
.text
.global main
main:
push rbp
mov rbp, rsp
sub rsp, 40
sub rsp, 40
lea r10, [rbp - 80]
push r10
lea r10, [rbp - 40]
push r10
pop r10
add r10, 32
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea r10, [rbp - 40]
push r10
lea r10, [rbp - 80]
push r10
pop r10
add r10, 32
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
mov r10, 10
push r10
lea r10, [rbp - 40]
push r10
pop r10
add r10, 0
push r10
mov r10, 0
push r10
pop r11
imul r11, 16
pop r10
add r10, r11
push r10
pop r10
add r10, 0
push r10
mov r10, 0
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
mov r10, 11
push r10
lea r10, [rbp - 80]
push r10
pop r10
add r10, 0
push r10
mov r10, 0
push r10
pop r11
imul r11, 16
pop r10
add r10, r11
push r10
pop r10
add r10, 0
push r10
mov r10, 0
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
mov r10, 20
push r10
lea r10, [rbp - 40]
push r10
pop r10
add r10, 0
push r10
mov r10, 0
push r10
pop r11
imul r11, 16
pop r10
add r10, r11
push r10
pop r10
add r10, 0
push r10
mov r10, 1
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
mov r10, 21
push r10
lea r10, [rbp - 80]
push r10
pop r10
add r10, 0
push r10
mov r10, 0
push r10
pop r11
imul r11, 16
pop r10
add r10, r11
push r10
pop r10
add r10, 0
push r10
mov r10, 1
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
mov r10, 30
push r10
lea r10, [rbp - 40]
push r10
pop r10
add r10, 0
push r10
mov r10, 1
push r10
pop r11
imul r11, 16
pop r10
add r10, r11
push r10
pop r10
add r10, 0
push r10
mov r10, 0
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
mov r10, 31
push r10
lea r10, [rbp - 80]
push r10
pop r10
add r10, 0
push r10
mov r10, 1
push r10
pop r11
imul r11, 16
pop r10
add r10, r11
push r10
pop r10
add r10, 0
push r10
mov r10, 0
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
mov r10, 40
push r10
lea r10, [rbp - 40]
push r10
pop r10
add r10, 0
push r10
mov r10, 1
push r10
pop r11
imul r11, 16
pop r10
add r10, r11
push r10
pop r10
add r10, 0
push r10
mov r10, 1
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
mov r10, 41
push r10
lea r10, [rbp - 80]
push r10
pop r10
add r10, 0
push r10
mov r10, 1
push r10
pop r11
imul r11, 16
pop r10
add r10, r11
push r10
pop r10
add r10, 0
push r10
mov r10, 1
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
mov r10, [rip + printf@GOTPCREL]
push r10
lea r10, [rip + L0]
push r10
lea r10, [rbp - 40]
push r10
pop r10
add r10, 32
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 32
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 32
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 32
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 0
push r10
mov r10, 1
push r10
pop r11
imul r11, 16
pop r10
add r10, r11
push r10
pop r10
add r10, 0
push r10
mov r10, 0
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
call rax
push rax
pop r10
mov r10, [rip + printf@GOTPCREL]
push r10
lea r10, [rip + L1]
push r10
lea r10, [rbp - 40]
push r10
pop r10
add r10, 32
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 32
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 32
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 32
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 32
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 0
push r10
mov r10, 1
push r10
pop r11
imul r11, 16
pop r10
add r10, r11
push r10
pop r10
add r10, 0
push r10
mov r10, 1
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
.align 8
L1: .asciz "%d\n"
