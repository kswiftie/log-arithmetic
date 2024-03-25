#ifndef _INCLUDE_LNS_H_
#define _INCLUDE_LNS_H_

#include <iostream>
#include <cmath>
// #include "bitset.h"

#if SIMPLE_REALIZATION
#define STANDARD_BITSET_TYPE BITSET
#else
#define STANDARD_BITSET_TYPE BITSET<>
#endif // SIMPLE_REALIZATION

template <class NUMTYPE = double>
class lns {
protected:
	NUMTYPE _logValue;
	bool _sign;
public:
	// -----------------(DEBUG FUNCTION)------------------------
	void print() { 
		std::cout << "(" << (this->_sign ? '+' : '-') << ")(" << this->_logValue << ")" << "\n";
	}
	// -----------------(end DEBUG FUNCTION)------------------------
	// ------------------(init functions)------------------------
	lns(bool sign = 1, NUMTYPE logvalue = 0) : _sign(sign), _logValue(logvalue) {}
	
	template<typename oldType>
	lns(oldType number) {
		if (number >= 0) {
			_sign = true;
			_logValue = log2((NUMTYPE)number);
		}
		else {
			_sign = false;
			_logValue = log2(-(NUMTYPE)number);
		}
	}
	// ------------------(end of init functions)------------------------
	
	// ---------------------(Arithmetic operators)--------------------
	lns& operator*(const lns& other) const {
		return *lns(this->_sign == other._sign, this->_logValue + other._logValue);
	}

	template<typename oldType>
	friend lns& operator*(const lns& lognumber, const oldType& number) {
		if (number >= 0)
			return *lns(lognumber._sign == true, lognumber._logValue + log2((NUMTYPE)number));
		else
			return *lns(lognumber._sign == false, lognumber._logValue + log2(-(NUMTYPE)number));
	}
	
	template<typename oldType>
	friend lns& operator*(const oldType& number, const lns& lognumber) {
		if (number >= 0)
			return *lns(lognumber._sign == true, lognumber._logValue + log2((NUMTYPE)number));
		else
			return *lns(lognumber._sign == false, lognumber._logValue + log2(-(NUMTYPE)number));
	}
	
	lns& operator/(const lns& other) const { // todo: maybe add check if other = 0+ or 0-
		return *lns(this->_sign == other._sign, this->_logValue - other._logValue);
	}

	template<typename oldType>
	friend lns& operator/(const lns& lognumber, const oldType& number) {
		if (number >= 0)
			return *lns(lognumber._sign == true, lognumber._logValue - log2((NUMTYPE)number));
		else
			return *lns(lognumber._sign == false, lognumber._logValue - log2(-(NUMTYPE)number));
	}
	
	template<typename oldType>
	friend lns& operator/(const oldType& number, const lns& lognumber) {
		if (number >= 0)
			return *lns(lognumber._sign == true, log2((NUMTYPE)number) - lognumber._logValue);
		else
			return *lns(lognumber._sign == false, log2((NUMTYPE)number) - lognumber._logValue);
	}
	
	lns& operator+() const {
		return *lns(this->_sign, this->_logValue);
	}

	lns& operator-() const {
		return *lns(!this->_sign, this->_logValue);
	}

	lns& operator++() { // really bad. need to fix
		NUMTYPE tmp = pow(2, this->_logValue) + (NUMTYPE)1;
		std::cout << tmp << "\n";
		if (tmp >= 0) {
			this->_logValue = log2(tmp);
			this->_sign = true;
		}
		else {
			this->_logValue = log2(-tmp);
			this->_sign = false;
		}
		return *lns(this->_sign, this->_logValue);
	}

	lns& operator++(int) { // same. need to fix
		NUMTYPE tmp = pow(2, this->_logValue) + (NUMTYPE)1;
		if (tmp >= 0) {
			this->_logValue = log2(tmp);
			this->_sign = true;
		}
		else {
			this->_logValue = log2(-tmp);
			this->_sign = false;
		}
		return *lns(this->_sign, log2(tmp - 1));
	}

	lns& operator--() { // really bad. need to fix
		NUMTYPE tmp = pow(2, this->_logValue) - (NUMTYPE)1;
		std::cout << tmp << "\n";
		if (tmp >= 0) {
			this->_logValue = log2(tmp);
			this->_sign = true;
		}
		else {
			this->_logValue = log2(-tmp);
			this->_sign = false;
		}
		return *lns(this->_sign, this->_logValue);
	}

	lns& operator--(int) { // same. need to fix
		NUMTYPE tmp = pow(2, this->_logValue) - (NUMTYPE)1;
		if (tmp >= 0) {
			this->_logValue = log2(tmp);
			this->_sign = true;
		}
		else {
			this->_logValue = log2(-tmp);
			this->_sign = false;
		}
		return *lns(this->_sign, log2(tmp + 1));
	}
	// TODO: a % b, a + b, a - b
	// ---------------------(end of arithmetic  operators)--------------------
	
	// ---------------------(Assignment operators)--------------------
	lns& operator=(const lns& other) {
		return *other;
	}

	lns& operator+=(const lns& other) {
		return *other;
	}

	lns& operator=(const lns& other) {
		return *other;
	}
	
	// ---------------------(end of assignment operators)--------------------
	// ---------------------(Comparison and relational operators)--------------------
	bool operator==(const lns& other)
	{
		return ((this->_sign == other._sign) && (this->_logValue == other._logValue) || 
		(this->_logValue == 0 && other._logValue == 0));
	}

	bool operator!=(const lns& other) {
		return ((this->_sign != other._sign || this->_logValue != other._logValue) &&
		!(this->_logValue == 0 && other._logValue == 0));
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
	
	bool operator>=(const lns& other) {
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
	// ---------------------(end of comparison and relational operators)--------------------
	/*

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

	// ---------------(to type operators)---------------------------
	template <typename OUTTYPE>
	operator OUTTYPE() const {
		//TODO: we should to check size of _logNumber, if it too big we got a problem
		if (_isPositive)
			return std::pow(2, (double)_logNumber);
		return -std::pow(2, (double)_logNumber);
	}
	*/
	friend std::ostream& operator<< (std::ostream& os, const lns<NUMTYPE>& m) {
		// TODO:here problem if number is too big we lost data when use (double)m, so we need to better wariant writing the answer.
		os << ((m._sign) ? "" : "-") << pow(2, m._logValue);
		return os;
	}
};




#endif // !_INCLUDE_LNS_H_