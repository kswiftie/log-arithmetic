#ifndef _MATH_FOR_FIXED_H_
#define _MATH_FOR_FIXED_H_
#include "fixedpoint.h"
namespace fixedMath
{

	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>>
	inline constexpr auto constexpr_abs(T const& x) noexcept
	{
		constexpr T zero(0);
		return zero < 0 ? -x : x;
	}

	inline constexpr double constexpr_pow(double base, int exponent) {
		if (exponent == 0)
			return 1;
		return base * constexpr_pow(base, exponent - 1);
	}

}



#endif // !_MATH_FOR_FIXED_H_