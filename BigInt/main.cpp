#pragma once
#include <iostream>
#include "BigInt.h"
#include <bitset>
#include <cassert>

using std::cout;
using std::endl;

int main()
{
	//+
	{
		cout << "************ Sum test ************" << endl;

		cout << "************ Sum A + B ************" << endl;
		BigInt a("99999999999999");
		cout << "A " << a << endl;
		BigInt b("2");
		cout << "B " << b << endl;;

		BigInt c = a + b;

		BigInt result("100000000000001");

		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);

		cout << "************ Sum A + -B ************" << endl;

		a = BigInt("2");
		cout << "A " << a << endl;
		b = BigInt("-99999999999999");
		cout << "B " << b << endl;

		c = a + b;

		result = BigInt("-99999999999997");
		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);
	}

	//-
	{
		cout << "************ Sub test ************" << endl;

		cout << "************ Sub A - B ************" << endl;

		BigInt a("99999999999999");
		cout << "A " << a << endl;
		BigInt b("99999999999999");
		cout << "B " << b << endl;;

		BigInt c = a - b;

		BigInt result("0");

		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);

		cout << "************ Sub A - -B ************" << endl;

		a = BigInt("99999999999999");
		cout << "A " << a << endl;
		b = BigInt("-99999999999999");
		cout << "B " << b << endl;

		c = a - b;

		result = BigInt("199999999999998");
		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);
	}

	//*
	{
		cout << "************ Multiply test ************" << endl;

		cout << "************ Multiply A * B ************" << endl;

		BigInt a("222333222333");
		cout << "A " << a << endl;
		BigInt b("888666444222");
		cout << "B " << b << endl;;

		BigInt c = a * b;

		BigInt result("197580074123086469209926");

		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);

		cout << "************ Multiply -A * -B ************" << endl;

		a = BigInt("-444222333");
		cout << "A " << a << endl;
		b = BigInt("-888111323");
		cout << "B " << b << endl;

		c = a * b;

		result = BigInt("394518883866776559");
		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);

		cout << "************ Multiply A * -B ************" << endl;

		a = BigInt("555333888999");
		cout << "A " << a << endl;
		b = BigInt("-1024");
		cout << "B " << b << endl;

		c = a * b;

		result = BigInt("-568661902334976");
		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);

		cout << "************ Multiply A * 1 ************" << endl;

		a = BigInt("250333212");
		cout << "A " << a << endl;
		b = BigInt("1");
		cout << "B " << b << endl;

		c = a * b;

		result = BigInt("250333212");
		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);

		cout << "************ Multiply -A * 1 ************" << endl;

		a = BigInt("-333225999413");
		cout << "A " << a << endl;
		b = BigInt("1");
		cout << "B " << b << endl;

		c = a * b;

		result = BigInt("-333225999413");
		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);
	}

	//:
	{
		cout << "************ Division test ************" << endl;

		cout << "************ Division A / B ************" << endl;

		BigInt a("0");
		cout << "A " << a << endl;
		BigInt b("888666444222");
		cout << "B " << b << endl;;

		BigInt c = a / b;

		BigInt result("0");

		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);

		cout << "************ Division Large A / Large B ************" << endl;

		a = BigInt("2000000000000000000000000000000");
		cout << "A " << a << endl;
		b = BigInt("10000000000");
		cout << "B " << b << endl;

		c = a / b;

		result = BigInt("200000000000000000000");
		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);

		cout << "************ Division Small A / Larger B ************" << endl;

		a = BigInt("25");
		cout << "A " << a << endl;
		b = BigInt("500");
		cout << "B " << b << endl;

		c = a / b;

		result = BigInt("0");
		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);

		cout << "************ Division Large A / Small B ************" << endl;

		a = BigInt("999752955823");
		cout << "A " << a << endl;
		b = BigInt("333");
		cout << "B " << b << endl;

		c = a / b;

		result = BigInt("3002261128");
		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);

		cout << "************ Division -A / -B ************" << endl;

		a = BigInt("-888111323");
		cout << "A " << a << endl;
		b = BigInt("-444222292");
		cout << "B " << b << endl;

		c = a / b;

		result = BigInt("1");
		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);

		cout << "************ Division A / -B ************" << endl;

		a = BigInt("555333888999");
		cout << "A " << a << endl;
		b = BigInt("-1024");
		cout << "B " << b << endl;

		c = a / b;

		result = BigInt("-542318250");
		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);

		cout << "************ Division A / 1 ************" << endl;

		a = BigInt("250333212");
		cout << "A " << a << endl;
		b = BigInt("1");
		cout << "B " << b << endl;

		c = a / b;

		result = BigInt("250333212");
		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);

		cout << "************ Division A / -1 ************" << endl;

		a = BigInt("250333212");
		cout << "A " << a << endl;
		b = BigInt("-1");
		cout << "B " << b << endl;

		c = a / b;

		result = BigInt("-250333212");
		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);

		cout << "************ Division -A / 1 ************" << endl;

		a = BigInt("-333225999413");
		cout << "A " << a << endl;
		b = BigInt("1");
		cout << "B " << b << endl;

		c = a / b;

		result = BigInt("-333225999413");
		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);

		cout << "************ Division A / A ************" << endl;

		a = BigInt("222555255397431");
		cout << "A " << a << endl;

		c = a / a;

		result = BigInt("1");
		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);

		cout << "************ Division -A / -A ************" << endl;

		cout << "A " << -a << endl;
		
		c = -a / -a;

		result = BigInt("1");
		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);
	}

	//%
	{
	cout << "************ Module test ************" << endl;
	cout << "************ Module A % B ************" << endl;

	BigInt a{ "10000000012" };
	cout << "A " << a << endl;
	BigInt b = { "100000" };
	cout << "B " << b << endl;

	BigInt c = a % b;

	BigInt result{ "12" };

	cout << "Result: \t" << c << endl;
	cout << "Attended: \t" << result << endl;
	assert(c == result);
	
	cout << "************ Module 0 % B ************" << endl;

	a = BigInt();
	cout << "A " << a << endl;
	cout << "B " << b << endl;

	c = a % b;

	result = a;

	cout << "Result: \t" << c << endl;
	cout << "Attended: \t" << result << endl;
	assert(c == result);
	}

	//Pow
	{
		cout << "************ Pow test ************" << endl;

		cout << "************ Pow A^B ************" << endl;
		BigInt a("1024");
		cout << "A " << a << endl;
		BigInt b("10");
		cout << "B " << b << endl;;

		BigInt c = pow(a, b);

		BigInt result("1267650600228229401496703205376");

		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);

		cout << "************ Pow A^1 ************" << endl;

		a = BigInt("250000000");
		cout << "A " << a << endl;
		b = BigInt("1");
		cout << "B " << b << endl;

		c = pow(a, b);

		result = a;

		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);

		cout << "************ Pow A^0 ************" << endl;

		a = BigInt("250000000");
		cout << "A " << a << endl;
		b = BigInt();
		cout << "B " << b << endl;

		c = pow(a, b);

		result = BigInt("1");

		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);
	}

	//>>
	{
		cout << "************ Shift right test ************" << endl;

		cout << "************ Shift A >> B ************" << endl;

		BigInt a("1000000000000000");
		cout << "A " << a << endl;
		BigInt b("5");
		cout << "B " << b << endl;;

		BigInt c = a >> b;

		BigInt result("31250000000000");

		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);

		cout << "************ Shift A >> 0 ************" << endl;

		cout << "A " << a << endl;
		b = BigInt();
		cout << "B " << b << endl;

		c = a >> b;

		result = a;
		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);
	}

	//<<
	{
		cout << "************ Shift left test ************" << endl;

		cout << "************ Shift A << 1 ************" << endl;

		BigInt a("1000000000000000");
		cout << "A " << a << endl;
		BigInt b("1");
		cout << "B " << b << endl;;

		BigInt c = a << b;

		BigInt result("2000000000000000");

		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);

		cout << "************ Shift A << 0 ************" << endl;

		cout << "A " << a << endl;
		b = BigInt();
		cout << "B " << b << endl;

		c = a << b;

		result = a;
		cout << "Result: \t" << c << endl;
		cout << "Attended: \t" << result << endl;
		assert(c == result);
	}

	//<<
	{
		auto boolToString = [](bool val) {return val ? "true" : "false";};
		cout << "************ Odd/Even test ************" << endl;

		cout << "************ A is odd? ************" << endl;

		BigInt a("1000000000000000");
		cout << "A " << a << endl;
		
		bool result = a.isOdd();
		bool expected = false;

		cout << "Result: \t" << boolToString(result) << endl;
		cout << "Attended: \t" << boolToString(expected) << endl;
		assert(result == expected);

		cout << "************ A is even? ************" << endl;

		cout << "A " << a << endl;

		result = a.isEven();
		expected = true;

		cout << "Result: \t" << boolToString(result) << endl;
		cout << "Attended: \t" << boolToString(expected) << endl;
		assert(result == expected);
	}
}