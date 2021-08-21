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
	constexpr BigInt::DoubleCapacityDataSeed max_power10_in_max_power2[] = { 0, 100, 10000, 10000000, 1000000000, 1000000000000, 100000000000000, 10000000000000000, 10000000000000000000 };

	constexpr BigInt::DoubleCapacityDataSeed max_power10()
	{
		return max_power10_in_max_power2[sizeof(BigInt::DataSeed)];
	}

	template <BigInt::DoubleCapacityDataSeed T>
	struct count_digits
	{
		static constexpr BigInt::DoubleCapacityDataSeed value = 1 + count_digits<T / 10>::value;

	};

	template <>
	struct count_digits<0>
	{
		static constexpr BigInt::DoubleCapacityDataSeed value = 0;
	};

	constexpr BigInt::DoubleCapacityDataSeed max_digits()
	{
		return count_digits<max_power10()>::value;
	}

	bool IsAllNumbers(const std::string& Value)
	{
		return !Value.empty() && std::find_if(Value.begin(), Value.end(), [](unsigned char c) { return !std::isdigit(c); }) == Value.end();
	}

	//reppresentation that maintains leading zeroes
	std::string ToStringFormatted(BigInt::DoubleCapacityDataSeed num)
	{
		constexpr BigInt::DoubleCapacityDataSeed baseDigits = max_digits();
		
		std::string format = "%0";
		format.append(std::to_string(baseDigits - 1)); ///only zeroes
		format.append(PRIu64);

		char buffer[baseDigits];
		sprintf_s(buffer, baseDigits, format.c_str(), num);

		return buffer;
	}

}

BigInt::DataSeed BigInt::UOperationResult::GetLowerHalf() const
{
	return static_cast<BigInt::DataSeed>(res);
}

