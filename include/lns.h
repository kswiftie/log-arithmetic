#pragma once
#ifndef _INCLUDE_LNS_H_
#define _INCLUDE_LNS_H_
#include <iostream>
#include "bitset.h"
#if SIMPLE_REALIZATION
#define STANDARD_BITSET_TYPE BITSET
#else
#define STANDARD_BITSET_TYPE BITSET<>
#endif // SIMPLE_REALIZATION

template <class NUMTYPE = STANDARD_BITSET_TYPE>
class lns
{
protected:
	NUMTYPE _logNumber = 0;
	bool _isPositive = 1;

	lns(bool isPositive, NUMTYPE logNumber) : _isPositive(isPositive), _logNumber(logNumber) {}
public:
	// -----------------(DEBUG FUNCTION)------------------------
	void print()
	{ 
		std::cout << "(" << (this->_isPositive ? '+' : '-') << ")(" << this->_logNumber << ")" << std::endl;
	}
	// ------------------(init function)------------------------
	lns(const lns& other) = default;

	lns() = default;

	template<typename oldType>
	lns(const oldType& number)
	{
		if (number >= 0)	this->lns::lns(true, (double)std::log2((double)number));
		else				this->lns::lns(false, (double)std::log2((double)number * (-1)));
	}

	// ---------------------(math operators)--------------------
	lns operator *(const lns& other) const
	{
		return lns(this->_isPositive == other._isPositive, this->_logNumber + other._logNumber);
	}

	lns operator /(const lns& other) const
	{ // todo: maybe add check if other = 0+ or 0-
		return lns(this->_isPositive == other._isPositive, this->_logNumber - other._logNumber);
	}

	lns& operator*=(const lns& other) // as easyy as it is.
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
		return lns((NUMTYPE)(*this) + (NUMTYPE)(other)); // TODO: chenge it for calculate it wis cpecial formula x + s(y - x).
	}

	lns operator-(const lns& other)
	{
		return lns((NUMTYPE)(*this) - (NUMTYPE)(other)); // TODO: chenge it for calculate it wis cpecial formula x + d(y - x).
	}

	lns& operator+=(const lns& other)
	{
		this->lns::lns(*this + other); // TODO: make it faster without makeing new lns us formula.
		return *this;
	}
	lns& operator-=(const lns& other)
	{
		this->lns::lns(*this - other); // TODO: make it faster without makeing new lns us formula.
		return *this;
	}
	lns operator-() const //It's good as for me.
	{
		return lns(!_isPositive, this->_logNumber);
	}
	lns operator+()
	{
		return *this;
	}
	// ------------------(compare operators)-----------------------------------
	
	//TODO: add operator>
	//TODO: add operator>=
	//TODO: add operator<=
	//TODO: add operator!=

	bool operator==(const lns& other) // TODO: Here exist problem: if this = -0 and other = +0 then it work wrong. TODO: solwe it. or think how to solwe it
	{
		return (this->_isPositive == other._isPositive) && (this->_logNumber == other._logNumber);
	}

	bool operator<(const lns& other)
	{ //TODO: Here exist problm: if this = -0 and other = +0 then it work wrong. TODO: solwe it.
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
	// ---------------(to type operators)---------------------------
	template <typename OUTTYPE>
	operator OUTTYPE() const
	{
		//TODO: we should to check size of _logNumber, if it too big we got a problem
		if (_isPositive)
			return std::pow(2, (double)_logNumber);
		return -std::pow(2, (double)_logNumber);
	}



};

template <typename LNSTYPE>
std::ostream& operator<< (std::ostream& os, const lns<LNSTYPE>& m) {
	// TODO:here problem if number is too big we lost data when use (double)m, so we need to better wariant writing the answer.
	os << (double)m;
	return os;
}


#endif // !_INCLUDE_LNS_H_