

;-----------------------------------------------------------
;  squareSolver.txt:7
;-----------------------------------------------------------
;         wyaaaaaa getLinearRoot (b, c)!
;         
;     ==> atti() chal
;             wyaaaaaa a wuahh  aohwawh!
;             wyaaaaaa b wuahh  aohwawh!
;-----------------------------------------------------------


:main


;-----------------------------------------------------------
;  squareSolver.txt:8
;-----------------------------------------------------------
;         
;         atti() chal
;     ==>     wyaaaaaa a wuahh  aohwawh!
;             wyaaaaaa b wuahh  aohwawh!
;             wyaaaaaa c wuahh  aohwawh!
;-----------------------------------------------------------


PUSH 0
PUSH 0
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:9
;-----------------------------------------------------------
;         atti() chal
;             wyaaaaaa a wuahh  aohwawh!
;     ==>     wyaaaaaa b wuahh  aohwawh!
;             wyaaaaaa c wuahh  aohwawh!
;         
;-----------------------------------------------------------


PUSH 0
PUSH 1
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:10
;-----------------------------------------------------------
;             wyaaaaaa a wuahh  aohwawh!
;             wyaaaaaa b wuahh  aohwawh!
;     ==>     wyaaaaaa c wuahh  aohwawh!
;         
;             wyaaaaaa d wuahh  aohwawh!
;-----------------------------------------------------------


PUSH 0
PUSH 2
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:12
;-----------------------------------------------------------
;             wyaaaaaa c wuahh  aohwawh!
;         
;     ==>     wyaaaaaa d wuahh  aohwawh!
;             wyaaaaaa numOfRoots wuahh  aohwawh!
;             wyaaaaaa x1 wuahh  aohwawh!
;-----------------------------------------------------------


PUSH 0
PUSH 3
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:13
;-----------------------------------------------------------
;         
;             wyaaaaaa d wuahh  aohwawh!
;     ==>     wyaaaaaa numOfRoots wuahh  aohwawh!
;             wyaaaaaa x1 wuahh  aohwawh!
;             wyaaaaaa x2 wuahh  aohwawh!
;-----------------------------------------------------------


PUSH 0
PUSH 4
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:14
;-----------------------------------------------------------
;             wyaaaaaa d wuahh  aohwawh!
;             wyaaaaaa numOfRoots wuahh  aohwawh!
;     ==>     wyaaaaaa x1 wuahh  aohwawh!
;             wyaaaaaa x2 wuahh  aohwawh!
;         
;-----------------------------------------------------------


PUSH 0
PUSH 5
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:15
;-----------------------------------------------------------
;             wyaaaaaa numOfRoots wuahh  aohwawh!
;             wyaaaaaa x1 wuahh  aohwawh!
;     ==>     wyaaaaaa x2 wuahh  aohwawh!
;         
;             haaag "a"!
;-----------------------------------------------------------


PUSH 0
PUSH 6
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:17
;-----------------------------------------------------------
;             wyaaaaaa x2 wuahh  aohwawh!
;         
;     ==>     haaag "a"!
;             haaag "b"!
;             haaag "c"!
;-----------------------------------------------------------


IN
PUSH 0
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:18
;-----------------------------------------------------------
;         
;             haaag "a"!
;     ==>     haaag "b"!
;             haaag "c"!
;         
;-----------------------------------------------------------


IN
PUSH 1
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:19
;-----------------------------------------------------------
;             haaag "a"!
;             haaag "b"!
;     ==>     haaag "c"!
;         
;             yaag (a uma aohwawh) chal
;-----------------------------------------------------------


IN
PUSH 2
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:21
;-----------------------------------------------------------
;             haaag "c"!
;         
;     ==>     yaag (a uma aohwawh) chal
;                 numOfRoots wuahh getNumOfLinearRoots(b, c)!
;         
;-----------------------------------------------------------


PUSH 0
PUSH 0
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
;  squareSolver.txt:22
;-----------------------------------------------------------
;         
;             yaag (a uma aohwawh) chal
;     ==>         numOfRoots wuahh getNumOfLinearRoots(b, c)!
;         
;                 yaag (numOfRoots uma ah)
;-----------------------------------------------------------


