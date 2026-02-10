#include <inttypes.h>
#include <avr/pgmspace.h>

#ifndef KECIL3x6_H
#define KECIL3x6_H

#define KECIL3x6_WIDTH 3
#define KECIL3x6_HEIGHT 6

const static uint8_t kecil3x6[] PROGMEM = {
    0x00, 0x00, //C0, // size
    0x03, // width
    0x07, // height
    0x20, // first char
    0x60, // char count
    
	    0x00, 0x00, 0x00,                        // Code for char  
        0x00, 0x2F, 0x00,                        // Code for char !
        0x03, 0x00, 0x03,                        // Code for char "
        0x00, 0x00, 0x00,                        // Code for char #
        0x00, 0x00, 0x00,                        // Code for char $
        0x00, 0x00, 0x00,                        // Code for char %
        0x00, 0x00, 0x00,                        // Code for char &
        0x00, 0x03, 0x00,                        // Code for char '
        0x1E, 0x21, 0x00,                        // Code for char (
        0x00, 0x21, 0x1E,                        // Code for char )
        0x05, 0x02, 0x05,                        // Code for char *
        0x04, 0x0E, 0x04,                        // Code for char +
        0x10, 0x30, 0x00,                        // Code for char ,
        0x04, 0x04, 0x04,                        // Code for char -
        0x00, 0x30, 0x30,                        // Code for char .
        0x18, 0x04, 0x03,                        // Code for char /
        0x3F, 0x21, 0x3F,                        // Code for char 0
        0x21, 0x3F, 0x20,                        // Code for char 1
        0x39, 0x29, 0x2F,                        // Code for char 2
        0x29, 0x29, 0x3F,                        // Code for char 3
        0x0F, 0x08, 0x3F,                        // Code for char 4
        0x2F, 0x29, 0x39,                        // Code for char 5
        0x3F, 0x29, 0x39,                        // Code for char 6
        0x01, 0x01, 0x3F,                        // Code for char 7
        0x3F, 0x29, 0x3F,                        // Code for char 8
        0x2F, 0x29, 0x3F,                        // Code for char 9
        0x00, 0x12, 0x12,                        // Code for char :
        0x00, 0x0A, 0x1A,                        // Code for char ;
        0x00, 0x00, 0x00,                        // Code for char <
        0x0A, 0x0A, 0x0A,                        // Code for char =
        0x00, 0x00, 0x00,                        // Code for char >
        0x00, 0x00, 0x00,                        // Code for char ?
        0x00, 0x00, 0x00,                        // Code for char @
        0x3E, 0x09, 0x3E,                        // Code for char A
        0x3F, 0x29, 0x16,                        // Code for char B
        0x1E, 0x21, 0x12,                        // Code for char C
        0x3F, 0x21, 0x1E,                        // Code for char D
        0x3F, 0x29, 0x29,                        // Code for char E
        0x3F, 0x09, 0x09,                        // Code for char F
        0x1E, 0x21, 0x1A,                        // Code for char G
        0x3F, 0x08, 0x3F,                        // Code for char H
        0x21, 0x3F, 0x21,                        // Code for char I
        0x10, 0x21, 0x1F,                        // Code for char J
        0x3F, 0x0C, 0x33,                        // Code for char K
        0x3F, 0x20, 0x20,                        // Code for char L
        0x3F, 0x06, 0x3F,                        // Code for char M
        0x3F, 0x01, 0x3E,                        // Code for char N
        0x1E, 0x21, 0x1E,                        // Code for char O
        0x3F, 0x09, 0x06,                        // Code for char P
        0x1E, 0x11, 0x2E,                        // Code for char Q
        0x3F, 0x19, 0x26,                        // Code for char R
        0x26, 0x29, 0x12,                        // Code for char S
        0x01, 0x3F, 0x01,                        // Code for char T
        0x1F, 0x20, 0x1F,                        // Code for char U
        0x0F, 0x10, 0x3F,                        // Code for char V
        0x3F, 0x18, 0x3F,                        // Code for char W
        0x33, 0x0C, 0x33,                        // Code for char X
        0x13, 0x24, 0x1F,                        // Code for char Y
        0x31, 0x29, 0x27,                        // Code for char Z
        0x00, 0x00, 0x00,                        // Code for char [
        0x00, 0x00, 0x00,                        // Code for char BackSlash
        0x00, 0x00, 0x00,                        // Code for char ]
        0x00, 0x00, 0x00,                        // Code for char ^
        0x00, 0x00, 0x00,                        // Code for char _
        0x00, 0x00, 0x00,                        // Code for char `
        0x12, 0x2A, 0x3C,                        // Code for char a
        0x3F, 0x24, 0x18,                        // Code for char b
        0x1C, 0x22, 0x22,                        // Code for char c
        0x18, 0x24, 0x3F,                        // Code for char d
        0x1C, 0x2A, 0x2C,                        // Code for char e
        0x04, 0x3E, 0x05,                        // Code for char f
        0x4C, 0x92, 0x7E,                        // Code for char g
        0x3F, 0x04, 0x38,                        // Code for char h
        0x24, 0x3D, 0x20,                        // Code for char i
        0x40, 0x84, 0x7D,                        // Code for char j
        0x3F, 0x18, 0x24,                        // Code for char k
        0x3F, 0x20, 0x00,                        // Code for char l
        0x3E, 0x0C, 0x3E,                        // Code for char m
        0x3E, 0x02, 0x3C,                        // Code for char n
        0x1C, 0x22, 0x1C,                        // Code for char o
        0x3E, 0x12, 0x0C,                        // Code for char p
        0x0C, 0x12, 0x3E,                        // Code for char q
        0x3E, 0x02, 0x04,                        // Code for char r
        0x24, 0x2A, 0x12,                        // Code for char s
        0x02, 0x1F, 0x22,                        // Code for char t
        0x1E, 0x20, 0x1E,                        // Code for char u
        0x0E, 0x10, 0x3E,                        // Code for char v
        0x3E, 0x18, 0x3E,                        // Code for char w
        0x36, 0x08, 0x36,                        // Code for char x
        0x46, 0x88, 0x7E,                        // Code for char y
        0x32, 0x2A, 0x26,                        // Code for char z
        0x00, 0x00, 0x00,                        // Code for char {
        0x00, 0x00, 0x00,                        // Code for char |
        0x00, 0x00, 0x00,                        // Code for char }
        0x00, 0x00, 0x00                         // Code for char ~
};

#endif
