#include "BigInt.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <cstdio> 
#include <inttypes.h>
#include <cassert>
#include <vector>

using std::cout;
using std::endl;

namespace
{
	bool IsAllNumbers(const std::string& Value)
	{
		return !Value.empty() && std::find_if(Value.begin(), Value.end(), [](unsigned char c) { return !std::isdigit(c); }) == Value.end();
	}
}

void BigInt::Sum(const BigInt& Other)
{
	//check if the other deque contains more element than this
	const size_t ResLength = std::max(m_value.size(), Other.m_value.size());
	m_value.resize(ResLength);

	DataSeed depositary(0);
	for (size_t index = 0; index < ResLength; ++index)
	{
		const DoubleCapacityDataSeed firstOperand = m_value[index];
		const DoubleCapacityDataSeed secondOperand = index < Other.m_value.size() ? Other.m_value[index] : 0;

		const DoubleCapacityDataSeed res = firstOperand + secondOperand + depositary;

		m_value[index] = res; //lower half of res
		depositary = (res >> sizeof(DataSeed)); //high half of res
	}

	if (depositary > 0)
	{
		m_value.push_back(depositary); //add depositary as new element in deque
	}
}

void BigInt::Sub(const BigInt& Other)
{
	const size_t ResLength = std::max(m_value.size(), Other.m_value.size());
	m_value.resize(ResLength);

	DataSeed depositary(0);

	bool IsGreaterOrEqualToOther = *this >= Other;

	const SignedDoubleCapacityDataSeed firstOperandSign = IsGreaterOrEqualToOther ? +1 : -1;
	const SignedDoubleCapacityDataSeed secondOperandSign = -firstOperandSign;

	for (int index = 0; index < ResLength; ++index)
	{
		SignedDoubleCapacityDataSeed firstOperandValue = m_value[index];
		SignedDoubleCapacityDataSeed secondOperandValue = index < Other.m_value.size() ? Other.m_value[index] : 0;

		SignedDoubleCapacityDataSeed res = (firstOperandSign * firstOperandValue) + (secondOperandSign * secondOperandValue) + depositary;

		if (res >= 0)
		{
			m_value[index] = res;
			depositary = 0;
		}
		else
		{
			m_value[index] = std::numeric_limits<DataSeed>::max() + res;
			depositary = -1; //remove one from the immediate greater decimal
		}
	}

	//delete all resetted elements
	while (m_value.back() == 0 && m_value.size() > 1)
	{
		m_value.pop_back();
	}

	bIsNegative = (bIsNegative == IsGreaterOrEqualToOther); //false && false (was not negative and less than second op) or true && true (was negative and greater than second op)
}

BigInt BigInt::Divide(const BigInt& Other)
{
	 assert(Other != 0);

	//check division for 1
	if (Other == 1)
	{
		return BigInt(*this);
	}

	// 0/Other division
	if (*this == 0)
	{
		bIsNegative = false;
		BIDigits newValue(1);
		m_value = std::move(newValue);

		return BigInt();
	}

	//if we're dividing the element for itself
	if (*this == Other)
	{
		bIsNegative = false;
		BIDigits newValue(1);
		m_value = std::move(newValue);

		BigInt res;
		res.m_value[0] = 1;

		return res;
	}

	//if divisor is greater than this
	if (*this < Other)
	{
		bIsNegative = false;
		BIDigits newValue(1);

		BigInt res(*this);

		m_value = std::move(newValue);

		return res;
	}

	bIsNegative = (bIsNegative != Other.bIsNegative);

	BigInt divisor(Other); //make a copy of other in order to force positivity
	divisor.bIsNegative = false;

	if (m_value.size() > divisor.m_value.size())
	{
		divisor.m_value.insert(divisor.m_value.begin(), (m_value.size() - divisor.m_value.size()), 0); //add zero padding to match dividend size
	}
	
	//naive impl TODO refactor
	BigInt res;

	while (*this >= divisor)
	{
		Sub(divisor);
		++res;
	}

	BigInt rest(*this);
	rest.bIsNegative = false;

	m_value = std::move(res.m_value);

	return rest;
}

BigInt::BigInt() : m_value(1), bIsNegative(false) {}

BigInt::BigInt(SignedDataSeed Value) : m_value(1)
{
	bIsNegative = Value < 0;
	m_value[0] = bIsNegative ? -Value : Value;
}


//TODO
/*BigInt::BigInt(const std::string& Value) : BigInt()
{
	if (Value.empty())
	{
		std::cerr << "Value is empty. Construction of an instance of BigInt with default constructor" << endl;
		return;
	}

	assert(Value.length() > 0);
	//check first element of string is either "+", "-" or a number
	const char firstEl = Value[0];
	assert(firstEl == '+' || firstEl == '-' || std::isdigit(firstEl));

	DataSeed value = 0;
	if (firstEl == '+' || firstEl == '-')
	{
		bIsNegative = firstEl == '+' ? false : true;
		value = stoi(Value.substr(1)); //from element 1 to end of string
	}
	else
	{
		bIsNegative = false;
		value = stoi(Value);
	}

	m_value.push_back(bIsNegative ? -value : value);
	//check string length

}*/


BigInt::BigInt(const BigInt& Other) : m_value(Other.m_value), bIsNegative(Other.bIsNegative) {}

BigInt::BigInt(BigInt&& Other) noexcept : m_value(std::move(Other.m_value)), bIsNegative(Other.bIsNegative) {}