PUSH 1
POPREG Ax
PUSHM [Ax]
PUSH 2
POPREG Ax
PUSHM [Ax]
CALL :getNumOfLinearRoots
PUSH 4
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:24
;-----------------------------------------------------------
;                 numOfRoots wuahh getNumOfLinearRoots(b, c)!
;         
;     ==>         yaag (numOfRoots uma ah)
;                     x1 wuahh getLinearRoot(b, c)!
;             ur-oh!
;-----------------------------------------------------------


PUSH 1
PUSH 4
POPREG Ax
PUSHM [Ax]
PUSH 0
POPREG Bx
JNE :endCompare2
PUSH 1
POPREG Bx
:endCompare2
PUSHREG Bx
PUSH 0
JE :endif2


;-----------------------------------------------------------
;  squareSolver.txt:25
;-----------------------------------------------------------
;         
;                 yaag (numOfRoots uma ah)
;     ==>             x1 wuahh getLinearRoot(b, c)!
;             ur-oh!
;         
;-----------------------------------------------------------


PUSH 1
POPREG Ax
PUSHM [Ax]
PUSH 2
POPREG Ax
PUSHM [Ax]
CALL :getLinearRoot
PUSH 5
POPREG Ax
POPM [Ax]
:endif2
:endif1


;-----------------------------------------------------------
;  squareSolver.txt:28
;-----------------------------------------------------------
;             ur-oh!
;         
;     ==>     yaag (a ~uma aohwawh) chal
;                 yaag (c uma aohwawh) chal
;                     x1 wuahh  aohwawh!
;-----------------------------------------------------------


PUSH 0
PUSH 0
POPREG Ax
PUSHM [Ax]
PUSH 0
POPREG Bx
JE :endCompare3
PUSH 1
POPREG Bx
:endCompare3
PUSHREG Bx
PUSH 0
JE :endif3


;-----------------------------------------------------------
;  squareSolver.txt:29
;-----------------------------------------------------------
;         
;             yaag (a ~uma aohwawh) chal
;     ==>         yaag (c uma aohwawh) chal
;                     x1 wuahh  aohwawh!
;                     numOfRoots wuahh getNumOfLinearRoots(a, b)!
;-----------------------------------------------------------


PUSH 0
PUSH 2
POPREG Ax
PUSHM [Ax]
PUSH 0
POPREG Bx
JNE :endCompare4
PUSH 1
POPREG Bx
:endCompare4
PUSHREG Bx
PUSH 0
JE :endif4


;-----------------------------------------------------------
;  squareSolver.txt:30
;-----------------------------------------------------------
;             yaag (a ~uma aohwawh) chal
;                 yaag (c uma aohwawh) chal
;     ==>             x1 wuahh  aohwawh!
;                     numOfRoots wuahh getNumOfLinearRoots(a, b)!
;         
;-----------------------------------------------------------


PUSH 0
PUSH 5
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:31
;-----------------------------------------------------------
;                 yaag (c uma aohwawh) chal
;                     x1 wuahh  aohwawh!
;     ==>             numOfRoots wuahh getNumOfLinearRoots(a, b)!
;         
;                     yaag (numOfRoots uma ah) chal
;-----------------------------------------------------------


PUSH 0
POPREG Ax
PUSHM [Ax]
PUSH 1
POPREG Ax
PUSHM [Ax]
CALL :getNumOfLinearRoots
PUSH 4
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:33
;-----------------------------------------------------------
;                     numOfRoots wuahh getNumOfLinearRoots(a, b)!
;         
;     ==>             yaag (numOfRoots uma ah) chal
;                         x2 wuahh getLinearRoot(a, b)!
;                     ur-oh!
;-----------------------------------------------------------


PUSH 1
PUSH 4
POPREG Ax
PUSHM [Ax]
PUSH 0
POPREG Bx
JNE :endCompare5
PUSH 1
POPREG Bx
:endCompare5
PUSHREG Bx
PUSH 0
JE :endif5


;-----------------------------------------------------------
;  squareSolver.txt:34
;-----------------------------------------------------------
;         
;                     yaag (numOfRoots uma ah) chal
;     ==>                 x2 wuahh getLinearRoot(a, b)!
;                     ur-oh!
;                     yaag(x1 ~uma x2)
;-----------------------------------------------------------


PUSH 0
POPREG Ax
PUSHM [Ax]
PUSH 1
POPREG Ax
PUSHM [Ax]
CALL :getLinearRoot
PUSH 6
POPREG Ax
POPM [Ax]
:endif5


