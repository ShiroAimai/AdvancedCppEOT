#pragma once

//TEST MODES
#define MM_TESTS 
#define GLOBAL_OP_OVERLOAD
#define LARGE_OBJ_TEST
#define STL_ALLOCATOR
#define BOTH_ALLOC_USED
#define ARRAY_TEST

#include <iostream>
#include "ShirosMemoryManager.h"
#include "ShirosSTLAllocator.h"
#include <ctime>
#include <chrono>
#include <cassert>

#ifdef GLOBAL_OP_OVERLOAD
#define GLOBAL_SHIRO_MM
#include "GlobalShirosMemoryManager.h"
#endif

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

using std::cout;
using std::endl;


//24 bytes
struct SmallObjTest {
	long long a;
	float b;
	float d;
	short c;
};

//2052 bytes
struct LargeObjTest {
	long a;
	char b[2048];
};


void SmallObjAllocPerformanceTest(ShirosMemoryManager& Instance)
{
	cout << "====== SmallObjects PERFORMANCE TEST ======" << endl;

	constexpr int TestSize = 1000000;
	Instance.PrintMemoryState();

	std::vector<SmallObjTest*> PointersToSmallObjTest;
	auto start_millisec = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	for (int i = 0; i < TestSize; ++i)
	{
		SmallObjTest* ptr = MM_NEW(alignof(SmallObjTest)) SmallObjTest();
		PointersToSmallObjTest.push_back(ptr);
	}
	Instance.PrintMemoryState();
	for (int i = 0; i < TestSize; ++i)
	{
		MM_DELETE(PointersToSmallObjTest[i], sizeof(SmallObjTest));
	}
	auto end_millisec = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	long long delta = end_millisec - start_millisec;
	Instance.PrintMemoryState();
	cout << "ShirosMemoryManager takes :" << std::to_string((float)delta / 1000) << " to complete" << endl; //convert to seconds

	std::vector<SmallObjTest*> PointersToSmallObjTest2;

	start_millisec = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	for (int i = 0; i < TestSize; ++i)
	{
		SmallObjTest* ptr = new SmallObjTest();
		PointersToSmallObjTest2.push_back(ptr);
	}
	for (int i = 0; i < TestSize; ++i)
	{
		delete PointersToSmallObjTest2[i];
	}
	end_millisec = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	delta = end_millisec - start_millisec;
	cout << "Default Allocator takes :" << std::to_string((float)delta / 1000) << " to complete" << endl; //convert to seconds

	cout << "====== END OF SmallObjects PERFORMANCE TEST ======" << endl;
}

void LargeObjAlloctPerformanceTest(ShirosMemoryManager& Instance)
{
	cout << "====== LargeObjects PERFORMANCE TEST ======" << endl;

	constexpr int TestSize = 10000;
	Instance.PrintMemoryState();

	std::vector<LargeObjTest*> PointersToSmallObjTest;
	auto start_millisec = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	for (int i = 0; i < TestSize; ++i)
	{
		LargeObjTest* ptr = MM_NEW(alignof(LargeObjTest)) LargeObjTest();
		PointersToSmallObjTest.push_back(ptr);
	}
	Instance.PrintMemoryState();
	for (int i = 0; i < TestSize; ++i)
	{
		MM_DELETE(PointersToSmallObjTest[i], sizeof(LargeObjTest));
	}
	auto end_millisec = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	long long delta = end_millisec - start_millisec;
	Instance.PrintMemoryState();
	cout << "ShirosMemoryManager takes :" << std::to_string((float)delta / 1000) << " to complete" << endl; //convert to seconds

	std::vector<LargeObjTest*> PointersToSmallObjTest2;

	start_millisec = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	for (int i = 0; i < TestSize; ++i)
	{
		LargeObjTest* ptr = new LargeObjTest();
		PointersToSmallObjTest2.push_back(ptr);
	}
	for (int i = 0; i < TestSize; ++i)
	{
		delete PointersToSmallObjTest2[i];
	}
	end_millisec = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	delta = end_millisec - start_millisec;
	cout << "Default Allocator takes :" << std::to_string((float)delta / 1000) << " to complete" << endl; //convert to seconds

	cout << "====== END OF LargeObjects PERFORMANCE TEST ======" << endl;

}

void MMPerformanceTest()
{
	cout << "====== MM PERFORMANCE TEST ======" << endl;

	ShirosMemoryManager& Instance = ShirosMemoryManager::Get();

	SmallObjAllocPerformanceTest(Instance);
	LargeObjAlloctPerformanceTest(Instance);

	cout << "====== END OF MM PERFORMANCE TEST ======" << endl;
}

