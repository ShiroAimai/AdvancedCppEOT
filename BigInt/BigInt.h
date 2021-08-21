#pragma once
#include <deque>
#include<iosfwd>
#include <string>

/**
 * This class models an unlimited integer. Can contain an unlimited number of digits (int32)
 *
 * No time efficient for very large integers
 * Space efficiency due to the deque
 * Random access due to the deque
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
	* Unsigned operation result value class
	* Internal use only
	*/
	struct UOperationResult
	{
		DoubleCapacityDataSeed res;

		DataSeed GetLowerHalf() const;
		DataSeed GetHighHalf() const;
	};
private:
	/** Sum other to this, the underlying impl is a sum between two values with same sign. Perform side effect on this*/
	void Sum(const BigInt& Other);
	/** Sub other from this, the underlying impl is a sum between a positive and a negative value. Perform side effect on this*/
	void Sub(const BigInt& Other);
	/** Multiply this for other. Perform side effect on this*/
	void Multiply(const BigInt& Other);
	/** Classic multiply digit by digit. O(n^2). Does not work for large values*/
	//BigInt NaiveMultiply(const BigInt& lhs, const BigInt& rhs);
	/** Based on classic multiply, simplified. Should be O(logn) but our <<= slow it down a bit*/
	BigInt VariantPeasantMultiply(const BigInt& lhs, const BigInt& rhs) const;
	/*Karatsuba alg version. O(n^1.59), Does not work, WIP*/
	//BigInt KaratsubaMultiply(const BigInt& lhs, const BigInt& rhs);
	/*Divide this for other. Perform sideffect on this, returns module of the division*/
	BigInt Divide(const BigInt& rhs);
	/** pair formed by division result and module*/
	std::pair <BigInt, BigInt> DivideNaiveImpl(const BigInt& Other) const;
	/** fast divide. Returns division result and module. It takes O(n) where n is the length of the bit occupied by dividend*/
	std::pair <BigInt, BigInt> DivideFastImpl(const BigInt& Other) const;
	/** get minimum number of bits needed to hold this value*/
	size_t GetBits() const;
	/** Get value (0 = false, 1 = true) for bit at index */
	bool GetBitValueAt(size_t index) const;
public:

	/* ==== CONSTRUCTORS ==== */
	BigInt();
	BigInt(SignedDataSeed Value);
	BigInt(DataSeed Value, bool Sign);
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

	/*ARITHMETIC OPERATORS*/
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

	friend BigInt pow(const BigInt&, const BigInt&);

	/*LOGICAL OPERATORS*/
	friend bool operator<(const BigInt& a, const BigInt& b);
	friend bool operator<=(const BigInt& a, const BigInt& b);
	friend bool operator>(const BigInt& a, const BigInt& b);
	friend bool operator>=(const BigInt& a, const BigInt& b);
	friend bool operator==(const BigInt& a, const BigInt& b);
	friend bool operator!=(const BigInt& a, const BigInt& b);

	/* BITWISE OPERATORS*/
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

	/* OUT OPERATOR */
	friend std::ostream& operator<<(std::ostream& os, const BigInt& value);

	/* ==== UTILITIES ==== */

	enum class ComparationResult {
		Equals,
		Greater,
		Less
	};
	/** Compares this digits with others digits. Does not consider sign*/
	ComparationResult AbsCompareWith(const BigInt& Other) const;
	/** Compares this digits with others digits. Does consider sign*/
	ComparationResult CompareWith(const BigInt& Other) const;
	
	/** Utilities to know if current stored number is odd or event */
	bool isOdd() const;
	bool isEven() const;
};