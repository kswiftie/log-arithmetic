#ifndef _BITSETEXTENDED_
#define _BITSETEXTENDED_
#include <bitset>
#include <iostream>


typedef unsigned long long ULL;
#define ULL_BITS ((int)(sizeof(ULL) * 8))


template<size_t N> constexpr ULL noexcept_to_ullong(const std::bitset<N>& _Right) noexcept {
    if constexpr ((int)N <= ULL_BITS) {
        return _Right;
    }
    else {
        return ((_Right << (N - ULL_BITS)) >> (N - ULL_BITS)).to_ullong();
    }
}

constexpr ULL noexcept_to_ullong(const ULL& _Right) noexcept {
    return _Right;
}


template<size_t N> 
constexpr int top_bit_set(const std::bitset<N>& _Base) noexcept {
    int i = N - 1;
    for (; i >= 0; i--)
        if (_Base.test(i)) break;
    return i; // return -1 if all zeros.
}

constexpr int top_bit_set(ULL _Number) {
    int i = 0;
    constexpr ULL helper = 1ULL << (ULL_BITS-1);
    for (i = (ULL_BITS - 1); i >= 0; i-=1) {
        if (_Number & helper) break;
        _Number <<= 1;
    }
    return i;
}

template<size_t N>
constexpr bool operator<(const std::bitset<N>& _Left, const std::bitset<N>& _Right) {
    for (int i = N - 1; i >= 0; i--) {
        if (_Left[i] ^ _Right[i]) return _Right[i];
    }
    return false;
}

template<size_t N>
constexpr bool operator>(const std::bitset<N>& _Left, const std::bitset<N>& _Right) {
    for (int i = N - 1; i >= 0; i--) {
        if (_Left[i] ^ _Right[i]) return _Left[i];
    }
    return false;
}

template<size_t N>
constexpr bool operator<=(const std::bitset<N>& _Left, const std::bitset<N>& _Right) {
    for (int i = N - 1; i >= 0; i--) {
        if (_Left[i] ^ _Right[i]) return _Right[i];
    }
    return true;
}

template<size_t N>
constexpr bool operator>=(const std::bitset<N>& _Left, const std::bitset<N>& _Right) {
    for (int i = N - 1; i >= 0; i--) {
        if (_Left[i] ^ _Right[i]) return _Left[i];
    }
    return true;
}




template <size_t N>
constexpr std::bitset<N> operator+(const std::bitset<N>& _Left, const std::bitset<N>& _Right) {
    std::bitset<N> result;
    char carry = 0;
    for (std::size_t i = 0; i < N; ++i) {
        result[i] = (_Left[i] ^ _Right[i]) ^ carry;
        carry = (_Left[i] & _Right[i]) | ((_Left[i] ^ _Right[i]) & carry);
    }
    return result;
}

template <size_t N>
constexpr std::bitset<N>& operator+=(std::bitset<N>& _Left, const std::bitset<N>& _Right) {
    _Left = _Left + _Right;
    return _Left;
}

template <size_t N>
constexpr std::bitset<N> operator-(const std::bitset<N>& _Left, const std::bitset<N>& _Right) {
    std::bitset<N> result(0);
    char borrow = 0;
    bool diff = 0;
    for (std::size_t i = 0; i < N; ++i) {
        if (borrow) {
            diff = !(_Left[i] ^ _Right[i]);
            borrow = !_Left[i] || (_Left[i] && _Right[i]);
        }
        else {
            diff = _Left[i] ^ _Right[i];
            borrow = !_Left[i] && _Right[i];
        }
        result[i] = diff;
    }
    return result;
}

template <size_t N>
constexpr std::bitset<N>& operator-=(std::bitset<N>& _Left, const std::bitset<N>& _Right) {
    _Left = _Left - _Right;
    return _Left;
}



template<size_t N>
constexpr std::bitset<N> operator/(const std::bitset<N>& _Left, const std::bitset<N>& _Right) {
    std::bitset<N> answer(0);
    std::bitset<N> current_dividend = _Left;
    int divisor_size = top_bit_set(_Right);
    if (divisor_size < 0) {
        answer.set();
        return answer;
    }
    int iter = top_bit_set(_Left) - divisor_size + 1;
    while ((--iter) >= 0) {
        if (!(current_dividend < (_Right << iter))) {
            current_dividend = current_dividend - (_Right << iter);
            answer.set(iter);
        }
    }
    return answer;
}

