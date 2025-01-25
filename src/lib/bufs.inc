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

__smucStrCnt2: db " lines of code in ", 10
____smucStrCnt2Len = $-__smucStrCnt2

NL db 10
NLLen = $-NL