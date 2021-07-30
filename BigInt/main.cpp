#pragma once
#include <iostream>
#include "BigInt.h"
#include <bitset>
#include <cassert>

using std::cout;
using std::endl;

int main()
{
	////+
	//{
	//	std::cout << "\n\n************\nSum test\n************\n" << std::endl;
	//	BigInt a{ "99999999999999" };
	//	BigInt b = { "2" };

	//	BigInt c = a + b;

	//	BigInt result{ "100000000000001" };

	//	std::cout << "Result: \t" << c << std::endl;
	//	std::cout << "Attended: \t" << result << std::endl;
	//	assert(c == result);
	//}

	////-
	//{
	//	std::cout << "\n\n************\nSubtraction test\n************\n" << std::endl;
	//	BigInt a{ "100000000000001" };
	//	BigInt b = { "100000000000002" };

	//	BigInt c = a - b;

	//	BigInt result{ "-1" };

	//	std::cout << "Result: \t" << c << std::endl;
	//	std::cout << "Attended: \t" << result << std::endl;
	//	assert(c == result);
	//}

	////*
	//{
	//	std::cout << "\n\n************\nMultiplication test\n************\n" << std::endl;
	//	BigInt a{ "-1234567" };
	//	BigInt b = { "-7654321" };

	//	BigInt c = a * b;

	//	BigInt result{ "9449772114007" };

	//	std::cout << "Result: \t" << c << std::endl;
	//	std::cout << "Attended: \t" << result << std::endl;
	//	assert(c == result);
	//}

	//*
	//{
	//	std::cout << "\n\n************\nDivision test\n************\n" << std::endl;
	//	BigInt a{ "12345678987" };
	//	BigInt b = { "-123123" };

	//	BigInt c = a / b;

	//	BigInt result{ "-100271" };

	//	std::cout << "Result: \t" << c << std::endl;
	//	std::cout << "Attended: \t" << result << std::endl;
	//	assert(c == result);
	//}

	////<<
	//{
	//	std::cout << "\n\n************\nLeftShift test\n************\n" << std::endl;
	//	BigInt a{ "111111111111" };

	//	BigInt c = a << 2;

	//	BigInt result{ "444444444444" };

	//	std::cout << "Result: \t" << c << std::endl;
	//	std::cout << "Attended: \t" << result << std::endl;
	//	assert(c == result);
	//}

	////>>
	//{
	//	std::cout << "\n\n************\nRightShift test\n************\n" << std::endl;
	//	BigInt a{ "100000000000" };

	//	BigInt c = a >> 2;

	//	BigInt result{ "25000000000" };

	//	std::cout << "Result: \t" << c << std::endl;
	//	std::cout << "Attended: \t" << result << std::endl;
	//	assert(c == result);
	//}

	//pow
	
		std::cout << "\n\n************\nPow test\n************\n" << std::endl;
		BigInt a{ "3" };
		BigInt b{ "30" };

		BigInt c = pow(a, b);

		BigInt result{ "205891132094649" };

		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);

}