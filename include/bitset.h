#pragma once
#ifndef _INCLUDE_BITSET_H_
#define _INCLUDE_BITSET_H_
#include <iostream>


//-----------------(variant for test(different realisation))-------------
#define SIMPLE_REALIZATION false //---------------------------------------
//-----------------(variant for test(different realisation))-------------





// .________________________.
// |						|
// |    BIT SET REALIZATION |
// |________________________|





#if SIMPLE_REALIZATION // switch this for simple realization

// --------------------(class BITSET simple)---------------
using BITSET = double;
// -----------------(end class BITSET simple)--------------



#else
// --------------------(class BITSET special)--------------
using namespace std;
typedef int64_t BITSET_BAZE;

const int AFTER_DECIMAL_BIT_NUMBERS = 32;

constexpr BITSET_BAZE GLOB_DIGITS = BITSET_BAZE(1) << AFTER_DECIMAL_BIT_NUMBERS; // экспонента
#define EPS 20  //is the constant for finding squere root where |real_root - root|*GLOB_DIGITS < EPS 

//TODO: problem: if numbers is too big we should check it to not owerload. if it to small do the same check.(when it out of bounds!!!.


template<typename BAZE_TYPE = BITSET_BAZE, BITSET_BAZE DIGITS = GLOB_DIGITS>
class BITSET {
protected:
    BAZE_TYPE x;

    inline BITSET PRIVATE_BITSET(BAZE_TYPE a) const{
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
        this->x = (BAZE_TYPE)(val * DIGITS);
    }


    template <typename TO_TYPE> requires (std::is_same<TO_TYPE, BITSET>::value == false) // not to use to init our class!
    operator TO_TYPE() const
    { // may be not so good wariant sometimes it is so bad.
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
        BITSET_BAZE c = this->x * other.x;
        if (c / other.x != this->x) { // may be we do not really need in it
            // Overflow!
            BAZE_TYPE i1 = this->x / DIGITS;
            BAZE_TYPE i2 = other.x / DIGITS;
            BAZE_TYPE f1 = (this->x & (DIGITS - 1));
            BAZE_TYPE f2 = (other.x & (DIGITS - 1));
            return PRIVATE_BITSET((i1 * i2) * DIGITS + (f1 * f2) / DIGITS + i1 * f2 + i2 * f1);
        }
        else {
            return PRIVATE_BITSET(c / DIGITS);
        }
    }

    BITSET operator/(const BITSET& other) const{
        if (this->x > (1 << 21)) {
            // Overflow!
            BAZE_TYPE i = this->x / DIGITS;
            BAZE_TYPE f = (this->x & (DIGITS - 1));
            return PRIVATE_BITSET(((i * DIGITS) / other.x) * DIGITS + (f * DIGITS) / other.x);
        }
        else {
            return PRIVATE_BITSET((this->x * DIGITS) / other.x);
        }
    }

    BITSET sqrt(BITSET k) {
        BITSET tmp = PRIVATE_BITSET(0);
        tmp.x = k.x / 2;
        BAZE_TYPE min = 0;
        BAZE_TYPE max = k.x;
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
template<typename BAZE_TYPE>
std::ostream& operator<< (std::ostream& os, const BITSET<BAZE_TYPE>& m) {
    
    os << (double)m;
    return os;
}
// -----------------(end class BITSET special)-------------

#endif // SIMPLE_REALIZATION

#endif // !_INCLUDE_BITSET_H_
