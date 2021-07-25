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
	//sum with sign
}

void BigInt::Multiply(const BigInt& Other)
{

}

void BigInt::Divide(const BigInt& Other)
{

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

}
*/

BigInt::BigInt(const BigInt& Other) : m_value(Other.m_value), bIsNegative(Other.bIsNegative) {}

BigInt::BigInt(BigInt&& Other) noexcept : m_value(std::move(Other.m_value)), bIsNegative(Other.bIsNegative)  {}

BigInt::~BigInt() {}

/*BigInt& BigInt::operator++()
{

}

BigInt BigInt::operator++(int)
{

}

BigInt& BigInt::operator--()
{

}

BigInt BigInt::operator--(int)
{

}

BigInt& BigInt::operator>>=(const BigInt& rhs)
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

BigInt& BigInt::operator%=(const BigInt& rhs)
{

}

BigInt& BigInt::operator/=(const BigInt& rhs)
{

}

BigInt& BigInt::operator*=(const BigInt& rhs)
{

}

BigInt& BigInt::operator-=(const BigInt& rhs)
{

}

BigInt& BigInt::operator+=(const BigInt& rhs)
{

}

BigInt& BigInt::operator=(BigInt&& other)
{

}

BigInt& BigInt::operator=(const BigInt& other)
{

}
*/
