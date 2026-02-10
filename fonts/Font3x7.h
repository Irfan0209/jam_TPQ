
/*
 *
 * Font3x5
 *
 * created with FontCreator
 * written by F. Maximilian Thiele
 *
 * http://www.apetech.de/fontCreator
 * me@apetech.de
 *
 * File Name           : Font3x5.h
 * Date                : 17.02.2012
 * Font size in bytes  : 1472
 * Font width          : 3
 * Font height         : 5
 * Font first char     : 32
 * Font last char      : 128
 * Font used chars     : 96
 *
 * The font data are defined as
 *
 * struct _FONT_ {
 *     uint16_t   font_Size_in_Bytes_over_all_included_Size_it_self;
 *     uint8_t    font_Width_in_Pixel_for_fixed_drawing;
 *     uint8_t    font_Height_in_Pixel_for_all_characters;
 *     unit8_t    font_First_Char;
 *     uint8_t    font_Char_Count;
 *
 *     uint8_t    font_Char_Widths[font_Last_Char - font_First_Char +1];
 *                  // for each character the separate width in pixels,
 *                  // characters < 128 have an implicit virtual right empty row
 *
 *     uint8_t    font_data[];
 *                  // bit field of all characters
 */

#include <inttypes.h>
#include <avr/pgmspace.h>

#ifndef FONT3X7_H
#define FONT3X7_H

#define FONT3X7_WIDTH 3
#define FONT3X7_HEIGHT 7

