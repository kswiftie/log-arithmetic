#pragma once
#ifndef _INCLUDE_BITSET_H_
#define _INCLUDE_BITSET_H_

#define SIMPLE_REALIZATION true
// .________________________.
// |						|
// |    BIT SET REALIZATION |
// |________________________|

#if SIMPLE_REALIZATION == true // switch this for simple realization

// --------------------(class BITSET simple)---------------
using BITSET = float;
// -----------------(end class BITSET simple)--------------



#else
// --------------------(class BITSET special)--------------
#define FRAC_PART 4
int TO_MUL_FRAC = 1 << FRAC_PART;
class BITSET
{
protected:
	int val;
public:
	template <typename TYPE>
	BITSET(const TYPE& other)
	{
		val = other * TO_MUL_FRAC;
	}
	//here add+, add-, add*, add/ and so on.

	bool operator <(const BITSET other) const
	{
		return this->val < other.val;
	}
	bool operator >(const BITSET other) const
	{
		return this->val > other.val;
	}
	bool operator >=(const BITSET other) const
	{
		return this->val >= other.val;
	}

	bool operator ==(const BITSET other) const
	{
		return this->val == other.val;
	}

	BITSET operator -(const BITSET other) const
	{

		return (this->val - other.val) / (float)(TO_MUL_FRAC);
	}
	BITSET operator +(const BITSET other) const
	{

		return (this->val + other.val) / (float)(TO_MUL_FRAC);
	}
	BITSET operator *(const BITSET other) const
	{

		return (this->val * other.val) / (float)(TO_MUL_FRAC << 1);//?
	}

	BITSET operator -=(const BITSET other)
	{
		this->val -= other.val;
		return *this;
	}
	BITSET operator +=(const BITSET other)
	{
		this->val += other.val;
		return *this;
	}

	BITSET operator-()
	{
		return -this->val / (float)(TO_MUL_FRAC);
	}
};
// -----------------(end class BITSET special)-------------

#endif // SIMPLE_REALIZATION

#endif // !_INCLUDE_BITSET_H_
