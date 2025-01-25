format ELF64 executable
include 'lib/smuclib.asm'

let a, int is 0
let b, int is 5
let s, str is "Hello World!"


func foo
    funcVarCnt 1

        flet 1, int is 12
        ; fprint 1
        
        

    freeFuncVars 1
funcEnd

func main
    call foo

    printNum 42
    printVarNum b
    
    printStr s

    exitOk

