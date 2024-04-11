#ifndef _BITSETEXTENDED_
#define _BITSETEXTENDED_
#include <bitset>
#include <iostream>

template<size_t N> constexpr unsigned long long noexcept_to_ullong(const std::bitset<N>& _Right) {
    if constexpr (N <= 64)
    {
        return _Right;
    }
    else
    {
        return ((_Right << (N - 64)) >> (N - 64)).to_ullong();
    }
}

constexpr unsigned long long noexcept_to_ullong(const unsigned long long& _Right) {
    return _Right;
}


template<size_t N> constexpr int top_bit_set(const std::bitset<N>& a) {
    int i = N - 1;
    for (; i >= 0; i--)
        if (a.test(i)) break;
    return i;
}

constexpr int top_bit_set(unsigned long long a) {
    int i = 0;
    constexpr auto helper = (unsigned long long)1 << 63;
    for (i = 63; i >= 0; i-=1) {
        if (a & helper) break;
        a = a << 1;
    }
    return i;
}




template<std::size_t N>
constexpr bool operator<(const std::bitset<N>& _base, const std::bitset<N>& y) {
    for (int i = N - 1; i >= 0; i--) {
        if (_base[i] ^ y[i]) return y[i];
    }
    return false;
}

template<std::size_t N>
constexpr bool operator>(const std::bitset<N>& _base, const std::bitset<N>& y) {
    for (int i = N - 1; i >= 0; i--) {
        if (_base[i] ^ y[i]) return _base[i];
    }
    return false;
}

template<std::size_t N>
constexpr bool operator<=(const std::bitset<N>& _base, const std::bitset<N>& y) {
    for (int i = N - 1; i >= 0; i--) {
        if (_base[i] ^ y[i]) return y[i];
    }
    return true;
}

template<std::size_t N>
constexpr bool operator>=(const std::bitset<N>& _base, const std::bitset<N>& y) {
    for (int i = N - 1; i >= 0; i--) {
        if (_base[i] ^ y[i]) return _base[i];
    }
    return true;
}




template <std::size_t N>
constexpr std::bitset<N> operator+(const std::bitset<N>& b1, const std::bitset<N>& b2) {
    std::bitset<N> result;
    char carry = 0;
    for (std::size_t i = 0; i < N; ++i) {
        result[i] = (b1[i] ^ b2[i]) ^ carry;
        carry = (b1[i] & b2[i]) | ((b1[i] ^ b2[i]) & carry);
    }
    return result;
}

template <std::size_t N>
constexpr std::bitset<N>& operator+=(std::bitset<N>& b1, const std::bitset<N>& b2) {
    b1 = b1 + b2;
    return b1;
}

template <std::size_t N>
constexpr std::bitset<N> operator-(const std::bitset<N>& b1, const std::bitset<N>& b2) {
    std::bitset<N> result(0);
    char borrow = 0;
    bool diff = 0;
    for (std::size_t i = 0; i < N; ++i) {
        if (borrow) {
            diff = !(b1[i] ^ b2[i]);
            borrow = !b1[i] || (b1[i] && b2[i]);
        }
        else {
            diff = b1[i] ^ b2[i];
            borrow = !b1[i] && b2[i];
        }
        result[i] = diff;
    }
    return result;
}

template <std::size_t N>
constexpr std::bitset<N>& operator-=(std::bitset<N>& b1, const std::bitset<N>& b2) {
    b1 = b1 - b2;
    return b1;
}


template<size_t N>
constexpr std::bitset<N> operator/(const std::bitset<N>& dividend, const std::bitset<N>& divisor) {
    std::bitset<N> answer(0);
    std::bitset<N> current_dividend = dividend;
    int divisor_size = top_bit_set(divisor);
    if (divisor_size < 0)
    {
        answer.set();
        return answer;
    }
    int iter = top_bit_set(dividend) - divisor_size + 1;
    
    while ((--iter) >= 0)
    {
        if (!(current_dividend < (divisor << iter)))
        {
            current_dividend = current_dividend - (divisor << iter);
            answer.set(iter);
        }
    }
    return answer;
}

template<size_t N>
constexpr std::bitset<N> operator*(const std::bitset<N>& first, const std::bitset<N>& second) {
    std::bitset<N> answer(0);
    int second_size = top_bit_set(second);
    for (int iter = 0; iter <= second_size; iter++)
    {
        if (second[iter]) answer += (first << iter);
    }
    return answer;
}

template<size_t N>
constexpr std::bitset<N> mul_vith_shift_right(const std::bitset<N>& first, const std::bitset<N>& second, const int to_shift) {
    std::bitset<N> answer(0);
    int second_size = top_bit_set(second);
    for (int iter = 0; iter <= second_size; iter++)
    {
        if (second[iter]){
            if (iter - to_shift > 0)    answer += (first << (iter - to_shift));
            else                        answer += (first >> (to_shift - iter));
        }
    }
    return answer;
}

constexpr unsigned long long mul_vith_shift_right(const unsigned long long left, const unsigned long long right, const int to_shift) {
    using ULL = unsigned long long;
    constexpr int halfULLBits = sizeof(ULL) * 4;
    ULL BEFORE_ONE = left >> halfULLBits; // get first 32 bits
    ULL AFTER__ONE = (left << halfULLBits) >> halfULLBits; // get second 32 bits
    ULL BEFORE_TWO = right >> halfULLBits;
    ULL AFTER__TWO = (right << halfULLBits) >> halfULLBits;

    ULL answer = 0;
    if (to_shift > 0) //undefined behavior if shift not in [0, sizeof(ULL)). It's very surprising.!!!
    {
        if (int shift = halfULLBits * 2 - to_shift; shift > 0)
            answer += (BEFORE_ONE * BEFORE_TWO) << (shift);
        else answer += (BEFORE_ONE * BEFORE_TWO) >> (-shift);
    }
    if (int shift = halfULLBits - to_shift; shift > 0)
        answer += (BEFORE_ONE * AFTER__TWO) << (shift);
    else answer += (BEFORE_ONE * AFTER__TWO) >> (-shift);

    if (int shift = halfULLBits - to_shift; shift > 0)
        answer += (AFTER__ONE * BEFORE_TWO) << (shift);
    else answer += (AFTER__ONE * BEFORE_TWO) >> (-shift);
    if (to_shift < 64) //undefined behavior predict.
    {
        if (to_shift > 0)
            answer += (AFTER__ONE * AFTER__TWO) >> (to_shift);
        else answer += (AFTER__ONE * AFTER__TWO) << (-to_shift);
    }
    return answer;

}




template<std::size_t N> //for bitset
constexpr bool to_bool(const std::bitset<N>& _base) {
    return _base.any();
}

template<typename T> //for integers
constexpr bool to_bool(T const& b) {
    return (bool)b;
}

#endif // !_BITSETEXTENDED_
