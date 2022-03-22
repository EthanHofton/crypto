#ifndef crypto_hpp
#define crypto_hpp

#include <iostream>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <types.hpp>
#include <random.hpp>

namespace crypto
{
    // calculate gcd of bigint a, bigint b
    // using euclids algorithm
    bigint gcd(bigint a, bigint b)
    {
        if (a == b)
        {
            return a;
        }

        return gcd(max(a, b) - min(a, b), min(a, b));
    }

    // calculate lcm of bigint a, bigint b
    // uses lcm = |ab| / gcd(a,b)
    bigint lcm(bigint a, bigint b)
    {
        return (a * b) / gcd(a, b);
    }

    // power function
    // performs x^y % p
    bigint power(bigint x, bigint y, bigint p)
    {
        bigint res = 1;
        x = x % p;

        while (y > 0)
        {
            if (y[0] & 1)
            {
                res = (res*x) % p;
            }
    
            divideBy2(y);
            x = (x*x) % p;
        }

        return res;
    }

    // miller rabin prime test
    bool millerRabin(bigint n, int k, prng &rng)
    {
        if (n == 2)
        {
            return true;
        }

        if (n % 2 == 0)
        {
            return false;
        }

        bigint r, s;
        r = 0;
        s = n - 1;

        while (s % 2 == 0)
        {
            r += 1;
            s /= 2;
        }

        for (int i = 0; i < k; i++)
        {
            bigint a = rng.randbi(2, n - 1);
            bigint x = power(a, s, n);

            if (x == 1 || x == (n - 1))
            {
                continue;
            }

            bool found = true;
            for (bigint j = 0; j < r - 1; j++)
            {
                x = power(x, 2, n);
                if (x == (n - 1))
                {
                    found = false;
                    break;
                }
            }

            if (found)
            {
                return false;
            }
        }

        return true;
    }

    bool millerRabin2(bigint n, int k, prng &rng);
    // generates a prime number of k bits
    // first generate odd number of k bits
    // then add 2 until prime found
    bigint genPrime(int k, prng &rng)
    {
        bigint prime = rng.randbi(k);
        if (prime % 2 == 0)
        {
            prime++;
        }

        do
        {
            prime += 2;
        } while (!millerRabin2(prime, 20, rng));

        return prime;
    }

    // struct used to calculate modular inverse multplicati with unsinged bigints
    struct bigintAndSign
    {
        bigint value;
        bool isNegative;
    };

    // calcuates the modular inverse multiplicative
    bigint mul_inv(bigint a, bigint b)
    {
        if (b <= 1)
            return 0;

        bigint b0 = b;
        bigintAndSign x0 = { 0, false }; // b = 1*b + 0*a
        bigintAndSign x1 = { 1, false }; // a = 0*b + 1*a

        while (a > 1)
        {
            if (b == 0) // means original A and B were not co-prime so there is no answer
                return 0;
                
            bigint q = a / b;
            // (b, a) := (a % b, b)
            // which is the same as
            // (b, a) := (a - q * b, b)
            bigint t = b; b = a % b; a = t;

            // (x0, x1) := (x1 - q * x0, x0)
            bigintAndSign t2 = x0;
            bigint qx0 = q * x0.value;
            if (x0.isNegative != x1.isNegative)
            {
                x0.value = x1.value + qx0;
                x0.isNegative = x1.isNegative;
            }
            else
            {
                x0.value = (x1.value > qx0) ? x1.value - qx0 : qx0 - x1.value;
                x0.isNegative = (x1.value > qx0) ? x1.isNegative : !x0.isNegative;
            }
            x1 = t2;
        }

        return x1.isNegative ? (b0 - x1.value) : x1.value;
    }

    // concatinates together byte stings representing text
    std::string stringToHex(std::string in)
    {
        std::string stringHex;
        for (char c : in)
        {
            std::stringstream stream;
            stream << std::hex << (int)c;
            stringHex += std::string(stream.str());
        }

        return stringHex;
    }

    // converts string-hex byte string back to strings
    std::string hexToString(std::string in)
    {
        if (in.length() % 2 != 0)
        {
            return "";
        }

        std::string hexString;

        for (int i = 0; i < in.length(); i+= 2)
        {
            std::string hexVal = in.substr(i, 2);
            hexString += std::string(1, (char)std::stoi(hexVal, nullptr, 16));
        }
        return hexString;
    }

    // one-time pad cipher
    // generates a key the same size as the input string,
    // shifts the input string by a random number [0, 256] % 256
    // encrypt and decrtpy must use 2 seperate random number generator functions with the same seed
    std::string ethShiftEncrypt(std::string in, prng &rng)
    {
        int *key = new int[in.size()];
        for (int i = 0; i < in.size(); i++)
        {
            key[i] = rng.randi(0, 256);
        }

        std::string encrypted = in;
        for (int i = 0; i < encrypted.size(); i++)
        {
            encrypted[i] = (encrypted[i] + key[i]) % 256;
        }

        delete[] key;
        return encrypted;
    }

    // one-time pad cipher
    // generates a key the same size as the input string,
    // deshifts the input string by a random number [0, 256] % 256
    // encrypt and decrtpy must use 2 seperate random number generator functions with the same seed
    std::string ethShiftDecrypt(std::string in, prng &rng)
    {
        int *key = new int[in.size()];
        for (int i = 0; i < in.size(); i++)
        {
            key[i] = rng.randi(0, 256);
        }

        std::string decrypted = in;
        for (int i = 0; i < decrypted.size(); i++)
        {
            decrypted[i] = (decrypted[i] - key[i]) % 256;
        }

        delete[] key;
        return decrypted;
    }

