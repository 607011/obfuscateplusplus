#ifndef __OBF_DEMO_OPTIMIZED_HPP__
#define __OBF_DEMO_OPTIMIZED_HPP__

#include <array>
#include <cstdlib>

#ifdef _WIN32
#include <Windows.h>
#else
#include <cstring>
#endif

template <size_t N>
class safe_string final : public std::array<char, N>
{
public:
    safe_string() = default;
    ~safe_string()
    {
        secure_erase_memory();
    }

private:
    void secure_erase_memory()
    {
#if defined(HAVE_SECUREZEROMEMORY)
        SecureZeroMemory(this->data(), this->size());
#elif defined(HAVE_EXPLICIT_BZERO)
        explicit_bzero(this->data(), this->size());
#elif defined(HAVE_MEMSET_S)
        memset_s(this->data(), this->size(), 0, this->size());
#else
        volatile char *p = this->data();
        size_t n = this->size();
        while (n--)
        {
            *p++ = '\0';
        }
#endif
    };

    template <size_t LENGTH>
    friend std::ostream &operator<<(std::ostream &, safe_string<LENGTH> const &);
};

template <size_t LENGTH>
std::ostream &operator<<(std::ostream &os, safe_string<LENGTH> const &ss)
{
    os << ss.data();
    return os;
}

template <uint32_t KEY, size_t N, uint32_t A, uint32_t C, uint32_t M>
struct obfuscated final
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
    void unmask(safe_string<N> &dst) const
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
    char data_[N];
};

#define OBFUSCATED_STR(key, str)                                 \
    []() -> auto                                                 \
    {                                                            \
        constexpr auto size = sizeof(str) / sizeof(str[0]);      \
        constexpr auto obfuscated_str =                          \
            obfuscated<key, size, 48271U, 0U, 2147483647U>(str); \
        static safe_string<size> original_string;                \
        obfuscated_str.unmask(original_string);                  \
        return original_string;                                  \
    }                                                            \
    ()


#endif // __OBF_DEMO_OPTIMIZED_HPP__
