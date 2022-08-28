.intel_syntax noprefix
.text
.global test
test:
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
.global test
mov r10, [rip + test@GOTPCREL]
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
.global test
mov r10, [rip + test@GOTPCREL]
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
add rsp, 0
jmp L3
L2:
mov r10, 1
push r10
pop rax
mov rsp, rbp
pop rbp
ret
add rsp, 0
jmp L5
L4:
L5:
L3:
add rsp, 0
jmp L1
L0:
mov r10, 1
push r10
pop rax
mov rsp, rbp
pop rbp
ret
add rsp, 0
jmp L7
L6:
L7:
L1:
add rsp, 0
add rsp, 8
mov rsp, rbp
pop rbp
ret
.global main
main:
push rbp
mov rbp, rsp
sub rsp, 0
.global test
mov r10, [rip + test@GOTPCREL]
push r10
mov r10, 12
push r10
pop rdi
pop rax
sub rsp, 8
call rax
add rsp, 8
push rax
pop rax
mov rsp, rbp
pop rbp
ret
add rsp, 0
add rsp, 0
mov rsp, rbp
pop rbp
ret

