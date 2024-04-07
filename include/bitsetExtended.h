#ifndef _BITSETEXTENDED_
#define _BITSETEXTENDED_
#include <bitset>
#include <iostream>
template<size_t N> int top_bit_set(const std::bitset<N>& a) {
    int i;
    for (i = N - 1; i >= 0; i--)
        if (a.test(i)) break;
    return i;
}

template <std::size_t N>
std::bitset<N> operator+(const std::bitset<N>& b1, const std::bitset<N>& b2) {
    std::bitset<N> result;
    char carry = 0;
    for (std::size_t i = 0; i < N; ++i) {
        // упрощенное от (b1 | b2 | carry) & (!carry | (b1 & b2 & carry))
        result[i] = (b1[i] ^ b2[i]) ^ carry;
        carry = (b1[i] & b2[i]) | ((b1[i] ^ b2[i]) & carry);
    }
    //if (carry) {
        //result.set();
      // тут еще можно на overflow проверить
      //std::cout << "Warning: Operation overflow." << std::endl;
    //}
    return result;
}

template <std::size_t N>
std::bitset<N>& operator+=(std::bitset<N>& b1, const std::bitset<N>& b2) {
    b1 = b1 + b2;
    return b1; //todo: chenge it to faster
}

template <std::size_t N>
std::bitset<N>& operator-=(std::bitset<N>& b1, const std::bitset<N>& b2) {
    b1 = b1 - b2;
    return b1; //todo: chenge it to faster
}

template <std::size_t N>
std::bitset<N> operator-(const std::bitset<N>& b1, const std::bitset<N>& b2) {
    std::bitset<N> result;
    char borrow = 0;
    for (std::size_t i = 0; i < N; ++i) {
        // упрощенное от (b1 | b2 | carry) & (!carry | (b1 & b2 & carry))
        bool diff;
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
    //if (carry) {
        //result.set();
      // тут еще можно на overflow проверить
      //std::cout << "Warning: Operation overflow." << std::endl;
    //}
    return result;
}

template<size_t N>
std::bitset<N> operator/(const std::bitset<N>& dividend, const std::bitset<N>& divisor) {
    std::bitset<N> answer(0);
    std::bitset<N> current_dividend = dividend;
    int divisor_size = top_bit_set(divisor);
    if (divisor_size < 0) return answer;
    int iter = top_bit_set(dividend) - divisor_size + 1;
    while (--iter >= 0)
    {
        if (current_dividend > (divisor << iter))
        {
            current_dividend -= (divisor << iter);
            answer.set(iter);
        }
            
    }
    return answer;
}

template<size_t N>
std::bitset<N> operator*(const std::bitset<N>& first, const std::bitset<N>& second) {
    std::bitset<N> answer(0);
    int second_size = top_bit_set(second);
    for (int iter = 0; iter <= second_size; iter++)
    {
        if (second[iter]) answer += (first << iter);
    }
    return answer;
}

template<size_t N>
inline std::bitset<N> mul_vith_shift_right(const std::bitset<N>& first, const std::bitset<N>& second, int to_shift) {
    std::bitset<N> answer(0);
    for (int iter = 0, second_size = top_bit_set(second); iter <= second_size; iter++){
        if (second[iter]){
            if (iter - to_shift > 0)    answer += (first << (iter - to_shift));
            else                        answer += (first >> (to_shift - iter));
        }
    }
    return answer;
}

inline unsigned long long mul_vith_shift_right(const unsigned long long left, const unsigned long long right, int to_shift)
{
    using ULL = unsigned long long;
    using uint = unsigned int;
    constexpr int uintPerULL = sizeof(ULL) / sizeof(uint);
    constexpr int halfULLBits = sizeof(ULL) * 4;
    static_assert(sizeof(ULL) == sizeof(uint) * 2, "(ull) != (uint|uint) - BAD PLATFORM REALIZATION");//todo:: add constexpr if maybe

    ULL BEFORE_ONE = left >> halfULLBits; // get first 32 bits
    ULL AFTER__ONE = (left << halfULLBits) >> halfULLBits; // get second 32 bits
    ULL BEFORE_TWO = right >> halfULLBits;
    ULL AFTER__TWO = (right << halfULLBits) >> halfULLBits;

    ULL answer = 0;
    if (int shift = halfULLBits * 2 - to_shift; shift > 0)
         answer += (BEFORE_ONE * BEFORE_TWO) << (shift);
    else answer += (BEFORE_ONE * BEFORE_TWO) >> (-shift);

    if (int shift = halfULLBits - to_shift; shift > 0)
         answer += (BEFORE_ONE * AFTER__TWO) << (shift);
    else answer += (BEFORE_ONE * AFTER__TWO) >> (-shift);

    if (int shift = halfULLBits - to_shift; shift > 0)
         answer += (AFTER__ONE * BEFORE_TWO) << (shift);
    else answer += (AFTER__ONE * BEFORE_TWO) >> (-shift);

    if (to_shift > 0)
         answer += (AFTER__ONE * AFTER__TWO) >> (to_shift);
    else answer += (AFTER__ONE * AFTER__TWO) << (-to_shift);
    return answer;

}

template<std::size_t N>
bool operator<(const std::bitset<N>& _base, const std::bitset<N>& y)
{
    for (int i = N - 1; i >= 0; i--) {
        if (_base[i] ^ y[i]) return y[i];
    }
    return false;
}

template<std::size_t N>
bool operator>(const std::bitset<N>& _base, const std::bitset<N>& y)
{
    for (int i = N - 1; i >= 0; i--) {
        if (_base[i] ^ y[i]) return _base[i];
    }
    return false;
}

template<std::size_t N> //for bitset
bool to_bool(const std::bitset<N>& _base)
{
    return _base.any();

}

template<typename T> //for integers
bool to_bool(T const& b)
{
    return (bool)b;
}

#endif // !_BITSETEXTENDED_
