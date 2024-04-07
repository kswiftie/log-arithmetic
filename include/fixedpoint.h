#pragma once
#ifndef _INCLUDE_BITSET_H_
#define _INCLUDE_BITSET_H_

//-----------------(variant for test(different realisation))-------------
#define SIMPLE_REALIZATION false //---------------------------------------
//-----------------(variant for test(different realisation))-------------





// .________________________.
// |						|
// |    BIT SET REALIZATION |
// |________________________|





#if SIMPLE_REALIZATION // switch this for simple realization

// --------------------(class FixedPoint simple)---------------
using FixedPoint = double;
// -----------------(end class FixedPoint simple)--------------



#else

// --------------------(class FixedPoint special)--------------
#include <iostream>
#include <bitset>
#include <type_traits>
#include "bitsetExtended.h"
#include <string>
#include <limits.h>
#include <assert.h>

using namespace std;


double constexpr constexpr_pow(double base, int exponent) {
    if (exponent == 0)
        return 1;
    return base * constexpr_pow(base, exponent - 1);
}



#define DECIMAL_BIT_NUMBERS     32
#define INT_BIT_NUMBERS         32
#define OUTPUT_BY_BITS          false

typedef unsigned long long ULL;
#define ULL_BITS ((int)(sizeof(ULL) * 8))


template<std::size_t BEFORE_DECIMAL_BITS = INT_BIT_NUMBERS, std::size_t AFTER_DECIMAL_BITS = DECIMAL_BIT_NUMBERS>
class FixedPoint {
protected:
    constexpr static int _howBits = BEFORE_DECIMAL_BITS + AFTER_DECIMAL_BITS;
public:
    //bitset is so slow, so if you want to use big nombers, you'll pay "twice".
    typedef std::conditional<(_howBits <= ULL_BITS), ULL, bitset<_howBits>>::type BASE_TYPE;
protected:
    BASE_TYPE _base = 0;

    bool _isPositive = true;



public:
    FixedPoint MakeFixed(BASE_TYPE base, bool get_isPositive) const{
        FixedPoint returnBITSET;
        returnBITSET._base = base;
        returnBITSET._isPositive = get_isPositive;
        return returnBITSET;
    }

    bool        get_isPositive()    const {return _isPositive;}
    const auto& get_base()       const {return _base;}

    //--init operators--
    FixedPoint() = default;

    constexpr FixedPoint(const FixedPoint& other) = default;

    template <typename TYPE_FROM>  requires (std::is_integral_v<TYPE_FROM> || std::is_floating_point_v<TYPE_FROM>)
    constexpr FixedPoint(const TYPE_FROM& val){
        if constexpr (std::is_integral_v<TYPE_FROM>)
        {
            if (val < 0)
            {
                this->_base = -val;
                _isPositive = false;
            }
            else
            {
                this->_base = +val;
                _isPositive = true;
            }
            this->_base <<= AFTER_DECIMAL_BITS;
        }
        else if constexpr (std::is_floating_point_v<TYPE_FROM>)
        {
            if (val != 0) //already init if a = 0
            {
                int toPow = ULL_BITS - log2(abs(val))-1;
                double firstPow = std::pow(2., toPow);
                double helper = abs(val * firstPow);
                ULL helper2 = helper;
                this->_base = helper2;
                if ((int)AFTER_DECIMAL_BITS - toPow > 0)
                    this->_base = this->_base << (AFTER_DECIMAL_BITS - toPow);
                else
                    this->_base = this->_base >> (toPow - AFTER_DECIMAL_BITS);
                if (val < 0)
                    _isPositive = false;
                else
                    _isPositive = true;
            }
        } 
    }

