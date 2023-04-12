#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>

#ifdef _WIN32
#include <Windows.h>
#endif

#ifndef OBFUSCATION_KEY
// to silence the IDE's code checker
#define OBFUSCATION_KEY (0x13371337U)
#endif

using std::cin;
using std::cout;
using std::ostream;
using std::string;

template <size_t N>
class safe_string final : public std::array<char, N>
{
public:
    safe_string() = delete;
    safe_string(char *const str) : str_(str) {}
    safe_string(safe_string &&other) : str_(other.str_)
    {
        other.str_ = nullptr;
    }
    safe_string &operator=(safe_string const &other) = delete;
    const char *c_str() const
    {
        return str_;
    }
    ~safe_string()
    {
        secure_erase_memory();
    }

private:
    char *str_;

    void secure_erase_memory()
    {
#if defined(HAVE_SECUREZEROMEMORY)
        SecureZeroMemory(str_, N);
#elif defined(HAVE_EXPLICIT_BZERO)
        explicit_bzero(str_, N);
#elif defined(HAVE_MEMSET_S)
        memset_s(str_, N, 0, N);
#else
        volatile char *p = str_;
        size_t n = N;
        while (n--)
        {
            *p++ = '\0';
        }
#endif
    };

    template <size_t Nvalue>
    friend std::ostream &operator<<(std::ostream &, safe_string<Nvalue> const &);
};

template <size_t Nvalue>
std::ostream &operator<<(std::ostream &os, safe_string<Nvalue> const &s)
{
    os << s.str_;
    return os;
}

template <uint32_t KEY, size_t N, uint32_t A, uint32_t C, uint32_t M>
struct obfuscated
{
    constexpr obfuscated(const char *src)
    {
        uint32_t key = KEY;
        for (size_t i = 0; i < N; ++i)
        {
            data_[i] = src[i] ^ static_cast<char>(key);
            key = (A * key + C) % M;
        }
    }
    void unmask(std::array<char, N> &dst) const
    {
        uint32_t key = KEY;
        auto d = dst.begin();
        for (const char *src = data_; src < data_ + N; ++src)
        {
            *d++ = *src ^ static_cast<char>(key);
            key = (A * key + C) % M;
        }
    }

private:
    // NOTE: std::array<char, N> would be nicer, but unfortunately
    // the std::array constructor isn't a constexpr.
    char data_[N];
};

#define OBFUSCATED_STR(key, str)                                 \
    []() -> auto                                                 \
    {                                                            \
        constexpr auto size = sizeof(str) / sizeof(str[0]);      \
        constexpr auto obfuscated_str =                          \
            obfuscated<key, size, 48271U, 0U, 2147483647U>(str); \
        static std::array<char, size> original_string;           \
        obfuscated_str.unmask(original_string);                  \
        return safe_string<size>(original_string.data());        \
    }                                                            \
    ()

int main(void)
{
    {
        auto o = OBFUSCATED_STR(OBFUSCATION_KEY, "Hallo, Welt!");
        std::cout << o << std::endl;
    }
    std::cin.get();
    return EXIT_SUCCESS;
}
