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
			_logValue = std::log2((NUMTYPE)number);
		}
		else {
			_sign = false;
			_logValue = std::log2(-(NUMTYPE)number);
		}
	}
	// ------------------(end of init functions)------------------------
	
	// ---------------------(Arithmetic operators)--------------------
	lns& operator*=(const lns& other) {
		this->_sign = (other._sign ? this->_sign : !this->_sign);
		this->_logValue += other._logValue;
		return *this;
	}

	template <typename othertype>
	lns& operator*=(const othertype& other) {
		this->_sign = (((NUMTYPE)other >= 0) ? this->_sign : !this->_sign);
		this->_logValue += std::log2((NUMTYPE)other);
		return *this;
	}

	lns& operator/=(const lns& other) {
		this->_sign = (other._sign ? this->_sign : !this->_sign);
		this->_logValue -= other._logValue;
		return *this;
	}

	template <typename othertype>
	lns& operator/=(const othertype& other) {
		this->_sign = (((NUMTYPE)other >= 0) ? this->_sign : !this->_sign);
		this->_logValue -= std::log2((NUMTYPE)other);
		return *this;
	}
	
	lns& operator+=(const lns& other) {
		if (!other._sign && this->_sign)
			return operator-=(lns(1, other._logValue));
		
		if (other._sign && !this->_sign) {
			NUMTYPE tmp = this->_logValue;
			this->_sign = other._sign;
			this->_logValue = other._logValue;
			return operator-=(lns(1, tmp));
		}
		
		this->_logValue += std::log2(1 + pow(2, other._logValue - this->_logValue));
		return *this;
	}

	template<typename othertype>
	lns& operator+=(const othertype& other) {
		if ((other < 0) && this->_sign)
			return operator-=(-other);
		
		if ((other >= 0) && !this->_sign) {
			NUMTYPE tmp = this->_logValue;
			this->_sign = (other >= 0) ? 1 : 0;
			this->_logValue = std::log2((NUMTYPE)other);
			return operator-=(pow(2, tmp));
		}
		NUMTYPE otherlog = (other >= 0) ? std::log2((NUMTYPE)other) : std::log2(-(NUMTYPE)other);
		this->_logValue += std::log2(1 + pow(2, otherlog  - this->_logValue));
		return *this;
	}

	lns& operator-=(const lns& other) {
		if (!other._sign)
			return operator+=(lns(1, other._logValue));
		
		if (other._sign && !this->_sign) {
			this->_sign = 0;
			this->_logValue = other._logValue;
			return operator+=(lns(0, other._logValue));
		}

		NUMTYPE sb = 1 - pow(2, other._logValue - this->_logValue);
		this->_sign = ((other._logValue > this->_logValue) ? 0 : this->_sign);
		this->_logValue += std::log2((sb >= 0) ? sb : -sb);
		return *this;
	}

	template<typename othertype>
	lns& operator-=(const othertype& other) {
		if (other < 0)
			return operator+=(-other);
		
		if ((other >= 0) && !this->_sign) {
			this->_sign = 0;
			this->_logValue = std::log2((NUMTYPE)other);
			return operator+=(-other);
		}

		NUMTYPE otherlog = (other >= 0) ? std::log2((NUMTYPE)other) : std::log2(-(NUMTYPE)other);
		NUMTYPE sb = 1 - pow(2, otherlog - this->_logValue);
		this->_sign = (otherlog > this->_logValue) ? 0 : this->_sign;
		this->_logValue += std::log2((sb >= 0) ? sb : -sb);
		return *this;
	}

	lns operator*(const lns& other) const {
		return lns((this->_sign) ? other._sign : !other._sign, this->_logValue + other._logValue);
	}
	
	
	template<typename othertype>
	friend lns operator*(const lns& lognumber, const othertype& number) {
		if (number >= 0)
			return lns(lognumber._sign == true, lognumber._logValue + log2((NUMTYPE)number));
		else
			return lns(lognumber._sign == false, lognumber._logValue + log2(-(NUMTYPE)number));
	}

	template<typename othertype>
	friend lns operator*(const othertype& number, const lns& lognumber) {
		if (number >= 0)
			return lns(lognumber._sign == true, lognumber._logValue + log2((NUMTYPE)number));
		else
			return lns(lognumber._sign == false, lognumber._logValue + log2(-(NUMTYPE)number));
	}
	
	
	lns operator/(const lns& other) const {
		return lns(this->_sign == other._sign, this->_logValue - other._logValue);
	}

	template<typename oldType>
	friend lns operator/(const lns& lognumber, const oldType& number) {
		if (number >= 0)
			return lns(lognumber._sign == true, lognumber._logValue - log2((NUMTYPE)number));
		else
			return lns(lognumber._sign == false, lognumber._logValue - log2(-(NUMTYPE)number));
	}
	
	template<typename oldType>
	friend lns operator/(const oldType& number, const lns& lognumber) {
		if (number >= 0)
			return lns(lognumber._sign == true, log2((NUMTYPE)number) - lognumber._logValue);
		else
			return lns(lognumber._sign == false, log2((NUMTYPE)number) - lognumber._logValue);
	}
	
	lns operator+(const lns& other) const {
		if (!other._sign && this->_sign)
			return operator-(lns(1, other._logValue));
		
		if (other._sign && !this->_sign) { // return other - this
			NUMTYPE sb = 1 - pow(2, this->_logValue - other._logValue);
			return lns(((this->_logValue > other._logValue) ? 0 : other._logValue), other._logValue + std::log2((sb >= 0) ? sb : -sb));
		}

		return lns(this->_sign, this->_logValue + std::log2(1 + pow(2, other._logValue - this->_logValue)));
	}

	lns operator-(const lns& other) const {
		if (!other._sign)
			return operator+(lns(1, other._logValue));
		
		if (other._sign && !this->_sign) { // return - other - this
			return lns(0, this->_logValue + std::log2(1 + pow(2, other._logValue - this->_logValue)));
		}

		NUMTYPE sb = 1 - pow(2, other._logValue - this->_logValue);
		return lns(((other._logValue > this->_logValue) ? 0 : this->_sign), this->_logValue + std::log2((sb >= 0) ? sb : -sb));
	}
	

	lns operator+() const {
		return lns(this->_sign, this->_logValue);
	}

	lns operator-() const {
		return lns(!this->_sign, this->_logValue);
	}
	
	lns& operator++() {
		if (this->_sign) {
			this->_logValue += std::log2(1 + pow(2, -this->_logValue));
			return *this;
		}

		NUMTYPE sb = 1 - pow(2, this->_logValue);
		this->_sign = (1 >= pow(2, this->_logValue)) ? 1 : 0;
		this->_logValue = std::log2((sb >= 0) ? sb : -sb);
		return *this;
	}

	lns operator++(int) {
		if (this->_sign) {
			lns copy {*this};
			this->_logValue += std::log2(1 + pow(2, -this->_logValue));
			return copy;
		}
		
		lns copy {*this};
		NUMTYPE sb = 1 - pow(2, this->_logValue);
		this->_sign = (1 >= pow(2, this->_logValue)) ? 1 : 0;
		this->_logValue = std::log2((sb >= 0) ? sb : -sb);
		return copy;
	}

	lns& operator--() {
		if (this->_sign) {
			this->_sign = (1 < pow(2, this->_logValue)) ? 1 : 0;
			NUMTYPE sb = 1 - pow(2, -this->_logValue);
			this->_logValue += std::log2((sb >= 0) ? sb : -sb);
			return *this;
		}
		this->_logValue += std::log2(1 + pow(2, -this->_logValue));
		return *this;
	}

	lns operator--(int) {
		if (this->_sign) {
			lns copy {*this};
			this->_sign = (1 < pow(2, this->_logValue)) ? 1 : 0;
			NUMTYPE sb = 1 - pow(2, -this->_logValue);
			this->_logValue += std::log2((sb >= 0) ? sb : -sb);
			return copy;
		}

		lns copy {*this};
		this->_logValue += std::log2(1 + pow(2, -this->_logValue));
		return copy;
	}

	// TODO: a % b
	// ---------------------(end of arithmetic  operators)--------------------
	
	// ---------------------(Assignment operators)--------------------
	lns& operator=(const lns& other) {
		this->_sign = other._sign;
		this->_logValue = other._logValue;
		return *this;
	}
	
	// ---------------------(end of assignment operators)--------------------
	// ---------------------(Comparison and relational operators)--------------------
	bool operator==(const lns& other) {
		return ((this->_sign == other._sign) && (this->_logValue == other._logValue) || 
		(this->_logValue == 0 && other._logValue == 0));
	}

	bool operator<(const lns& other) { // Here exist problm: if this = -0 and other = +0 then it work wrong. TODO: solwe it.
		if (this->_sign) {
			if (other._sign)
				return this->_logValue < other._logValue;
			return false;
		}
		else {
			if (other._sign)
				return true;
			return this->_logValue > other._logValue;
		}
	}
	
	bool operator>(const lns& other) {
		return !(operator==(other)) || !(operator<(other));
	}
	
	bool operator>=(const lns& other) {
		return (operator>(other)) || (operator==(other));
	}

	bool operator<=(const lns& other) {
		return (operator<(other)) || (operator==(other));
	}

	bool operator!=(const lns& other) {
		return !(operator==(other));
	}

	// ---------------------(end of comparison and relational operators)--------------------
	template <typename TO_TYPE>
	operator TO_TYPE() const {
		return (TO_TYPE)((this->_sign) ? pow(2, this->_logValue) : -pow(2, this->_logValue));
	}

	friend std::ostream& operator<<(std::ostream& os, const lns<NUMTYPE>& m) {
		// TODO:here problem if number is too big we lost data when use (double)m, so we need to better wariant writing the answer.
		os << ((m._sign) ? "" : "-") << pow(2, m._logValue);
		return os;
	}
};




#endif // !_INCLUDE_LNS_H_