    template<std::size_t BEFORE, std::size_t DECIMAL> requires (BEFORE != BEFORE_DECIMAL_BITS || DECIMAL != AFTER_DECIMAL_BITS)
    constexpr FixedPoint(const FixedPoint<BEFORE, DECIMAL>& other)
    {
        this->_isPositive = other.get_isPositive();
        const int difference_decimal = AFTER_DECIMAL_BITS - DECIMAL;
        const int difference_before = BEFORE_DECIMAL_BITS - BEFORE;
        const unsigned int start_in_old = (difference_decimal > 0 ? 0 : -difference_decimal);
        const unsigned int start_in_new = (difference_decimal > 0 ? difference_decimal : 0);
        const int how_to_set = (difference_decimal > 0 ? DECIMAL : AFTER_DECIMAL_BITS) + (difference_before > 0 ? BEFORE : BEFORE_DECIMAL_BITS);
        if constexpr (!is_integral<BASE_TYPE>::value) {
            if constexpr (!is_integral<FixedPoint<BEFORE, DECIMAL>::BASE_TYPE>::value){
                for (int i = 0, iter_in_old = start_in_old, iter_in_new = start_in_new; i < how_to_set; i++, iter_in_old++, iter_in_new++)
                    this->_base.set(iter_in_new, other.get_base()[iter_in_old]);
            }
            else{
                bitset<ULL_BITS> helper_other = other.get_base();
                for (int i = 0, iter_in_old = start_in_old, iter_in_new = start_in_new; i < how_to_set; i++, iter_in_old++, iter_in_new++)
                    this->_base.set(iter_in_new, helper_other[iter_in_old]);
            }
        }
        else{
            bitset<ULL_BITS> helper_base;
            if constexpr (!is_integral<FixedPoint<BEFORE, DECIMAL>::BASE_TYPE>::value){
                for (int i = 0, iter_in_old = start_in_old, iter_in_new = start_in_new; i < how_to_set; i++, iter_in_old++, iter_in_new++)
                    helper_base.set(iter_in_new, other.get_base()[iter_in_old]);
            }
            else{
                bitset<ULL_BITS> helper_other = other.get_base();
                for (int i = 0, iter_in_old = start_in_old, iter_in_new = start_in_new; i < how_to_set; i++, iter_in_old++, iter_in_new++)
                    helper_base.set(iter_in_new, helper_other[iter_in_old]);
            }
            this->_base = helper_base.to_ullong();
        }
    }

    //--arithmetic operators--
    FixedPoint operator+(const FixedPoint& other) const
    { //is done
        if (this->_isPositive == other._isPositive)
            return this->MakeFixed(this->_base + other._base, this->_isPositive);
        else if (this->_base > other._base)
            return this->MakeFixed(this->_base - other._base, this->_isPositive);
        else
            return this->MakeFixed(other._base - this->_base, other._isPositive);
    }

    FixedPoint operator-(const FixedPoint& other) const 
    { //is done
        if (this->_isPositive != other._isPositive)
            return this->MakeFixed(this->_base + other._base, this->_isPositive);
        else if (this->_base > other._base)
            return this->MakeFixed(this->_base - other._base, this->_isPositive);
        else
            return this->MakeFixed(other._base - this->_base, not this->_isPositive);
    }

    FixedPoint& operator+=(const FixedPoint& other)
    {
        if (this->_isPositive == other._isPositive)
        {
            this->_base += other._base;
        }
        else if (this->_base > other._base)
        {
            this->_base -= other._base;
        }
        else
        {
            this->_base = other._base - this->_base;
            this->_isPositive = other._isPositive;
        }
        return *this;
    }

    FixedPoint& operator-=(const FixedPoint& other)
    {
        if (this->_isPositive != other._isPositive)
        {
            this->_base += other._base;
        }
        else if (this->_base > other._base)
        {
            this->_base -= other._base;
        }
        else
        {
            this->_base = other._base - this->_base;
            this->_isPositive = not this->_isPositive;
        }
        return *this;
    }

    FixedPoint operator*(const FixedPoint& other) const
    { 
        return MakeFixed(mul_vith_shift_right(this->get_base(), other.get_base(), AFTER_DECIMAL_BITS), this->_isPositive == other._isPositive);
    }

