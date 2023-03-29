#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifndef XOR_KEY
// to silence the IDE's code checker
#define XOR_KEY static_cast<char>(0x55)
#endif

template <char KEY, size_t N>
struct obfuscated_xor
{
    constexpr obfuscated_xor(const char *src)
    {
        for (auto i = 0; i < N; ++i)
        {
            data_[i] = src[i] ^ KEY;
        }
    }
    void unmask(char *dst) const
    {
        int i = 0;
        do
        {
            dst[i] = data_[i] ^ KEY;
            ++i;
        } while (dst[i - 1] != '\0');
    }

private:
    char data_[N];
};

#define XOR_STR(key, str)                                   \
    []() -> char * {                                        \
        constexpr auto size = sizeof(str) / sizeof(str[0]); \
        constexpr auto obfuscated_str =                     \
            obfuscated_xor<key, size>(str);                 \
        static char original_string[size];                  \
        obfuscated_str.unmask(original_string);             \
        return original_string;                             \
    }()

int main(void)
{
    printf("%s", XOR_STR(XOR_KEY, "Hallo, Welt!\n"));
    // std::cin.get();
    return EXIT_SUCCESS;
}
