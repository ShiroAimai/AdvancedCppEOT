#pragma once
#include <iostream>
#include "BigInt.h"
#include <bitset>
#include <cassert>

using std::cout;
using std::endl;

int main()
{
	BigInt b("-22222223333333");
	BigInt::DoubleCapacityDataSeed res = (BigInt::DoubleCapacityDataSeed)b;

	cout << res;
}