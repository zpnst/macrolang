format ELF64 executable

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;[ BUFFERS ];;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; system buffers
__sumcNumberBuffer rb 128
__sumcNumberBufferLen = $-__sumcNumberBuffer

__smucFileBuffer: rb 10*1024
__smucFileBufferCap = $-__smucFileBuffer
__smucFileBufferLen: rq 1

__fileLinesCounter: dq 0
__fileLinesIterator: dq 0

__smucStrCnt1: db "There are ", 0
____smucStrCnt1Len = $-__smucStrCnt1

__smucStrCnt2: db " lines of code in '", 0
____smucStrCnt2Len = $-__smucStrCnt2

__smucStrCnt3: db "'", 0
____smucStrCnt3Len = $-__smucStrCnt3

NL db 10
NLLen = $-NL

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;[ SYSVALLS ];;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; fds
STDIN_FILENO equ 0
STDOUT_FILENO equ 1
STDERR_FILENO equ 2

; syscalls
SYS_READ equ 0
SYS_WRITE equ 1
SYS_OPEN equ 2
SYS_CLOSE equ 3
SYS_EXIT equ 60

; flags
O_RDONLY equ 0

; syscalls

macro syscall1 num, arg1 {
    mov rax, num
    mov rdi, arg1
    syscall
}

macro syscall3 num, arg1, arg2, arg3 {
    mov rax, num
    mov rdi, arg1
    mov rsi, arg2
    mov rdx, arg3
    syscall
}

;; syscalls wrappers tier 1

macro write fd, buf, bufSize {
    syscall3 SYS_WRITE, fd, buf, bufSize
}

macro read fd, buf, bufSize {
    syscall3 SYS_READ, fd, buf, bufSize
}

macro exit code {
    syscall1 SYS_EXIT, code
}

;; syscalls wrappers tier 2

macro printStr string {
    write STDOUT_FILENO, string, __#string#Len
}

macro printNL {
    write STDOUT_FILENO, NL, NLLen
}

macro exitOk {
    exit nil
}

macro printStrNL string {
    printStr string
    printNL
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;[ I/O ];;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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


; numbers printing

macro printNum number {
    save2 rax, r14
    mov rax, number
    call _print_number
    extract2 rax, r14
}

macro printVarNum number {
    save2 rax, r14
    mov rax, [number]
    call _print_number
    extract2 rax, r14
}

macro printNumNL number {
    save2 rax, r14
    mov rax, number
    call _print_number_nl
    extract2 rax, r14
}

macro printVarNumNL number {
    save2 rax, r14
    mov rax, [number]
    call _print_number_nl
    extract2 rax, r14
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;[ FUNCTIONS ];;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; stack
macro prolog {
    push rbp
    mov rbp, rsp
}

macro funcVarCnt bytesCnt {
    sub rsp, (bytesCnt*8)
}
 
macro freeFuncVars bytesCnt {
    add rsp, (bytesCnt*8)
}

macro epilog  {
    pop rbp
    ret
}

; function definitions

varscount fix ,

macro func funcName, varC {
    if funcName in <main>
        entry _start
        _start:
            prolog
            funcVarCnt varC
    else
        funcName:
            prolog
            funcVarCnt varC
    end if
}

macro funcEnd varC {
    freeFuncVars varC
    epilog
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;[ FILES ];;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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
    jl _fail_read_file#fileName
    write STDOUT_FILENO, __smucFileBuffer, __smucFileBufferLen
    jmp _nextHop#fileName
_fail_read_file#fileName:
    exit rax
_nextHop#fileName:
}

macro dumpFileLinesNumber fileName {
    for __fileLinesIterator, 0, [__smucFileBufferLen]
        mov rax, __smucFileBuffer
        add rax, [__fileLinesIterator]
        mov bl, byte [rax]
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
    printStr fileName
    printStrNL __smucStrCnt3

    mov qword [__fileLinesCounter], 0
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;[ SOME STUFF ];;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; for loop

macro for i, from, to {
    mov QWORD [i], from
.#i#_again: 
    save1 r14
    mov r14, QWORD [i]
    cmp r14, to
    extract1 r14
    jge .#i#_over
}

macro endfor i {
    inc QWORD [i]
    jmp .#i#_again
.#i#_over:
}

; variables

is fix ,

macro let varName, varType, varVal {
    if varType in <int>
        varName: dq varVal
    else
        if varType in <str>
            varName: db varVal, 0
            __##varName#Len = $-varName
        end if
    end if
}


; functions stack variables

macro flet varSeqNo, type, varVal {
    if type in <int>
        mov [rbp-(varSeqNo*8)], dword varVal
    else
        if type in <str>
          
        else

        end if
    end if
}

macro fprintNum varSeqNo {
    printNum [rbp-(varSeqNo*8)]
}

macro fprintNumNL varSeqNo {
    printNumNL [rbp-(varSeqNo*8)]
}


; some stuff
nil equ 0

macro save1 reg1 {
    push reg1
}

macro extract1 reg1 {
    pop reg1
}

macro save2 reg1, reg2 {
    save1 reg1
    save1 reg2
}

macro extract2 reg1, reg2 {
    extract1 reg1
    extract1 reg2
}