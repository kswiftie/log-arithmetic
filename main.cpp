#include "include/lns.h"
#include "include/bitset.h"
#include <complex>
template<typename NUM_TYPE>
void test(NUM_TYPE a, NUM_TYPE b)
{
	std::cout << a << " + " << b << " = " << a + b << std::endl;
	std::cout << a << " - " << b << " = " << a - b << std::endl;
	std::cout << a << " / " << b << " = " << a / b << std::endl;
	std::cout << a << " * " << b << " = " << a * b << std::endl;
}
int main()
{
	using complexLns = std::complex<lns<>>;
	BITSET aa = 5.555;
	BITSET bb = 4.444;
	test(aa, bb);

	auto a = complexLns(1E5, 1E2);
	auto b = complexLns(1E3, 1E4);
	test(a, b);
	return 0;
}
