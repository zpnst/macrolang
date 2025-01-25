

include 'bufs.inc'
include 'sys.inc'
include 'funcs.inc'
include 'fcntl.inc'

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


; strings 

macro cmpStrings string1, string2 {
    save2 r13, r14

    mov r13, __#string1#Len
    mov r14, __#string2#Len
     
    extract2 r13, r14
}

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

