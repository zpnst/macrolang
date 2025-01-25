; rdi - file path
read_file:
    push rbp
    mov rbp, rsp
    sub rsp, 8

    ; mov rdi, file_path rdi already contains file_path
    syscall3 SYS_OPEN, rdi, O_RDONLY, 0
    cmp rax, 0
    jl .failed

    mov [rbp-8], rax ; file descriptor

    syscall3 SYS_READ, [rbp-8], __smucFileBuffer, __smucFileBufferCap
    cmp rax, 0
    jl .failed

    mov [__smucFileBufferLen], rax

    syscall1 SYS_CLOSE, [rbp-8]

.failed: 
    add rsp, 8
    pop rbp
    ret

macro dumpFileContent fileName {
    mov rdi, fileName
    call read_file 
    cmp rax, 0
    jl _fail_read_file
    write STDOUT_FILENO, __smucFileBuffer, __smucFileBufferLen
    jmp _nextHop#fileName
_fail_read_file:
    exit rax
_nextHop#fileName:
}

macro dumpFileLinesNumber fileName {
    for __fileLinesIterator, 0, [__smucFileBufferLen]

        mov rax, __smucFileBuffer
        add rax, [__fileLinesIterator]
        mov bl, BYTE [rax]
        mov rcx, [__fileLinesCounter]
        mov rdx, rcx
        inc rcx
        cmp bl, 10
        cmove rdx, rcx ; restore previous count val from rax if current byte not '\n'.
        mov [__fileLinesCounter], rdx
    endfor __fileLinesIterator

    printNL

    printStr __smucStrCnt1
    printVarNum __fileLinesCounter
    printStr __smucStrCnt2
    printStrNL fileName

    mov qword [__fileLinesCounter], 0
}