#ifndef __DEOBFUSCATOR_HPP__
#define __DEOBFUSCATOR_HPP__

#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>

namespace obfuscated
{
    template <uint32_t KEY, size_t N, uint32_t A, uint32_t C, uint32_t M>
    struct data
    {
        inline char const *c_str() const { return reinterpret_cast<char const *>(data_); }
        inline std::string string() const { return c_str(); }
        inline std::string operator()() const { return data_; }
        constexpr size_t size() const { return N; }
        data(const unsigned char *src)
        {
            uint32_t key = KEY;
            for (size_t i = 0; i < N; ++i)
            {
                data_[i] = src[i] ^ static_cast<unsigned char>(key);
                key = (A * key + C) % M;
            }
        }
        ~data()
        {
            secure_erase_memory();
        }

    private:
        unsigned char data_[N];

        inline void secure_erase_memory()
        {
#if HAVE_EXPLICIT_BZERO
            explicit_bzero(data_, N);
#elif HAVE_MEMSET_S
            memset_s(data_, N, 0, N);
#elif HAVE_MEMSET
            memset(data_, N, 0);
#else
#pragma warning "secure_erase_memory() will not work because there's no function to zero memory"
#endif
        }
    };
}

#endif // __DEOBFUSCATOR_HPP__
