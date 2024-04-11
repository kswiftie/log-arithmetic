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
#include "constexprMath.h"

using namespace std;



constexpr size_t how_sign_for_exp_m(size_t after_bits)
{
    int i = after_bits;
    double k = 3;
    size_t n = 1;
    while (i > 0)
    {
        k *= n;
        n++;
        while (k > 2.)
        {
            k /= 2;
            i--;
        }
    }
    return n;
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

    constexpr FixedPoint MakeFixed(BASE_TYPE base, bool get_isPositive) const {
        FixedPoint returnBITSET;
        returnBITSET._base = base;
        returnBITSET._isPositive = get_isPositive;
        return returnBITSET;
    }

    constexpr bool        get_isPositive()    const { return _isPositive; }
    constexpr const auto& get_base()       const { return _base; }

    //--init operators--
    FixedPoint() = default;

    constexpr FixedPoint(const FixedPoint& other) = default;

    template <typename TYPE_FROM>  requires (std::is_integral_v<TYPE_FROM> || std::is_floating_point_v<TYPE_FROM>)
    constexpr FixedPoint(const TYPE_FROM& val) {
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
                int toPow = ULL_BITS - log2(abs(val)) - 1;//problem: these log2 and abs are not constexpr.
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
    constexpr FixedPoint(const FixedPoint<BEFORE, DECIMAL>& other) {
        this->_isPositive = other.get_isPositive();
        const int difference_decimal = AFTER_DECIMAL_BITS - DECIMAL;
        const int difference_before = BEFORE_DECIMAL_BITS - BEFORE;
        const unsigned int start_in_old = (difference_decimal > 0 ? 0 : -difference_decimal);
        const unsigned int start_in_new = (difference_decimal > 0 ? difference_decimal : 0);
        const int how_to_set = (difference_decimal > 0 ? DECIMAL : AFTER_DECIMAL_BITS) + (difference_before > 0 ? BEFORE : BEFORE_DECIMAL_BITS);
        if constexpr (!is_integral<BASE_TYPE>::value) {
            if constexpr (!is_integral<FixedPoint<BEFORE, DECIMAL>::BASE_TYPE>::value) {
                for (int i = 0, iter_in_old = start_in_old, iter_in_new = start_in_new; i < how_to_set; i++, iter_in_old++, iter_in_new++)
                    this->_base.set(iter_in_new, other.get_base()[iter_in_old]);
            }
            else {
                bitset<ULL_BITS> helper_other = other.get_base();
                for (int i = 0, iter_in_old = start_in_old, iter_in_new = start_in_new; i < how_to_set; i++, iter_in_old++, iter_in_new++)
                    this->_base.set(iter_in_new, helper_other[iter_in_old]);
            }
        }
        else {
            bitset<ULL_BITS> helper_base;
            if constexpr (!is_integral<FixedPoint<BEFORE, DECIMAL>::BASE_TYPE>::value) {
                for (int i = 0, iter_in_old = start_in_old, iter_in_new = start_in_new; i < how_to_set; i++, iter_in_old++, iter_in_new++)
                    helper_base.set(iter_in_new, other.get_base()[iter_in_old]);
            }
            else {
                bitset<ULL_BITS> helper_other = other.get_base();
                for (int i = 0, iter_in_old = start_in_old, iter_in_new = start_in_new; i < how_to_set; i++, iter_in_old++, iter_in_new++)
                    helper_base.set(iter_in_new, helper_other[iter_in_old]);
            }
            this->_base = helper_base.to_ullong();
        }
    }

    //--arithmetic operators--
    constexpr FixedPoint operator+(const FixedPoint& other) const {
        if (this->_isPositive == other._isPositive)
            return this->MakeFixed(this->_base + other._base, this->_isPositive);
        else if (this->_base > other._base)
            return this->MakeFixed(this->_base - other._base, this->_isPositive);
        else
            return this->MakeFixed(other._base - this->_base, other._isPositive);
    }

    constexpr FixedPoint operator-(const FixedPoint& other) const {
        if (this->_isPositive != other._isPositive)
            return this->MakeFixed(this->_base + other._base, this->_isPositive);
        else if (this->_base > other._base)
            return this->MakeFixed(this->_base - other._base, this->_isPositive);
        else
            return this->MakeFixed(other._base - this->_base, not this->_isPositive);
    }

    constexpr FixedPoint& operator+=(const FixedPoint& other) {
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

    constexpr FixedPoint& operator-=(const FixedPoint& other) {
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

    constexpr FixedPoint operator*(const FixedPoint& other) const {
        return MakeFixed(mul_vith_shift_right(this->_base, other._base, AFTER_DECIMAL_BITS), this->_isPositive == other._isPositive);
    }

    constexpr FixedPoint operator/(const FixedPoint& other) const {
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
        return extended().MakeFixed(newBase, new_isPositive);
        //using extended = FixedPoint<_howBits, AFTER_DECIMAL_BITS>;
        //auto newBase = ((*this).get_base() << AFTER_DECIMAL_BITS) / (other.get_base());
        //return MakeFixed(newBase, new_isPositive);
    }

    constexpr FixedPoint& operator*=(const FixedPoint& other) {
        this->_base = mul_vith_shift_right(this->get_base(), other.get_base(), AFTER_DECIMAL_BITS);
        this->_isPositive = (this->_isPositive == other._isPositive);
        return *this;
    }

    constexpr FixedPoint& operator/=(const FixedPoint& other) {
        *this = *this / other;
        return *this;
    }

    constexpr FixedPoint operator<< (const size_t To_shift) const {
        return MakeFixed(this->_base << To_shift, this->_isPositive);
    }

    constexpr FixedPoint operator>> (const size_t To_shift) const {
        return MakeFixed(this->_base >> To_shift, this->_isPositive);
    }

    constexpr FixedPoint& operator<<= (const size_t To_shift) {
        this->_base <<= To_shift;
        return *this;
    }

    constexpr FixedPoint& operator>>= (const size_t To_shift) {
        this->_base >>= To_shift;
        return *this;
    }

    //--compare operators--
    constexpr bool operator==(const FixedPoint& other) const {
        if (this->_base == other._base){
            if (this->_isPositive == other._isPositive) return true;
            if (not to_bool(this->_base)) return true; // -00000000,000000 == +00000000,000000 -> true
        }
        return false;
    }

    constexpr bool operator!=(const FixedPoint& other) const {
        if (this->_base == other._base) {
            if (this->_isPositive == other._isPositive) return false;
            if (not to_bool(this->_base)) return false; // -00000000,000000 != +00000000,000000 -> false
        }
        return true;
    }

    constexpr bool operator<(const FixedPoint& other) const {
        FixedPoint diff = other - *this;
        if (diff._isPositive and to_bool(diff._base))
            return true;
        return false;
    }

    constexpr bool operator>(const FixedPoint& other) const {
        FixedPoint diff = *this - other;
        if (diff._isPositive and to_bool(diff._base))
            return true;
        return false;
    }

    constexpr bool operator>=(const FixedPoint& other) const {
        FixedPoint diff = *this - other;
        if (diff._isPositive or (not to_bool(diff._base)))
            return true;
        return false;
    }

    constexpr bool operator<=(const FixedPoint& other) const {
        FixedPoint diff = other - *this;
        if (diff._isPositive or (not to_bool(diff._base)))
            return true;
        return false;
    }

    //--unary operators--
    constexpr FixedPoint operator+() const {
        return *this;
    }

    constexpr FixedPoint operator-() const {
        FixedPoint newFixed = *this;
        newFixed._isPositive = not newFixed._isPositive;
        return newFixed;
    }

    constexpr FixedPoint& operator++() {
        constexpr auto ONE = FixedPoint(1);
        return *this += ONE;
    } //prefix

    constexpr FixedPoint operator++(int) {
        constexpr auto ONE = FixedPoint(1);
        FixedPoint returnObject = *this;
        *this += ONE;
        return returnObject;
    } //postfix

    constexpr FixedPoint& operator--() {
        constexpr auto ONE = FixedPoint(1);
        return *this -= ONE;
    } //prefix

    constexpr FixedPoint operator--(int) {
        constexpr auto ONE = FixedPoint(1);
        FixedPoint returnObject = *this;
        *this -= ONE;
        return returnObject;
    } //postfix 

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
        constexpr double to_int_mul = 1. / fixedMath::constexpr_pow(2., AFTER_DECIMAL_BITS);
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
        constexpr double to_int_mul = 1. / fixedMath::constexpr_pow(2., AFTER_DECIMAL_BITS);
        if constexpr (std::is_integral_v<BASE_TYPE>)
        { // if it's is int
            return (_isPositive ? ""s : "-"s) + std::to_string(_base * to_int_mul);
        }
        else
        {
            return std::to_string(double(*this)); //TODO:::: it's bad because double is not enough for seeing all bits!!!!
        }
    }

    //--special functions--

    constexpr FixedPoint log_for_short_m() const
    {
        using F = FixedPoint<BEFORE_DECIMAL_BITS, AFTER_DECIMAL_BITS>;
        constexpr auto two = F(2);
        constexpr auto one = F(1);
        constexpr F one_div_two = (one >> size_t(1));
        constexpr int iteration = AFTER_DECIMAL_BITS / 3; // around 3.4 bit of accuracy per one number
        F k = one - two / (*this + one);
        F p = k * k;
        F cur_p = one;
        F compare = two * k;
        F answer = compare;
        for (int i = 3, iter = 1; iter < iteration; i += 2, iter++)
            answer += (compare / F(i) * (cur_p *= p));
        return answer;

    }

    constexpr FixedPoint log_m() const
    {
        //tailor series for log((n+1)/n) where (*this) = (n+1)/n
        using F = FixedPoint<BEFORE_DECIMAL_BITS, AFTER_DECIMAL_BITS>;
        constexpr F two = F(2);
        constexpr F one = F(1);
        constexpr F one_div_two = (one >> size_t(1));
        constexpr int iteration = AFTER_DECIMAL_BITS / 3;

        F log_2 = two.log_for_short_m();
        if (*this <= 0) return *this; // without exception
        int shifted = 0;
        F to_solvePart = *this;




        while (to_solvePart >= two) { to_solvePart >>= size_t(1); shifted++; }//todo: change it!!!!! it's cost a lot!!!! we just need to shift the base! but we make new fixed many times!
        while (to_solvePart <= one_div_two) { to_solvePart <<= size_t(1); shifted--; }//todo: change it!!!!!
        F k = one - two / (to_solvePart + one);
        F p = k * k;
        F cur_p = one;
        F compare = two * k;
        F answer = compare;
        for (int i = 3, iter = 1; iter < iteration; i += 2, iter++)
            answer += (compare / F(i) * (cur_p *= p));
        return answer + log_2 * F(shifted);

    }

    constexpr FixedPoint exp_for_short_m() const {
        using F = FixedPoint<BEFORE_DECIMAL_BITS, AFTER_DECIMAL_BITS>;
        constexpr size_t accuracy = how_sign_for_exp_m(AFTER_DECIMAL_BITS);
        constexpr F one = 1;
        //todo: add constexpr arr of elements : 1, 1/2, 1/3, 1/4 and so on
        F answer = 1;
        for (size_t i = accuracy - 1; i > 0; i--)
        {
            answer *= ((*this) / F(i));
            answer += one;
        }

        return answer;
    }

    constexpr FixedPoint exp_m() const {
        using F = FixedPoint<BEFORE_DECIMAL_BITS, AFTER_DECIMAL_BITS>;
        constexpr F one = F(1);
        F to_solvePart = *this;
        int top_bit_seted = top_bit_set(_base);
        if (top_bit_seted == -1) return one;
        int shifted = top_bit_seted - (int)AFTER_DECIMAL_BITS;
        if (shifted > 0) {
            to_solvePart >>= shifted;
        }
        else {
            shifted = 0;
        }
        //while (to_solvePart.get_base() >= two.get_base()) { to_solvePart *= one_div_two; shifted++; }
        F answer = to_solvePart.exp_for_short_m();
        if (shifted > 0) {
            for (int i = 0; i < shifted; i++) {
                answer = answer * answer;
            }
        }
        return answer;
    }

    friend constexpr FixedPoint log2(const FixedPoint& other) {
        using extended = FixedPoint<BEFORE_DECIMAL_BITS, AFTER_DECIMAL_BITS + BEFORE_DECIMAL_BITS + 4>;

        extended log_2 = extended(2).log_m();//todo: make it constexpr
        return ((extended)other).log_m() / log_2;
        //return other.log_m() / FixedPoint(2).log_m();
    }

    friend constexpr FixedPoint log(const FixedPoint& other) {
        using extended = FixedPoint<BEFORE_DECIMAL_BITS, AFTER_DECIMAL_BITS + BEFORE_DECIMAL_BITS + 4>; //when we add numbers we can lost sum of some last bits
        return ((extended)other).log_m();
        //return other.log_m();
    }

    friend constexpr FixedPoint exp(const FixedPoint& other) {
        return other.exp_m();
    }

    friend constexpr FixedPoint pow(const FixedPoint& base, const FixedPoint& degree) {
        return exp(log(base) * degree);
    }

    friend std::ostream& operator<< (std::ostream& os, const FixedPoint& m) {
        return os << std::string(m);
    }
};
//-----------------(end class FixedPoint special)-------------

#endif // SIMPLE_REALIZATION

#endif // !_INCLUDE_BITSET_H_


//------------TO DO NOW------------
//todo: solwe bug when after decimal bits more then 500(problem with double)
//todo: add constexpr for floating point
//todo: chenge mull by 2 and 0.5 in the log in the fixed
//todo: add exp for negative numbers.

//------------TO DO------------
//todo: add operator%
//todo: add operator%=
//todo: add realization bitset->big integers string - it's too boring but shell to do!!!



//----------------DO NOT necessary------------
// different answer in end of the decimal part for different realisation(but it's too small actually)
//todo: add /= make better
//todo: rebuild realization bitset += bitset and -= for faster.