#pragma once
#include <iostream>
#include "SList.h"

using std::cout;
using std::endl;


int main()
{
	SList<int> list1 {1, 2, 3, 5, 9, 11, 12};


	for (SList<int>::iterator it = list1.begin(); it != list1.end(); ++it)
	{
		cout << *it << endl;
	}
		
}