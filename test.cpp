
#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include "bitdecoder.hpp"

int main(){
    // use fully-qualified std::string (or add `using std::string;`)
    char msg[81] = {"ass\n"};
    char output[81] = {'\0'};
    // default-initialize the bool array
    std::array<bool, 8> bitBuffer = {0};
    std::array<bool, 8 * 81> bits = {0};

    char checksum1 = 0x00;
    char checksum2 = 0x00;
    
    int i = 0;
    char curchar;
    // encode message
    while(i <= 80){
        curchar = msg[i];
        encodeChar(bits.data(), i*8, curchar);
        i++;
        // check for end of message
        if(curchar == '\0') break;
    }
    for (bool bit : bits){
        checksum1 += bit;
    }

    // print the bits
    for (int i = 0; i < 81; i++){
        for (int j = 0; j < 8; j++){
            bool curbit = bits[i*8 + j];
            std::cout << curbit;
        }
        std::cout << " ";
    }
    std::cout << std::endl;

    // decode
    i = 0;
    while (i <= 80){
        for (int j = 0; j < 8; j++){
            bool curbit = bits[i*8+j];
            checksum2 += curbit;
            bitBuffer[j] = curbit;
        }
        curchar = decodeBits(bitBuffer.data(), 0);
        if (i == 80 && curchar != 0x00) std::cout<< "error, no transmission end.\n";
        output[i] = curchar;
        i++;
    }
    std::cout << output;
    std::cout << static_cast<int>(checksum1) << std::endl;
    std::cout << static_cast<int>(checksum2) << std::endl;
    std::cout << "end test\n";
    return 0;
}