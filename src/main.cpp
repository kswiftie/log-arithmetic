#include "..\\include\\lns.h"
#include "..\\include\\fixedpoint.h"
#include "..\\include\\bitsetExtended.h"
#include <complex>
#include <random>

#define _SILENCE_NONFLOATING_COMPLEX_DEPRECATION_WARNING
template<typename NUM_TYPE>

void test(NUM_TYPE a, NUM_TYPE b) {
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

	using comple = std::complex<FixedPoint<31,32>>;
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

int test5()
{
	test4();
	complex_test();
	auto f1 = 11.5234;
	auto f2 = 0;
	FixedPoint<60, 40> i = f1;
	decltype(i) j = f2;
	std::cout << i << " / " << j << " = " << i / j << std::endl;
	FixedPoint<20, 40> i2 = f1;
	decltype(i) j2 = f2;
	std::cout << i2 << " / " << j2 << " = " << i2 / j2 << std::endl;
	std::cout << f1 << " * " << f2 << " = " << f1 * f2 << std::endl;
	std::cout << i2 << " *= " << j2 << " => " << (i2*=j2) << std::endl;


	FixedPoint<500, 500> test_for_to_ULL = 123;
	std::cout << "test_in_base: " << test_for_to_ULL << std::endl;
	std::cout << "test_to_int: " << test_for_to_ULL << std::endl;
	//std::cout << i.get_base() << std::endl;
	//std::cout << j.get_base() << std::endl;
	//std::cout << (i * j).get_base() << std::endl;
	return 0;
}

int test6()
{
	test5();
	constexpr int t = 10;
	constexpr FixedPoint<4, 32> num { t };
	//constexpr auto ans = log2(num);
	FixedPoint ans1 = num / num;
	constexpr bitset<64> p1 = 300;
	constexpr bitset<64> p2 = 500;
	//constexpr bitset<64> v1 = p1 / p2;
	constexpr auto ans333 = top_bit_set(p1);
	auto answerrrr = log(num);
	//std::cout << v1.to_ullong() << std::endl;
	std::cout << answerrrr << " " << log(t) << std::endl;
	double last = 1;
	using F = FixedPoint<64, 64>;
	std::cout << "exp_test: " << pow((F)2.532, (F)1.234) << std::endl;
	std::cout << "exp_test: " << pow(2.532, 1.234) << std::endl;

	std::cout << "exp_test: " << pow((F)2, log2((F)75.234)) << std::endl;//it's work!!!!!!!!!!!!

	std::cout << "exp_test: " << exp(1.234) << std::endl;
	//for (int i = 0; i < 100; i++)
	//{
	//	std::cout << how_sign_for(0, i) << std::endl;
	//}
	//for (int i = 4; i < 1000; i+=2)
	//{
	//	double dif = log(t) - log_e(t, i);
	//	std::cout << "elem: " << (i-3)/2 + 2 << " result: " << log(t) - log_e(t, i)  << "better x times: " << last / dif << std::endl;
	//	last = dif;
	//
	//}
	return 0;
}

void lnstest1() {
    std::cout << "-----------test1-----------\n";
    double a = -25.1235, b = 10, c = 25.124;
    std::cout << "a = " << a << " b = " << b << " c = " << c << "\n";
    lns la(a), lb(b), lc(c);
    std::cout << "la + lb: " << la + lb << " a + b: " << a + b << "\n";
    std::cout << "la * lb: " << la * lb << " a * b: " << a * b << "\n";
    std::cout << "la - lb: " << la - lb << " a - b: " << a - b << "\n";
    std::cout << "la / lb: " << la / lb << " a / b: " << a / b << "\n";
    std::cout << "la + lb * lc: " << la + lb * lc << " a + b * c: " << a + b * c << "\n";
    la *= lb;
    a *= b;
    std::cout << "la *= lb: " << la << " a *= b: " << a << "\n";
    la += lb;
    a += b;
    std::cout << "la += lb: " << la << " a += b: " << a << "\n";
    la -= lc;
    a -= c;
    std::cout << "la -= lc: " << la << " a -= c: " << a << "\n";
    std::cout << "-----------test1-----------\n";
}

void lnscomplextest() {
    std::cout << "-----------complextest-----------\n";
    double a = 129.122194, b = 13.12423;
    double c = 0.259834, d = 50.128745;
    std::complex<double> z1(a, b);
    std::complex<double> z2(c, d);
    lns la(a), lb(b), lc(c), ld(d);
    std::cout << "z1 = " << z1 << " z2 = " << z2 << "\n";
    std::complex<lns<F>> lz1(la, lb), lz2(lc, ld);
    std::cout << "lz1 + lz2: " << lz1 + lz2 << " z1 + z2: " << z1 + z2 << "\n";
    std::cout << "lz1 * lz2: " << lz1 * lz2 << " z1 * z2: " << z1 * z2 << "\n";
    std::cout << "lz1 - lz2: " << lz1 - lz2 << " z1 - z2: " << z1 - z2 << "\n";
    std::cout << "lz1 / lz2: " << lz1 / lz2 << " z1 / z2: " << z1 / z2 << "\n";
    lz1 *= lz2;
    z1 *= z2;
    std::cout << "lz1 *= lz2: " << lz1 << " z1 *= z2: " << z1 << "\n";
    std::cout << "-----------complextest-----------\n";
}

void lnstest3() {
    std::cout << "-----------test3-----------\n";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(1, 100);
    double a = dist(gen), b = dist(gen);
    lns la(a), lb(b);
    std::cout << "a = " << a << " " << " b = " << b << "\n";
    std::cout << "la * b: " << la * b << " a * b: " << a * b << "\n";
    std::cout << "la / b: " << la / b << " a / b: " << a / b << "\n";
    la += b;
    a += b;
    std::cout << "la += b: " << la << " a += b: " << a << "\n";
    la /= b;
    a /= b;
    std::cout << "la /= b: " << la << " a /= b: " << a << "\n";
    std::cout << "-----------test3-----------\n";
}


int main() {
	test6();
	using F = FixedPoint<14, 50>;
	std::cout << log(pow(exp(F(1)), F(2))) << std::endl;
	std::cout << exp(log(F(500.f))) << std::endl;
	std::cout << log(pow(exp(F(-1)), F(-1))) << std::endl;
	std::cout << log(pow(exp(F(-1)), F(-1))) << std::endl;
	std::cout << exp(F(4)) << std::endl;
	std::cout << exp(4) << std::endl;
	for (double i = 0; i < 40; i += 1.234)// += abs(i) / 20 + 0.1)
	{
		std::cout << exp(log(F(i))) << " == " << F(i) << std::endl;
	}
	
	for (double i = 0; i < 1; i += 0.234)// += abs(i) / 20 + 0.1)
	{
		std::cout << pow((F)2, log2((F)i)) << " == " << (F)i << std::endl;
	}
	
	lnstest1();
	lnscomplextest();
	lnstest3();
	
	//bitset<64> i = 9;
	//bitset<64> j = 10;
	//bitset<64> k = div_vith_shift_left(i, j, 30);
	//std::cout << i << std::endl;
	//std::cout << j << std::endl;
	//std::cout << k.to_ullong() << std::endl << std::endl;
	//
	//constexpr ULL ii = 9;
	//constexpr ULL jj = 10;
	//constexpr ULL kk = div_vith_shift_left(ii, jj, 30);
	//
	//std::cout << bitset<64>(kk) << std::endl;
	//std::cout << (kk) << std::endl;
	return 0;
}