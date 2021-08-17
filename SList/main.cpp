#pragma once
#include <iostream>
#include "SList.h"

using std::cout;
using std::endl;


int main()
{
	SList<int> list1 {1,15, 1, 1, 3, 4, 3, 3};
	SList<int> list2{ 2 };

	list1.swap(list2);

	cout << "LIST 1" << endl;
	for (SList<int>::iterator it = list1.begin(); it != list1.end(); ++it)
	{
		cout << *it << endl;
	}
	list2.clear();

	cout << "LIST 2" << endl;
	for (SList<int>::iterator it = list2.begin(); it != list2.end(); ++it)
	{
		cout << *it << endl;
	}
		
}