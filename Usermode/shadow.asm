.code

EXTERNDEF NtShutdownSystem:PROC

public __shadow_call

__shadow_call proc
    
    ; allocate shadow space
    sub rsp, 32

    ; place signature
    mov WORD PTR [rsp + 30], 0DEADh

    ; store arg in shadow space
    mov QWORD PTR [rsp + 22], rcx

    ; zero out registers
    xor rax, rax
    xor rbx, rbx
    xor rcx, rcx
    xor rdx, rdx
    xor rsi, rsi
    xor rdi, rdi
    xor rbp, rbp
    xor r8, r8
    xor r9, r9
    xor r10, r10
    xor r11, r11
    xor r12, r12
    xor r13, r13
    xor r14, r14
    xor r15, r15

    ; moving first arg
    mov rcx, 1

    ; call
    call QWORD PTR NtShutdownSystem

    ; Restore stack
    add rsp, 32

    ret

__shadow_call endp

END