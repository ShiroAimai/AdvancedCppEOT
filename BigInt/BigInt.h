#pragma once
#include <deque>
#include<iosfwd>
#include <string>

/**
 * This class models an unlimited integer. Can contains an unlimited number of digits
 * 
 * Not time efficient
 * Space efficiency thanks to the deque
 * Random access thanks to the deque
 */

class BigInt
{
private:
	//convenient typedefs for our class
	//using fixed size integer types
	using DataSeed = uint32_t;
	using SignedDataSeed = int32_t;
	using DoubleCapacityDataSeed = uint64_t;
	using SignedDoubleCapacityDataSeed = int64_t;
	using BIDigits = std::deque<DataSeed>;

	BIDigits m_value; //head (leftmost) indicates the low digital position
	bool bIsNegative; 
	
	/*
	Unsigned operation result type
	* Internal use only
	*/
	struct UOperationResult
	{
		SignedDoubleCapacityDataSeed res;

		DataSeed GetLowerHalf() const;
		DataSeed GetHighHalf() const;
	};
private:
	/*Sum other to this*/
	void Sum(const BigInt& Other);
	/*Sub other from this*/
	void Sub(const BigInt& Other);
	/*Divide this for other*/
	BigInt Divide(const BigInt& Other);

public:
	/* ==== CONSTRUCTORS ==== */
	BigInt();
	BigInt(SignedDataSeed Value);
	BigInt(const std::string& Value);
	BigInt(const BigInt& Other);
	BigInt(BigInt&& Other) noexcept;

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

	/* ==== UTILITIES ==== */
	enum class ComparationResult {
		Equals,
		Greater,
		Less
	};
	ComparationResult CompareWith(const BigInt& Other) const;
};

inline BigInt operator+(const BigInt& a);
inline BigInt operator-(const BigInt& a);
inline BigInt operator+(const BigInt& a, const BigInt& b);
inline BigInt operator-(const BigInt& a, const BigInt& b);
inline BigInt operator*(const BigInt& a, const BigInt& b);
inline BigInt operator/(const BigInt& a, const BigInt& b);
inline BigInt operator%(const BigInt& a, const BigInt& b);
inline bool operator<(const BigInt& a, const BigInt& b);
inline bool operator<=(const BigInt& a, const BigInt& b);
inline bool operator>(const BigInt& a, const BigInt& b);
inline bool operator>=(const BigInt& a, const BigInt& b);
inline bool operator==(const BigInt& a, const BigInt& b);
inline bool operator!=(const BigInt& a, const BigInt& b);