#pragma once
#ifndef _INCLUDE_LNS_H_
#define _INCLUDE_LNS_H_
#include <iostream>
#include "bitset.h"

#define STANDARD_BITSET_TYPE BITSET

template <class NUMTYPE = STANDARD_BITSET_TYPE>
class lns
{
protected:
	NUMTYPE _logNumber = 0;
	bool _isPositive = 1;

	lns(bool isPositive, NUMTYPE logNumber) : _isPositive(isPositive), _logNumber(logNumber) {}
public:

	lns(const lns& other) = default;
	lns() = default;

	template<typename oldType>
	lns(const oldType& number)
	{
		//Before i think we can use only one times for object it's init operator.
		if (number >= 0)	this->lns::lns(true, std::log2(number));
		else				this->lns::lns(false, std::log2(number * (-1)));
	}
	void print()
	{
		std::cout << "(" << (this->_isPositive ? '+' : '-') << ")(" << this->_logNumber << ")" << std::endl;
	}
	lns operator *(const lns& other) const
	{
		return lns(this->_isPositive == other._isPositive, this->_logNumber + other._logNumber);
	}

	lns operator /(const lns& other) const
	{
		return lns(this->_isPositive == other._isPositive, this->_logNumber - other._logNumber);
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

	lns& operator+=(const lns& other)
	{
		this->lns::lns(*this + other); // TODO: chenge it for not use lns and int.
		return *this;
	}
	lns& operator-=(const lns& other)
	{
		this->lns::lns(*this - other); // TODO: chenge it for not use lns and int.
		return *this;
	}
	lns operator-() const
	{
		return lns(!_isPositive, this->_logNumber);
	}
	lns operator+()
	{
		return *this;
	}

	bool operator==(const lns& other) // Here exist problm: if this = -0 and other = +0 then it work wrong. TODO: solwe it.
	{
		return (this->_isPositive == other._isPositive) && (this->_logNumber == other._logNumber);
	}

	bool operator<(const lns& other)
	{ // Here exist problm: if this = -0 and other = +0 then it work wrong. TODO: solwe it.
		if (this->_isPositive)
		{
			if (other._isPositive)
				return this->_logNumber < other._logNumber;
			return true;
		}
		else
		{
			if (other._isPositive)
				return true;
			return this->_logNumber > other._logNumber;
		}
	}
	// Convert to any type!!! it's ausem that we can do like down here.

	template <typename OUTTYPE>
	operator OUTTYPE() const
	{
		//std::cout << "( " << (_logNumber - 8) << ") ";
		if (_isPositive)
			return std::pow(2, _logNumber);
		return -std::pow(2, _logNumber);
	}



};
template <typename LNSTYPE>
std::ostream& operator<< (std::ostream& os, const lns<LNSTYPE>& m) {
	os << (double)m;
	return os;
}


#endif // !_INCLUDE_LNS_H_