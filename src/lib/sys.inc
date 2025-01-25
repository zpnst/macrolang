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

macro exitOk {
    exit nil
}