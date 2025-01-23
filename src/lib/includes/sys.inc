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