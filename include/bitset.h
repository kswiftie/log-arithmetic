#pragma once
#ifndef _INCLUDE_BITSET_H_
#define _INCLUDE_BITSET_H_
#include <iostream>
#define SIMPLE_REALIZATION true
// .________________________.
// |						|
// |    BIT SET REALIZATION |
// |________________________|

#if SIMPLE_REALIZATION == false // switch this for simple realization

// --------------------(class BITSET simple)---------------
using BITSET = double;
// -----------------(end class BITSET simple)--------------



#else
// --------------------(class BITSET special)--------------
int DIGITS = 1024 * 32; //1024 // экспонента
#define EPS 20 // константа устанавливающая границы приближенности вычисления корня

using namespace std;
typedef signed int __int64_t;
//TODO: problem: if numbers is too big we should check it to not owerload. if it to small do the same check.
class BITSET {
protected:
    signed int x;

    inline BITSET PRIVATE_BITSET(signed int a) const{
        BITSET returnBITSET;
        returnBITSET.x = a;
        return returnBITSET;
    }
public:
    BITSET() {
        x = 0;
    }
    BITSET(const BITSET& other) = default;


    template <typename TYPE_FROM>
    BITSET(const TYPE_FROM& val){
        this->x = (signed int)(val * DIGITS);
    }


    template <typename TO_TYPE> requires (std::is_same<TO_TYPE, BITSET>::value == false) // not to use to init our class!
    operator TO_TYPE() const
    {
        return ((TO_TYPE)x) / DIGITS;
    }


    BITSET operator+(const BITSET& other) const{
        return PRIVATE_BITSET(this->x + other.x);
    }


    BITSET operator-(const BITSET& other) const{
        return PRIVATE_BITSET(this->x - other.x);
    }

    bool operator<(const BITSET& other) const
    {
        return this->x < other.x;
    }

    bool operator<=(const BITSET& other) const
    {
        return this->x <= other.x;
    }

    bool operator>=(const BITSET& other) const
    {
        return this->x >= other.x;
    }

    bool operator>(const BITSET& other) const
    {
        return this->x > other.x;
    }

    bool operator==(const BITSET& other) const
    {
        return this->x == other.x;
    }

    bool operator!=(const BITSET& other) const
    {
        return this->x != other.x;
    }

    BITSET& operator*=(const BITSET& other)
    {
        //TODO: it's not so good maybe, we can do it faster.
        *this = *this * other;
        return *this;
    }

    BITSET& operator/=(const BITSET& other)
    {
        //TODO: it's not so good maybe, we can do it faster.
        *this = *this / other;
        return *this;
    }

    BITSET& operator+=(const BITSET& other)
    {
        //TODO: it's not so good maybe, we can do it faster.
        this->x += other.x;
        return *this;
    }

    BITSET& operator-=(const BITSET& other)
    {
        //TODO: it's not so good maybe, we can do it faster.
        this->x -= other.x;
        return *this;
    }

    BITSET operator*(const BITSET& other) const{
        signed int c = this->x * other.x;
        if (c / other.x != this->x) { // may be we do not really need in it
            // Overflow!
            signed int i1 = this->x / DIGITS;
            signed int i2 = other.x / DIGITS;
            signed int f1 = (this->x & (DIGITS - 1));
            signed int f2 = (other.x & (DIGITS - 1));
            return PRIVATE_BITSET((i1 * i2) * DIGITS + (f1 * f2) / DIGITS + i1 * f2 + i2 * f1);
        }
        else {
            return PRIVATE_BITSET(c / DIGITS);
        }
    }

    BITSET operator/(const BITSET& other) const{
        if (this->x > (1 << 21)) {
            // Overflow!
            signed int i = this->x / DIGITS;
            signed int f = (this->x & (DIGITS - 1));
            return PRIVATE_BITSET(((i * DIGITS) / other.x) * DIGITS + (f * DIGITS) / other.x);
        }
        else {
            return PRIVATE_BITSET((this->x * DIGITS) / other.x);
        }
    }

    BITSET sqrt(BITSET k) {
        BITSET tmp = PRIVATE_BITSET(0);
        tmp.x = k.x / 2;
        signed int min = 0;
        signed int max = k.x;
        BITSET quick = PRIVATE_BITSET(0);
        do {
            tmp.x = (min + max) / 2;
            quick = tmp * tmp;
            if (abs(quick.x - k.x) < EPS) return BITSET(tmp);
            if (quick.x > k.x) {
                max = tmp.x;
            }
            else {
                min = tmp.x;
            }
        } while (true);
    }
};

std::ostream& operator<< (std::ostream& os, const BITSET& m) {
    os << (double)m;
    return os;
}
// -----------------(end class BITSET special)-------------

#endif // SIMPLE_REALIZATION

#endif // !_INCLUDE_BITSET_H_
