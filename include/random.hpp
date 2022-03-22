#ifndef random_hpp
#define random_hpp

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <random>
#include <unistd.h>
#include <types.hpp>

namespace crypto
{
    class prng
    {
    public:

        prng(std::string t_seed = "")
        {
            std::random_device rd;
            m_generator = std::mt19937(rd());

            m_seed = (t_seed == "") ? std::to_string(getpid() * time(NULL)) : t_seed;

            std::seed_seq seed(m_seed.begin(), m_seed.end());

            m_generator.seed(seed);
        }

        float randf(float t_lower = 0.0, float t_upper = 1.0)
        {
            std::uniform_real_distribution<> dist(t_lower, t_upper);
            return dist(m_generator);
        }

        int randi(int t_lower = 0, int t_upper = 1)
        {
            std::uniform_int_distribution<> dist(t_lower, t_upper);
            return dist(m_generator);
        }

        bigint randbi(int bits)
        {
            bool bitarray[bits];

            for (int i = 0; i < bits; i++)
            {
                bitarray[i] = randi(0, 1);
            }

            bigint num;
            for (int i = 0; i < bits; i++)
            {
                num += bigint((unsigned long long)bitarray[i]) * (bigint(2ULL) ^ bigint((unsigned long long)i));
            }

            return num;
        }

        bigint randbi(bigint lower, bigint upper)
        {
            bigint a = randbi(16);
            return map(a, 0, (bigint(2) ^ 16) - 1, lower, upper);
        }

    private:

        std::string m_seed;
        std::mt19937 m_generator;
    };
}

#endif