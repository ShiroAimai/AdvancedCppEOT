#pragma once
#include <iostream>
#include "BigInt.h"
#include <bitset>
#include <cassert>

using std::cout;
using std::endl;

int main()
{
	BigInt a("222222333333");
	cout << a << endl;
	BigInt b("121212121200");	
	cout << b << endl;

	cout << a / b;
}