;-----------------------------------------------------------
;  squareSolver.txt:36
;-----------------------------------------------------------
;                         x2 wuahh getLinearRoot(a, b)!
;                     ur-oh!
;     ==>             yaag(x1 ~uma x2)
;                         numOfRoots wuahh ah-ah!
;                 ur-oh!
;-----------------------------------------------------------


PUSH 6
POPREG Ax
PUSHM [Ax]
PUSH 5
POPREG Ax
PUSHM [Ax]
PUSH 0
POPREG Bx
JE :endCompare6
PUSH 1
POPREG Bx
:endCompare6
PUSHREG Bx
PUSH 0
JE :endif6


;-----------------------------------------------------------
;  squareSolver.txt:37
;-----------------------------------------------------------
;                     ur-oh!
;                     yaag(x1 ~uma x2)
;     ==>                 numOfRoots wuahh ah-ah!
;                 ur-oh!
;                 yaag (c ~uma aohwawh) chal
;-----------------------------------------------------------


PUSH 2
PUSH 4
POPREG Ax
POPM [Ax]
:endif6
:endif4


;-----------------------------------------------------------
;  squareSolver.txt:39
;-----------------------------------------------------------
;                         numOfRoots wuahh ah-ah!
;                 ur-oh!
;     ==>         yaag (c ~uma aohwawh) chal
;         
;                     d wuahh getDiscriminant(a, b, c)!
;-----------------------------------------------------------


PUSH 0
PUSH 2
POPREG Ax
PUSHM [Ax]
PUSH 0
POPREG Bx
JE :endCompare7
PUSH 1
POPREG Bx
:endCompare7
PUSHREG Bx
PUSH 0
JE :endif7


;-----------------------------------------------------------
;  squareSolver.txt:41
;-----------------------------------------------------------
;                 yaag (c ~uma aohwawh) chal
;         
;     ==>             d wuahh getDiscriminant(a, b, c)!
;         
;                     yaag (d issh aohwawh) chal
;-----------------------------------------------------------


PUSH 0
POPREG Ax
PUSHM [Ax]
PUSH 1
POPREG Ax
PUSHM [Ax]
PUSH 2
POPREG Ax
PUSHM [Ax]
CALL :getDiscriminant
PUSH 3
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:43
;-----------------------------------------------------------
;                     d wuahh getDiscriminant(a, b, c)!
;         
;     ==>             yaag (d issh aohwawh) chal
;                         numOfRoots wuahh  aohwawh!
;                     ur-oh!
;-----------------------------------------------------------


PUSH 0
PUSH 3
POPREG Ax
PUSHM [Ax]
PUSH 0
POPREG Bx
JAE :endCompare8
PUSH 1
POPREG Bx
:endCompare8
PUSHREG Bx
PUSH 0
JE :endif8


;-----------------------------------------------------------
;  squareSolver.txt:44
;-----------------------------------------------------------
;         
;                     yaag (d issh aohwawh) chal
;     ==>                 numOfRoots wuahh  aohwawh!
;                     ur-oh!
;         
;-----------------------------------------------------------


PUSH 0
PUSH 4
POPREG Ax
POPM [Ax]
:endif8


;-----------------------------------------------------------
;  squareSolver.txt:47
;-----------------------------------------------------------
;                     ur-oh!
;         
;     ==>             yaag (d uma aohwawh) chal
;                         numOfRoots wuahh ah!
;                         x1 wuahh ( aohwawh muawa b) yukshin (ah-ah muaarga a)!
;-----------------------------------------------------------


PUSH 0
PUSH 3
POPREG Ax
PUSHM [Ax]
PUSH 0
POPREG Bx
JNE :endCompare9
PUSH 1
POPREG Bx
:endCompare9
PUSHREG Bx
PUSH 0
JE :endif9


;-----------------------------------------------------------
;  squareSolver.txt:48
;-----------------------------------------------------------
;         
;                     yaag (d uma aohwawh) chal
;     ==>                 numOfRoots wuahh ah!
;                         x1 wuahh ( aohwawh muawa b) yukshin (ah-ah muaarga a)!
;                     ur-oh!
;-----------------------------------------------------------


