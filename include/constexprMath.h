#ifndef _MATH_FOR_FIXED_H_
#define _MATH_FOR_FIXED_H_
#include "fixedpoint.h"
namespace fixedMath
{
	//template<typename T>
	//inline constexpr T log_for_short(T x) //it's work!!!!
	//{
	//	constexpr T two = T(2);
	//	constexpr T one = T(1);
	//	T one_div_two = T(0.5);
	//	const int iteration = 6;
	//	T k = one - two / (x + one);
	//	T p = k * k;
	//	T cur_p = one;
	//	T compare = two * k;
	//	T answer = compare;
	//	for (int i = 3, iter = 1; iter < iteration; i += 2, iter++)
	//		answer += (compare / T(i) * (cur_p *= p));
	//	return answer;
	//
	//}
	//
	//template<typename T>
	//inline constexpr T log(T x) //it's work!!!!
	//{
	//	//tailor series for log((n+1)/n) where x = (n+1)/n
	//	constexpr T two = T(2);
	//	constexpr T one = T(1);
	//	T one_div_two = T(0.5);
	//	T log_e_2 = log_for_short(two);
	//	int shifted = 0;
	//	while (x >= two) { x *= one_div_two; shifted++; }
	//	while (x <= one_div_two) { x *= two; shifted--; }
	//	const int iteration = 6;
	//	T k = one - two / (x + one);
	//	T p = k * k;
	//	T cur_p = one;
	//	T compare = two * k;
	//	T answer = compare;
	//	for (int i = 3, iter = 1; iter < iteration; i += 2, iter++)
	//		answer += (compare / T(i) * (cur_p *= p));
	//	return answer + log_e_2 * T(shifted);
	//
	//}
	//template<typename T>
	//inline constexpr T log2(T x) //it's work!!!!
	//{
	//	auto log_2 = log(T(2));
	//	return log(x) / log_2;
	//}


	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>>
	inline constexpr auto constexpr_abs(T const& x) noexcept
	{
		constexpr T zero(0);
		return zero < 0 ? -x : x;
	}

	inline double constexpr constexpr_pow(double base, int exponent) {
		if (exponent == 0)
			return 1;
		return base * constexpr_pow(base, exponent - 1);
	}

}



#endif // !_MATH_FOR_FIXED_H_
