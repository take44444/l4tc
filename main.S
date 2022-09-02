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
lea rsp, [rbp - 16]
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
lea rsp, [rbp - 16]
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
lea rsp, [rbp - 8]
jmp L1
L0:
.global add
mov r10, [rip + add@GOTPCREL]
push r10
pop rax
mov rsp, rbp
pop rbp
ret
lea rsp, [rbp - 8]
jmp L3
L2:
L3:
L1:
lea rsp, [rbp - 8]
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
sub rsp, 8
.global g1
mov r10, [rip + g1@GOTPCREL]
push r10
lea r10, [rbp - 8]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
mov r10, 100
push r10
.global g1
mov r10, [rip + g1@GOTPCREL]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
mov r10, 200
push r10
.global g2
mov r10, [rip + g2@GOTPCREL]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
mov r10, 0
push r10
lea r10, [rbp - 16]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea r10, [rbp - 16]
push r10
lea r10, [rbp - 24]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
.global f
mov r10, [rip + f@GOTPCREL]
push r10
lea r10, [rbp - 32]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea r10, [rbp - 32]
push r10
lea r10, [rbp - 40]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
sub rsp, 16
sub rsp, 8
sub rsp, 8
lea r10, [rbp - 64]
push r10
lea r10, [rbp - 72]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea r10, [rbp - 56]
push r10
lea r10, [rbp - 64]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea r10, [rip + L4]
push r10
lea r10, [rbp - 56]
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
.global printf
mov r10, [rip + printf@GOTPCREL]
push r10
lea r10, [rbp - 72]
push r10
pop r10
mov r10, [r10]
push r10
pop rdi
mov rdi, [rdi]
pop rax
sub rsp, 8
call rax
add rsp, 8
push rax
pop r10
lea rsp, [rbp - 40]
.global printf
mov r10, [rip + printf@GOTPCREL]
push r10
lea r10, [rip + L5]
push r10
lea r10, [rbp - 40]
push r10
pop r10
mov r10, [r10]
push r10
lea r10, [rbp - 24]
push r10
pop r10
mov r10, [r10]
push r10
pop rdi
mov rdi, [rdi]
pop rax
mov rax, [rax]
sub rsp, 8
call rax
add rsp, 8
push rax
lea r10, [rbp - 8]
push r10
pop r10
mov r10, [r10]
push r10
.global g2
mov r10, [rip + g2@GOTPCREL]
push r10
pop rsi
mov rsi, [rsi]
pop rdi
mov rdi, [rdi]
pop rax
call rax
push rax
pop rsi
pop rdi
pop rax
sub rsp, 8
call rax
add rsp, 8
push rax
pop r10
lea r10, [rbp - 24]
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
lea r10, [rbp - 24]
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
lea r10, [rip + L6]
push r10
lea r10, [rbp - 40]
push r10
pop r10
mov r10, [r10]
push r10
lea r10, [rbp - 16]
push r10
pop rdi
mov rdi, [rdi]
pop rax
mov rax, [rax]
sub rsp, 8
call rax
add rsp, 8
push rax
.global g1
mov r10, [rip + g1@GOTPCREL]
push r10
.global g2
mov r10, [rip + g2@GOTPCREL]
push r10
pop rsi
mov rsi, [rsi]
pop rdi
mov rdi, [rdi]
pop rax
call rax
push rax
pop rsi
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
L4: .asciz "Hello, World!\n"
.align 8
L5: .asciz "%d\n"
.align 8
L6: .asciz "%d\n"
.align 8
.global g1
g1:
.byte 0, 0, 0, 0, 0, 0, 0, 0
.align 8
.global g2
g2:
.byte 0, 0, 0, 0, 0, 0, 0, 0