const static uint8_t Font3x7[] PROGMEM = {
    0x03, 0xC0, // size
    0x03, // width
    0x07, // height
    0x20, // first char
    0x60, // char count
   
    // char widths
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, // ,!,",#,$,%,&,',(,)
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, //*,+,,-,.,/,0,1,2,3
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x02, 0x03, 0x03, 0x03, //4,5,6,7,8,9,:,;,<,=
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, //>,?,@,A,B,C,D,E,F,G
    0x03, 0x03, 0x03, 0x03, 0x03, 0x05, 0x05, 0x03, 0x03, 0x03, //H,I,J,K,L,M,N,O,P,Q
    0x03, 0x03, 0x03, 0x05, 0x05, 0x05, 0x03, 0x03, 0x03, 0x03, //R,S,T,U,V,W,X,Y,Z,[
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x05, 0x04, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x05, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
   
    // font data
        0x00, 0x00, 0x00,                        // Code for char  
        0x00, 0x00, 0x00,                        // Code for char !
        0x00, 0x00, 0x00,                        // Code for char "
        0x00, 0x00, 0x00,                        // Code for char #
        0x00, 0x00, 0x00,                        // Code for char $
        0x73, 0x08, 0x67,                        // Code for char %
        0x3A, 0x55, 0x2A,                        // Code for char &
        0x00, 0x03, 0x00,                        // Code for char '
        0x00, 0x41, 0x3E,                        // Code for char (
        0x3E, 0x41, 0x00,                        // Code for char )
        0x02, 0x05, 0x02,                        // Code for char *
        0x08, 0x1C, 0x08,                        // Code for char +
        0x40, 0x20, 0x00,                        // Code for char ,
        0x08, 0x08, 0x08,                        // Code for char -
        0x00, 0x20, 0x00,                        // Code for char .
        0x70, 0x08, 0x07,                        // Code for char /
        0x7F, 0x41, 0x7F,                        // Code for char 0
        0x42, 0x7F, 0x40,                        // Code for char 1
        0x79, 0x49, 0x4F,                        // Code for char 2
        0x49, 0x49, 0x7F,                        // Code for char 3
        0x0F, 0x08, 0x7F,                        // Code for char 4
        0x4F, 0x49, 0x79,                        // Code for char 5
        0x7F, 0x49, 0x79,                        // Code for char 6
        0x01, 0x01, 0x7F,                        // Code for char 7
        0x7F, 0x49, 0x7F,                        // Code for char 8
        0x4F, 0x49, 0x7F,                        // Code for char 9
        0x36, 0x36,                        // Code for char :
        0x00, 0xAC, 0x60,                        // Code for char ;
        0x38, 0x44, 0x82,                        // Code for char <
        0x28, 0x28, 0x28,                        // Code for char =
        0x82, 0x44, 0x38,                        // Code for char >
        0x04, 0xA2, 0x1C,                        // Code for char ?
        0xBC, 0xB2, 0x7C,                        // Code for char @
        0x7E, 0x09, 0x7E,                        // Code for char A
        0x7F, 0x49, 0x36,                        // Code for char B
        0x3E, 0x41, 0x22,                        // Code for char C
        0x7F, 0x41, 0x3E,                        // Code for char D
        0x7F, 0x49, 0x49,                        // Code for char E
        0x7F, 0x09, 0x09,                        // Code for char F
        0x3E, 0x49, 0x3A,                        // Code for char G
        0x7F, 0x08, 0x7F,                        // Code for char H
        0x41, 0x7F, 0x41,                        // Code for char I
        0x20, 0x41, 0x3F,                        // Code for char J
        0x7F, 0x14, 0x63,                        // Code for char K
        0x7F, 0x40, 0x40,                        // Code for char L
        0x7F, 0x02, 0x0C, 0x02, 0x7F,            // Code for char M
        0x7F, 0x02, 0x04, 0x08, 0x7F,            // Code for char N
        0x3E, 0x41, 0x3E,                        // Code for char O
        0x7F, 0x09, 0x06,                        // Code for char P
        0x3E, 0x61, 0x5E,                        // Code for char Q
        0x7F, 0x19, 0x66,                        // Code for char R
        0x46, 0x49, 0x31,                        // Code for char S
        0x01, 0x7F, 0x01,                        // Code for char T
        0x3F, 0x40, 0x3F,                        // Code for char U
        0x3E, 0x40, 0x80, 0x40, 0x3E,            // Code for char V
        0x7E, 0x80, 0x70, 0x80, 0x7E,            // Code for char W
        0x82, 0x44, 0x38, 0x44, 0x82,            // Code for char X
        0x0E, 0xF0, 0x0E,                        // Code for char Y
        0xE2, 0x92, 0x8E,                        // Code for char Z
        0xFE, 0x82, 0x00,                        // Code for char [
        0x06, 0x38, 0xC0,                        // Code for char BackSlash
        0x00, 0x82, 0xFE,                        // Code for char ]
        0x04, 0x02, 0x04,                        // Code for char ^
        0x04, 0x02, 0x02,                        // Code for char _
        0x00, 0x02, 0x00,                        // Code for char `
        0x64, 0x94, 0xF8,                        // Code for char a
        0xFE, 0x88, 0x70,                        // Code for char b
        0x78, 0x84, 0x48,                        // Code for char c
        0x70, 0x88, 0xFE,                        // Code for char d
        0x78, 0x94, 0x58,                        // Code for char e
        0x08, 0xFC, 0x0A,                        // Code for char f
        0x4C, 0x92, 0x7E,                        // Code for char g
        0xFE, 0x08, 0xF0,                        // Code for char h
        0x88, 0xFA, 0x80,                        // Code for char i
        0x40, 0x88, 0x7A,                        // Code for char j
        0xFE, 0x60, 0x90,                        // Code for char k
        0x7E, 0x80, 0x80,                        // Code for char l
        0xFC, 0x04, 0xF8, 0x04, 0xF8,            // Code for char m
        0xFC, 0x08, 0x04, 0xF8,                  // Code for char n
        0x78, 0x84, 0x78,                        // Code for char o
        0xFC, 0x24, 0x18,                        // Code for char p
        0x18, 0x24, 0xFC,                        // Code for char q
        0xFC, 0x08, 0x04,                        // Code for char r
        0x98, 0xA4, 0x44,                        // Code for char s
        0x08, 0x7E, 0x88,                        // Code for char t
        0x7C, 0x80, 0xFC,                        // Code for char u
        0x00, 0x00, 0x00,                        // Code for char v
        0x00, 0x00, 0x00,                        // Code for char w
        0x00, 0x00, 0x00,                        // Code for char x
        0x00, 0x00, 0x00,                        // Code for char y
        0x00, 0x00, 0x00,                        // Code for char z
        0x00, 0x00, 0x00,                        // Code for char {
        0x00, 0x00, 0x00,                        // Code for char |
        0x00, 0x00, 0x00,                        // Code for char }
        0x00, 0x00, 0x00,                        // Code for char ~
        0x00, 0x00, 0x00                         // Code for char 
        };

#endif