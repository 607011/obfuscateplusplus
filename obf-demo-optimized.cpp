#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifndef OBFUSCATION_KEY
// to silence the IDE's code checker
#define OBFUSCATION_KEY (0x00031337)
#endif

struct safe_string
{
    char *const str;
    size_t const size;
    ~safe_string()
    {
        secure_erase_memory();
    }
    void secure_erase_memory()
    {
#if HAVE_EXPLICIT_BZERO
        explicit_bzero(str, size);
#elif HAVE_MEMSET_S
        memset_s(str, size, 0, size);
#elif HAVE_MEMSET
        memset(str, size, 0);
#else
#pragma warning "secure_erase_memory() will not work because there's no function to zero memory"
#endif
    };
    friend std::ostream& operator<<(std::ostream&, safe_string const &);
};

std::ostream& operator<<(std::ostream &os, safe_string const &s)
{
    os << s.str;
    return os;
}

template <uint32_t KEY, size_t N, uint32_t A, uint32_t C, uint32_t M>
struct obfuscated
{
    constexpr obfuscated(const char *src)
    {
        uint32_t key = KEY;
        for (auto i = 0; i < N; ++i)
        {
            data_[i] = src[i] ^ static_cast<uint8_t>(key);
            key = (A * key + C) % M;
        }
    }
    void unmask(char *dst) const
    {
        int i = 0;
        uint32_t key = KEY;
        do
        {
            dst[i] = data_[i] ^ static_cast<char>(key);
            key = (A * key + C) % M;
            ++i;
        } while (dst[i - 1] != '\0');
    }

private:
    uint8_t data_[N];
};

#define OBFUSCATED_STR(key, str)                                 \
    []() -> safe_string {                                        \
        constexpr auto size = sizeof(str) / sizeof(str[0]);      \
        constexpr auto obfuscated_str =                          \
            obfuscated<key, size, 48271U, 0U, 2147483647U>(str); \
        static char original_string[size];                       \
        obfuscated_str.unmask(original_string);                  \
        return {original_string, size};                          \
    }()

int main(void)
{
    std::cout << OBFUSCATED_STR(OBFUSCATION_KEY, "Hallo, Welt!");
    std::cin.get();
    return EXIT_SUCCESS;
}