PUSH 1
PUSH 4
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:49
;-----------------------------------------------------------
;                     yaag (d uma aohwawh) chal
;                         numOfRoots wuahh ah!
;     ==>                 x1 wuahh ( aohwawh muawa b) yukshin (ah-ah muaarga a)!
;                     ur-oh!
;         
;-----------------------------------------------------------


PUSH 0
PUSH 1
POPREG Ax
PUSHM [Ax]
SUB
PUSH 2
PUSH 0
POPREG Ax
PUSHM [Ax]
MUL
DIV
PUSH 5
POPREG Ax
POPM [Ax]
:endif9


;-----------------------------------------------------------
;  squareSolver.txt:52
;-----------------------------------------------------------
;                     ur-oh!
;         
;     ==>             yaag (d kerri aohwawh) chal
;                             numOfRoots wuahh ah-ah!
;                             x1 wuahh ( aohwawh muawa b muawa agaahaa (d)) yukshin (ah-ah muaarga a)!
;-----------------------------------------------------------


PUSH 0
PUSH 3
POPREG Ax
PUSHM [Ax]
PUSH 0
POPREG Bx
JBE :endCompare10
PUSH 1
POPREG Bx
:endCompare10
PUSHREG Bx
PUSH 0
JE :endif10


;-----------------------------------------------------------
;  squareSolver.txt:53
;-----------------------------------------------------------
;         
;                     yaag (d kerri aohwawh) chal
;     ==>                     numOfRoots wuahh ah-ah!
;                             x1 wuahh ( aohwawh muawa b muawa agaahaa (d)) yukshin (ah-ah muaarga a)!
;                             x2 wuahh ( aohwawh muawa b bacca agaahaa (d)) yukshin (ah-ah muaarga a)!
;-----------------------------------------------------------


PUSH 2
PUSH 4
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:54
;-----------------------------------------------------------
;                     yaag (d kerri aohwawh) chal
;                             numOfRoots wuahh ah-ah!
;     ==>                     x1 wuahh ( aohwawh muawa b muawa agaahaa (d)) yukshin (ah-ah muaarga a)!
;                             x2 wuahh ( aohwawh muawa b bacca agaahaa (d)) yukshin (ah-ah muaarga a)!
;                     ur-oh!
;-----------------------------------------------------------


PUSH 0
PUSH 1
POPREG Ax
PUSHM [Ax]
SUB
PUSH 3
POPREG Ax
PUSHM [Ax]
SQRT
SUB
PUSH 2
PUSH 0
POPREG Ax
PUSHM [Ax]
MUL
DIV
PUSH 5
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:55
;-----------------------------------------------------------
;                             numOfRoots wuahh ah-ah!
;                             x1 wuahh ( aohwawh muawa b muawa agaahaa (d)) yukshin (ah-ah muaarga a)!
;     ==>                     x2 wuahh ( aohwawh muawa b bacca agaahaa (d)) yukshin (ah-ah muaarga a)!
;                     ur-oh!
;                 ur-oh!
;-----------------------------------------------------------


PUSH 0
PUSH 1
POPREG Ax
PUSHM [Ax]
SUB
PUSH 3
POPREG Ax
PUSHM [Ax]
SQRT
ADD
PUSH 2
PUSH 0
POPREG Ax
PUSHM [Ax]
MUL
DIV
PUSH 6
POPREG Ax
POPM [Ax]
:endif10
:endif7
:endif3


;-----------------------------------------------------------
;  squareSolver.txt:60
;-----------------------------------------------------------
;             ur-oh!
;         
;     ==>     lanna "numOfRoots"!
;         
;             yaag (numOfRoots uma ah)
;-----------------------------------------------------------


PUSH 4
POPREG Ax
PUSHM [Ax]
OUT


;-----------------------------------------------------------
;  squareSolver.txt:62
;-----------------------------------------------------------
;             lanna "numOfRoots"!
;         
;     ==>     yaag (numOfRoots uma ah)
;                 lanna "x1"!
;         
;-----------------------------------------------------------


PUSH 1
PUSH 4
POPREG Ax
PUSHM [Ax]
PUSH 0
POPREG Bx
JNE :endCompare11
PUSH 1
POPREG Bx
:endCompare11
PUSHREG Bx
PUSH 0
JE :endif11


