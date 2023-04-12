#ifndef __DEOBFUSCATOR_HPP__
#define __DEOBFUSCATOR_HPP__

#include <cstdlib>
#include <cstring>
#include <string>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace obfuscated
{
    using std::string;

    template <uint32_t KEY, size_t N, uint32_t A, uint32_t C, uint32_t M>
    class data final
    {
    public:
        char const *c_str() const { return data_; }
        inline std::string string() const { return c_str(); }
        inline std::string operator()() const { return data_; }
        constexpr size_t size() const { return N; }
        data(const unsigned char *const src)
        {
            uint32_t key = KEY;
            for (size_t i = 0; i < N; ++i)
            {
                data_[i] = src[i] ^ static_cast<char>(key);
                key = (A * key + C) % M;
            }
        }
        ~data()
        {
            secure_erase_memory();
        }

    private:
        char data_[N];

        void secure_erase_memory()
        {
#if defined(HAVE_SECUREZEROMEMORY)
            SecureZeroMemory(data_, N);
#elif defined(HAVE_EXPLICIT_BZERO)
            explicit_bzero(data_, N);
#elif defined(HAVE_MEMSET_S)
            memset_s(data_, N, 0, N);
#else
            volatile unsigned char *p = data_;
            size_t n = N;
            while (n--)
            {
                *p++ = 0;
            }
#endif
        }
    };
}

#endif // __DEOBFUSCATOR_HPP__
