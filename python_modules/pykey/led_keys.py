PGDN = 0
PGUP = 1
INSERT = 2
DEL = 3

F12 = 4
F11 = 5
F10 = 6
F9 = 7

F8 = 8
F7 = 9
F6 = 10
F5 = 11

F4 = 12
F3 = 13
F2 = 14
F1 = 15

ESC = 16

#FN = 17
NUMPAD_MUL = 17
NUMPAD_DIV = 18
NUMLOCK = 19

BACKSPACE = 20
EQUALS = 21
MINUS = 22
ZERO = 23
NINE = 24
EIGHT = 25
SEVEN = 26
SIX = 27
FIVE = 28
FOUR = 29
THREE = 30
TWO = 31
ONE = 32
GRAVE = 33

NUMPAD_MIN = 34
NUMPAD_9 = 35
NUMPAD_8 = 36
NUMPAD_7 = 37

BACKSLASH = 38
RIGHT_BRACKET = 39
LEFT_BRACKET = 40
P = 41
O = 42
I = 43
U = 44
Y = 45
T = 46
R = 47
E = 48
W = 49
Q = 50
TAB = 51

NUMPAD_ADD = 52
NUMPAD_6 = 53
NUMPAD_5 = 54
NUMPAD_4 = 55

ENTER = 56
APOSTHROPHE = 57
SEMICOLON = 58
L = 59
K = 60
J = 61
H = 62
G = 63
F = 64
D = 65
S = 66
A = 67
CAPSLOCK = 68

NUMPAD_ENTER = 69
NUMPAD_3 = 70
NUMPAD_2 = 71
NUMPAD_1 = 72

RSHIFT = 73
QUESTION = 74
DOT = 75
COMMA = 76
M = 77
N = 78
B = 79
V = 80
C = 81
X = 82
Z = 83
LSHIFT = 84

NUMPAD_DOT = 85
NUMPAD_0 = 86

ARROW_RIGHT = 87
ARROW_DOWN = 88
ARROW_UP = 89
ARROW_LEFT = 90

RCTRL = 91
RALT = 92
SPACEBAR_RIGHT = 93
SPACEBAR_CENTER = 94
SPACEBAR_LEFT = 95
LALT = 96
LWIN = 97
LCTRL = 98

keymap = {}
keymap[0x04]=A
keymap[0x05]=B
keymap[0x06]=C
keymap[0x07]=D
keymap[0x08]=E
keymap[0x09]=F
keymap[0x0A]=G
keymap[0x0B]=H
keymap[0x0C]=I
keymap[0x0D]=J
keymap[0x0E]=K
keymap[0x0F]=L
keymap[0x10]=M
keymap[0x11]=N
keymap[0x12]=O
keymap[0x13]=P
keymap[0x14]=Q
keymap[0x15]=R
keymap[0x16]=S
keymap[0x17]=T
keymap[0x18]=U
keymap[0x19]=V
keymap[0x1A]=W
keymap[0x1B]=X
keymap[0x1C]=Y
keymap[0x1D]=Z
keymap[0x1E]=ONE
keymap[0x1F]=TWO
keymap[0x20]=THREE
keymap[0x21]=FOUR
keymap[0x22]=FIVE
keymap[0x23]=SIX
keymap[0x24]=SEVEN
keymap[0x25]=EIGHT
keymap[0x26]=NINE
keymap[0x27]=ZERO
keymap[0x28]=ENTER
keymap[0x29]=ESC
keymap[0x2A]=BACKSPACE
keymap[0x2B]=TAB
keymap[0x2C]=SPACEBAR_CENTER
keymap[0x2D]=MINUS
keymap[0x2E]=EQUALS
keymap[0x2F]=LEFT_BRACKET
keymap[0x30]=RIGHT_BRACKET
keymap[0x31]=BACKSLASH
keymap[0x33]=SEMICOLON
keymap[0x34]=APOSTHROPHE
keymap[0x35]=GRAVE
keymap[0x36]=COMMA
keymap[0x37]=DOT
keymap[0x38]=QUESTION
keymap[0x39]=CAPSLOCK
keymap[0x3A]=F1
keymap[0x3B]=F2
keymap[0x3C]=F3
keymap[0x3D]=F4
keymap[0x3E]=F5
keymap[0x3F]=F6
keymap[0x40]=F7
keymap[0x41]=F8
keymap[0x42]=F9
keymap[0x43]=F10
keymap[0x44]=F11
keymap[0x45]=F12
keymap[0x49]=INSERT
keymap[0x4B]=PGUP
keymap[0x4C]=DEL
keymap[0x4E]=PGDN
keymap[0x4F]=ARROW_RIGHT
keymap[0x50]=ARROW_LEFT
keymap[0x51]=ARROW_DOWN
keymap[0x52]=ARROW_UP
keymap[0x53]=NUMLOCK
keymap[0x54]=NUMPAD_DIV
keymap[0x55]=NUMPAD_MUL
keymap[0x56]=NUMPAD_MIN
keymap[0x57]=NUMPAD_ADD
keymap[0x58]=NUMPAD_ENTER
keymap[0x59]=NUMPAD_1
keymap[0x5A]=NUMPAD_2
keymap[0x5B]=NUMPAD_3
keymap[0x5C]=NUMPAD_4
keymap[0x5D]=NUMPAD_5
keymap[0x5E]=NUMPAD_6
keymap[0x5F]=NUMPAD_7
keymap[0x60]=NUMPAD_8
keymap[0x61]=NUMPAD_9
keymap[0x62]=NUMPAD_0
keymap[0x63]=NUMPAD_DOT
keymap[0xE0]=LCTRL
keymap[0xE1]=LSHIFT
keymap[0xE2]=LALT
keymap[0xE3]=LWIN
keymap[0xE4]=RCTRL
keymap[0xE5]=RSHIFT
keymap[0xE6]=RALT
#keymap[0xFF]=FN

def keycode_lookup(key):
    if key in keymap:
        return keymap[key]
    return -1

