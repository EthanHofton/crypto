#ifndef types_hpp
#define types_hpp

#include <iostream>
#include <vector>

namespace crypto
{
    class bigint
    {
    private:

        std::string digits;

    public:

        // Constructors
        bigint(unsigned long long n = 0);
        bigint(std::string s);
        bigint(bigint &);

        // Helper Functions:
        friend void divideBy2(bigint &a);
        friend bool null(const bigint &);
        friend int length(const bigint &);
        int operator[](const int)const;
        std::string value();

        // Direct assignment
        bigint &operator=(const bigint &);

        // Post/Pre - Incrementation
        bigint &operator++();
        bigint operator++(int temp);
        bigint &operator--();
        bigint operator--(int temp);

        // Addition and Subtraction
        friend bigint &operator+=(bigint &, const bigint &);
        friend bigint operator+(const bigint &, const bigint &);
        friend bigint operator-(const bigint &, const bigint &);
        friend bigint &operator-=(bigint &, const bigint &);

        // Comparison operators
        friend bool operator==(const bigint &, const bigint &);
        friend bool operator!=(const bigint &, const bigint &);
    
        friend bool operator>(const bigint &, const bigint &);
        friend bool operator>=(const bigint &, const bigint &);
        friend bool operator<(const bigint &, const bigint &);
        friend bool operator<=(const bigint &, const bigint &);

        // Multiplication and Division
        friend bigint &operator*=(bigint &, const bigint &);
        friend bigint operator*(const bigint &, const bigint &);
        friend bigint &operator/=(bigint &, const bigint &);
        friend bigint operator/(const bigint &, const bigint &);
    
        // Modulo
        friend bigint operator%(const bigint, const bigint);
        friend bigint &operator%=(bigint &, const bigint &);
    
        // Power Function
        friend bigint &operator^=(bigint &,const bigint &);
        friend bigint operator^(bigint &, const bigint &);

        // Square Root Function
        friend bigint sqrt(bigint &a);

        // min, max
        friend bigint min(bigint &a, bigint &b);
        friend bigint max(bigint &a, bigint &b);

        // toHex, hexToBigint
        friend std::string bigintToHex(bigint in);
        friend std::string bigintToHex(bigint in, int bytes);
        friend bigint hexToBigint(std::string hex);

        // Read and Write
        friend std::ostream &operator<<(std::ostream &,const bigint &);
        friend std::istream &operator>>(std::istream &, bigint &);

        // Others
        friend bigint factorial(int n);
        friend bigint map(bigint x, bigint a, bigint b, bigint c, bigint d);
    };

    bigint::bigint(unsigned long long nr)
    {
        do {
            digits.push_back(nr % 10);
            nr /= 10;
        } while (nr);
    }

    bigint::bigint(std::string s)
    {
        digits = "";
        int n = s.size();
        for (int i = n - 1; i >= 0;i--)
        {
            if (!isdigit(s[i]))
            {
                throw("ERROR");
            }
            digits.push_back(s[i] - '0');
        }
    }

    bigint::bigint(bigint &a)
    {
        digits = a.digits;
    }

    bool null(const bigint& a)
    {
        if (a.digits.size() == 1 && a.digits[0] == 0)
        {
            return true;
        }
        return false;
    }

    int length(const bigint & a)
    {
        return a.digits.size();
    }

    int bigint::operator[] (const int index) const
    {
        if (digits.size() <= index || index < 0)
        {
            throw("ERROR");
        }
        return digits[index];
    }

    std::string bigint::value()
    {
        std::string val;
        for (int i = 0; i < digits.size(); i++)
        {
            val.insert(0, std::to_string(digits[i]));
        }

        return val;
    }

    bool operator== (const bigint &a, const bigint &b)
    {
        return a.digits == b.digits;
    }

    bool operator!= (const bigint &a, const bigint &b)
    {
        return !(a == b);
    }

    bool operator< (const bigint &a, const bigint &b)
    {
        int n = length(a), m = length(b);
        if (n != m)
        {
            return n < m;
        }

        while(n--)
        {
            if (a.digits[n] != b.digits[n])
            {
                return a.digits[n] < b.digits[n];
            }
        }
        return false;
    }

    bool operator> (const bigint &a, const bigint &b)
    {
        return b < a;
    }

    bool operator>= (const bigint &a, const bigint &b)
    {
        return !(a < b);
    }

    bool operator<= (const bigint &a, const bigint &b)
    {
        return !(a > b);
    }

    bigint& bigint::operator= (const bigint &a)
    {
        digits = a.digits;
        return *this;
    }

