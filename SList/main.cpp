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
	//DEFAULT CTOR
	{
		cout << "======== DEFAULT CONSTRUCTOR ========" << endl;
		
		SList<int> l;

		print(l.cbegin(), l.cend());

		bool IsEmpty = l.empty();
		bool exptectedResult = true;
		cout << "Result: " << (IsEmpty == exptectedResult ? "Empty" : "Not Empty") << endl;
		cout << "Expected Result: " << (exptectedResult ? "Empty" : "Not Empty") << endl;
		
		assert(IsEmpty == exptectedResult);
		cout << endl;
	}
	//COPY CTOR
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
	//N DEFAULT ELEMENTS CTOR
	{
		cout << "======== N DEFAULT ELEMENTS CONSTRUCTOR ========" << endl;

		SList<int>::size_type count = 15;
		cout << "N = " << count << endl;
		SList<int> l(count);

		print(l.cbegin(), l.cend());

		int result = 0;
		for (SList<int>::const_iterator cit = l.cbegin(); cit != l.cend(); ++cit)
		{
			++result;
		}

		cout << "Result: " << result << endl;
		cout << "Expected Result: " << count << endl;

		assert(result == count);
		for (SList<int>::const_iterator cit = l.cbegin(); cit != l.cend(); ++cit)
		{
			assert(*cit == 0);
		}
		cout << endl;
	}
	//N ELEMENTS COPY OF VAL CTOR
	{
		cout << "======== N ELEMENTS COPY OF VAL CONSTRUCTOR ========" << endl;
		int val = 4;
		cout << "VAL = " << val << endl;
		SList<int>::size_type count = 15;
		cout << "N = " << count << endl;

		SList<int> l(count, val);

		print(l.cbegin(), l.cend());

		int result = 0;
		for (SList<int>::const_iterator cit = l.cbegin(); cit != l.cend(); ++cit)
		{
			++result;
		}

		cout << "Result: " << result << endl;
		cout << "Expected Result: " << count << endl;

		assert(result == count);
		for (SList<int>::const_iterator cit = l.cbegin(); cit != l.cend(); ++cit)
		{
			assert(*cit == val);
		}
		cout << endl;
	}
	//FIRST LAST ITERATOR CTOR
	{
		cout << "======== FIRST LAST ITERATOR CONSTRUCTOR ========" << endl;
		
		SList<int> l1{ 1, 2, 3, 4, 5, 6};

		cout << "ORIGINAL LIST" << endl;
		print(l1.cbegin(), l1.cend());
		
		SList<int> l2(l1.cbegin(), l1.cend());

		cout << "Result: ";
		print(l2.cbegin(), l2.cend());

		cout << "Expected Result: ";
		print(l1.cbegin(), l1.cend());

		SList<int>::const_iterator cit2 = l2.cbegin();
		SList<int>::const_iterator cit1 = l1.cbegin();
		for (; cit2 != l2.cend() && cit1 != l1.cend(); ++cit1, ++cit2)
		{
			assert(*cit2 == *cit1);
		}

		assert(cit2 == l2.cend() && cit1 == l1.cend());

		cout << endl;
	}
	//INITIALIZER LIST CTOR
	{
		cout << "======== INITIALIZER LIST CONSTRUCTOR ========" << endl;
		std::initializer_list<int> il{ 1, 2, 3, 4, 5, 6 };
		cout << "INIT LIST: ";
		print(il.begin(), il.end());

		SList<int> l1(il);

		cout << "Result: ";
		print(l1.cbegin(), l1.cend());

		cout << "Expected Result: ";
		print(il.begin(), il.end());

		std::initializer_list<int>::iterator ilIt = il.begin();
		SList<int>::const_iterator cit1 = l1.cbegin();
		for (; ilIt != il.end() && cit1 != l1.cend(); ++cit1, ++ilIt)
		{
			assert(*ilIt == *cit1);
		}

		assert(ilIt == il.end() && cit1 == l1.cend());

		cout << endl;
	}
}