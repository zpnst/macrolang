
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