    FixedPoint operator/(const FixedPoint& other) const
    {
        bool new_isPositive = this->_isPositive == other._isPositive;
        if constexpr (std::is_integral_v<BASE_TYPE>) // checking for zero!!
        {
            if (not (bool)other._base)
                return this->MakeFixed(ULLONG_MAX, new_isPositive); //return maximum number base!
        }
        else
        {
            if (other._base.none())
                return this->MakeFixed(BASE_TYPE().set(), new_isPositive); //max base!
        }
        using extended = FixedPoint<_howBits, AFTER_DECIMAL_BITS>;
        auto newBase = (extended(*this).get_base() << AFTER_DECIMAL_BITS) / (extended(other).get_base());
        return (decltype(*this))(extended().MakeFixed(newBase, new_isPositive));
    }

    FixedPoint& operator*=(const FixedPoint& other)
    {
        this->_base = mul_vith_shift_right(this->get_base(), other.get_base(), AFTER_DECIMAL_BITS);
        this->_isPositive = (this->_isPositive == other._isPositive);
        return *this;
    }

    FixedPoint& operator/=(const FixedPoint& other)
    {
        *this = *this / other; //it's not easy way to make it faster. so let it here.
        return *this;
    }

    //--compare operators--
    bool operator==(const FixedPoint& other) const
    {
        if (this->_base == other._base)
        {
            if (this->_isPositive == other._isPositive) return true;
            if (not to_bool(this->_base)) return true; // -00000000,000000 == +00000000,000000 -> true
        }
        return false;
    }

    bool operator!=(const FixedPoint& other) const
    {
        if (this->_base == other._base)
        {
            if (this->_isPositive == other._isPositive) return false;
            if (not to_bool(this->_base)) return false; // -00000000,000000 != +00000000,000000 -> false
        }
        return true;
    }

    bool operator<(const FixedPoint & other) const
    {
        FixedPoint diff = other  - *this;
        if (diff._isPositive and to_bool(diff._base))
            return true;
        return false;
    }

    bool operator>(const FixedPoint& other) const
    {
        FixedPoint diff = *this - other;
        if (diff._isPositive and to_bool(diff._base))
            return true;
        return false;
    }

    bool operator>=(const FixedPoint& other) const
    {
        FixedPoint diff = *this - other;
        if (diff._isPositive or (not to_bool(diff._base)))
            return true;
        return false;
    }

    bool operator<=(const FixedPoint& other) const
    {
        FixedPoint diff = other - *this;
        if (diff._isPositive or (not to_bool(diff._base)))
            return true;
        return false;
    }

    //--unary operators--
    FixedPoint operator+() const
    {
        return *this;
    }

    FixedPoint operator-() const
    {
        FixedPoint newFixed = *this;
        newFixed._isPositive = not newFixed._isPositive;
        return newFixed;
    }

    FixedPoint& operator++() //prefix
    {
        constexpr auto ONE = FixedPoint(1);
        return *this += ONE;
    }

    FixedPoint operator++(int) //postfix
    {
        constexpr auto ONE = FixedPoint(1);
        FixedPoint returnObject = *this;
        *this += ONE;
        return returnObject;
    }

    FixedPoint& operator--() //prefix
    {
        constexpr auto ONE = FixedPoint(1);
        return *this -= ONE;
    }

    FixedPoint operator--(int) //postfix
    {
        constexpr auto ONE = FixedPoint(1);
        FixedPoint returnObject = *this;
        *this -= ONE;
        return returnObject;
    }

    //--to type operators--
    template<typename TO_TYPE> requires(std::is_integral_v<TO_TYPE>)
    operator TO_TYPE() const
    {
        if constexpr (std::is_integral_v<BASE_TYPE>)
        {
            if (_isPositive)
                return (_base >> AFTER_DECIMAL_BITS);
            return -(_base >> AFTER_DECIMAL_BITS);
        }
        else
        {
            // __V_____we use this if because if there are any set before 64 bits, then to_ullong() -> error.
            if constexpr (BEFORE_DECIMAL_BITS < ULL_BITS) 
            {
                if (_isPositive)
                    return (_base >> AFTER_DECIMAL_BITS).to_ullong();
                return -(_base >> AFTER_DECIMAL_BITS).to_ullong();
            }
            else
            {//cutting off the hiest bits for predict error.
                if (_isPositive) 
                    return (_base << (BEFORE_DECIMAL_BITS - ULL_BITS) >> (_howBits - ULL_BITS)).to_ullong();
                return -(_base << (BEFORE_DECIMAL_BITS - ULL_BITS) >> (_howBits - ULL_BITS)).to_ullong();
            }//actually if TO_TYPE is signed then return value can be less then 0. as i understand it's okay.
        }
    }

