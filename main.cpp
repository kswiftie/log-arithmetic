#include "include/lns.h"
#include <complex>
int main()
{
	using complexLns = std::complex<lns<>>;

	auto a = complexLns(1000000, 0);
	auto b = complexLns(0, 10000000);
	std::cout << a << " + " << b << " = " << a + b << std::endl;
	std::cout << a << " - " << b << " = " << a - b << std::endl;
	std::cout << a << " / " << b << " = " << a / b << std::endl;
	std::cout << a << " * " << b << " = " << a * b << std::endl;
	return 0;
}