    bigint &bigint::operator++()
    {
        int i, n = digits.size();
        for (i = 0; i < n && digits[i] == 9;i++)
        {
            digits[i] = 0;
        }

        if (i == n)
        {
            digits.push_back(1);
        } else {
            digits[i]++;
        }

        return *this;
    }

    bigint bigint::operator++(int temp)
    {
        bigint aux;
        aux = *this;
        ++(*this);
        return aux;
    }

    bigint &bigint::operator--()
    {
        if (digits[0] == 0 && digits.size() == 1)
        {
            throw("UNDERFLOW");
        }

        int i, n = digits.size();
        for (i = 0; digits[i] == 0 && i < n;i++)
        {
            digits[i] = 9;
        }

        digits[i]--;
        if (n > 1 && digits[n - 1] == 0)
        {
            digits.pop_back();
        }

        return *this;
    }

    bigint bigint::operator--(int temp)
    {
        bigint aux;
        aux = *this;
        --(*this);
        return aux;
    }

    bigint &operator+=(bigint &a,const bigint& b)
    {
        int t = 0, s, i;
        int n = length(a), m = length(b);
        if (m > n)
        {
            a.digits.append(m - n, 0);
        }
        n = length(a);
        for (i = 0; i < n;i++)
        {
            if (i < m)
            {
                s = (a.digits[i] + b.digits[i]) + t;
            } else {
                s = a.digits[i] + t;
            }
            t = s / 10;
            a.digits[i] = (s % 10);
        }

        if (t)
        {
            a.digits.push_back(t);
        }

        return a;
    }

    bigint operator+(const bigint &a, const bigint &b)
    {
        bigint temp;
        temp = a;
        temp += b;
        return temp;
    }

    bigint &operator-=(bigint &a, const bigint &b)
    {
        if (a < b)
        {
            throw("UNDERFLOW");
        }

        int n = length(a), m = length(b);
        int i, t = 0, s;
        for (i = 0; i < n;i++)
        {
            if (i < m)
            {
                s = a.digits[i] - b.digits[i]+ t;
            } else {
                s = a.digits[i]+ t;
            }

            if (s < 0)
            {
                s += 10,
                t = -1;
            } else {
                t = 0;
            }

            a.digits[i] = s;
        }

        while (n > 1 && a.digits[n - 1] == 0)
        {
            a.digits.pop_back(),
            n--;
        }

        return a;
    }

    bigint operator-(const bigint &a, const bigint &b)
    {
        bigint temp;
        temp = a;
        temp -= b;
        return temp;
    }

    bigint &operator*=(bigint &a, const bigint &b)
    {
        if (null(a) || null(b)){
            a = bigint();
            return a;
        }

        int n = a.digits.size(), m = b.digits.size();
        std::vector<int> v(n + m, 0);
        for (int i = 0; i < n;i++)
        {
            for (int j = 0; j < m;j++)
            {
                v[i + j] += (a.digits[i] ) * (b.digits[j]);
            }
        }
        n += m;
        a.digits.resize(v.size());
        for (int s, i = 0, t = 0; i < n; i++)
        {
            s = t + v[i];
            v[i] = s % 10;
            t = s / 10;
            a.digits[i] = v[i] ;
        }
        
        for (int i = n - 1; i >= 1 && !v[i];i--)
        {
            a.digits.pop_back();
        }

        return a;
    }

    bigint operator*(const bigint &a, const bigint &b)
    {
        bigint temp;
        temp = a;
        temp *= b;
        return temp;
    }

    bigint &operator/=(bigint &a, const bigint &b)
    {
        if (null(b))
        {
            throw("Arithmetic Error: Division By 0");
        }

        if (a < b)
        {
            a = bigint();
            return a;
        }

        if (a == b)
        {
            a = bigint(1);
            return a;
        }

        int i, lgcat = 0, cc;
        int n = length(a);
        std::vector<int> cat(n, 0);
        bigint t;
        for (i = n - 1; t * 10 + a.digits[i]  < b;i--)
        {
            t *= 10;
            t += a.digits[i];
        }

        for (; i >= 0; i--)
        {
            t = t * 10 + a.digits[i];
            for (cc = 9; cc * b > t;cc--);
            t -= cc * b;
            cat[lgcat++] = cc;
        }

        a.digits.resize(cat.size());
        for (i = 0; i < lgcat;i++)
        {
            a.digits[i] = cat[lgcat - i - 1];
        }
        a.digits.resize(lgcat);
        return a;
    }

