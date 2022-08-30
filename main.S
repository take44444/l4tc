.intel_syntax noprefix
.text
.global fib
fib:
push rbp
mov rbp, rsp
sub rsp, 16
mov [rbp - 8], rdi
lea r10, [rbp - 8]
push r10
pop r10
mov r10, [r10]
cmp r10, 0
setnz r10b
jz L0
lea r10, [rbp - 8]
push r10
mov r10, 1
push r10
pop r11
pop r10
mov r10, [r10]
sub r10, r11
push r10
pop r10
cmp r10, 0
setnz r10b
jz L2
.global fib
mov r10, [rip + fib@GOTPCREL]
push r10
lea r10, [rbp - 8]
push r10
mov r10, 2
push r10
pop r11
pop r10
mov r10, [r10]
sub r10, r11
push r10
pop rdi
pop rax
call rax
push rax
.global fib
mov r10, [rip + fib@GOTPCREL]
push r10
lea r10, [rbp - 8]
push r10
mov r10, 1
push r10
pop r11
pop r10
mov r10, [r10]
sub r10, r11
push r10
pop rdi
pop rax
sub rsp, 8
call rax
add rsp, 8
push rax
pop r11
pop r10
add r10, r11
push r10
pop rax
mov rsp, rbp
pop rbp
ret
mov rsp, -16
jmp L3
L2:
mov r10, 1
push r10
pop rax
mov rsp, rbp
pop rbp
ret
mov rsp, -16
jmp L5
L4:
L5:
L3:
mov rsp, -16
jmp L1
L0:
mov r10, 1
push r10
pop rax
mov rsp, rbp
pop rbp
ret
mov rsp, -16
jmp L7
L6:
L7:
L1:
mov rsp, -16
mov rsp, rbp
pop rbp
ret
.global main
main:
push rbp
mov rbp, rsp
sub rsp, 0
.global printf
mov r10, [rip + printf@GOTPCREL]
push r10
lea r10, [rip + L8]
push r10
.global fib
mov r10, [rip + fib@GOTPCREL]
push r10
mov r10, 30
push r10
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
L8: .asciz "%d\n"
