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
	//MOVE CTOR
	{
		cout << "======== MOVE CONSTRUCTOR ========" << endl;

		SList<int> l1;
		l1.push_front(1);
		l1.push_front(2);
		l1.push_front(3);

		SList<int> l2 = std::move(l1);
		
		assert(!l2.empty());
		assert(l1.empty());

		SList<int> expectedResult{ 3, 2, 1 };

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
	//FRONT AND CONST FRONT
	{
		cout << "======== FRONT ========" << endl;
		
		int First = 1;
		cout << "First El: " << First << endl;
		SList<int> l1 {First, 2, 3};
		const SList<int> constL1{ First, 2, 3 };

		cout << "Result: " << l1.front() << endl;

		cout << "Expected Result: " << First << endl;

		assert(constL1.front() == l1.front() && constL1.front() == First);

		int NewFirst = 4;
		cout << "First El: " << NewFirst << endl;
		l1.push_front(NewFirst);

		cout << "Result: " << l1.front() << endl;

		cout << "Expected Result: " << NewFirst << endl;

		assert(l1.front() == NewFirst);

		cout << endl;
	}
	//CLEAR
	{
		cout << "======== CLEAR ========" << endl;
		SList<int> l1{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		cout << "Initial List:";
		print(l1.cbegin(), l1.cend());

		l1.clear();

		cout << "Result: " << (l1.empty() ? "Empty" : "Not Empty") << endl;

		cout << "Expected Result: Empty" << endl;

		assert(l1.empty() == true);

		cout << endl;
	}
	//ASSIGN FIRST LAST ITERATOR
	{
		cout << "======== FIRST LAST ITERATOR ASSIGN ========" << endl;
		SList<int> l1 { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		cout << "List to assign:";
		print(l1.cbegin(), l1.cend());
		SList<int> l2 {11, 12, 13, 14 };
		cout << "List assigned:";
		print(l2.cbegin(), l2.cend());

		l2.assign(l1.cbegin(), l1.cend());

		cout << "Result: ";
		print(l2.cbegin(), l2.cend());

		cout << "Expected Result: ";
		print(l1.cbegin(), l1.cend());

		SList<int>::const_iterator cit2 = l2.cbegin();
		SList<int>::const_iterator cit1 = l1.cbegin();
		for (; cit2 != l2.end() && cit1 != l1.cend(); ++cit1, ++cit2)
		{
			assert(*cit2 == *cit1);
		}

		assert(cit2 == l2.end() && cit1 == l1.cend());

		cout << endl;
	}
	//ASSIGN N ELEMENTS COPY OF VAL
	{
		cout << "======== ASSIGN N ELEMENTS COPY OF VAL ========" << endl;
		SList<int>::size_type N = 22; 
		cout << "N: " << N << endl;
		int Val = 42;
		cout << "VAL: " << Val << endl;

		SList<int> l2 { 1, 2, 3, 5, 7, 11};
		cout << "Original List: ";
		print(l2.cbegin(), l2.cend());

		l2.assign(N, Val);

		cout << "Result: ";
		print(l2.cbegin(), l2.cend());

		SList<int> expected(N, Val);
		cout << "Expected Result: ";
		print(expected.cbegin(), expected.cend());

		SList<int>::const_iterator cit2 = l2.cbegin();
		SList<int>::const_iterator cExpc = expected.cbegin();
		for (; cit2 != l2.end() && cExpc != expected.cend(); ++cExpc, ++cit2)
		{
			assert(*cit2 == *cExpc);
		}

		assert(cit2 == l2.end() && cExpc == expected.cend());

		cout << endl;
	}
	//ASSIGN INIT LIST
	{
		cout << "======== ASSIGN INIT LIST ========" << endl;
		std::initializer_list<int> il = {10, 100, 1000, 10000};
		cout << "Init List: ";
		print(il.begin(), il.end());

		SList<int> l2{ 1, 2, 3, 5, 7, 11 };
		cout << "Original List: ";
		print(l2.cbegin(), l2.cend());

		l2.assign(il);

		cout << "Result: ";
		print(l2.cbegin(), l2.cend());

		cout << "Expected Result: ";
		print(il.begin(), il.end());

		SList<int>::const_iterator cit2 = l2.cbegin();
		std::initializer_list<int>::iterator ilIt = il.begin();
		for (; cit2 != l2.end() && ilIt != il.end(); ++ilIt, ++cit2)
		{
			assert(*cit2 == *ilIt);
		}

		assert(cit2 == l2.end() && ilIt == il.end());

		cout << endl;
	}
}