include 'includes/bufs.inc'
include 'includes/syscalls.inc'
include 'includes/funcs.inc'
include 'includes/io.inc'
include 'includes/files.inc'

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
