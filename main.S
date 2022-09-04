.intel_syntax noprefix
.text
.global main
main:
push rbp
mov rbp, rsp
sub rsp, 32
sub rsp, 56
mov r10, 10
push r10
lea r10, [rbp - 32]
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
lea r10, [rbp - 32]
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
lea r10, [rbp - 32]
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
lea r10, [rbp - 88]
push r10
pop r10
add r10, 0
push r10
pop r10
pop r11
push r11
mov r12, [r11]
mov [r10], r12
add r10, 8
add r11, 8
mov r12, [r11]
mov [r10], r12
add r10, 8
add r11, 8
pop r10
lea r10, [rbp - 32]
push r10
lea r10, [rbp - 88]
push r10
pop r10
add r10, 16
push r10
pop r10
pop r11
push r11
mov r12, [r11]
mov [r10], r12
add r10, 8
add r11, 8
mov r12, [r11]
mov [r10], r12
add r10, 8
add r11, 8
mov r12, [r11]
mov [r10], r12
add r10, 8
add r11, 8
mov r12, [r11]
mov [r10], r12
add r10, 8
add r11, 8
pop r10
lea r10, [rbp - 32]
push r10
lea r10, [rbp - 88]
push r10
pop r10
add r10, 48
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
mov r10, 100
push r10
lea r10, [rbp - 32]
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
mov r10, 200
push r10
lea r10, [rbp - 88]
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
mov r10, 300
push r10
lea r10, [rbp - 88]
push r10
pop r10
add r10, 16
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
.global printf
mov r10, [rip + printf@GOTPCREL]
push r10
lea r10, [rip + L0]
push r10
lea r10, [rbp - 32]
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
sub rsp, 8
call rax
add rsp, 8
push rax
pop r10
.global printf
mov r10, [rip + printf@GOTPCREL]
push r10
lea r10, [rip + L1]
push r10
lea r10, [rbp - 32]
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
sub rsp, 8
call rax
add rsp, 8
push rax
pop r10
.global printf
mov r10, [rip + printf@GOTPCREL]
push r10
lea r10, [rip + L2]
push r10
lea r10, [rbp - 88]
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
sub rsp, 8
call rax
add rsp, 8
push rax
pop r10
.global printf
mov r10, [rip + printf@GOTPCREL]
push r10
lea r10, [rip + L3]
push r10
lea r10, [rbp - 88]
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
sub rsp, 8
call rax
add rsp, 8
push rax
pop r10
.global printf
mov r10, [rip + printf@GOTPCREL]
push r10
lea r10, [rip + L4]
push r10
lea r10, [rbp - 88]
push r10
pop r10
add r10, 16
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
sub rsp, 8
call rax
add rsp, 8
push rax
pop r10
.global printf
mov r10, [rip + printf@GOTPCREL]
push r10
lea r10, [rip + L5]
push r10
lea r10, [rbp - 88]
push r10
pop r10
add r10, 16
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
sub rsp, 8
call rax
add rsp, 8
push rax
pop r10
.global printf
mov r10, [rip + printf@GOTPCREL]
push r10
lea r10, [rip + L6]
push r10
lea r10, [rbp - 88]
push r10
pop r10
add r10, 48
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
sub rsp, 8
call rax
add rsp, 8
push rax
pop r10
.global printf
mov r10, [rip + printf@GOTPCREL]
push r10
lea r10, [rip + L7]
push r10
lea r10, [rbp - 88]
push r10
pop r10
add r10, 48
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
.align 8
L1: .asciz "%d\n"
.align 8
L2: .asciz "%d\n"
.align 8
L3: .asciz "%d\n"
.align 8
L4: .asciz "%d\n"
.align 8
L5: .asciz "%d\n"
.align 8
L6: .asciz "%d\n"
.align 8
L7: .asciz "%d\n"
