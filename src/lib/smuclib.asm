

include 'sys.inc'
include 'proc.inc'

; system buffers

__sumcNumberBuffer rb 128
__sumcNumberBufferLen = $-__sumcNumberBuffer

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

; variables

is fix ,

macro let varName, varType, varVal {
    if varType in <int>
        varName: dq varVal
    else
        if varType in <str>
            varName: db varVal, 0, 10
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

; macro fprint {

; }


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

macro func funcName {
    if funcName in <main>
        entry _start
        _start:
        prolog
    else
        funcName:
        prolog
    end if
}

macro funcEnd {
    epilog
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