BigInt::DataSeed BigInt::UOperationResult::GetHighHalf() const
{
	return static_cast<BigInt::DataSeed>((res >> sizeof(BigInt::DataSeed) * CHAR_BIT));
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

		UOperationResult opResult;
		opResult.res = firstOperand + secondOperand + depositary;

		m_value[index] = opResult.GetLowerHalf(); //lower half of res
		depositary = opResult.GetHighHalf(); //high half of res
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

	SignedDoubleCapacityDataSeed depositary(0);

	bool IsGreaterOrEqualToOther = AbsCompareWith(Other) != ComparationResult::Less;

	const SignedDoubleCapacityDataSeed firstOperandSign = IsGreaterOrEqualToOther ? +1 : -1;
	const SignedDoubleCapacityDataSeed secondOperandSign = -firstOperandSign;

	for (size_t index = 0; index < ResLength; ++index)
	{
		SignedDoubleCapacityDataSeed firstOperandValue = m_value[index];
		SignedDoubleCapacityDataSeed secondOperandValue = index < Other.m_value.size() ? Other.m_value[index] : 0;

		SignedDoubleCapacityDataSeed res = (firstOperandSign * firstOperandValue) + (secondOperandSign * secondOperandValue) + depositary;

		if (res >= 0)
		{
			m_value[index] = res; //willingly taking only the lowest bits
			depositary = 0;
		}
		else
		{
			m_value[index] = std::numeric_limits<DataSeed>::max() + (res + 1); //willingly taking only the lowest bits
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

void BigInt::Multiply(const BigInt& rhs)
{
	if (rhs == 0)
	{
		bIsNegative = false;
		BIDigits newValue(1);
		m_value = std::move(newValue);
	}

	//we have nothing to do
	if (rhs == 1) return;

	bIsNegative = (bIsNegative != rhs.bIsNegative);

	BigInt res = VariantPeasantMultiply(*this, rhs);

	m_value = std::move(res.m_value);
}

/** Uses occupied bit of lhs to add multiplier, properly shifted, to result
 * always sum rhs to result
 * lhs is used only at bitwise level, no arithmetic is performed
*/
BigInt BigInt::VariantPeasantMultiply(const BigInt& lhs, const BigInt& rhs) const
{
	BigInt Mutliplier = rhs;
	Mutliplier.bIsNegative = false; //make sure we consider only positive values, sign is checked outside
	BigInt sum = 0;
	for (BigInt::BIDigits::size_type x = 0; x < lhs.GetBits(); x++) {
		if (lhs.GetBitValueAt(x)) //check if lhs has a one in position x
		{
			//if lhs has a one sum current multiplier
			sum += Mutliplier;
		}

		Mutliplier <<= 1; //multiply by 2
	}
	return sum;
}

/*BigInt BigInt::NaiveMultiply(const BigInt& lhs, const BigInt& rhs)
{
	//check if the other deque contains more element than this
	const size_t ResLength = lhs.m_value.size() + rhs.m_value.size() - 1;

	BigInt result;
	result.m_value.resize(ResLength);

	DataSeed depositary(0);
	//multiply each element of m_value times each element of rhs.m_value

	for (size_t i = 0; i < lhs.m_value.size(); ++i)
	{
		const DoubleCapacityDataSeed firstOperand = lhs.m_value[i];

		for (size_t j = 0; j < rhs.m_value.size(); ++j)
		{
			const DoubleCapacityDataSeed secondOperand = rhs.m_value[j];

			UOperationResult opResult;
			opResult.res = (firstOperand * secondOperand) + depositary;

			result.m_value[i + j] += opResult.GetLowerHalf();
			depositary = opResult.GetHighHalf();
		}
	}

	if (depositary > 0)
	{
		result.m_value.push_back(depositary); //add depositary as new element in deque
	}

	return result;
}*/

/*BigInt BigInt::KaratsubaMultiply(const BigInt& lhs, const BigInt& rhs)
{
	constexpr BigInt::DoubleCapacityDataSeed base = max_power10(); //base
	//if operands value is less than divisor fallback on naive multiply

	if (lhs <= base || rhs <= base)
		return PeasantMultiply(lhs, rhs);

	size_t DivisorMultiplier = std::min(lhs.m_value.size(), rhs.m_value.size()) / 2;
	BigInt SizeDivisor = base;
	if (DivisorMultiplier > 0)
	{
		for (size_t i = 0; i < DivisorMultiplier; ++i)
		{
			SizeDivisor = PeasantMultiply(SizeDivisor, base);
		}
	}

	//pair division result and module
	std::pair<BigInt, BigInt> x = lhs.DivideFastImpl(base);
	std::pair<BigInt, BigInt> y = rhs.DivideFastImpl(base);

	BigInt x0 = x.second;
	BigInt x1 = x.first;

	BigInt y0 = y.second;
	BigInt y1 = y.first;

	BigInt z0 = KaratsubaMultiply(x0, y0); //recursively multiply modules
	BigInt z2 = KaratsubaMultiply(x1, y1);//recursively multiply divisions results
	BigInt z1 = ((KaratsubaMultiply(x1 + x0, y1 + y0) - z2) - z0);

	BigInt DoubledSizeDivisor = base;
	if (DivisorMultiplier > 0)
	{
		for (size_t i = 0; i <(DivisorMultiplier * 2); ++i)
		{
			DoubledSizeDivisor = NaiveMultiply2(DoubledSizeDivisor, base);
		}
	}

	BigInt resFirstPart = z2 * DoubledSizeDivisor;
	BigInt resSecondPart = z1 * SizeDivisor;
	//combine results
	BigInt res = resFirstPart + resSecondPart + z0;

	return res;
}*/

BigInt BigInt::Divide(const BigInt& Other)
{
	assert(Other != 0); //manage division by 0

	//check division for 1
	if (Other == 1 || Other == -1)
	{
		bIsNegative = bIsNegative != Other.bIsNegative;
		return BigInt();
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
		m_value[0] = 1;

		return BigInt();
	}

	//if divisor is greater than this
	if (AbsCompareWith(Other) == ComparationResult::Less)
	{
		bIsNegative = false;
		BIDigits newValue(1);

		BigInt res(*this);

		m_value = std::move(newValue);

		return res;
	}

	//std::pair <BigInt, BigInt> res = DivideNaiveImpl(Other);
	std::pair <BigInt, BigInt> res = DivideFastImpl(Other);

	m_value = std::move(res.first.m_value);
	bIsNegative = (bIsNegative != Other.bIsNegative);

	return res.second;
}

std::pair <BigInt, BigInt> BigInt::DivideNaiveImpl(const BigInt& Other)const
{
	std::pair <BigInt, BigInt> qr(0, *this);

	BigInt divisor(Other); //make a copy of other in order to force positivity
	divisor.bIsNegative = false;

	while (qr.second >= divisor)
	{
		qr.second.Sub(divisor);
		++qr.first;
	}

	return qr;
}

std::pair <BigInt, BigInt> BigInt::DivideFastImpl(const BigInt& Other) const
{
	BigInt Divisor(Other);
	Divisor.bIsNegative = false; //copy has to be positive, sign is evaluated outside, here we're just computing the value
	//quotient and module
	std::pair <BigInt, BigInt> qr(0, 0);
	for (BIDigits::size_type x = GetBits(); x > 0; x--) {
		//shift left by one in order to process next element in list
		//multiply elements by 2
		qr.first <<= 1;
		qr.second <<= 1;

		//check bit at position x - 1 
		if (GetBitValueAt(x - 1)) {
			qr.second++; //if its a 1 increment module, consume bit in dividend
		}

		//if after the incrementation module is greater or equals to divisor 
		if (qr.second >= Divisor) {
			qr.second -= Divisor; //then subtract divisor from module
			qr.first++; //Increment quotient
		}
	}
	return qr;
}

size_t BigInt::GetBits() const
{
	size_t out = (m_value.empty() ? 0 : (m_value.size() - 1)) * (sizeof(DataSeed) * CHAR_BIT);
	DataSeed msb = m_value.empty() ? 0 : m_value.back();
	while (msb) {
		msb >>= 1;
		out++;
	}

	return out;
}

bool BigInt::GetBitValueAt(size_t index) const
{
	static constexpr size_t bits = sizeof(DataSeed) * CHAR_BIT;
	DataSeed interestedValue = m_value[index / bits];
	return (interestedValue >> index) & 1;
}

BigInt::BigInt() : m_value(1), bIsNegative(false) {}

BigInt::BigInt(SignedDataSeed Value) : m_value(1)
{
	bIsNegative = Value < 0;
	m_value[0] = bIsNegative ? -Value : Value;
}

BigInt::BigInt(DataSeed Value, bool Sign) : m_value(1)
{
	bIsNegative = Sign;
	m_value[0] = Value;
}

BigInt::BigInt(const std::string& Value) : BigInt()
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

	std::string valToProcess;
	if (firstEl == '+' || firstEl == '-')
	{
		bIsNegative = firstEl == '+' ? false : true;
		valToProcess = Value.substr(1); //from element 1 to end of string
	}
	else
	{
		bIsNegative = false;
		valToProcess = Value;
	}

	for (size_t i = 0; i < valToProcess.length(); ++i)
	{
		DataSeed digit = valToProcess[i] - '0';

		*this *= 10; //advance decimal count for each digit in string
		Sum(digit);
	}
}

BigInt::BigInt(const BigInt& Other) : m_value(Other.m_value), bIsNegative(Other.bIsNegative) {}

BigInt::BigInt(BigInt&& Other) noexcept : m_value(std::move(Other.m_value)), bIsNegative(Other.bIsNegative) {}

BigInt::operator DataSeed() const
{
	return m_value[0];
}

BigInt::operator DoubleCapacityDataSeed() const
{
	DoubleCapacityDataSeed res = 0;
	if (m_value.size() > 1)
	{
		res = m_value[1];
	}

	res <<= sizeof(DataSeed) * CHAR_BIT;
	res |= m_value[0];

	return res;
}

BigInt::operator bool() const {
	return !m_value.empty();
}

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
	Multiply(rhs);
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

BigInt& BigInt::operator&=(const BigInt& rhs)
{
	//execute & based on my length, if rhs is longer ignore the next part
	for (size_t index = 0; index < m_value.size(); ++index)
	{
		DataSeed rhsIndexedValue = index < rhs.m_value.size() ? rhs.m_value[index] : 0;

		m_value[index] &= rhsIndexedValue;
	}

	return *this;
}

BigInt& BigInt::operator|=(const BigInt& rhs)
{
	//execute | based on my length, if rhs is longer ignore the next part
	for (size_t index = 0; index < m_value.size(); ++index)
	{
		DataSeed rhsIndexedValue = index < rhs.m_value.size() ? rhs.m_value[index] : 0;

		m_value[index] |= rhsIndexedValue;
	}

	return *this;
}

BigInt& BigInt::operator^=(const BigInt& rhs)
{
	//execute ^(xor) based on my length, if rhs is longer ignore the next part
	for (size_t index = 0; index < m_value.size(); ++index)
	{
		DataSeed rhsIndexedValue = index < rhs.m_value.size() ? rhs.m_value[index] : 0;

		m_value[index] ^= rhsIndexedValue;
	}

	return *this;
}

BigInt& BigInt::operator<<=(const BigInt& rhs)
{
	//manage edge cases: this is negative, rhs is negative, m_value is 0
	if (bIsNegative || rhs <= 0)
		return *this;

	BigInt ShiftTotal(rhs);
	constexpr size_t MaxShiftForEntry = sizeof(DataSeed) * CHAR_BIT;

	while (ShiftTotal > 0)
	{
		//CurrentShift is a portion of ShiftTotal (max sizeof(DataSeed) * CHAR_BIT)
		//shift total size of CurrentShift
		DataSeed CurrentShift = (ShiftTotal >= MaxShiftForEntry) ? MaxShiftForEntry : ShiftTotal.m_value[0];

		ShiftTotal -= MaxShiftForEntry;

		DataSeed depositary = 0;
		for (size_t i = 0; i < m_value.size(); ++i)
		{
			UOperationResult opResult{ m_value[i] };

			opResult.res <<= CurrentShift;
			opResult.res |= depositary;

			m_value[i] = opResult.GetLowerHalf();
			depositary = opResult.GetHighHalf();
		}

		if (depositary > 0)
		{
			m_value.push_back(depositary); //add to tail
		}
	}

	return *this;
}

BigInt& BigInt::operator>>=(const BigInt& rhs)
{
	//manage edge cases: this is negative, rhs is negative, m_value is 0
	if (bIsNegative || rhs <= 0) return *this;

	BigInt ShiftTotal(rhs);
	constexpr size_t MaxShiftForEntry = sizeof(DataSeed) * CHAR_BIT;

	while (ShiftTotal > 0)
	{
		//CurrentShift is a portion of ShiftTotal (max sizeof(DataSeed) * CHAR_BIT)
		//shift total size of CurrentShift
		DataSeed CurrentShift = (ShiftTotal < MaxShiftForEntry) ? ShiftTotal.m_value[0] : MaxShiftForEntry;
		DataSeed depositary = 0;
		for (long long i = m_value.size() - 1; i >= 0; --i)
		{
			UOperationResult opResult{ m_value[i] };

			opResult.res <<= MaxShiftForEntry; //move value to the left 
			opResult.res >>= CurrentShift; //shift right

			m_value[i] = opResult.GetHighHalf();
			m_value[i] |= depositary;
			depositary = opResult.GetLowerHalf(); //save rest for previous element in m_value 
		}

		if (m_value.size() > 1 && m_value.back() == 0)
		{
			m_value.pop_back();
		}

		ShiftTotal -= MaxShiftForEntry;
	}

	return *this;
}

BigInt::ComparationResult BigInt::AbsCompareWith(const BigInt& Other) const
{
	if (m_value.size() != Other.m_value.size())
		return m_value.size() > Other.m_value.size() ? ComparationResult::Greater : ComparationResult::Less;

	assert(m_value.size() == Other.m_value.size());

	for (long long index = m_value.size() - 1; index >= 0; index--)
	{
		if (m_value[index] == Other.m_value[index])
			continue;
		return m_value[index] > Other.m_value[index] ? ComparationResult::Greater : ComparationResult::Less;
	}

	return ComparationResult::Equals;
}

BigInt::ComparationResult BigInt::CompareWith(const BigInt& Other) const
{
	if (bIsNegative != Other.bIsNegative)
	{
		return (!bIsNegative && Other.bIsNegative) ? ComparationResult::Greater : ComparationResult::Less;
	}

	assert(bIsNegative == Other.bIsNegative);

	ComparationResult res = AbsCompareWith(Other);

	//if they're both negative, means we have to invert the result because the closer a number is to 0 the greater it is
	if(!bIsNegative) return res;

	assert(bIsNegative);

	if(res == ComparationResult::Greater) return ComparationResult::Less;
	else if (res == ComparationResult::Less) return ComparationResult::Greater;

	return res; //they're negative and equals
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
	return b < a;
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

BigInt operator~(const BigInt& a)
{
	BigInt res(a);

	for (size_t i = 0; i < res.m_value.size(); ++i)
		res.m_value[i] = ~res.m_value[i];

	return res;
}

BigInt operator&(const BigInt& a, const BigInt& b)
{
	BigInt res(a);
	res &= b;

	return res;
}

BigInt operator|(const BigInt& a, const BigInt& b)
{
	BigInt res(a);
	res |= b;

	return res;
}

BigInt operator^(const BigInt& a, const BigInt& b)
{
	BigInt res(a);
	res ^= b;

	return res;
}

BigInt operator<<(const BigInt& a, const BigInt& b)
{
	BigInt res(a);
	res <<= b;

	return res;
}

BigInt operator>>(const BigInt& a, const BigInt& b)
{
	BigInt res(a);
	res >>= b;

	return res;
}

BigInt pow(const BigInt& base, const BigInt& exp)
{
	BigInt _base(base);
	BigInt _exp(exp);
	BigInt result(1);

	while (_exp > 0)
	{
		if (_exp.isOdd())
		{
			result *= _base; //apply accumulated base
		}

		_exp >>= 1; //equivalent to dividing by two
		if(_exp > 0)
		{
			_base *= _base; //accumulate base
		}
	}

	return result;
}

/** Make us of a vector<string> to hold all digits and sign */
std::ostream& operator<<(std::ostream& os, const BigInt& value)
{
	std::vector<std::string> out;

	BigInt a{ value };
	a.bIsNegative = false; // make positive to avoid undefined behavior in division

	constexpr BigInt::DoubleCapacityDataSeed divisor = max_power10(); //divisor is used to "Pop" elements from left to right in m_value
	while (a > divisor) //every iteration it returns the exact value in decimal of the current a.m_value[0]
	{
		const BigInt rest = a.Divide(divisor);
		const std::string resultDigit = ToStringFormatted(rest.m_value[0]);

		out.push_back(resultDigit);
	}

	const std::string resultDigit = std::to_string(a.m_value[0]); //value remaining from original tail
	out.push_back(resultDigit);

	if (value.bIsNegative)
	{
		out.push_back("-");
	}

	std::reverse(out.begin(), out.end());

	for (size_t i = 0; i < out.size(); i++)
	{
		os << out[i];
	}

	return os;
}

bool BigInt::isOdd() const
{
	return m_value[0] & 1;
}

bool BigInt::isEven() const
{
	return !isOdd();
}