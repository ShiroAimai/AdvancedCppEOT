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
		std::cout << "************ Sum test ************" << std::endl;

		std::cout << "************ Sum A + B ************" << std::endl;
		BigInt a("99999999999999");
		cout << "A " << a << endl;
		BigInt b("2");
		cout << "B " << b << endl;;

		BigInt c = a + b;

		BigInt result("100000000000001");

		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);

		std::cout << "************ Sum A + -B ************" << std::endl;

		a = BigInt("2");
		cout << "B " << a << endl;
		b = BigInt("-99999999999999");
		cout << "A " << b << endl;

		c = a + b;

		result = BigInt("-99999999999997");
		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);
	}

	//-
	{
		std::cout << "************ Sub test ************" << std::endl;

		std::cout << "************ Sub A - B ************" << std::endl;

		BigInt a("99999999999999");
		cout << "A " << a << endl;
		BigInt b("99999999999999");
		cout << "B " << b << endl;;

		BigInt c = a - b;

		BigInt result("0");

		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);

		std::cout << "************ Sub A - -B ************" << std::endl;

		a = BigInt("99999999999999");
		cout << "A " << a << endl;
		b = BigInt("-99999999999999");
		cout << "B " << b << endl;

		c = a - b;

		result = BigInt("199999999999998");
		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);
	}

	//*
	{
		std::cout << "************ Multiply test ************" << std::endl;

		std::cout << "************ Multiply A * B ************" << std::endl;

		BigInt a("222333222333");
		cout << "A " << a << endl;
		BigInt b("888666444222");
		cout << "B " << b << endl;;

		BigInt c = a * b;

		BigInt result("197580074123086469209926");

		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);

		std::cout << "************ Multiply -A * -B ************" << std::endl;

		a = BigInt("-444222333");
		cout << "A " << a << endl;
		b = BigInt("-888111323");
		cout << "B " << b << endl;

		c = a * b;

		result = BigInt("394518883866776559");
		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);

		std::cout << "************ Multiply A * -B ************" << std::endl;

		a = BigInt("555333888999");
		cout << "A " << a << endl;
		b = BigInt("-1024");
		cout << "B " << b << endl;

		c = a * b;

		result = BigInt("-568661902334976");
		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);

		std::cout << "************ Multiply A * 1 ************" << std::endl;

		a = BigInt("250333212");
		cout << "A " << a << endl;
		b = BigInt("1");
		cout << "B " << b << endl;

		c = a * b;

		result = BigInt("250333212");
		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);

		std::cout << "************ Multiply -A * 1 ************" << std::endl;

		a = BigInt("-333225999413");
		cout << "A " << a << endl;
		b = BigInt("1");
		cout << "B " << b << endl;

		c = a * b;

		result = BigInt("-333225999413");
		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);
	}

	//:
	{
		std::cout << "************ Division test ************" << std::endl;

		std::cout << "************ Division A / B ************" << std::endl;

		BigInt a("0");
		cout << "A " << a << endl;
		BigInt b("888666444222");
		cout << "B " << b << endl;;

		BigInt c = a / b;

		BigInt result("0");

		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);

		std::cout << "************ Division Large A / Large B ************" << std::endl;

		a = BigInt("2000000000000000000000000000000");
		cout << "A " << a << endl;
		b = BigInt("10000000000");
		cout << "B " << b << endl;

		c = a / b;

		result = BigInt("200000000000000000000");
		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);

		std::cout << "************ Division Small A / Larger B ************" << std::endl;

		a = BigInt("25");
		cout << "A " << a << endl;
		b = BigInt("500");
		cout << "B " << b << endl;

		c = a / b;

		result = BigInt("0");
		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);

		std::cout << "************ Division Large A / Small B ************" << std::endl;

		a = BigInt("999752955823");
		cout << "A " << a << endl;
		b = BigInt("333");
		cout << "B " << b << endl;

		c = a / b;

		result = BigInt("3002261128");
		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);

		std::cout << "************ Division -A / -B ************" << std::endl;

		a = BigInt("-888111323");
		cout << "A " << a << endl;
		b = BigInt("-444222292");
		cout << "B " << b << endl;

		c = a / b;

		result = BigInt("1");
		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);

		std::cout << "************ Division A / -B ************" << std::endl;

		a = BigInt("555333888999");
		cout << "A " << a << endl;
		b = BigInt("-1024");
		cout << "B " << b << endl;

		c = a / b;

		result = BigInt("-542318250");
		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);

		std::cout << "************ Division A / 1 ************" << std::endl;

		a = BigInt("250333212");
		cout << "A " << a << endl;
		b = BigInt("1");
		cout << "B " << b << endl;

		c = a / b;

		result = BigInt("250333212");
		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);

		std::cout << "************ Division A / -1 ************" << std::endl;

		a = BigInt("250333212");
		cout << "A " << a << endl;
		b = BigInt("-1");
		cout << "B " << b << endl;

		c = a / b;

		result = BigInt("-250333212");
		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);

		std::cout << "************ Division -A / 1 ************" << std::endl;

		a = BigInt("-333225999413");
		cout << "A " << a << endl;
		b = BigInt("1");
		cout << "B " << b << endl;

		c = a / b;

		result = BigInt("-333225999413");
		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);

		std::cout << "************ Division A / A ************" << std::endl;

		a = BigInt("222555255397431");
		cout << "A " << a << endl;
		//b = BigInt("222555255397431");
		//cout << "B " << b << endl;

		c = a / a;

		result = BigInt("1");
		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);

		std::cout << "************ Division -A / -A ************" << std::endl;

		cout << "A " << -a << endl;
		
		c = -a / -a;

		result = BigInt("1");
		std::cout << "Result: \t" << c << std::endl;
		std::cout << "Attended: \t" << result << std::endl;
		assert(c == result);
	}
}