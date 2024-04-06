#include "include/lns.h"
#include "include/fixedpoint.h"
#include "include/bitsetExtended.h"
#include <complex>
#define _SILENCE_NONFLOATING_COMPLEX_DEPRECATION_WARNING
template<typename NUM_TYPE>
void test(NUM_TYPE a, NUM_TYPE b)
{
	std::cout << a << " + " << b << " = " << a + b << std::endl;
	std::cout << a << " - " << b << " = " << a - b << std::endl;
	std::cout << a << " / " << b << " = " << a / b << std::endl;
	std::cout << a << " * " << b << " = " << a * b << std::endl;
}


int test2()
{
	//using complexLns = std::complex<lns<>>;
	//FixedPoint aa = 5.555;
	//FixedPoint bb = 4;
	//test(aa, bb);
	//auto a = complexLns(1E5, 1E2);
	//auto b = complexLns(1E3, 1E4);
	//test(a, b);
	return 0;
}
int testForSystem()
{
	for (int i = -1E2; i < 1E2; i += 1)
		for (int j = +1E2; j > -1E2; j -= 1)
		{
			FixedPoint a;
			a = i;
			FixedPoint b;
			b = j;
			FixedPoint c;
			c = a;
			c += b;
			FixedPoint d;
			d = (i + j);
			if (c != d)
			{
				std::cout << "correct + :" << i << " + " << j << "=" << i+j << std::endl;
				std::cout << "wrong   + :" << a << " + " << b << "=" << c << std::endl;
				return 0;
			}
			if ((a < b) != (i < j))
			{
				std::cout << "correct < : " << i << " < " << j << "==" << (i < j) << std::endl;
				std::cout << "wrong   < : " << a << " < " << b << "==" << (a < b) << std::endl;
				return 0;
			}
			if ((a > b) != (i > j))
			{
				std::cout << "correct < : " << i << " > " << j << "==" << (i > j) << std::endl;
				std::cout << "wrong   > : " << a << " > " << b << "==" << (a > b) << std::endl;
				return 0;
			}
			if ((a >= b) != (i >= j))
			{
				std::cout << "correct <= : " << i << " >= " << j << "==" << (i >= j) << std::endl;
				std::cout << "wrong   >= : " << a << " >= " << b << "==" << (a >= b) << std::endl;
				return 0;
			}
			if ((a <= b) != (i <= j))
			{
				std::cout << "correct <= : " << i << " <= " << j << "==" << (i <= j) << std::endl;
				std::cout << "wrong   <= : " << a << " <= " << b << "==" << (a <= b) << std::endl;
				return 0;
			}
			if (+a != a)
			{
				std::cout << "correct +() : " << i << "=>" << +i << std::endl;
				std::cout << "wrong   +() : " << a << "=>" << +a << std::endl;
			}
			if (-a != FixedPoint(- i))
			{
				std::cout << "correct -() : " << i << "=>" << -i << std::endl;
				std::cout << "wrong   -() : " << a << "=>" << -a << std::endl;
			}
		}
	std::cout << "all tests are good" << std::endl;
	return 0;
}





int test3()
{
	testForSystem();

	FixedPoint<55, 22> a = -10.75;
	decltype(a) b = +537.75;
	std::cout << a << " - " << b << " = " << a - b << "\n";
	std::cout << ++a << " " << a++ << " " << a++ << std::endl;
	std::cout << a-- << " " << a-- << " " << a-- << std::endl;
	std::cout << a << std::endl;
	//std::cout << "log2(" << -a << ") = " << log2(-a) << std::endl;
	FixedPoint<100, 20> c = a;
	float four = c;
	std::cout << "four= " << (float)a << std::endl;
	std::cout << "four= " << four << std::endl;
	float three = -a;

	//std::cout << "(" << a << " / " << b << " = " << a / b << " )"<< std::endl;
	//std::cout << "((" << float(a) << ") / (" << float(b) << ") = (" << double(a) / double(b) << ") )" << std::endl;
	return 0;
}
int test4()
{
	testForSystem();
	FixedPoint<150, 100>i = 1.E17; //and herew bug!!!
	std::cout << (double)FixedPoint<1500, 2> {1.123123123E300} << std::endl; // workaet
	std::cout << (double)FixedPoint<100, 2> {1.E23} << std::endl;
	std::cout << (double)FixedPoint<100, 2> {1.E22} << std::endl;
	std::cout << (double)FixedPoint<100, 2> {1.E21} << std::endl;
	std::cout << (double)FixedPoint<100, 2> {1.E20} << std::endl;
	std::cout << (double)FixedPoint<100, 2> {1.E19} << std::endl;
	std::cout << (double)FixedPoint<100, 2> {1.E18} << std::endl;
	std::cout << (double)FixedPoint<100, 2> {1.E17} << std::endl;
	std::cout << (double)FixedPoint<100, 2> {1.E16} << std::endl;
	std::cout << (double)FixedPoint<100, 2> {1.E15} << std::endl;
	std::cout << (double)FixedPoint<100, 2> {1.E14} << std::endl;
	std::cout << (double)FixedPoint<100, 2> {1.E13} << std::endl;
	std::cout << (double)FixedPoint<100, 2> {1.E12} << std::endl;
	std::cout << (double)FixedPoint<100, 2> {1.E11} << std::endl;
	std::cout << (double)FixedPoint<100, 2> {1.E10} << std::endl;
	std::cout << (double)FixedPoint<100, 2> {1.E9} << std::endl;
	//FixedPoint<100, 100>i = 1.123123E10;
	FixedPoint<64, 0> j = i;//lost data becouse cutting
	FixedPoint<128, 15> k = i;
	std::cout << (double)i << " " << (double)j << " " << (double)k << std::endl;
	k = FixedPoint<10, 20>(11.33);

	std::cout << "Last: " << (FixedPoint<4, 30>(-10.65)) << std::endl;
	std::cout << "Last: " << (FixedPoint<4, 100>(-10.65)) << std::endl;
	return 0;
}

int complex_test()
{

	using comple = std::complex<FixedPoint<64,32>>;
	double i_real = 1.3;
	double i_image = 1.5;
	double j_real = 1.;
	double j_image = 2.;
	comple i1 = { i_real, i_image};
	comple j1 = { j_real, j_image};
	std::complex<double> i2 = { i_real, i_image };
	std::complex<double> j2 { j_real, j_image };
	test(i2, j2);
	test(i1, j1);
	return 0;
}

int main()
{
	//test4();
	auto f1 = 11.5234;
	auto f2 = 4.3234;
	FixedPoint<120, 50> i = f1;
	decltype(i) j = f2;
	std::cout << i << " * " << j << " = " << i * j << std::endl;
	FixedPoint<10, 55> i2 = f1;
	decltype(i) j2 = f2;
	std::cout << i2 << " * " << j2 << " = " << i2 * j2 << std::endl;
	std::cout << f1 << " * " << f2 << " = " << f1 * f2 << std::endl;
	//std::cout << i.get_base() << std::endl;
	//std::cout << j.get_base() << std::endl;
	//std::cout << (i * j).get_base() << std::endl;
	return 0;
}