BigInt::~BigInt() {}

BigInt& BigInt::operator=(BigInt&& other) noexcept
{
	if (this != &other)
	{
		std::swap(m_value, other.m_value);
		bIsNegative = other.bIsNegative;
	}

	return *this;
}

BigInt& BigInt::operator=(const BigInt& other)
{
	if (this != &other)
	{
		bIsNegative = other.bIsNegative;
		m_value = other.m_value;
	}

	return *this;
}

BigInt& BigInt::operator+=(const BigInt& rhs)
{
	if (bIsNegative == rhs.bIsNegative)
	{
		Sum(rhs);
	}
	else
	{
		Sub(rhs);
	}

	return *this;
}

BigInt& BigInt::operator-=(const BigInt& rhs)
{
	if (bIsNegative != rhs.bIsNegative)
	{
		Sum(rhs);
	}
	else {
		Sub(rhs);
	}

	return *this;
}

BigInt& BigInt::operator*=(const BigInt& rhs)
{
	bIsNegative = (bIsNegative != rhs.bIsNegative);

	BIDigits mCopy;
	std::swap(mCopy, m_value);
	//check if the other deque contains more element than this
	const size_t ResLength = m_value.size() + rhs.m_value.size() - 1;
	m_value.resize(ResLength);

	DataSeed depositary(0);
	//multiply each element of m_value times each element of rhs.m_value
	for (size_t i = 0; i < mCopy.size(); ++i)
	{
		const DoubleCapacityDataSeed firstOperand = mCopy[i];

		for (size_t j = 0; j < rhs.m_value.size(); ++j)
		{
			const DoubleCapacityDataSeed secondOperand = rhs.m_value[j];

			SignedDoubleCapacityDataSeed resVal = firstOperand * secondOperand + depositary;

			m_value[i + j] += resVal;
			depositary = (resVal >> sizeof(DataSeed));
		}
	}

	if (depositary > 0)
	{
		m_value.push_back(depositary); //add depositary as new element in deque
	}

	return *this;
}

BigInt& BigInt::operator/=(const BigInt& rhs)
{
	Divide(rhs); //we ignore the rest
	return *this;
}

BigInt& BigInt::operator%=(const BigInt& rhs)
{
	*this = Divide(rhs); //we override this value with the mod value returned by Divide method
	return *this;
}

BigInt::ComparationResult BigInt::CompareWith(const BigInt& Other) const
{
	if (m_value.size() != Other.m_value.size())
		return m_value.size() > Other.m_value.size() ? ComparationResult::Greater : ComparationResult::Less;

	assert(m_value.size() == Other.m_value.size());

	if (bIsNegative != Other.bIsNegative)
	{
		return (bIsNegative && !Other.bIsNegative) ? ComparationResult::Greater : ComparationResult::Less;
	}

	assert(bIsNegative == Other.bIsNegative);

	for (int index = m_value.size() - 1; index >= 0; index--)
	{
		if (m_value[index] == Other.m_value[index])
			continue;
		return m_value[index] > Other.m_value[index] ? ComparationResult::Greater : ComparationResult::Less;
	}

	return ComparationResult::Equals;
}

BigInt& BigInt::operator++()
{
	*this += 1;
	return *this;
}

BigInt BigInt::operator++(int)
{
	BigInt res(*this);
	++(*this);
	return res;
}

BigInt& BigInt::operator--()
{
	*this -= 1;
	return *this;
}

BigInt BigInt::operator--(int)
{
	BigInt res(*this);
	--(*this);
	return res;
}

/*BigInt& BigInt::operator>>=(const BigInt& rhs)
{

}

BigInt& BigInt::operator<<=(const BigInt& rhs)
{

}

BigInt& BigInt::operator^=(const BigInt& rhs)
{

}

BigInt& BigInt::operator|=(const BigInt& rhs)
{

}

BigInt& BigInt::operator&=(const BigInt& rhs)
{

}
-
*/

BigInt operator+(const BigInt& a)
{
	return a;
}

BigInt operator-(const BigInt& a)
{
	BigInt res(a);
	res.bIsNegative = !a.bIsNegative;
	return res;
}

BigInt operator+(const BigInt& a, const BigInt& b)
{
	BigInt res(a);
	res += b;
	return res;
}

BigInt operator-(const BigInt& a, const BigInt& b)
{
	BigInt res(a);
	res -= b;
	return res;
}

BigInt operator*(const BigInt& a, const BigInt& b)
{
	BigInt res(a);
	res *= b;
	return res;
}

BigInt operator/(const BigInt& a, const BigInt& b)
{
	BigInt res(a);
	res /= b;
	return res;
}

BigInt operator%(const BigInt& a, const BigInt& b)
{
	BigInt res(a);
	res %= b;
	return res;
}

bool operator<(const BigInt& a, const BigInt& b)
{
	return a.CompareWith(b) == BigInt::ComparationResult::Less;
}

bool operator<=(const BigInt& a, const BigInt& b)
{
	return !(a > b);
}

bool operator>(const BigInt& a, const BigInt& b)
{
	return !(a < b);
}

bool operator>=(const BigInt& a, const BigInt& b)
{
	return !(a < b);
}

bool operator!=(const BigInt& a, const BigInt& b)
{
	return !(a == b);
}

bool operator==(const BigInt& a, const BigInt& b)
{
	return a.CompareWith(b) == BigInt::ComparationResult::Equals;
}
