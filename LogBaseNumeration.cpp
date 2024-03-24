// LogBaseNumeration.cpp: определяет точку входа для приложения.
//

#include "LogBaseNumeration.h"
//TODO: add realyzation of my own bazeType!!!!!
#define STANDARD_BITSET_TYPE long long

using namespace std;


template <class NUMTYPE = STANDARD_BITSET_TYPE>
class lns {
protected:
	NUMTYPE _logNumber;
	bool _isPositive;
public:
	lns(bool isPositive = 1, NUMTYPE logNumber = 0) : _isPositive(isPositive), _logNumber(logNumber) {}
	lns(const lns& other) = default;
	lns() = default;

	template<typename oldType>
	lns(const oldType& number)
	{
		// Before i think we can use only one times for object it's init operator.
		if (number >= 0)	this->lns::lns(true, log2(number));
		else				this->lns::lns(false, log2(-number));
	}

	void print()
	{
		std::cout << "(" << (this->_isPositive ? '+' : '-') << ")(" << this->_logNumber << ")" << endl;
	}
	
	lns operator= (const lns& other) {
		this->_logNumber = other;
		this->_isPositive = (other > 0) ? 1 : 0;
		return *this;
	}

	lns& operator+=(const lns& other)
	{
		this->lns::lns(*this + other); // TODO: change it for not use lns and int.
		return *this;
	}
	
	lns& operator-=(const lns& other)
	{
		this->lns::lns(*this - other); // TODO: change it for not use lns and int.
		return *this;
	}
	
	lns& operator*=(const lns& other)
	{
		this->_isPositive = (this->_isPositive == other._isPositive);
		this->_logNumber += other._logNumber;
		return *this;
	}

	lns& operator/=(const lns& other)
	{
		this->_isPositive = (this->_isPositive == other._isPositive);
		this->_logNumber -= other._logNumber;
		return *this;
	}

	lns operator+(const lns& other)
	{
		return lns((NUMTYPE)(*this) + (NUMTYPE)(other)); // TODO: chenge it for not use lns and int.
	}

	lns operator-(const lns& other)
	{
		return lns((NUMTYPE)(*this) - (NUMTYPE)(other)); // TODO: chenge it for not use lns and int.
	}
	
	lns operator* (const lns& other) const
	{
		return lns(this->_isPositive == other._isPositive, this->_logNumber + other._logNumber);
	}

	lns operator/ (const lns& other) const
	{
		return lns(this->_isPositive == other._isPositive, this->_logNumber - other._logNumber);
	}

	lns operator-()
	{
		return lns(!_isPositive, this->_logNumber);
	}
	
	lns operator+()
	{
		return *this;
	}

	bool operator==(const lns& other)
	{
		return ((this->_isPositive == other._isPositive) && (this->_logNumber == other._logNumber) || 
		(this->_logNumber == 0 && other._logNumber == 0));
	}

	bool operator!=(const lns& other) {
		return ((this->_isPositive != other._isPositive || this->_logNumber != other._logNumber) &&
		!(this->_logNumber == 0 && other._logNumber == 0));
	}

	bool operator<(const lns& other) { // Here exist problm: if this = -0 and other = +0 then it work wrong. TODO: solwe it.
		if (this->_isPositive) {
			if (other._isPositive)
				return this->_logNumber < other._logNumber;
			return false;
		}
		else {
			if (other._isPositive)
				return true;
			return this->_logNumber > other._logNumber;
		}
	}
	
	bool operator>(const lns& other) {
		if (this->_isPositive) {
			if (other._isPositive)
				return this->_logNumber > other._logNumber;
			return true;
		}
		else {
			if (other._isPositive)
				return false;
			return this->_logNumber < other._logNumber;
		}
	}
	
	// Convert to any type!!! it's ausem that we can do like down here.
	template <typename OUTTYPE>
	operator OUTTYPE() const
	{
		//std::cout << "( " << (_logNumber - 8) << ") ";
		if (_isPositive)
			return pow(2, _logNumber);
		return -pow(2, _logNumber);
	}
};

template <typename LNSTYPE>
std::ostream& operator<< (std::ostream& os, const lns<LNSTYPE>& m) {
	os << (double)m;
	return os;
}

int main() {
	using type = float;
	auto a = std::complex<lns<type>>(10, 0);
	auto b = std::complex<lns<type>>(0, 1);
	std::cout << a << " + " << b << " = " << a + b << endl;
	std::cout << a << " - " << b << " = " << a - b << endl;
	std::cout << a << " / " << b << " = " << a / b << endl;
	std::cout << a << " * " << b << " = " << a * b << endl;
	return 0;
}
