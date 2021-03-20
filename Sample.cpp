/*
 * File: Sample.cpp
 * Project: TVJ_Forward_List
 * --------------------------
 *
 * @author: Teddy van Jerry
 * @licence: The MIT Licence
 * @compiler: at least C++/11 (tested on MSVC and MinGW)
 *
 * @version 1.0 2021/03/20
 * - initial version
 *
 */
#include <iostream>
#include <vector>
#include "TVJ_Forward_List.h"
using namespace std;
using namespace tvj; // tvj::forward_list

#ifndef printForwardList
#define printForwardList(list__) \
for (const auto& c_ : list__) {  \
	std::cout << c_ << ' ';      \
}
#endif

int main()
{
	vector<int> vec{ 10,20,24 };
	int arr[9] = { 1, 2, 3 ,3 ,3 ,4, 7, 7, 10 };
	tvj::forward_list<int> list1, list2(vec.cbegin(), vec.cend()), list3(arr, arr + 8);
	list1.push_front(9);
	list1.push_back(-12);
	list1.push_back(7);
	list1.insert_after(list1.begin() + 1, 1024);
	cout << "list1: ";
	printForwardList(list1);
	cout << "\nlist2: ";
	printForwardList(list2);
	cout << "\nlist3: ";
	printForwardList(list3);
	cout << "\nlist1 is " << (list1.sorted() ? "sorted" : "not sorted") << endl;
	list1.sort();
	cout << "list1: ";
	printForwardList(list1);
	cout << "\nlist1 is " << (list1.sorted() ? "sorted" : "not sorted") << endl;
	list3.unique();
	cout << "list3: ";
	printForwardList(list3);
	cout << endl;
	cout << "list1: ";
	printForwardList(list1);
	auto iter = list1.begin();
	list1.merge(list3);
	cout << "\nlist1: ";
	printForwardList(list1);
	// back() is the iterator pointing to the last element
	// which is the one before end()
	list1.insert_after(list1.cbefore_begin(), *(list2.back()));
	list1.insert_after(list1.front() + 4, 9, 5);
	cout << "\nlist1: ";
	printForwardList(list1);
	cout << "\nlist1 contains " << list1.count(9) << " \"9\"s" << endl;
	list1.assign(list1.find(1024), 2048);
	cout << "list1: ";
	printForwardList(list1);
	list1.link(list2);
	cout << "\nlist1: (size:" << list1.size() << ") ";
	printForwardList(list1);
	// erase [begin + 4, begin ++ 8)
	list1.erase_after(list1.begin() + 4, list1.begin() + 8);
	cout << "\nlist1: (size:" << list1.size() << ") ";
	printForwardList(list1);
	list1.clear();
	cout << "\nlist1: (size:" << list1.size() << ") ";
	printForwardList(list1);
	cout << endl;
	return 0;
}

// ALL RIGHTS RESERVED (C) 2021 Teddy van Jerry