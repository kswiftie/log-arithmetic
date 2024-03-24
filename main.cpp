#include "include/lns.h"
#include "include/bitset.h"
#include <complex>
int main()
{
	using complexLns = std::complex<lns<>>;

	auto a = complexLns(1E3, 1E1);
	auto b = complexLns(1E1, 1E2);
	std::cout << a << " + " << b << " = " << a + b << std::endl;
	std::cout << a << " - " << b << " = " << a - b << std::endl;
	std::cout << a << " / " << b << " = " << a / b << std::endl;
	std::cout << a << " * " << b << " = " << a * b << std::endl;
	//BITSET aaa = 1.;
	//BITSET bbb = 25;
	//std::cout << (aaa / bbb / bbb / bbb) << std::endl;

	return 0;
}
