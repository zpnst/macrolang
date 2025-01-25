_print_number_buffer:
    syscall3 1, 1, r14, 1
    dec r14

    cmp r14, __sumcNumberBuffer ;; same way to comapre byte [r14], 10
    jge _print_number_buffer

    ret

_fill_in_the_buffer:
    mov rdx, 0    ;; to store a remainder
    mov rbx, 10   ;; divider
    div rbx       ;; rax = rax / rbx and remainder in rdx if rdx is zero

    mov rcx, 48
    add rcx, rdx
    mov byte [r14], cl
    inc r14

    cmp rax, 0
    jne _fill_in_the_buffer

    dec r14
    call _print_number_buffer

    ret

; number in register rax
_print_number:
    mov r14, __sumcNumberBuffer ;; __sumcNumberBuffer iterator
    ; mov qword [r14], 10         ;; new line
    ; inc r14   
    jmp _fill_in_the_buffer


; number in register rax
_print_number_nl:
    mov r14, __sumcNumberBuffer ;; __sumcNumberBuffer iterator
    mov qword [r14], 10         ;; new line
    inc r14
    jmp _fill_in_the_buffer
