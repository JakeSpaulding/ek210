#pragma once
#include <cstddef>
#define MSG_LENGTH 81

void clearMsg(char* msg){
    for (int i = 0; i < MSG_LENGTH - 1; i++){
        msg[i] = 0;
    }
    msg[MSG_LENGTH - 1] = 0x04; // set the end transmission code
}

// decode 8 bits starting at `offset` and return the assembled char
inline char decodeBits(const bool* const bits, const unsigned int offset = 0){
    unsigned char fin = 0;
    for (unsigned int i = 0; i < 8; ++i){
        fin |= static_cast<unsigned char>(bits[offset + i]) << i;
    }
    return static_cast<char>(fin);
}

// writes the bits of `c` into bits[offset..offset+7], LSB at offset
inline void encodeChar(bool* bits, unsigned int offset, const char c){
    for (unsigned int i = 0; i < 8; ++i){
        bits[offset + i] = ((c >> i) & 1) != 0;
    }
}


// converts the msg string to binary in the signal bool array, outputs the checksum
inline void msg2binary(bool* signal, char* msg){
    char curChar; // stores the current character
    for (int i = 0; i < MSG_LENGTH - 1; i++){
        curChar = msg[i];
        encodeChar(signal, i*8, curChar);
    }
    // set the last byte to the transmit code
    msg[MSG_LENGTH - 1] = 0x04;
    encodeChar(signal, (MSG_LENGTH - 1) * 8, 0x04);

}

inline bool binary2msg(const bool* signal, char* msg){
    char curChar = 0x00;
    for (int i = 0; i < MSG_LENGTH; i++){
        curChar = decodeBits(signal, i * 8);
        // check for end of transmission
        if (i == MSG_LENGTH - 1 && curChar != 0x04) return false;
        msg[i] = curChar;
    }
    return true;
}

