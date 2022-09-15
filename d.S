.intel_syntax noprefix
.text
.global main
init:
push rbp
mov rbp, rsp
sub rsp, 8
mov [rbp - 8], rdi
mov r10, 0
push r10
lea r10, [rbp - 8]
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 0
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
mov r10, 1
push r10
lea r10, [rbp - 8]
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 16
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
mov r10, [rip + realloc@GOTPCREL]
push r10
mov r10, 0
push r10
mov r10, 8
push r10
pop rsi
pop rdi
pop rax
sub rsp, 8
call rax
add rsp, 8
push rax
lea r10, [rbp - 8]
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 8
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
L0:
mov r10, 1
push r10
pop r10
cmp r10, 0
jz L1
lea r10, [rbp - 8]
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 8
push r10
pop r10
mov r10, [r10]
cmp r10, 0
jz L2
jmp L1
lea rsp, [rbp - 8]
jmp L3
L2:
L3:
mov r10, [rip + realloc@GOTPCREL]
push r10
mov r10, 0
push r10
mov r10, 8
push r10
pop rsi
pop rdi
pop rax
sub rsp, 8
call rax
add rsp, 8
push rax
lea r10, [rbp - 8]
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 8
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea rsp, [rbp - 8]
jmp L0
L1:
mov r10, 0
push r10
pop rax
mov rsp, rbp
pop rbp
ret
lea rsp, [rbp - 8]
mov rsp, rbp
pop rbp
ret
push:
push rbp
mov rbp, rsp
sub rsp, 8
mov [rbp - 8], rdi
sub rsp, 8
mov [rbp - 16], rsi
lea r10, [rbp - 8]
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 0
push r10
lea r10, [rbp - 8]
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 16
push r10
pop r11
pop r10
mov r10, [r10]
mov r11, [r11]
cmp r10, r11
sete r10b
movzx r10, r10b
push r10
pop r10
cmp r10, 0
jz L4
sub rsp, 8
lea r10, [rbp - 8]
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 16
push r10
mov r10, 2
push r10
xor rdx, rdx
pop r10
pop rax
mov rax, [rax]
imul r10
mov r10, rax
push r10
lea r10, [rbp - 8]
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 16
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
mov r10, [rip + realloc@GOTPCREL]
push r10
lea r10, [rbp - 8]
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 8
push r10
lea r10, [rbp - 8]
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 16
push r10
mov r10, 8
push r10
xor rdx, rdx
pop r10
pop rax
mov rax, [rax]
imul r10
mov r10, rax
push r10
pop rsi
pop rdi
mov rdi, [rdi]
pop rax
sub rsp, 8
call rax
add rsp, 8
push rax
lea r10, [rbp - 24]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
L6:
mov r10, 1
push r10
pop r10
cmp r10, 0
jz L7
lea r10, [rbp - 24]
push r10
pop r10
mov r10, [r10]
cmp r10, 0
jz L8
jmp L7
lea rsp, [rbp - 24]
jmp L9
L8:
L9:
mov r10, [rip + realloc@GOTPCREL]
push r10
lea r10, [rbp - 8]
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 8
push r10
lea r10, [rbp - 8]
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 16
push r10
mov r10, 8
push r10
xor rdx, rdx
pop r10
pop rax
mov rax, [rax]
imul r10
mov r10, rax
push r10
pop rsi
pop rdi
mov rdi, [rdi]
pop rax
sub rsp, 8
call rax
add rsp, 8
push rax
lea r10, [rbp - 24]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea rsp, [rbp - 24]
jmp L6
L7:
lea r10, [rbp - 24]
push r10
lea r10, [rbp - 8]
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 8
push r10
pop r10
pop r11
mov r11, [r11]
mov [r10], r11
push r11
pop r10
lea rsp, [rbp - 16]
jmp L5
L4:
L5:
lea r10, [rbp - 16]
push r10
lea r10, [rbp - 8]
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 8
push r10
pop r10
mov r10, [r10]
push r10
lea r10, [rbp - 8]
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 0
push r10
pop r11
mov r11, [r11]
imul r11, 8
pop r10
add r10, r11
push r10
pop r10
pop r11
mov r11, [r11]
mov [r10], r11
push r11
pop r10
lea r10, [rbp - 8]
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
pop r10
mov r10, [r10]
add r10, r11
push r10
lea r10, [rbp - 8]
push r10
pop r10
mov r10, [r10]
push r10
pop r10
add r10, 0
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
mov r10, 0
push r10
pop rax
mov rsp, rbp
pop rbp
ret
lea rsp, [rbp - 16]
mov rsp, rbp
pop rbp
ret
dfs:
push rbp
mov rbp, rsp
sub rsp, 8
mov [rbp - 8], rdi
sub rsp, 8
mov [rbp - 16], rsi
sub rsp, 8
mov [rbp - 24], rdx
sub rsp, 8
mov [rbp - 32], rcx
sub rsp, 8
mov [rbp - 40], r8
sub rsp, 8
mov r10, 1
push r10
lea r10, [rbp - 24]
push r10
pop r10
mov r10, [r10]
push r10
lea r10, [rbp - 8]
push r10
pop r11
mov r11, [r11]
imul r11, 8
pop r10
add r10, r11
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
mov r10, 0
push r10
lea r10, [rbp - 48]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
L10:
lea r10, [rbp - 48]
push r10
lea r10, [rbp - 16]
push r10
pop r10
mov r10, [r10]
push r10
lea r10, [rbp - 8]
push r10
pop r11
mov r11, [r11]
imul r11, 24
pop r10
add r10, r11
push r10
pop r10
add r10, 0
push r10
pop r11
pop r10
mov r10, [r10]
mov r11, [r11]
cmp r10, r11
setl r10b
movzx r10, r10b
push r10
pop r10
cmp r10, 0
jz L11
lea r10, [rbp - 24]
push r10
pop r10
mov r10, [r10]
push r10
lea r10, [rbp - 16]
push r10
pop r10
mov r10, [r10]
push r10
lea r10, [rbp - 8]
push r10
pop r11
mov r11, [r11]
imul r11, 24
pop r10
add r10, r11
push r10
pop r10
add r10, 8
push r10
pop r10
mov r10, [r10]
push r10
lea r10, [rbp - 48]
push r10
pop r11
mov r11, [r11]
imul r11, 8
pop r10
add r10, r11
push r10
pop r11
mov r11, [r11]
imul r11, 8
pop r10
add r10, r11
push r10
mov r10, 0
push r10
pop r11
pop r10
mov r10, [r10]
cmp r10, r11
sete r10b
movzx r10, r10b
push r10
pop r10
cmp r10, 0
jz L12
mov r10, [rip + dfs@GOTPCREL]
push r10
lea r10, [rbp - 16]
push r10
pop r10
mov r10, [r10]
push r10
lea r10, [rbp - 8]
push r10
pop r11
mov r11, [r11]
imul r11, 24
pop r10
add r10, r11
push r10
pop r10
add r10, 8
push r10
pop r10
mov r10, [r10]
push r10
lea r10, [rbp - 48]
push r10
pop r11
mov r11, [r11]
imul r11, 8
pop r10
add r10, r11
push r10
lea r10, [rbp - 16]
push r10
lea r10, [rbp - 24]
push r10
lea r10, [rbp - 32]
push r10
lea r10, [rbp - 40]
push r10
pop r8
mov r8, [r8]
pop rcx
mov rcx, [rcx]
pop rdx
mov rdx, [rdx]
pop rsi
mov rsi, [rsi]
pop rdi
mov rdi, [rdi]
pop rax
call rax
push rax
pop r10
lea rsp, [rbp - 48]
jmp L13
L12:
L13:
lea r10, [rbp - 48]
push r10
mov r10, 1
push r10
pop r11
pop r10
mov r10, [r10]
add r10, r11
push r10
lea r10, [rbp - 48]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea rsp, [rbp - 48]
jmp L10
L11:
lea r10, [rbp - 8]
push r10
lea r10, [rbp - 40]
push r10
pop r10
mov r10, [r10]
push r10
lea r10, [rbp - 32]
push r10
pop r10
mov r10, [r10]
push r10
pop r11
mov r11, [r11]
imul r11, 8
pop r10
add r10, r11
push r10
pop r10
pop r11
mov r11, [r11]
mov [r10], r11
push r11
pop r10
lea r10, [rbp - 32]
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
lea r10, [rbp - 32]
push r10
pop r10
mov r10, [r10]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
mov r10, 0
push r10
pop rax
mov rsp, rbp
pop rbp
ret
lea rsp, [rbp - 40]
mov rsp, rbp
pop rbp
ret
main:
push rbp
mov rbp, rsp
sub rsp, 8
sub rsp, 8
sub rsp, 2400000
sub rsp, 2400000
sub rsp, 8
sub rsp, 800000
sub rsp, 8
sub rsp, 800000
sub rsp, 800000
sub rsp, 800000
mov r10, [rip + scanf@GOTPCREL]
push r10
lea r10, [rip + L14]
push r10
lea r10, [rbp - 8]
push r10
lea r10, [rbp - 16]
push r10
pop rdx
pop rsi
pop rdi
pop rax
call rax
push rax
pop r10
mov r10, 0
push r10
lea r10, [rbp - 4800024]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
L15:
lea r10, [rbp - 4800024]
push r10
lea r10, [rbp - 8]
push r10
pop r11
pop r10
mov r10, [r10]
mov r11, [r11]
cmp r10, r11
setl r10b
movzx r10, r10b
push r10
pop r10
cmp r10, 0
jz L16
mov r10, [rip + init@GOTPCREL]
push r10
lea r10, [rbp - 2400016]
push r10
lea r10, [rbp - 4800024]
push r10
pop r11
mov r11, [r11]
imul r11, 24
pop r10
add r10, r11
push r10
pop rdi
pop rax
call rax
push rax
pop r10
mov r10, [rip + init@GOTPCREL]
push r10
lea r10, [rbp - 4800016]
push r10
lea r10, [rbp - 4800024]
push r10
pop r11
mov r11, [r11]
imul r11, 24
pop r10
add r10, r11
push r10
pop rdi
pop rax
call rax
push rax
pop r10
lea r10, [rbp - 4800024]
push r10
mov r10, 1
push r10
pop r11
pop r10
mov r10, [r10]
add r10, r11
push r10
lea r10, [rbp - 4800024]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea rsp, [rbp - 8000032]
jmp L15
L16:
mov r10, 0
push r10
lea r10, [rbp - 4800024]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
L17:
lea r10, [rbp - 4800024]
push r10
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
mov r10, 1
push r10
pop r11
pop r10
sub r10, r11
push r10
pop r11
pop r10
mov r10, [r10]
cmp r10, r11
setl r10b
movzx r10, r10b
push r10
pop r10
cmp r10, 0
jz L18
sub rsp, 8
sub rsp, 8
mov r10, [rip + scanf@GOTPCREL]
push r10
lea r10, [rip + L19]
push r10
lea r10, [rbp - 8000040]
push r10
lea r10, [rbp - 8000048]
push r10
pop rdx
pop rsi
pop rdi
pop rax
call rax
push rax
pop r10
mov r10, [rip + push@GOTPCREL]
push r10
lea r10, [rbp - 2400016]
push r10
lea r10, [rbp - 8000040]
push r10
mov r10, 1
push r10
pop r11
pop r10
mov r10, [r10]
sub r10, r11
push r10
pop r11
imul r11, 24
pop r10
add r10, r11
push r10
lea r10, [rbp - 8000048]
push r10
mov r10, 1
push r10
pop r11
pop r10
mov r10, [r10]
sub r10, r11
push r10
pop rsi
pop rdi
pop rax
call rax
push rax
pop r10
mov r10, [rip + push@GOTPCREL]
push r10
lea r10, [rbp - 4800016]
push r10
lea r10, [rbp - 8000048]
push r10
mov r10, 1
push r10
pop r11
pop r10
mov r10, [r10]
sub r10, r11
push r10
pop r11
imul r11, 24
pop r10
add r10, r11
push r10
lea r10, [rbp - 8000040]
push r10
mov r10, 1
push r10
pop r11
pop r10
mov r10, [r10]
sub r10, r11
push r10
pop rsi
pop rdi
pop rax
call rax
push rax
pop r10
lea r10, [rbp - 4800024]
push r10
mov r10, 1
push r10
pop r11
pop r10
mov r10, [r10]
add r10, r11
push r10
lea r10, [rbp - 4800024]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea rsp, [rbp - 8000032]
jmp L17
L18:
mov r10, 0
push r10
lea r10, [rbp - 5600032]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
mov r10, 0
push r10
lea r10, [rbp - 4800024]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
L20:
lea r10, [rbp - 4800024]
push r10
lea r10, [rbp - 8]
push r10
pop r11
pop r10
mov r10, [r10]
mov r11, [r11]
cmp r10, r11
setl r10b
movzx r10, r10b
push r10
pop r10
cmp r10, 0
jz L21
mov r10, 0
push r10
lea r10, [rbp - 6400032]
push r10
lea r10, [rbp - 4800024]
push r10
pop r11
mov r11, [r11]
imul r11, 8
pop r10
add r10, r11
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea r10, [rbp - 4800024]
push r10
mov r10, 1
push r10
pop r11
pop r10
mov r10, [r10]
add r10, r11
push r10
lea r10, [rbp - 4800024]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea rsp, [rbp - 8000032]
jmp L20
L21:
mov r10, 0
push r10
lea r10, [rbp - 4800024]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
L22:
lea r10, [rbp - 4800024]
push r10
lea r10, [rbp - 8]
push r10
pop r11
pop r10
mov r10, [r10]
mov r11, [r11]
cmp r10, r11
setl r10b
movzx r10, r10b
push r10
pop r10
cmp r10, 0
jz L23
lea r10, [rbp - 6400032]
push r10
lea r10, [rbp - 4800024]
push r10
pop r11
mov r11, [r11]
imul r11, 8
pop r10
add r10, r11
push r10
mov r10, 0
push r10
pop r11
pop r10
mov r10, [r10]
cmp r10, r11
sete r10b
movzx r10, r10b
push r10
pop r10
cmp r10, 0
jz L24
mov r10, [rip + dfs@GOTPCREL]
push r10
lea r10, [rbp - 4800024]
push r10
lea r10, [rbp - 2400016]
push r10
lea r10, [rbp - 6400032]
push r10
lea r10, [rbp - 5600032]
push r10
lea r10, [rbp - 5600024]
push r10
pop r8
pop rcx
pop rdx
pop rsi
pop rdi
mov rdi, [rdi]
pop rax
call rax
push rax
pop r10
lea rsp, [rbp - 8000032]
jmp L25
L24:
L25:
lea r10, [rbp - 4800024]
push r10
mov r10, 1
push r10
pop r11
pop r10
mov r10, [r10]
add r10, r11
push r10
lea r10, [rbp - 4800024]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea rsp, [rbp - 8000032]
jmp L22
L23:
mov r10, 0
push r10
lea r10, [rbp - 4800024]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
L26:
lea r10, [rbp - 4800024]
push r10
lea r10, [rbp - 8]
push r10
pop r11
pop r10
mov r10, [r10]
mov r11, [r11]
cmp r10, r11
setl r10b
movzx r10, r10b
push r10
pop r10
cmp r10, 0
jz L27
lea r10, [rbp - 4800024]
push r10
lea r10, [rbp - 8000032]
push r10
lea r10, [rbp - 5600024]
push r10
lea r10, [rbp - 4800024]
push r10
pop r11
mov r11, [r11]
imul r11, 8
pop r10
add r10, r11
push r10
pop r11
mov r11, [r11]
imul r11, 8
pop r10
add r10, r11
push r10
pop r10
pop r11
mov r11, [r11]
mov [r10], r11
push r11
pop r10
lea r10, [rbp - 4800024]
push r10
mov r10, 1
push r10
pop r11
pop r10
mov r10, [r10]
add r10, r11
push r10
lea r10, [rbp - 4800024]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea rsp, [rbp - 8000032]
jmp L26
L27:
mov r10, 0
push r10
lea r10, [rbp - 4800024]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
L28:
lea r10, [rbp - 4800024]
push r10
lea r10, [rbp - 8]
push r10
pop r11
pop r10
mov r10, [r10]
mov r11, [r11]
cmp r10, r11
setl r10b
movzx r10, r10b
push r10
pop r10
cmp r10, 0
jz L29
sub rsp, 8
sub rsp, 8
lea r10, [rbp - 4800016]
push r10
lea r10, [rbp - 4800024]
push r10
pop r11
mov r11, [r11]
imul r11, 24
pop r10
add r10, r11
push r10
pop r10
add r10, 0
push r10
mov r10, 0
push r10
pop r11
pop r10
mov r10, [r10]
cmp r10, r11
sete r10b
movzx r10, r10b
push r10
pop r10
cmp r10, 0
jz L30
mov r10, 0
push r10
lea r10, [rbp - 7200032]
push r10
lea r10, [rbp - 4800024]
push r10
pop r11
mov r11, [r11]
imul r11, 8
pop r10
add r10, r11
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea r10, [rbp - 4800024]
push r10
mov r10, 1
push r10
pop r11
pop r10
mov r10, [r10]
add r10, r11
push r10
lea r10, [rbp - 4800024]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
jmp L28
lea rsp, [rbp - 8000048]
jmp L31
L30:
L31:
mov r10, 1000000
push r10
lea r10, [rbp - 8000040]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
mov r10, 0
push r10
lea r10, [rbp - 8000048]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
L32:
lea r10, [rbp - 8000048]
push r10
lea r10, [rbp - 4800016]
push r10
lea r10, [rbp - 4800024]
push r10
pop r11
mov r11, [r11]
imul r11, 24
pop r10
add r10, r11
push r10
pop r10
add r10, 0
push r10
pop r11
pop r10
mov r10, [r10]
mov r11, [r11]
cmp r10, r11
setl r10b
movzx r10, r10b
push r10
pop r10
cmp r10, 0
jz L33
lea r10, [rbp - 8000040]
push r10
lea r10, [rbp - 8000032]
push r10
lea r10, [rbp - 4800016]
push r10
lea r10, [rbp - 4800024]
push r10
pop r11
mov r11, [r11]
imul r11, 24
pop r10
add r10, r11
push r10
pop r10
add r10, 8
push r10
pop r10
mov r10, [r10]
push r10
lea r10, [rbp - 8000048]
push r10
pop r11
mov r11, [r11]
imul r11, 8
pop r10
add r10, r11
push r10
pop r11
mov r11, [r11]
imul r11, 8
pop r10
add r10, r11
push r10
pop r11
pop r10
mov r10, [r10]
mov r11, [r11]
cmp r10, r11
setg r10b
movzx r10, r10b
push r10
pop r10
cmp r10, 0
jz L34
lea r10, [rbp - 8000032]
push r10
lea r10, [rbp - 4800016]
push r10
lea r10, [rbp - 4800024]
push r10
pop r11
mov r11, [r11]
imul r11, 24
pop r10
add r10, r11
push r10
pop r10
add r10, 8
push r10
pop r10
mov r10, [r10]
push r10
lea r10, [rbp - 8000048]
push r10
pop r11
mov r11, [r11]
imul r11, 8
pop r10
add r10, r11
push r10
pop r11
mov r11, [r11]
imul r11, 8
pop r10
add r10, r11
push r10
lea r10, [rbp - 8000040]
push r10
pop r10
pop r11
mov r11, [r11]
mov [r10], r11
push r11
pop r10
lea r10, [rbp - 5600024]
push r10
lea r10, [rbp - 8000040]
push r10
pop r11
mov r11, [r11]
imul r11, 8
pop r10
add r10, r11
push r10
mov r10, 1
push r10
pop r11
pop r10
mov r10, [r10]
add r10, r11
push r10
lea r10, [rbp - 7200032]
push r10
lea r10, [rbp - 4800024]
push r10
pop r11
mov r11, [r11]
imul r11, 8
pop r10
add r10, r11
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea rsp, [rbp - 8000048]
jmp L35
L34:
L35:
lea r10, [rbp - 8000048]
push r10
mov r10, 1
push r10
pop r11
pop r10
mov r10, [r10]
add r10, r11
push r10
lea r10, [rbp - 8000048]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea rsp, [rbp - 8000048]
jmp L32
L33:
lea r10, [rbp - 4800024]
push r10
mov r10, 1
push r10
pop r11
pop r10
mov r10, [r10]
add r10, r11
push r10
lea r10, [rbp - 4800024]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea rsp, [rbp - 8000032]
jmp L28
L29:
mov r10, 0
push r10
lea r10, [rbp - 4800024]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
L36:
lea r10, [rbp - 4800024]
push r10
lea r10, [rbp - 8]
push r10
pop r11
pop r10
mov r10, [r10]
mov r11, [r11]
cmp r10, r11
setl r10b
movzx r10, r10b
push r10
pop r10
cmp r10, 0
jz L37
mov r10, [rip + free@GOTPCREL]
push r10
lea r10, [rbp - 2400016]
push r10
lea r10, [rbp - 4800024]
push r10
pop r11
mov r11, [r11]
imul r11, 24
pop r10
add r10, r11
push r10
pop r10
add r10, 8
push r10
pop rdi
mov rdi, [rdi]
pop rax
call rax
push rax
pop r10
mov r10, [rip + free@GOTPCREL]
push r10
lea r10, [rbp - 4800016]
push r10
lea r10, [rbp - 4800024]
push r10
pop r11
mov r11, [r11]
imul r11, 24
pop r10
add r10, r11
push r10
pop r10
add r10, 8
push r10
pop rdi
mov rdi, [rdi]
pop rax
call rax
push rax
pop r10
mov r10, [rip + printf@GOTPCREL]
push r10
lea r10, [rip + L38]
push r10
lea r10, [rbp - 7200032]
push r10
lea r10, [rbp - 4800024]
push r10
pop r11
mov r11, [r11]
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
lea r10, [rbp - 4800024]
push r10
mov r10, 1
push r10
pop r11
pop r10
mov r10, [r10]
add r10, r11
push r10
lea r10, [rbp - 4800024]
push r10
pop r10
pop r11
mov [r10], r11
push r11
pop r10
lea rsp, [rbp - 8000032]
jmp L36
L37:
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
L14: .asciz "%lld %lld%*c"
.align 8
L19: .asciz "%lld %lld%*c"
.align 8
L38: .asciz "%lld\n"
