#include <atomic>
#include <cstdio>
#include <cstdlib>

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
#if defined(__clang__)
        #pragma clang loop unroll(disable)
#elif defined(__GNUG__)
        #pragma GCC unroll 0
#elif defined(_MSC_VER)
        #pragma loop( no_vector )
#else
        #pragma message("WARNING: cannot disable loop-unrolling. Check binary for unwanted cleartext strings!")
#endif
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
    return EXIT_SUCCESS;
}
