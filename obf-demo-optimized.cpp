#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif

#ifndef OBFUSCATION_KEY
// to silence the IDE's code checker
#define OBFUSCATION_KEY (0x13371337U)
#endif

class safe_string
{
private:
    char *str_;
    size_t size_;

public:
    safe_string()
        : str_(nullptr), size_(0U)
    {
    }
    safe_string(char *const str, size_t size)
        : str_(str), size_(size)
    {
    }
    safe_string(safe_string &&other)
        : str_(other.str_), size_(other.size_)
    {
        other.str_ = nullptr;
        other.size_ = 0U;
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
    void secure_erase_memory()
    {
#if defined(HAVE_SECUREZEROMEMORY)
        SecureZeroMemory(str_, size_);
#elif defined(HAVE_EXPLICIT_BZERO)
        explicit_bzero(str_, size_);
#elif defined(HAVE_MEMSET_S)
        memset_s(str_, size_, 0, size_);
#else
        volatile char *p = str_;
        size_t n = size_;
        while (n--) {
            *p++ = '\0';
        }
#endif
    };
    friend std::ostream &operator<<(std::ostream &, safe_string const &);
};

std::ostream &operator<<(std::ostream &os, safe_string const &s)
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
        for (auto i = 0; i < N; ++i)
        {
            data_[i] = src[i] ^ static_cast<char>(key);
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
    char data_[N];
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
    {
        std::cout << OBFUSCATED_STR(OBFUSCATION_KEY, "Hallo, Welt!");
    }
    std::cin.get();
    return EXIT_SUCCESS;
}
