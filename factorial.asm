

;-----------------------------------------------------------
;  factorial.txt:3
;-----------------------------------------------------------
;         wyaaaaaa factorial(n)!
;         
;     ==> atti() chal
;             wyaaaaaa n wuahh aohwawh!
;             wyaaaaaa fact wuahh  aohwawh!
;-----------------------------------------------------------


:main


;-----------------------------------------------------------
;  factorial.txt:4
;-----------------------------------------------------------
;         
;         atti() chal
;     ==>     wyaaaaaa n wuahh aohwawh!
;             wyaaaaaa fact wuahh  aohwawh!
;         
;-----------------------------------------------------------


PUSH 0
PUSH 0
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  factorial.txt:5
;-----------------------------------------------------------
;         atti() chal
;             wyaaaaaa n wuahh aohwawh!
;     ==>     wyaaaaaa fact wuahh  aohwawh!
;         
;             haaag "n"!
;-----------------------------------------------------------


PUSH 0
PUSH 1
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  factorial.txt:7
;-----------------------------------------------------------
;             wyaaaaaa fact wuahh  aohwawh!
;         
;     ==>     haaag "n"!
;             fact wuahh factorial(n)!
;         
;-----------------------------------------------------------


IN
PUSH 0
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  factorial.txt:8
;-----------------------------------------------------------
;         
;             haaag "n"!
;     ==>     fact wuahh factorial(n)!
;         
;             lanna "fact"!
;-----------------------------------------------------------


PUSH 0
POPREG Ax
PUSHM [Ax]
CALL :factorial
PUSH 1
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  factorial.txt:10
;-----------------------------------------------------------
;             fact wuahh factorial(n)!
;         
;     ==>     lanna "fact"!
;             mu!
;         ur-oh!
;-----------------------------------------------------------


PUSH 1
POPREG Ax
PUSHM [Ax]
OUT


;-----------------------------------------------------------
;  factorial.txt:11
;-----------------------------------------------------------
;         
;             lanna "fact"!
;     ==>     mu!
;         ur-oh!
;         
;-----------------------------------------------------------


HLT


;-----------------------------------------------------------
;  factorial.txt:14
;-----------------------------------------------------------
;         ur-oh!
;         
;     ==> factorial(n) chal
;             yaag (n uma ah)
;                 ruow ah!
;-----------------------------------------------------------


:factorial
PUSH 2
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  factorial.txt:15
;-----------------------------------------------------------
;         
;         factorial(n) chal
;     ==>     yaag (n uma ah)
;                 ruow ah!
;             yaag (n ~uma ah)
;-----------------------------------------------------------


PUSH 1
PUSH 2
POPREG Ax
PUSHM [Ax]
PUSH 0
POPREG Bx
JNE :endCompare1
PUSH 1
POPREG Bx
:endCompare1
PUSHREG Bx
PUSH 0
JE :endif1


;-----------------------------------------------------------
;  factorial.txt:16
;-----------------------------------------------------------
;         factorial(n) chal
;             yaag (n uma ah)
;     ==>         ruow ah!
;             yaag (n ~uma ah)
;                 ruow n muaarga factorial(n muawa ah)!
;-----------------------------------------------------------


PUSH 1
RET
:endif1


;-----------------------------------------------------------
;  factorial.txt:17
;-----------------------------------------------------------
;             yaag (n uma ah)
;                 ruow ah!
;     ==>     yaag (n ~uma ah)
;                 ruow n muaarga factorial(n muawa ah)!
;         ur-oh!
;-----------------------------------------------------------


PUSH 1
PUSH 2
POPREG Ax
PUSHM [Ax]
PUSH 0
POPREG Bx
JE :endCompare2
PUSH 1
POPREG Bx
:endCompare2
PUSHREG Bx
PUSH 0
JE :endif2


;-----------------------------------------------------------
;  factorial.txt:18
;-----------------------------------------------------------
;                 ruow ah!
;             yaag (n ~uma ah)
;     ==>         ruow n muaarga factorial(n muawa ah)!
;         ur-oh!
;-----------------------------------------------------------


PUSH 2
POPREG Ax
PUSHM [Ax]
PUSH 2
POPREG Ax
PUSHM [Ax]
PUSH 1
SUB
CALL :factorial
MUL
RET
:endif2