    bigint operator/(const bigint &a,const bigint &b)
    {
        bigint temp;
        temp = a;
        temp /= b;
        return temp;
    }

    bigint &operator%=(bigint &a, const bigint &b)
    {
        if (null(b))
        {
            throw("Arithmetic Error: Division By 0");
        }

        if (a < b)
        {
            return a;
        }

        if (a == b)
        {
            a = bigint();
            return a;
        }

        int i, lgcat = 0, cc;
        int n = length(a);
        std::vector<int> cat(n, 0);
        bigint t;
        for (i = n - 1; t * 10 + a.digits[i] < b;i--)
        {
            t *= 10;
            t += a.digits[i];
        }

        for (; i >= 0; i--)
        {
            t = t * 10 + a.digits[i];
            for (cc = 9; cc * b > t;cc--);
            t -= cc * b;
            cat[lgcat++] = cc;
        }

        a = t;
        return a;
    }

    bigint operator%(const bigint a, bigint b)
    {
        bigint temp;
        temp = a;
        temp %= b;
        return temp;
    }

    bigint &operator^=(bigint &a, const bigint &b)
    {
        bigint Exponent, Base(a);
        Exponent = b;
        a = 1;

        while(!null(Exponent))
        {
            if (Exponent[0] & 1)
            {
                a *= Base;
            }

            Base *= Base;
            divideBy2(Exponent);
        }

        return a;
    }

    bigint operator^(bigint a, bigint b)
    {
        bigint temp(a);
        temp ^= b;
        return temp;
    }

    void divideBy2(bigint &a)
    {
        int add = 0;
        for (int i = a.digits.size() - 1; i >= 0;i--)
        {
            int digit = (a.digits[i] >> 1) + add;
            add = ((a.digits[i] & 1) * 5);
            a.digits[i] = digit;
        }

        while(a.digits.size() > 1 && !a.digits.back())
        {
            a.digits.pop_back();
        }
    }

    bigint sqrt(bigint & a)
    {
        bigint left(1), right(a), v(1), mid, prod;
        divideBy2(right);

        while(left <= right)
        {
            mid += left;
            mid += right;
            divideBy2(mid);
            prod = (mid * mid);
            if (prod <= a)
            {
                v = mid;
                ++mid;
                left = mid;
            } else{
                --mid;
                right = mid;
            }
            mid = bigint();
        }
        return v;
    }

    bigint min(bigint &a, bigint &b)
    {
        if (a < b)
        {
            return a;
        }

        return b;
    }

    bigint max(bigint &a, bigint &b)
    {
        if (a > b)
        {
            return a;
        }

        return b;
    }

    std::string bigintToHex(bigint in)
    {
        std::string hex;
        std::string digits = "0123456789ABCDEF";
        while (in != 0)
        {
            hex.insert(0, std::string(1, digits[(std::stoi((in % 16).value()))]));

            in = in / 16;
        }

        return hex;
    }

    std::string bigintToHex(bigint in, int bytes)
    {
        std::string hex = bigintToHex(in);
        std::string res = std::string(bytes * 2, '0');

        if (res.size() < hex.size())
        {
            throw("ERROR");
        }

        int j = res.size() - 1;
        for (int i = hex.size() - 1; i >= 0; i--)
        {
            res[j--] = hex[i];
        }

        return res;
    }

    bigint hexToBigint(std::string hex)
    {
        bigint num;
        for (int i = 0; i < hex.length(); i++)
        {
            num += std::stoi(std::string(1, hex[i]), nullptr, 16) * (bigint(16) ^ (hex.length() - i - 1));
        }

        return num;
    }

    bigint factorial(int n)
    {
        bigint f(1);
        for (int i = 2; i <= n;i++)
        {
            f *= i;
        }
        
        return f;
    }

    bigint map(bigint x, bigint a, bigint b, bigint c, bigint d)
    {
        return (x - a) * (d - c) / (b - a) + c;
    }

    std::istream &operator>>(std::istream &in, crypto::bigint &a)
    {
        std::string s;
        in >> s;
        int n = s.size();

        for (int i = n - 1; i >= 0;i--)
        {
            if (!isdigit(s[i]))
            {
                throw("INVALID NUMBER");
            }
            a.digits[n - i - 1] = s[i];
        }
        return in;
    }
    
    std::ostream &operator<<(std::ostream &out, const crypto::bigint &a)
    {
        for (int i = a.digits.size() - 1; i >= 0;i--)
        {
            out << (short)a.digits[i];
        }
        return out;
    }
}

#endif