void CheckMemoryLeak() {
	cout << "====== MEMORY LEAK TEST ======" << endl;
	ShirosMemoryManager& Instance = ShirosMemoryManager::Get();

	Instance.PrintMemoryState();
	
	{
		MM_NEW(alignof(SmallObjTest)) SmallObjTest();
	}

	Instance.PrintMemoryState();
	
	assert(Instance.GetCurrentlyUsedMemory() && Instance.GetMemoryRequested() > Instance.GetMemoryFreed() && "Memory Leak identified");

	Instance.Reset();

	Instance.PrintMemoryState();

	cout << "====== END OF MEMORY LEAK TEST ======" << endl;
}

void CheckStandardBehavior()
{
	cout << "====== STANDARD BEHAVIOR TEST ======" << endl;
	ShirosMemoryManager& Instance = ShirosMemoryManager::Get();

	Instance.PrintMemoryState();
	SmallObjTest* ptr = MM_NEW(alignof(SmallObjTest)) SmallObjTest();
	Instance.PrintMemoryState();
	MM_DELETE(ptr, sizeof(SmallObjTest));
	
	assert(Instance.GetCurrentlyUsedMemory() == 0 && Instance.GetMemoryRequested() == Instance.GetMemoryFreed() && "Memory Allocated and deallocated correctly");
	Instance.PrintMemoryState();

	cout << "====== END OF STANDARD BEHAVIOR TEST ======" << endl;
}