    template<typename TO_TYPE> requires(std::is_floating_point_v<TO_TYPE>)
    operator TO_TYPE() const
    {
        constexpr double to_int_mul = 1. / constexpr_pow(2., AFTER_DECIMAL_BITS);
        if constexpr (std::is_integral_v<BASE_TYPE>)
        {
            if (_isPositive)
                return TO_TYPE(_base) * to_int_mul;
            return -TO_TYPE(_base) * to_int_mul;
        }
        else
        {
            int iter = 0;
            while (_base[_howBits - 1 - iter] == 0 and iter < _howBits - 1) iter++;
            if (_base[_howBits - 1 - iter] == 0) return 0;

            int to_shift = 0;
            int may_shift = (_howBits - iter - (int)(sizeof(ULL) * 8));
            if (may_shift > 0)
                to_shift = may_shift;
            TO_TYPE number = (TO_TYPE((_base >> to_shift).to_ullong())) * std::pow(2., to_shift) * to_int_mul;
            if (_isPositive)
                return number;
            return -number;
        }
    }

    operator std::string() const
    {
        constexpr double to_int_mul = 1. / constexpr_pow(2., AFTER_DECIMAL_BITS);
        if constexpr (std::is_integral_v<BASE_TYPE>)
        { // if it's is int
            return (_isPositive ? ""s : "-"s) + std::to_string(_base * to_int_mul);
        }
        else
        {
            return std::to_string(double(*this)); //TODO:::: it's bad because double is not enough for seeing all bits!!!!
        }
    }
};
template<int BEFORE, int DECIMAL>
FixedPoint<BEFORE, DECIMAL> log2(const FixedPoint<BEFORE, DECIMAL>& other)
{
    //one of the most important function for accuracy!!
    //i don't know how to do it because has no realisation for int base.
    return log2((double)other); // big lost the data!!! fixed -> double -> log(double) -> double -> fixed -> return.
}


template<int BEFORE, int DECIMAL>
std::ostream& operator<< (std::ostream& os, const FixedPoint<BEFORE, DECIMAL>& m){
    return os << std::string(m);
}

//-----------------(end class FixedPoint special)-------------

#endif // SIMPLE_REALIZATION

#endif // !_INCLUDE_BITSET_H_
//------------DONE------------
//done: add <
//done: add > 
//done: add <= 
//done: add >=
//done: add ==     
//done: add !=     
//done: add +      
//done: add -      
//done: add +simple
//done: add -simple
//done: add ++()
//done: add ()++
//done: add --()
//done: add ()--
//done: add float/double -> bitset<three, fore>
//done: add += -- add realization += to bitset and change it!!!
//done: add -= -- add realization += to bitset and change it!!!
//done: add operator int() -(TO_TYPE)
//done: add operator double() -(TO_TYPE)
//done: think about better way double -> fixed!!!!
//done: let avay to_ullong - becouse of it's overflow :(
//done: add bitset<one, two> -> bitset<three, fore>
//done: double -> fixed - make better, becouse long long not so good for wery big double like 1E300 - if we will use fixed<1000, 1000>, it's can save it.
//todo: add * and / for bitset base!!!!
//done: add / for bitset
//done: add * for bitset
//done: add *
//done: add /
//done: add *= make better
//done: problem with mul of decimal parts!!
//done: solve crashes: (for (operator int()) it's crashes sometimes)

//------------TO DO NOW------------
//todo: have a problem: if decimal part is too big then it's crashes for (fixed point <- double)
//todo: solwe bug when after decimal bits more then 500

//------------TO DO------------
//todo: add log2
//todo: add pow
//todo: add operator%
//todo: add operator%=
//todo: add realization bitset->big integers string - it's too boring but shell to do!!!


//----------------DO NOT necessary------------
// different answer in end of the decimal part for different realisation(but it's too small actually)
//todo: add /= make better
//todo: rebuild realization bitset += bitset and -= for faster.