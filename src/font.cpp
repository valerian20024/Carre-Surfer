#include "font.hpp"

#include "text.hpp"

#include <cstring>
#include <iostream>

using Glyph = std::array<uint8_t, LETTER_HEIGHT>;

Glyph to_glyph(char c) {

    constexpr Glyph BLANK = {
        0b000'00000, 
        0b000'00000, 
        0b000'00000, 
        0b000'00000, 
        0b000'00000, 
        0b000'00000, 
        0b000'00000
    };
    
    constexpr Glyph SPACE = {
        0b000'00000, 
        0b000'00000, 
        0b000'00000, 
        0b000'00000, 
        0b000'00000, 
        0b000'00000, 
        0b000'00000
    };

    constexpr Glyph EXCLAMATION = {
        0b000'00100, 
        0b000'00100, 
        0b000'00100, 
        0b000'00100, 
        0b000'00100, 
        0b000'00000, 
        0b000'00100
    };

    constexpr Glyph HYPHEN = {
        0b000'00000, 
        0b000'00000, 
        0b000'00000, 
        0b000'11111, 
        0b000'00000, 
        0b000'00000, 
        0b000'00000
    };

    constexpr Glyph BAR = {
        0b000'00100, 
        0b000'00100, 
        0b000'00100, 
        0b000'00100, 
        0b000'00100, 
        0b000'00100, 
        0b000'00100
    };

    constexpr Glyph COLON = {
        0b000'00000, 
        0b000'01100, 
        0b000'01100, 
        0b000'00000, 
        0b000'01100, 
        0b000'01100, 
        0b000'00000
    };

    constexpr Glyph LPAR = {
        0b000'00010, 
        0b000'00100, 
        0b000'01000, 
        0b000'01000, 
        0b000'01000, 
        0b000'00100, 
        0b000'00010
    };

    constexpr Glyph RPAR = {
        0b000'01000, 
        0b000'00100, 
        0b000'00010, 
        0b000'00010, 
        0b000'00010, 
        0b000'00100, 
        0b000'01000
    };

    constexpr Glyph LBRACKET = {
        0b000'01110, 
        0b000'01000, 
        0b000'01000, 
        0b000'01000, 
        0b000'01000, 
        0b000'01000, 
        0b000'01110
    };

    constexpr Glyph RBRACKET = {
        0b000'01110, 
        0b000'00010, 
        0b000'00010, 
        0b000'00010, 
        0b000'00010, 
        0b000'00010, 
        0b000'01110
    };
    
    constexpr Glyph DIGIT_0 = {
        0b000'01110, 
        0b000'10001, 
        0b000'10011, 
        0b000'10101, 
        0b000'11001, 
        0b000'10001, 
        0b000'01110
    };
    
    constexpr Glyph DIGIT_1 = {
        0b000'00100, 
        0b000'01100, 
        0b000'00100, 
        0b000'00100, 
        0b000'00100, 
        0b000'00100, 
        0b000'01110
    };
    
    constexpr Glyph DIGIT_2 = {
        0b000'01110, 
        0b000'10001, 
        0b000'00001, 
        0b000'00010, 
        0b000'00100, 
        0b000'01000, 
        0b000'11111
    };

    constexpr Glyph DIGIT_3 = {
        0b000'11110, 
        0b000'00001, 
        0b000'00001, 
        0b000'01110, 
        0b000'00001, 
        0b000'00001, 
        0b000'11110
    };

    constexpr Glyph DIGIT_4 = {
        0b000'00010, 
        0b000'00110, 
        0b000'01010, 
        0b000'10010, 
        0b000'11111, 
        0b000'00010, 
        0b000'00010
    };

    constexpr Glyph DIGIT_5 = {
        0b000'11111, 
        0b000'10000, 
        0b000'10000, 
        0b000'11110, 
        0b000'00001, 
        0b000'00001, 
        0b000'11110
    };

    constexpr Glyph DIGIT_6 = {
        0b000'01110, 
        0b000'10000, 
        0b000'10000, 
        0b000'11110, 
        0b000'10001, 
        0b000'10001, 
        0b000'01110
    };

    constexpr Glyph DIGIT_7 = {
        0b000'11111, 
        0b000'00001, 
        0b000'00010, 
        0b000'00100, 
        0b000'01000, 
        0b000'01000, 
        0b000'01000
    };

    constexpr Glyph DIGIT_8 = {
        0b000'01110, 
        0b000'10001, 
        0b000'10001, 
        0b000'01110, 
        0b000'10001, 
        0b000'10001, 
        0b000'01110
    };

    constexpr Glyph DIGIT_9 = {
        0b000'01110, 
        0b000'10001, 
        0b000'10001, 
        0b000'01111, 
        0b000'00001, 
        0b000'00001, 
        0b000'01110
    };
    
    constexpr Glyph CHAR_A = {
        0b000'01110, 
        0b000'10001, 
        0b000'10001, 
        0b000'11111, 
        0b000'10001, 
        0b000'10001, 
        0b000'10001
    };
    
    constexpr Glyph CHAR_B = {
        0b000'11110, 
        0b000'10001, 
        0b000'10001, 
        0b000'11110, 
        0b000'10001, 
        0b000'10001, 
        0b000'11110
    };
    
    constexpr Glyph CHAR_C = {
        0b000'01110, 
        0b000'10001, 
        0b000'10000, 
        0b000'10000, 
        0b000'10000, 
        0b000'10001, 
        0b000'01110
    };

    constexpr Glyph CHAR_D = {
        0b000'11100, 
        0b000'10010, 
        0b000'10001, 
        0b000'10001, 
        0b000'10001, 
        0b000'10010, 
        0b000'11100
    };

    constexpr Glyph CHAR_E = {
        0b000'11111, 
        0b000'10000, 
        0b000'10000, 
        0b000'11110, 
        0b000'10000, 
        0b000'10000, 
        0b000'11111
    };

    constexpr Glyph CHAR_F = {
        0b000'11111, 
        0b000'10000, 
        0b000'10000, 
        0b000'11110, 
        0b000'10000, 
        0b000'10000, 
        0b000'10000
    };

    constexpr Glyph CHAR_G = {
        0b000'01110, 
        0b000'10001, 
        0b000'10000, 
        0b000'10111, 
        0b000'10001, 
        0b000'10001, 
        0b000'01110
    };

    constexpr Glyph CHAR_H = {
        0b000'10001, 
        0b000'10001, 
        0b000'10001, 
        0b000'11111, 
        0b000'10001, 
        0b000'10001, 
        0b000'10001
    };

    constexpr Glyph CHAR_I = {
        0b000'01110, 
        0b000'00100, 
        0b000'00100, 
        0b000'00100, 
        0b000'00100, 
        0b000'00100, 
        0b000'01110
    };

    constexpr Glyph CHAR_J = {
        0b000'00111, 
        0b000'00010, 
        0b000'00010, 
        0b000'00010, 
        0b000'00010, 
        0b000'10010, 
        0b000'01100
    };

    constexpr Glyph CHAR_K = {
        0b000'10001, 
        0b000'10010, 
        0b000'10100, 
        0b000'11000, 
        0b000'10100, 
        0b000'10010, 
        0b000'10001
    };

    constexpr Glyph CHAR_L = {
        0b000'10000, 
        0b000'10000, 
        0b000'10000, 
        0b000'10000, 
        0b000'10000, 
        0b000'10000, 
        0b000'11111
    };

    constexpr Glyph CHAR_M = {
        0b000'10001, 
        0b000'11011, 
        0b000'10101, 
        0b000'10001, 
        0b000'10001, 
        0b000'10001, 
        0b000'10001
    };

    constexpr Glyph CHAR_N = {
        0b000'10001, 
        0b000'11001, 
        0b000'10101, 
        0b000'10011, 
        0b000'10001, 
        0b000'10001, 
        0b000'10001
    };

    constexpr Glyph CHAR_O = {
        0b000'01110, 
        0b000'10001, 
        0b000'10001, 
        0b000'10001, 
        0b000'10001, 
        0b000'10001, 
        0b000'01110
    };

    constexpr Glyph CHAR_P = {
        0b000'11110, 
        0b000'10001, 
        0b000'10001, 
        0b000'11110, 
        0b000'10000, 
        0b000'10000, 
        0b000'10000
    };

    constexpr Glyph CHAR_Q = {
        0b000'01110, 
        0b000'10001, 
        0b000'10001, 
        0b000'10001, 
        0b000'10101, 
        0b000'10010, 
        0b000'01101
    };

    constexpr Glyph CHAR_R = {
        0b000'11110, 
        0b000'10001, 
        0b000'10001, 
        0b000'11110, 
        0b000'10100, 
        0b000'10010, 
        0b000'10001
    };

    constexpr Glyph CHAR_S = {
        0b000'01111, 
        0b000'10000, 
        0b000'10000, 
        0b000'01110, 
        0b000'00001, 
        0b000'00001, 
        0b000'11110
    };

    constexpr Glyph CHAR_T = {
        0b000'11111, 
        0b000'00100, 
        0b000'00100, 
        0b000'00100, 
        0b000'00100, 
        0b000'00100, 
        0b000'00100
    };

    constexpr Glyph CHAR_U = {
        0b000'10001, 
        0b000'10001, 
        0b000'10001, 
        0b000'10001, 
        0b000'10001, 
        0b000'10001, 
        0b000'01110
    };

    constexpr Glyph CHAR_V = {
        0b000'10001, 
        0b000'10001, 
        0b000'10001, 
        0b000'10001, 
        0b000'10001, 
        0b000'01010, 
        0b000'00100
    };

    constexpr Glyph CHAR_W = {
        0b000'10001, 
        0b000'10001, 
        0b000'10001, 
        0b000'10101, 
        0b000'10101, 
        0b000'11011, 
        0b000'10001
    };

    constexpr Glyph CHAR_X = {
        0b000'10001, 
        0b000'10001, 
        0b000'01010, 
        0b000'00100, 
        0b000'01010, 
        0b000'10001, 
        0b000'10001
    };

    constexpr Glyph CHAR_Y = {
        0b000'10001, 
        0b000'10001, 
        0b000'01010, 
        0b000'00100, 
        0b000'00100, 
        0b000'00100, 
        0b000'00100
    };

    constexpr Glyph CHAR_Z = {
        0b000'11111, 
        0b000'00001, 
        0b000'00010, 
        0b000'00100, 
        0b000'01000, 
        0b000'10000, 
        0b000'11111
    };

    switch (c) { 
        case ' ':  return SPACE;
        case '!':  return EXCLAMATION;
        case '-':  return HYPHEN;
        case '|':  return BAR;
        case ':':  return COLON;
        case '(':  return LPAR;
        case ')':  return RPAR;
        case '[':  return LBRACKET;
        case ']':  return RBRACKET;
        case '0':  return DIGIT_0;
        case '1':  return DIGIT_1;
        case '2':  return DIGIT_2;
        case '3':  return DIGIT_3;
        case '4':  return DIGIT_4;
        case '5':  return DIGIT_5;
        case '6':  return DIGIT_6;
        case '7':  return DIGIT_7;
        case '8':  return DIGIT_8;
        case '9':  return DIGIT_9;
        case 'A': case 'a': return CHAR_A;
        case 'B': case 'b': return CHAR_B;
        case 'C': case 'c': return CHAR_C;
        case 'D': case 'd': return CHAR_D;
        case 'E': case 'e': return CHAR_E;
        case 'F': case 'f': return CHAR_F;
        case 'G': case 'g': return CHAR_G;
        case 'H': case 'h': return CHAR_H;
        case 'I': case 'i': return CHAR_I;
        case 'J': case 'j': return CHAR_J;
        case 'K': case 'k': return CHAR_K;
        case 'L': case 'l': return CHAR_L;
        case 'M': case 'm': return CHAR_M;
        case 'N': case 'n': return CHAR_N;
        case 'O': case 'o': return CHAR_O;
        case 'P': case 'p': return CHAR_P;
        case 'Q': case 'q': return CHAR_Q;
        case 'R': case 'r': return CHAR_R;
        case 'S': case 's': return CHAR_S;
        case 'T': case 't': return CHAR_T;
        case 'U': case 'u': return CHAR_U;
        case 'V': case 'v': return CHAR_V;
        case 'W': case 'w': return CHAR_W;
        case 'X': case 'x': return CHAR_X;
        case 'Y': case 'y': return CHAR_Y;
        case 'Z': case 'z': return CHAR_Z;
        default:   return BLANK;
    }
}

void print_glyph(char c) {
    Glyph glyph = to_glyph(c);

    for (size_t row = 0; row < LETTER_HEIGHT; ++row) {
        uint8_t row_bits = glyph[row];

        for (int col = 0; col < LETTER_WIDTH; ++col) {
            bool bit_set = (row_bits >> (4 - col)) & 0x01;
            
            std::cout << (bit_set ? "*" : " ");
        }

        std::cout << '\n';
    }
}