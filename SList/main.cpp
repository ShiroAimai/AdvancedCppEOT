#pragma once
#include <iostream>
#include "SList.h"

using std::cout;
using std::endl;

template <class ForwardIterator>
void print(ForwardIterator begin, ForwardIterator end)
{
	while (begin != end)
	{
		cout << *begin++ << " ";
	}

	cout << endl;
}

int main()
{
	{
		cout << "======== DEFAULT CONSTRUCTOR ========" << endl;
		
		SList<int> l;

		print(l.begin(), l.end());

		bool IsEmpty = l.empty();
		bool exptectedResult = true;
		cout << "Result: " << (IsEmpty == exptectedResult ? "Empty" : "Not Empty") << endl;
		cout << "Expected Result: " << (exptectedResult ? "Empty" : "Not Empty") << endl;
		
		assert(IsEmpty == exptectedResult);
		cout << endl;
	}

	{
		cout << "======== COPY CONSTRUCTOR ========" << endl;

		SList<int> l1;
		l1.push_front(1);
		l1.push_front(2);
		l1.push_front(3);

		SList<int> l2 = l1;
		assert(!l2.empty());

		SList<int> expectedResult {3, 2, 1};
		
		cout << "Result : ";
		print(l2.cbegin(), l2.cend());
		cout << "Expected Result: ";
		print(expectedResult.cbegin(), expectedResult.cend());

		SList<int>::const_iterator itL2 = l2.cbegin();
		SList<int>::const_iterator itExpectedRes = expectedResult.cbegin();
		for (; itL2 != l2.cend() && itExpectedRes != expectedResult.cend(); ++itExpectedRes, ++itL2)
		{
			assert(*itL2 == *itExpectedRes);
		}

		assert(itL2 == l2.cend() && itExpectedRes == expectedResult.cend());

		cout << endl;
	}
}