template<size_t N>
constexpr std::bitset<N> operator*(const std::bitset<N>& _Left, const std::bitset<N>& _Right) {
    std::bitset<N> answer(0);
    int _Right_size = top_bit_set(_Right);
    for (int iter = 0; iter <= _Right_size; iter++) {
        if (_Right[iter]) answer += (_Left << iter);
    }
    return answer;
}


template<size_t N>
constexpr std::bitset<N> mul_vith_shift_right(const std::bitset<N>& _Left, const std::bitset<N>& second, const int _Right) {
    std::bitset<N> answer(0);
    int _Right_size = top_bit_set(second);
    for (int iter = 0; iter <= _Right_size; iter++)
    {
        if (second[iter]){
            if (iter - _Right > 0)    answer += (_Left << (iter - _Right));
            else                      answer += (_Left >> (_Right - iter));
        }
    }
    return answer;
}

constexpr ULL mul_vith_shift_right(const ULL _Left, const ULL _Right, const int to_shift) {
    constexpr int halfULLBits = sizeof(ULL) * 4;
    ULL BEFORE_ONE = _Left >> halfULLBits; // get first 32 bits
    ULL AFTER__ONE = (_Left << halfULLBits) >> halfULLBits; // get second 32 bits
    ULL BEFORE_TWO = _Right >> halfULLBits;
    ULL AFTER__TWO = (_Right << halfULLBits) >> halfULLBits;

    ULL answer = 0;
    //undefined behavior if shift not in [0, sizeof(ULL)). It's very surprising.!!!
    if (to_shift > 0) {
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
    if (to_shift < ULL_BITS) {//undefined behavior predict.
        if (to_shift > 0)
            answer += (AFTER__ONE * AFTER__TWO) >> (to_shift);
        else answer += (AFTER__ONE * AFTER__TWO) << (-to_shift);
    }
    return answer;

}


template<size_t N>
constexpr std::bitset<N> div_vith_shift_left(const std::bitset<N>& _Left, const std::bitset<N>& _Right, const int to_shift) {
    std::bitset<N> answer(0);
    int top_Left_set = top_bit_set(_Left);
    int top_Right_set = top_bit_set(_Right);

    if (top_Right_set < 0) return answer.set();
    if (top_Left_set < 0) return answer;

    std::bitset<N> _Left_copy = ((_Left << ((int)N - top_Left_set - 1)) >> 1);
    std::bitset<N> _Right_copy = ((_Right << ((int)N - top_Right_set - 1)) >> 1);

    int iter = to_shift + top_Left_set - top_Right_set + 1;
    while ((--iter) >= 0) {
        if (_Left_copy >= _Right_copy) {
            _Left_copy -= _Right_copy;
            if (iter < (int)N)
                answer.set(iter);
        }
        _Left_copy <<= 1;
    }

    return answer;

}

constexpr ULL div_vith_shift_left(const ULL _Left, const ULL _Right, const int to_shift) {
    ULL answer(0);
    int top_Left_set = top_bit_set(_Left);
    int top_Right_set = top_bit_set(_Right);

    if (top_Right_set < 0) return ULLONG_MAX;
    if (top_Left_set < 0) return answer;

    ULL _Left_copy = ((_Left << (ULL_BITS - top_Left_set - 1)) >> 1);
    ULL _Right_copy = ((_Right << (ULL_BITS - top_Right_set - 1)) >> 1);
    
    int iter = to_shift + top_Left_set - top_Right_set + 1;
    while ((--iter) >= 0) {
        if (_Left_copy >= _Right_copy) {
            _Left_copy -= _Right_copy;
            if (iter < ULL_BITS)
                answer^= (ULL(1) << iter);
        }
        _Left_copy <<= 1;
    }

    return answer;

}




template<std::size_t N> //for bitset
constexpr bool to_bool(const std::bitset<N>& _Base) {
    return _Base.any();
}

template<typename T> //for integers
constexpr bool to_bool(T const& _Base) {
    return (bool)_Base;
}

#endif // !_BITSETEXTENDED_