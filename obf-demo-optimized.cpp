#include <iostream>

#include "obf-demo-optimized.hpp"

int main(void)
{
    {
        auto o = OBFUSCATED_STR(OBFUSCATION_KEY, "Hallo, Welt!");
        std::cout << o << std::endl;
    }
    std::cin.get();
    return EXIT_SUCCESS;
}
