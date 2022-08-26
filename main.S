.intel_syntax noprefix
.text
.global add
add:
push rbp
mov rbp, rsp
sub rsp, 16
mov [rbp - 8], rdi
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
add rsp, 0
add rsp, 16
mov rsp, rbp
pop rbp
ret
.global main
main:
push rbp
mov rbp, rsp
sub rsp, 0
sub rsp, 24
mov r10, 10
push r10
lea r10, [rbp - 16]
push r10
pop r10
pop r11
mov [r10], r11
push r11
lea r10, [rbp - 8]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
mov r10, 5
push r10
lea r10, [rbp - 24]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
.global add
mov r10, [rip + add@GOTPCREL]
push r10
lea r10, [rbp - 8]
push r10
.global add
mov r10, [rip + add@GOTPCREL]
push r10
lea r10, [rbp - 24]
push r10
.global add
mov r10, [rip + add@GOTPCREL]
push r10
mov r10, 123
push r10
lea r10, [rbp - 16]
push r10
pop rsi
mov rsi, [rsi]
pop rdi
pop rax
call rax
push rax
pop rsi
pop rdi
mov rdi, [rdi]
pop rax
call rax
push rax
pop rsi
pop rdi
mov rdi, [rdi]
pop rax
call rax
push rax
pop rax
mov rsp, rbp
pop rbp
ret
add rsp, 24
add rsp, 0
mov rsp, rbp
pop rbp
ret

