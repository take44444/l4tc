.intel_syntax noprefix
.text
.global add
add:
push rbp
mov rbp, rsp
sub rsp, 8
mov [rbp - 8], rdi
sub rsp, 8
mov [rbp - 16], rsi
lea r10, [rbp - 8]
push r10
lea r10, [rbp - 16]
push r10
pop r11
pop r10
mov r10, [r10]
mov r11, [r11]
add r10, r11
push r10
pop rax
mov rsp, rbp
pop rbp
ret
mov rsp, -16
mov rsp, rbp
pop rbp
ret
.global sub
sub:
push rbp
mov rbp, rsp
sub rsp, 8
mov [rbp - 8], rdi
sub rsp, 8
mov [rbp - 16], rsi
lea r10, [rbp - 8]
push r10
lea r10, [rbp - 16]
push r10
pop r11
pop r10
mov r10, [r10]
mov r11, [r11]
sub r10, r11
push r10
pop rax
mov rsp, rbp
pop rbp
ret
mov rsp, -16
mov rsp, rbp
pop rbp
ret
.global f
f:
push rbp
mov rbp, rsp
sub rsp, 8
mov [rbp - 8], rdi
lea r10, [rbp - 8]
push r10
pop r10
mov r10, [r10]
cmp r10, 0
setnz r10b
jz L0
.global sub
mov r10, [rip + sub@GOTPCREL]
push r10
pop rax
mov rsp, rbp
pop rbp
ret
mov rsp, -8
jmp L1
L0:
.global add
mov r10, [rip + add@GOTPCREL]
push r10
pop rax
mov rsp, rbp
pop rbp
ret
mov rsp, -8
jmp L3
L2:
L3:
L1:
mov rsp, -8
mov rsp, rbp
pop rbp
ret
.global main
main:
push rbp
mov rbp, rsp
sub rsp, 8
sub rsp, 8
sub rsp, 8
sub rsp, 8
mov r10, 0
push r10
lea r10, [rbp - 8]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea r10, [rbp - 8]
push r10
lea r10, [rbp - 16]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
.global f
mov r10, [rip + f@GOTPCREL]
push r10
lea r10, [rbp - 24]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea r10, [rbp - 24]
push r10
lea r10, [rbp - 32]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
.global printf
mov r10, [rip + printf@GOTPCREL]
push r10
lea r10, [rip + L4]
push r10
lea r10, [rbp - 32]
push r10
pop r10
mov r10, [r10]
push r10
lea r10, [rbp - 16]
push r10
pop r10
mov r10, [r10]
push r10
pop rdi
mov rdi, [rdi]
pop rax
mov rax, [rax]
call rax
push rax
mov r10, 100
push r10
mov r10, 200
push r10
pop rsi
pop rdi
pop rax
call rax
push rax
pop rsi
pop rdi
pop rax
call rax
push rax
pop r10
lea r10, [rbp - 16]
push r10
pop r10
mov r10, [r10]
push r10
mov r10, 1
push r10
pop r11
pop r10
mov r10, [r10]
add r10, r11
push r10
lea r10, [rbp - 16]
push r10
pop r10
mov r10, [r10]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
.global printf
mov r10, [rip + printf@GOTPCREL]
push r10
lea r10, [rip + L5]
push r10
lea r10, [rbp - 32]
push r10
pop r10
mov r10, [r10]
push r10
lea r10, [rbp - 8]
push r10
pop rdi
mov rdi, [rdi]
pop rax
mov rax, [rax]
call rax
push rax
mov r10, 100
push r10
mov r10, 200
push r10
pop rsi
pop rdi
pop rax
call rax
push rax
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
mov rsp, 0
mov rsp, rbp
pop rbp
ret
.data
.align 8
L4: .asciz "%d\n"
.align 8
L5: .asciz "%d\n"
