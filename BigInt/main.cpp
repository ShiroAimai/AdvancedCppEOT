#pragma once
#include <iostream>
#include "BigInt.h"
#include <bitset>
#include <cassert>

using std::cout;
using std::endl;

int main()
{
	//cout << pow(BigInt(1024), 10);
	BigInt a("313");
	cout << a << endl;
	BigInt b("350754231312");
	cout << b << endl;
	
	cout << endl << endl;
	BigInt c = a * b;

	cout << c << endl;

	cout << std::bitset<sizeof(BigInt::DataSeed) * CHAR_BIT>(1019) << endl;
	cout << std::bitset<sizeof(BigInt::DataSeed)* CHAR_BIT>(25) << endl;
}