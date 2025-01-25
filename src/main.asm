format ELF64 executable
include 'lib/smuclib.inc'


let iter, int is 0
let jter, int is 0

let a, int is 42

let s0, str is "Hello # "
let s1, str is ") - "
let s2, str is "Hello World!"

let foo1str, str is "Hello from foo1!"
let foo2str, str is "Hello from foo2!"

let fileName, str is "src/main.asm"

;; function foo 2

func foo2 varscount 0
    printStrNL foo2str
funcEnd 0

;; function foo1

func foo1 varscount 2
    
    flet 1, int is 54
    flet 2, int is 96

    printStrNL foo1str
    call foo2 

    printNL

    fprintNumNL 2
    fprintNumNL 1
        
funcEnd 2

;; function main

func main varscount 0
    
    printVarNumNL a

    printNL

    for iter, 1, 3
        printStr s0
        printVarNumNL iter

        for jter, 1, 3
            printVarNum jter
            printStr s1
            printStrNL s2
        endfor jter

    endfor iter

    printNL

    call foo1
    

    dumpFileContent fileName
    dumpFileLinesNumber fileName


    exitOk



