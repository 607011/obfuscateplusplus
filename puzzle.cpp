#include <iostream>

#include "deobfuscator.hpp"
#include "hint.txt.h"

#ifndef OBFUSCATION_KEY
// to silence the IDE's code checker
#define OBFUSCATION_KEY (0x00031337)
#endif

int main(int argc, char *argv[])
{
    obfuscated::data<OBFUSCATION_KEY,
                     hint_txt_size,
                     48271U,
                     0U,
                     2147483647U>
        hidden(hint_txt);
    std::cout << hidden.c_str()
              << std::endl;
    std::cin.get();
}