;-----------------------------------------------------------
;  squareSolver.txt:63
;-----------------------------------------------------------
;         
;             yaag (numOfRoots uma ah)
;     ==>         lanna "x1"!
;         
;             yaag (numOfRoots uma ah-ah) chal
;-----------------------------------------------------------


PUSH 5
POPREG Ax
PUSHM [Ax]
OUT
:endif11


;-----------------------------------------------------------
;  squareSolver.txt:65
;-----------------------------------------------------------
;                 lanna "x1"!
;         
;     ==>     yaag (numOfRoots uma ah-ah) chal
;                 lanna "x1"!
;                 lanna "x2"!
;-----------------------------------------------------------


PUSH 2
PUSH 4
POPREG Ax
PUSHM [Ax]
PUSH 0
POPREG Bx
JNE :endCompare12
PUSH 1
POPREG Bx
:endCompare12
PUSHREG Bx
PUSH 0
JE :endif12


;-----------------------------------------------------------
;  squareSolver.txt:66
;-----------------------------------------------------------
;         
;             yaag (numOfRoots uma ah-ah) chal
;     ==>         lanna "x1"!
;                 lanna "x2"!
;             ur-oh!
;-----------------------------------------------------------


PUSH 5
POPREG Ax
PUSHM [Ax]
OUT


;-----------------------------------------------------------
;  squareSolver.txt:67
;-----------------------------------------------------------
;             yaag (numOfRoots uma ah-ah) chal
;                 lanna "x1"!
;     ==>         lanna "x2"!
;             ur-oh!
;         
;-----------------------------------------------------------


PUSH 6
POPREG Ax
PUSHM [Ax]
OUT
:endif12


;-----------------------------------------------------------
;  squareSolver.txt:70
;-----------------------------------------------------------
;             ur-oh!
;         
;     ==>     mu!
;         ur-oh!
;         
;-----------------------------------------------------------


HLT


;-----------------------------------------------------------
;  squareSolver.txt:74
;-----------------------------------------------------------
;         
;         
;     ==> getDiscriminant(a, b, c) chal
;             wyaaaaaa d wuahh b  muaarga b muawa wyoorg muaarga a  muaarga c!
;             ruow d!
;-----------------------------------------------------------


:getDiscriminant
PUSH 7
POPREG Ax
POPM [Ax]
PUSH 8
POPREG Ax
POPM [Ax]
PUSH 9
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:75
;-----------------------------------------------------------
;         
;         getDiscriminant(a, b, c) chal
;     ==>     wyaaaaaa d wuahh b  muaarga b muawa wyoorg muaarga a  muaarga c!
;             ruow d!
;         ur-oh!
;-----------------------------------------------------------


PUSH 8
POPREG Ax
PUSHM [Ax]
PUSH 8
POPREG Ax
PUSHM [Ax]
MUL
PUSH 4
PUSH 9
POPREG Ax
PUSHM [Ax]
MUL
PUSH 7
POPREG Ax
PUSHM [Ax]
MUL
SUB
PUSH 10
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:76
;-----------------------------------------------------------
;         getDiscriminant(a, b, c) chal
;             wyaaaaaa d wuahh b  muaarga b muawa wyoorg muaarga a  muaarga c!
;     ==>     ruow d!
;         ur-oh!
;         
;-----------------------------------------------------------


PUSH 10
POPREG Ax
PUSHM [Ax]
RET


;-----------------------------------------------------------
;  squareSolver.txt:79
;-----------------------------------------------------------
;         ur-oh!
;         
;     ==> getNumOfLinearRoots(b, c) chal
;             yaag (b uma  aohwawh) chal
;                 yaag (c uma  aohwawh) chal
;-----------------------------------------------------------


:getNumOfLinearRoots
PUSH 11
POPREG Ax
POPM [Ax]
PUSH 12
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:80
;-----------------------------------------------------------
;         
;         getNumOfLinearRoots(b, c) chal
;     ==>     yaag (b uma  aohwawh) chal
;                 yaag (c uma  aohwawh) chal
;                     ruow ~ah! // Бесконечно много
;-----------------------------------------------------------


PUSH 0
PUSH 12
POPREG Ax
PUSHM [Ax]
PUSH 0
POPREG Bx
JNE :endCompare13
PUSH 1
POPREG Bx
:endCompare13
PUSHREG Bx
PUSH 0
JE :endif13