int main()
{
#ifdef MM_TESTS
	/* To properly check this test results disable macro GLOBAL_OP_OVERLOAD
	 * Execute test in Release configuration in order to have a good comparison between default allocator and custom one
	*/
	//MMPerformanceTest(); 
	CheckMemoryLeak();
	CheckStandardBehavior();
#endif
#ifdef GLOBAL_OP_OVERLOAD
	ShirosMemoryManager::Get().PrintMemoryState();
	SmallObjTest* ptr_glob = new SmallObjTest();
	SmallObjTest* ptr_glob2 = new SmallObjTest();
	SmallObjTest* ptr_glob3 = new SmallObjTest();
	SmallObjTest* ptr_glob4 = new SmallObjTest();
	SmallObjTest* ptr_glob5 = new SmallObjTest();
	SmallObjTest* ptr_glob6 = new SmallObjTest();
	SmallObjTest* ptr_glob7 = new SmallObjTest();
	SmallObjTest* ptr_glob8 = new SmallObjTest();
	SmallObjTest* ptr_glob9 = new SmallObjTest();
	SmallObjTest* ptr_glob10 = new SmallObjTest();
	ShirosMemoryManager::Get().PrintMemoryState();
	delete ptr_glob;
	delete ptr_glob2;
	delete ptr_glob3;
	delete ptr_glob4;
	delete ptr_glob5;
	delete ptr_glob6;
	delete ptr_glob7;
	delete ptr_glob8;
	delete ptr_glob9;
	delete ptr_glob10;
	ShirosMemoryManager::Get().PrintMemoryState();

	ShirosMemoryManager::Get().PrintMemoryState();
	SmallObjTest* arr = new SmallObjTest[5];
	SmallObjTest* arr2 = new SmallObjTest[100];
	ShirosMemoryManager::Get().PrintMemoryState();
	delete[] arr;
	delete[] arr2;
	ShirosMemoryManager::Get().PrintMemoryState();

#endif
#ifdef LARGE_OBJ_TEST
	ShirosMemoryManager::Get().PrintMemoryState();

	LargeObjTest* l_ptr = MM_NEW(alignof(LargeObjTest)) LargeObjTest();
	LargeObjTest* l_ptr2 = MM_NEW(alignof(LargeObjTest)) LargeObjTest();
	LargeObjTest* l_ptr3 = MM_NEW(alignof(LargeObjTest)) LargeObjTest();
	LargeObjTest* l_ptr4 = MM_NEW(alignof(LargeObjTest)) LargeObjTest();
	LargeObjTest* l_ptr5 = MM_NEW(alignof(LargeObjTest)) LargeObjTest();
	LargeObjTest* l_ptr6 = MM_NEW(alignof(LargeObjTest)) LargeObjTest();
	LargeObjTest* l_ptr7 = MM_NEW(alignof(LargeObjTest)) LargeObjTest();
	LargeObjTest* l_ptr8 = MM_NEW(alignof(LargeObjTest)) LargeObjTest();

	ShirosMemoryManager::Get().PrintMemoryState();
	
	MM_DELETE(l_ptr, sizeof(LargeObjTest));
	MM_DELETE(l_ptr2, sizeof(LargeObjTest));
	MM_DELETE(l_ptr3, sizeof(LargeObjTest));
	MM_DELETE(l_ptr4, sizeof(LargeObjTest));
	MM_DELETE(l_ptr5, sizeof(LargeObjTest));
	MM_DELETE(l_ptr6, sizeof(LargeObjTest));

	ShirosMemoryManager::Get().PrintMemoryState();

	LargeObjTest* l_ptr9 = MM_NEW(alignof(LargeObjTest)) LargeObjTest();
	LargeObjTest* l_ptr10 = MM_NEW(alignof(LargeObjTest)) LargeObjTest();

	MM_DELETE(l_ptr7, sizeof(LargeObjTest));
	MM_DELETE(l_ptr8, sizeof(LargeObjTest));
	MM_DELETE(l_ptr9, sizeof(LargeObjTest));
	MM_DELETE(l_ptr10, sizeof(LargeObjTest));

	LargeObjTest* l_ptr11= MM_NEW_A(LargeObjTest, 8);
	LargeObjTest* l_ptr12 = MM_NEW_A(LargeObjTest, 4);

	MM_DELETE_A(l_ptr11, 8);
	MM_DELETE_A(l_ptr12, 4);

	ShirosMemoryManager::Get().PrintMemoryState();
#endif
#ifdef BOTH_ALLOC_USED
	ShirosMemoryManager::Get().PrintMemoryState();
	LargeObjTest* ptr = MM_NEW(alignof(LargeObjTest)) LargeObjTest();
	SmallObjTest* ptr1 = MM_NEW(alignof(SmallObjTest)) SmallObjTest();
	SmallObjTest* ptr2 = MM_NEW(alignof(SmallObjTest)) SmallObjTest();
	SmallObjTest* ptr3 = MM_NEW(alignof(SmallObjTest)) SmallObjTest();
	LargeObjTest* ptr4 = MM_NEW(alignof(LargeObjTest)) LargeObjTest();
	SmallObjTest* ptr5 = MM_NEW(alignof(SmallObjTest)) SmallObjTest();
	SmallObjTest* ptr6 = MM_NEW(alignof(SmallObjTest)) SmallObjTest();
	SmallObjTest* ptr7 = MM_NEW(alignof(SmallObjTest)) SmallObjTest();
	LargeObjTest* ptr8 = MM_NEW(alignof(LargeObjTest)) LargeObjTest();
	ShirosMemoryManager::Get().PrintMemoryState();
	MM_DELETE(ptr, sizeof(LargeObjTest));
	MM_DELETE(ptr1, sizeof(SmallObjTest));
	MM_DELETE(ptr2, sizeof(SmallObjTest));
	MM_DELETE(ptr3, sizeof(SmallObjTest));
	MM_DELETE(ptr4, sizeof(LargeObjTest));
	MM_DELETE(ptr5, sizeof(SmallObjTest));
	MM_DELETE(ptr6, sizeof(SmallObjTest));
	MM_DELETE(ptr7, sizeof(SmallObjTest));
	MM_DELETE(ptr8, sizeof(LargeObjTest));
	ShirosMemoryManager::Get().PrintMemoryState();
#endif
#ifdef ARRAY_TEST
	ShirosMemoryManager::Get().PrintMemoryState();
	void* ptr_largeArr = MM_NEW_A(LargeObjTest, 20);
	void* ptr_smallArr = MM_NEW_A(SmallObjTest, 5);
	ShirosMemoryManager::Get().PrintMemoryState();
	MM_DELETE_A(ptr_smallArr, 5);
	MM_DELETE_A(ptr_largeArr, 20);
	ShirosMemoryManager::Get().PrintMemoryState();
#endif
#ifdef STL_ALLOCATOR
	std::vector<SmallObjTest, ShirosSTLAllocator<SmallObjTest>> a;
	ShirosMemoryManager::Get().PrintMemoryState();
	a.push_back(SmallObjTest());
	ShirosMemoryManager::Get().PrintMemoryState();
	a.push_back(SmallObjTest());
	ShirosMemoryManager::Get().PrintMemoryState();
	a.push_back(SmallObjTest());
	ShirosMemoryManager::Get().PrintMemoryState();
	a.clear();
#endif

	return 0;

}