#pragma once
#include <deque>
#include<iosfwd>
#include <string>

/**
 * This class models an unlimited integer. Can contains an unlimited number of digits
 *
 * No time efficient
 * Space efficiency thanks to the deque
 * Random access thanks to the deque
 */

class BigInt
{
public:
	//convenient typedefs for our class
		//using fixed size integer types
	using DataSeed = uint32_t;
	using SignedDataSeed = int32_t;
	using DoubleCapacityDataSeed = uint64_t;
	using SignedDoubleCapacityDataSeed = int64_t;
private:
	using BIDigits = std::deque<DataSeed>;
	//core internal state
	BIDigits m_value; //head (leftmost) indicates the low digital position
	bool bIsNegative;

	/*
	Unsigned operation result value class
	* Internal use only
	*/
	struct UOperationResult
	{
		DoubleCapacityDataSeed res;

		DataSeed GetLowerHalf() const;
		DataSeed GetHighHalf() const;
	};
private:
	/*Sum other to this, the underlying impl is a sum between two values with same sign*/
	void Sum(const BigInt& Other);
	/*Sub other from this, the underlying impl is a sum between a positive and a negative value*/
	void Sub(const BigInt& Other);
	/*Divide this for other*/
	BigInt Divide(const BigInt& Other);
	/*pair formed by division result and module*/
	std::pair <BigInt, BigInt> DivideNaiveImpl(const BigInt& Other);
	/* get minimum number of bits needed to hold this value*/
	size_t GetBits() const;
	bool GetBitValueAt(size_t index) const;
public:

	/* ==== CONSTRUCTORS ==== */
	BigInt();
	BigInt(SignedDataSeed Value);
	BigInt(const std::string& Value);
	BigInt(const BigInt& Other);
	BigInt(BigInt&& Other) noexcept;

	explicit operator bool() const;
	explicit operator DataSeed() const;
	explicit operator DoubleCapacityDataSeed() const;

	/* ==== DESTRUCTOR ==== */
	~BigInt();

	/* ==== OPERATORS ==== */
	BigInt& operator=(const BigInt& other);
	BigInt& operator=(BigInt&& other) noexcept;

	BigInt& operator++();
	BigInt operator++(int);
	BigInt& operator--();
	BigInt operator--(int);

	BigInt& operator+=(const BigInt& rhs);
	BigInt& operator-=(const BigInt& rhs);
	BigInt& operator*=(const BigInt& rhs);
	BigInt& operator/=(const BigInt& rhs);
	BigInt& operator%=(const BigInt& rhs);

	friend BigInt operator+(const BigInt& a);
	friend BigInt operator-(const BigInt& a);
	friend BigInt operator+(const BigInt& a, const BigInt& b);
	friend BigInt operator-(const BigInt& a, const BigInt& b);
	friend BigInt operator*(const BigInt& a, const BigInt& b);
	friend BigInt operator/(const BigInt& a, const BigInt& b);
	friend BigInt operator%(const BigInt& a, const BigInt& b);

	friend bool operator<(const BigInt& a, const BigInt& b);
	friend bool operator<=(const BigInt& a, const BigInt& b);
	friend bool operator>(const BigInt& a, const BigInt& b);
	friend bool operator>=(const BigInt& a, const BigInt& b);
	friend bool operator==(const BigInt& a, const BigInt& b);
	friend bool operator!=(const BigInt& a, const BigInt& b);

	BigInt& operator&=(const BigInt& rhs);
	BigInt& operator|=(const BigInt& rhs);
	BigInt& operator^=(const BigInt& rhs);
	BigInt& operator<<=(const BigInt& rhs);
	BigInt& operator>>=(const BigInt& rhs);

	friend BigInt operator~(const BigInt& a);
	friend BigInt operator&(const BigInt& a, const BigInt& b);
	friend BigInt operator|(const BigInt& a, const BigInt& b);
	friend BigInt operator^(const BigInt& a, const BigInt& b);
	friend BigInt operator<<(const BigInt& a, const BigInt& b);
	friend BigInt operator>>(const BigInt& a, const BigInt& b);

	friend BigInt pow(const BigInt&, const BigInt&);

	friend std::ostream& operator<<(std::ostream& os, const BigInt& value);

	/* ==== UTILITIES ==== */
	enum class ComparationResult {
		Equals,
		Greater,
		Less
	};
	ComparationResult AbsCompareWith(const BigInt& Other) const;
	ComparationResult CompareWith(const BigInt& Other) const;
};