    // stores n and e
    // the rsa public key
    // can freely be distrubuted
    struct rsaPublicKey
    {
        bigint n;
        bigint e;
    };

    // stores n and d
    // the rsa private key
    // must be kept secret
    struct rsaPrivateKey
    {
        bigint n;
        bigint d;
    };

    // rsa public key and private key combo
    struct rsaKeys
    {
        rsaPublicKey publicKey;
        rsaPrivateKey privateKey;
    };

    // generate rsa public-private key pair
    rsaKeys genKeys(int k, prng &rng)
    {
        bigint p, q, e;
        p = genPrime(k / 2, rng);
        q = genPrime(k - k / 2, rng);
        e = 65537;

        bigint n = p * q;

        bigint lambda = lcm(p-1, q-1);
        bigint d = mul_inv(e, lambda);

        rsaKeys key;
        key.publicKey.e = e;
        key.publicKey.n = n;
        key.privateKey.n = n;
        key.privateKey.d = d;

        return key;
    }

    // encrypts an integer ussing rsa pub key
    bigint encrypt(rsaPublicKey k, bigint m)
    {
        return power(m, k.e, k.n);
    }

    // decrypts an integer using rsa private key
    bigint decrypt(rsaPrivateKey k, bigint c)
    {
        return power(c, k.d, k.n);
    }

    // miller rabin faster (?)
    bool millerRabin2(bigint n, int k, prng &rng)
    {
        if (n == 2)
        {
            return true;
        }

        if (n % 2 == 0)
        {
            return false;
        }

        // find d
        bigint r = 0;
        bigint d;
        do
        {
            r++;
            d = (n - 1) / (bigint(2) ^ r);
        } while (n != (bigint(2) ^ r) * d + 1);

        for (int i = 0; i < k; i++)
        {
            bigint a = rng.randbi(2, n - 2);
            bigint x = power(a, d, n);

            if (x == 1 || x == n - 1)
            {
                continue;
            }

            bool cont = false;
            for (bigint j = 0; j < r - 1; j++)
            {
                x = power(x, 2, n);
                if (x == n - 1)
                {
                    cont = true;
                    break;
                }
            }

            if (cont)
            {
                continue;
            }

            return false;
        }

        return true;
    }

    // encryption using pkcs v1.5
    std::string pkcs1_5(rsaPublicKey k, std::string hex, prng &rng)
    {
        int k_abs = bigintToHex(k.n).size() / 2;
        int d = hex.size() / 2;
        std::string PK = bigintToHex(rng.randbi((k_abs - d - 3) * 8));
        std::string EB = std::string("00") + "02" + PK + "00" + hex;

        return EB;
    }

    // get key from random padded pkcs v1.5 byte string
    std::string pkcs1_5d(std::string pkcs)
    {
        std::string m;
        for (int i = pkcs.size() - 1; i > 1; i--)
        {
            if (pkcs[i] == '0' && pkcs[i - 1] == '0')
            {
                break;
            }

            m.insert(0, std::string(1, pkcs[i]));
        }

        return m;
    }

    // save rsa keys to filepath
    void saveKeys(rsaKeys &keys, std::string filename)
    {
        std::ofstream keyFile(filename, std::ios::out | std::ios::binary);

        if (!keyFile) 
        {
            throw std::runtime_error("could not open file");
        }

        keyFile.write((char*)&keys, sizeof(rsaKeys));
        keyFile.close();
    }

    // save rsa private key to filepath
    void savePrivateKey(rsaPrivateKey &pkey, std::string filename)
    {
        std::ofstream keyFile(filename, std::ios::out | std::ios::binary);

        if (!keyFile) 
        {
            throw std::runtime_error("could not open file");
        }

        keyFile.write((char*)&pkey, sizeof(rsaPrivateKey));
        keyFile.close();
    }

    // save rsa public key to filepath
    void savePublicKey(rsaPublicKey &pkey, std::string filename)
    {
        std::ofstream keyFile(filename, std::ios::out | std::ios::binary);

        if (!keyFile) 
        {
            throw std::runtime_error("could not open file");
        }

        keyFile.write((char*)&pkey, sizeof(rsaPublicKey));
        keyFile.close();
    }

    // load rsa keys from file
    rsaKeys loadKeys(std::string filename)
    {
        std::ifstream keyFile(filename, std::ios::out | std::ios::binary);
        if (!keyFile)
        {
            throw std::runtime_error("file not found error");
        }

        rsaKeys keys;
        keyFile.read((char*)&keys, sizeof(rsaKeys));

        keyFile.close();

        return keys;
    }

    // load rsa private key from file
    rsaPrivateKey loadPrivateKey(std::string filename)
    {
        std::ifstream keyFile(filename, std::ios::out | std::ios::binary);
        if (!keyFile)
        {
            throw std::runtime_error("file not found error");
        }

        rsaPrivateKey key;
        keyFile.read((char*)&key, sizeof(rsaPrivateKey));

        keyFile.close();

        return key;
    }

    // load rsa public key from file
    rsaPublicKey loadPublicKey(std::string filename)
    {
        std::ifstream keyFile(filename, std::ios::out | std::ios::binary);
        if (!keyFile)
        {
            throw std::runtime_error("file not found error");
        }

        rsaPublicKey key;
        keyFile.read((char*)&key, sizeof(rsaPublicKey));

        keyFile.close();

        return key;
    }
}

#endif