;-----------------------------------------------------------
;  squareSolver.txt:81
;-----------------------------------------------------------
;         getNumOfLinearRoots(b, c) chal
;             yaag (b uma  aohwawh) chal
;     ==>         yaag (c uma  aohwawh) chal
;                     ruow ~ah! // Бесконечно много
;                 ur-oh!
;-----------------------------------------------------------


PUSH 0
PUSH 11
POPREG Ax
PUSHM [Ax]
PUSH 0
POPREG Bx
JNE :endCompare14
PUSH 1
POPREG Bx
:endCompare14
PUSHREG Bx
PUSH 0
JE :endif14


;-----------------------------------------------------------
;  squareSolver.txt:82
;-----------------------------------------------------------
;             yaag (b uma  aohwawh) chal
;                 yaag (c uma  aohwawh) chal
;     ==>             ruow ~ah! // Бесконечно много
;                 ur-oh!
;                 yaag (c  ~uma  aohwawh) chal
;-----------------------------------------------------------


PUSH -1
RET
:endif14


;-----------------------------------------------------------
;  squareSolver.txt:84
;-----------------------------------------------------------
;                     ruow ~ah! // Бесконечно много
;                 ur-oh!
;     ==>         yaag (c  ~uma  aohwawh) chal
;                     ruow aohwawh! // Нет решений
;                 ur-oh!
;-----------------------------------------------------------


PUSH 0
PUSH 11
POPREG Ax
PUSHM [Ax]
PUSH 0
POPREG Bx
JE :endCompare15
PUSH 1
POPREG Bx
:endCompare15
PUSHREG Bx
PUSH 0
JE :endif15


;-----------------------------------------------------------
;  squareSolver.txt:85
;-----------------------------------------------------------
;                 ur-oh!
;                 yaag (c  ~uma  aohwawh) chal
;     ==>             ruow aohwawh! // Нет решений
;                 ur-oh!
;             ur-oh!
;-----------------------------------------------------------


PUSH 0
RET
:endif15
:endif13


;-----------------------------------------------------------
;  squareSolver.txt:89
;-----------------------------------------------------------
;             ur-oh!
;         
;     ==>     yaag (b ~uma  aohwawh) chal
;                 ruow ah! // Один корень
;             ur-oh!
;-----------------------------------------------------------


PUSH 0
PUSH 12
POPREG Ax
PUSHM [Ax]
PUSH 0
POPREG Bx
JE :endCompare16
PUSH 1
POPREG Bx
:endCompare16
PUSHREG Bx
PUSH 0
JE :endif16


;-----------------------------------------------------------
;  squareSolver.txt:90
;-----------------------------------------------------------
;         
;             yaag (b ~uma  aohwawh) chal
;     ==>         ruow ah! // Один корень
;             ur-oh!
;         
;-----------------------------------------------------------


PUSH 1
RET
:endif16


;-----------------------------------------------------------
;  squareSolver.txt:93
;-----------------------------------------------------------
;             ur-oh!
;         
;     ==>     ruow  aohwawh!
;         ur-oh!
;         
;-----------------------------------------------------------


PUSH 0
RET


;-----------------------------------------------------------
;  squareSolver.txt:96
;-----------------------------------------------------------
;         ur-oh!
;         
;     ==> getLinearRoot(b, c) chal
;             wyaaaaaa root wuahh (aohwawh muawa c) yukshin b!
;             ruow root!
;-----------------------------------------------------------


:getLinearRoot
PUSH 13
POPREG Ax
POPM [Ax]
PUSH 14
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:97
;-----------------------------------------------------------
;         
;         getLinearRoot(b, c) chal
;     ==>     wyaaaaaa root wuahh (aohwawh muawa c) yukshin b!
;             ruow root!
;         ur-oh!
;-----------------------------------------------------------


PUSH 0
PUSH 13
POPREG Ax
PUSHM [Ax]
SUB
PUSH 14
POPREG Ax
PUSHM [Ax]
DIV
PUSH 15
POPREG Ax
POPM [Ax]


;-----------------------------------------------------------
;  squareSolver.txt:98
;-----------------------------------------------------------
;         getLinearRoot(b, c) chal
;             wyaaaaaa root wuahh (aohwawh muawa c) yukshin b!
;     ==>     ruow root!
;         ur-oh!
;-----------------------------------------------------------


PUSH 15
POPREG Ax
PUSHM [Ax]
RET
