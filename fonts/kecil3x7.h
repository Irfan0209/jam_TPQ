#include <inttypes.h>
#include <avr/pgmspace.h>

#ifndef KECIL3x7_H
#define KECIL3x7_H

#define KECIL3x7_WIDTH 3
#define KECIL3x7_HEIGHT 7

const static uint8_t kecil3x7[] PROGMEM = {
    0x00, 0x00, //C0, // size
    0x03, // width
    0x07, // height
    0x20, // first char
    0x60, // char count
    
	    0x00, 0x00, 0x00,                        // Code for char  
        0x00, 0xBE, 0x00,                        // Code for char !
        0x00, 0x00, 0x00,                        // Code for char "
        0x00, 0x00, 0x00,                        // Code for char #
        0x00, 0x00, 0x00,                        // Code for char $
        0x00, 0x00, 0x00,                        // Code for char %
        0x00, 0x00, 0x00,                        // Code for char &
        0x00, 0x03, 0x00,                        // Code for char '
        0x3E, 0x41, 0x00,                        // Code for char (
        0x00, 0x41, 0x3E,                        // Code for char )
        0x05, 0x02, 0x05,                        // Code for char *
        0x08, 0x1C, 0x08,                        // Code for char +
        0x00, 0x60, 0x00,                        // Code for char ,
        0x08, 0x08, 0x08,                        // Code for char -
        0x00, 0x40, 0x00,                        // Code for char .
        0x30, 0x08, 0x06,                        // Code for char /
        0x7F, 0x41, 0x7F,                        // Code for char 0
        0x41, 0x7F, 0x40,                        // Code for char 1
        0x79, 0x49, 0x4F,                        // Code for char 2
        0x49, 0x49, 0x7F,                        // Code for char 3
        0x0F, 0x08, 0x7F,                        // Code for char 4
        0x4F, 0x49, 0x79,                        // Code for char 5
        0x7F, 0x49, 0x79,                        // Code for char 6
        0x01, 0x01, 0x7F,                        // Code for char 7
        0x7F, 0x49, 0x7F,                        // Code for char 8
        0x4F, 0x49, 0x7F,                        // Code for char 9
        0x00, 0x36, 0x36,                        // Code for char :
        0x00, 0x16, 0x36,                        // Code for char ;
        0x08, 0x14, 0x22,                        // Code for char <
        0x14, 0x14, 0x14,                        // Code for char =
        0x22, 0x14, 0x08,                        // Code for char >
        0x02, 0x51, 0x0E,                        // Code for char ?
        0x1E, 0x59, 0x3E,                        // Code for char @
        0x7E, 0x09, 0x7E,                        // Code for char A
        0x7F, 0x49, 0x36,                        // Code for char B
        0x3E, 0x41, 0x22,                        // Code for char C
        0x7F, 0x41, 0x3E,                        // Code for char D
        0x7F, 0x49, 0x49,                        // Code for char E
        0x7F, 0x09, 0x09,                        // Code for char F
        0x3E, 0x41, 0x32,                        // Code for char G
        0x7F, 0x08, 0x7F,                        // Code for char H
        0x41, 0x7F, 0x41,                        // Code for char I
        0x20, 0x41, 0x3F,                        // Code for char J
        0x7F, 0x1C, 0x63,                        // Code for char K
        0x7F, 0x40, 0x40,                        // Code for char L
        0x7F, 0x06, 0x7F,                        // Code for char M
        0x7F, 0x01, 0x7E,                        // Code for char N
        0x3E, 0x41, 0x3E,                        // Code for char O
        0x7F, 0x09, 0x06,                        // Code for char P
        0x3E, 0x21, 0x5E,                        // Code for char Q
        0x7F, 0x19, 0x66,                        // Code for char R
        0x26, 0x49, 0x32,                        // Code for char S
        0x01, 0x7F, 0x01,                        // Code for char T
        0x3F, 0x40, 0x3F,                        // Code for char U
        0x3F, 0x60, 0x3F,                        // Code for char V
        0x7F, 0x30, 0x7F,                        // Code for char W
        0x63, 0x1C, 0x63,                        // Code for char X
        0x07, 0x78, 0x07,                        // Code for char Y
        0x71, 0x49, 0x47,                        // Code for char Z
        0x7F, 0x41, 0x00,                        // Code for char [
        0x06, 0x08, 0x30,                        // Code for char BackSlash
        0x00, 0x41, 0x7F,                        // Code for char ]
        0x02, 0x01, 0x02,                        // Code for char ^
        0x40, 0x40, 0x40,                        // Code for char _
        0x00, 0x01, 0x02,                        // Code for char `
        0x24, 0x54, 0x78,                        // Code for char a
        0x7F, 0x48, 0x30,                        // Code for char b
        0x38, 0x44, 0x44,                        // Code for char c
        0x30, 0x48, 0x7F,                        // Code for char d
        0x38, 0x54, 0x58,                        // Code for char e
        0x08, 0x7E, 0x09,                        // Code for char f
        0x48, 0x54, 0x3C,                        // Code for char g
        0x7F, 0x04, 0x78,                        // Code for char h
        0x44, 0x7D, 0x40,                        // Code for char i
        0x20, 0x44, 0x3D,                        // Code for char j
        0x7F, 0x30, 0x4C,                        // Code for char k
        0x7F, 0x00, 0x00,                        // Code for char l
        0x7C, 0x18, 0x7C,                        // Code for char m
        0x7C, 0x04, 0x78,                        // Code for char n
        0x38, 0x44, 0x38,                        // Code for char o
        0x7C, 0x24, 0x18,                        // Code for char p
        0x18, 0x24, 0x7C,                        // Code for char q
        0x7C, 0x04, 0x08,                        // Code for char r
        0x48, 0x54, 0x24,                        // Code for char s
        0x04, 0x3F, 0x44,                        // Code for char t
        0x3C, 0x40, 0x7C,                        // Code for char u
        0x1C, 0x20, 0x7C,                        // Code for char v
        0x7C, 0x30, 0x7C,                        // Code for char w
        0x6C, 0x10, 0x6C,                        // Code for char x
        0x2C, 0x48, 0x3C,                        // Code for char y
        0x64, 0x54, 0x4C,                        // Code for char z
        0x00, 0x00, 0x00,                        // Code for char {
        0x00, 0x00, 0x00,                        // Code for char |
        0x00, 0x00, 0x00,                        // Code for char }
        0x00, 0x00, 0x00                         // Code for char ~
};

#endif
