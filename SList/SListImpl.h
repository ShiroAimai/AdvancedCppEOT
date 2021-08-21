#pragma once
#include "SList.h"
#include <algorithm>
#include <cassert>

#pragma region SLIST SORT UTILS
template<class T>
typename SList<T>::size_type SList<T>::list_size_from(const_iterator start) const
{
	size_type size = 0;
	for (; start.m_ptr != nullptr; ++start)
	{
		++size;
	}

	return size;
}

template<class T>
template<class Comparator>
void SList<T>::merge_sort(iterator& it, Comparator comp)
{
	if(it.m_ptr == nullptr || it.m_ptr->Next == nullptr) return; //no sort can be applied

	size_type half_list_size = list_size_from(it) / 2;

	iterator it_half_list = it; //last node in low half list
	for (size_type i = 0; i < (half_list_size - 1); ++i)
	{
		++it_half_list; 
	}

	const_iterator cit_low_half_list_end = it_half_list;
	++it_half_list;
	cit_low_half_list_end.m_ptr->Next = nullptr; //split half lists

	merge_sort(it, comp);
	merge_sort(it_half_list, comp);

	//merge sorted sub_lists
	iterator itSorted(nullptr);
	if (comp(*it, *it_half_list))
	{
		itSorted = it;
		++it;
	}
	else
	{
		itSorted = it_half_list;
		++it_half_list;
	}

	iterator StartSortedList = itSorted;
	const_iterator cend = nullptr;

	for (; it != cend && it_half_list != cend;)
	{
		if (comp(*it, *it_half_list))
		{
			itSorted.m_ptr->Next = it.m_ptr;
			++itSorted;
			++it;
		}
		else
		{
			itSorted.m_ptr->Next = it_half_list.m_ptr;
			++itSorted;
			++it_half_list;
		}
	}

	//check if sublists were of different length, if yes add in queue
	if (it != cend)
	{
		itSorted.m_ptr->Next = it.m_ptr;
	}
	else if(it_half_list != cend)
	{
		itSorted.m_ptr->Next = it_half_list.m_ptr;
	}

	it = StartSortedList;
}

#pragma endregion
template<class T>
SList<T>::SList() : before_head(new Node()), tail(nullptr)
{

}

template<class T>
SList<T>::~SList()
{
	if (!before_head.m_ptr || before_head.m_ptr->Next == nullptr) return;

	delete before_head.m_ptr;
}

template<class T>
SList<T>::SList(SList::size_type n) : SList()
{
	insert_after(before_head, n, value_type());
}

template<class T>
SList<T>::SList(size_type n, const value_type& val) : SList()
{
	insert_after(before_head, n, val);
}

template<class T>
template<class InputIterator>
SList<T>::SList(InputIterator first, InputIterator last) : SList()
{
	insert_after(before_head, first, last);
}

template<class T>
SList<T>::SList(std::initializer_list<SList<T>::value_type> il) : SList()
{
	insert_after(before_head, il);
}

template<class T>
SList<T>::SList(const SList& list) : SList()
{
	insert_after(before_head, list.cbegin(), list.cend());
}

template<class T>
SList<T>::SList(SList&& list) : before_head(nullptr), tail(nullptr)
{
	std::swap(before_head, list.before_head);
	std::swap(tail, list.tail);
}

template<class T>
typename SList<T>::iterator SList<T>::before_begin() noexcept
{
	return before_head;
}

template<class T>
typename SList<T>::const_iterator SList<T>::cbefore_begin() const noexcept
{
	return before_head;
}

template<class T>
typename SList<T>::iterator SList<T>::begin() noexcept
{
	iterator begin = before_head;
	++begin;
	return begin;
}

template<class T>
typename SList<T>::const_iterator SList<T>::cbegin() const noexcept
{
	const_iterator cbegin = cbefore_begin();
	cbegin++;
	return cbegin;
}

template<class T>
typename SList<T>::iterator SList<T>::end() noexcept
{
	if(tail.m_ptr)
	{
		return iterator(tail.m_ptr->Next);
	}

	return tail; //here tail is an invalid iterator, should not be dereferenced
}

template<class T>
typename SList<T>::const_iterator SList<T>::cend() const noexcept
{
	if (tail.m_ptr)
	{
		return const_iterator(tail.m_ptr->Next);
	}

	return const_iterator(tail.m_ptr); //here tail is an invalid iterator, should not be dereferenced
}

template<class T>
typename SList<T>& SList<T>::operator=(const SList& other)
{
	if (this != &other)
	{
		iterator lastUpdateIt = before_head;
		iterator it = before_head;
		iterator oIt = other.before_head;

		//copy the available elements from other in this
		//if either other or this equals to end stop
		const_iterator itEnd = cend();
		const_iterator otherEnd = other.cend();
		for (; it != itEnd && oIt != otherEnd; ++it, ++oIt)
		{
			*it = *oIt;
			lastUpdateIt = it;
		}

		//this has less elements than other, add the remainings in other
		if (it == itEnd && oIt != otherEnd)
		{
			insert_after(lastUpdateIt, oIt, otherEnd);
		}
		//this has more elems, delete the remainings in this
		else if (it != itEnd && oIt == otherEnd)
		{
			erase_after(lastUpdateIt, itEnd);
		}
	}

	return *this;
}

template<class T>
typename SList<T>& SList<T>::operator=(SList&& other)
{
	if (this != &other)
	{
		std::swap(before_head, other.before_head); //swap heads
		std::swap(tail, other.tail); //swap tails
	}

	return *this;
}

template<class T>
typename SList<T>& SList<T>::operator=(std::initializer_list<value_type> il)
{
	iterator it = begin();
	iterator lastUpdateIt = before_head;
	typename std::initializer_list<value_type>::iterator ilIt = il.begin();

	const_iterator itEnd = cend();
	const_iterator ilEnd = il.end();
	//copy the available elements from other in this
	//if either other or this equals to end stop
	for (; it != itEnd && ilIt != ilEnd; ++it, ++ilIt)
	{
		*it = *ilIt;
		lastUpdateIt = it;
	}

	//this has less elements than other, add the remainings in other
	if (it == itEnd && ilIt != ilEnd)
	{
		insert_after(lastUpdateIt, ilIt, ilEnd);
	}
	//this has more elems, delete the remainings in this
	else if (it != itEnd && ilIt == ilEnd)
	{
		erase_after(lastUpdateIt, itEnd);
	}

	return *this;
}

template<class T>
bool SList<T>::empty() const noexcept
{
	return cbegin() == cend();
}

template<class T>
typename SList<T>::size_type SList<T>::max_size() const noexcept
{
	//total_size / el_size;
	return std::numeric_limits<SList<T>::size_type>::max() / sizeof(SList<T>::Node);
}

template<typename T>
typename SList<T>::reference SList<T>::front()
{
	return *begin();
}

template<class T>
typename SList<T>::const_reference SList<T>::front() const
{
	return *cbegin();
}

template<class T>
template<class InputIterator>
void SList<T>::assign(InputIterator first, InputIterator last)
{
	clear();
	insert_after(before_head, first, last);
}

template<class T>
void SList<T>::assign(size_type n, const value_type& val)
{
	clear();
	insert_after(before_head, n, val);
}

template<class T>
void SList<T>::assign(std::initializer_list<value_type> il)
{
	assign(il.begin(), il.end());
}

template<class T>
template<class... Args>
void SList<T>::emplace_front(Args&&... args)
{
	insert_after(before_head, value_type(std::forward<Args>(args)...));
}

template<class T>
template <class... Args>
typename SList<T>::iterator SList<T>::emplace_after(const_iterator position, Args&&... args)
{
	return insert_after(position, value_type(std::forward<Args>(args)...));
}

template<class T>
void SList<T>::push_front(const value_type& val)
{
	insert_after(before_head, val);
}

template<class T>
void SList<T>::push_front(value_type&& val)
{
	insert_after(before_head, std::forward<value_type>(val));
}

template<class T>
void SList<T>::pop_front()
{
	erase_after(before_head);
}

template<class T>
typename SList<T>::iterator SList<T>::insert_after(const_iterator position, const value_type& val)
{
	iterator prevIt(position.m_ptr);
	const_iterator NextPos = position;
	++NextPos;

	Node* NewEl = new Node(val, NextPos.m_ptr);
	prevIt.m_ptr->Next = NewEl;
	
	if (NextPos.m_ptr == nullptr)
	{
		tail.m_ptr = NewEl; //new tail is the new element
	}

	return iterator(NewEl);
}

template<class T>
typename SList<T>::iterator SList<T>::insert_after(const_iterator position, value_type&& val)
{
	iterator prevIt = iterator(position.m_ptr);
	const_iterator NextPos = position;
	++NextPos;

	Node* NewEl = new Node();
	NewEl->info = std::move(val);
	NewEl->Next = NextPos.m_ptr;
	
	prevIt.m_ptr->Next = NewEl;

	if (NextPos.m_ptr == nullptr)
	{
		tail.m_ptr = NewEl; //new tail is the new element
	}

	return iterator(NewEl);
}

template<class T>
typename SList<T>::iterator SList<T>::insert_after(const_iterator position, size_type n, const value_type& val)
{
	for (size_type i = 0; i < n; ++i, ++position)
	{
		insert_after(position, val);
	}

	return iterator(position.m_ptr);
}

template<class T>
template <class InputIterator>
typename SList<T>::iterator SList<T>::insert_after(const_iterator position, InputIterator first, InputIterator last)
{
	for (; first != last; ++first, ++position)
	{
		insert_after(position, *first);
	}

	return iterator(position.m_ptr);
}

template<class T>
typename SList<T>::iterator SList<T>::insert_after(const_iterator position, std::initializer_list<value_type> il)
{
	return insert_after(position, il.begin(), il.end());
}

template<class T>
typename SList<T>::iterator SList<T>::erase_after(const_iterator position)
{
	const_iterator prevIt = position;
	const_iterator it = position;
	++it;

	//if element to erase is current tail then new tail is prev
	if (it == tail) 
	{
		Node* tailPtr = prevIt != cbefore_begin() ? prevIt.m_ptr : nullptr;
		tail.m_ptr = tailPtr;
	}

	if (it.m_ptr != nullptr)
	{
		prevIt.m_ptr->Next = it.m_ptr->Next;
		Node* NodeToErase = it.m_ptr;

		++it;

		NodeToErase->Next = nullptr;
		delete NodeToErase;
	}

	return iterator(it.m_ptr);
}

template<class T>
typename SList<T>::iterator SList<T>::erase_after(const_iterator position, const_iterator last)
{
	const_iterator eraseResult = position;
	for (; eraseResult != last;)
	{
		eraseResult = erase_after(position);
	}

	assert(eraseResult == last);

	return iterator(eraseResult.m_ptr);
}

template<class T>
void SList<T>::swap(SList& other)
{
	std::swap(before_head, other.before_head);
	std::swap(tail, other.tail);
}

template<class T>
void SList<T>::resize(size_type n)
{
	resize(n, value_type());
}

template<class T>
void SList<T>::resize(size_type n, const value_type& val)
{
	size_type list_size = 0;

	SList<T>::iterator it = begin();
	SList<T>::iterator lastIt = before_head;
	for (; it != end() && list_size < n; ++it)
	{
		++list_size;
		lastIt = it;
	}

	//we do not consider case in which list_size  == n, we do nothing in that case
	if (list_size < n) // add (n - list_size) elements
	{
		size_t SizeDiff = n - list_size;
		insert_after(lastIt, SizeDiff, val);
	}
	else //erase all elements from lastIt to tail
	{
		erase_after(lastIt, end());
	}
}

template<class T>
void SList<T>::clear() noexcept
{
	erase_after(cbefore_begin(), cend());
}

template<class T>
void SList<T>::remove(const value_type& value)
{
	SList<T>::iterator it = begin();
	SList<T>::iterator lastIt = before_head;
	for (; it != end();)
	{
		if (*it == value)
		{
			//erase element after lastIt
			//assign iterator to element after canceled element to it
			it = erase_after(lastIt);
		}
		else {
			lastIt = it;
			++it;
		}
	}
}

template<class T>
template <class Predicate>
void SList<T>::remove_if(Predicate pred)
{
	SList<T>::iterator it = begin();
	SList<T>::iterator lastIt = before_head;
	for (; it != end();)
	{
		if (pred(*it))
		{
			//erase element after lastIt
			//assign iterator to element after canceled element to it
			it = erase_after(lastIt);
		}
		else {
			lastIt = it;
			++it;
		}
	}
}

template<class T>
void SList<T>::reverse() noexcept
{
	iterator it = begin();

	if (it == end() || it == tail) return; //there's nothing to reverse

	iterator prevIt = it;
	++it;
	prevIt.m_ptr->Next = nullptr;
	tail = prevIt;

	before_head.m_ptr->Next = nullptr;
	iterator nextIt(it.m_ptr->Next);
	iterator _end = end();
	
	for (; it != _end;)
	{
		it.m_ptr->Next = prevIt.m_ptr;
		prevIt = it;
		it = nextIt;
		if (it != _end)
		{
			nextIt = it.m_ptr->Next;
		}
	}

	before_head.m_ptr->Next = prevIt.m_ptr;
}

template<class T>
void SList<T>::unique()
{
	SList<T>::iterator it = begin();
	SList<T>::iterator lastIt = it;
	++it;
	for (; it != end();)
	{
		if (*lastIt == *it)
		{
			it = erase_after(lastIt);
		}
		else
		{
			lastIt = it;
			++it;
		}
	}
}

template<class T>
template<class BinaryPredicate>
void SList<T>::unique(BinaryPredicate binary_pred)
{
	SList<T>::iterator it = begin();
	SList<T>::iterator lastIt = it;
	++it;

	for (; it != end();)
	{
		if (binary_pred(*it, *lastIt))
		{
			it = erase_after(lastIt);
		}
		else
		{
			lastIt = it;
			++it;
		}
	}
}

template<class T>
void SList<T>::splice_after(const_iterator position, SList& other)
{
	if (this == &other) return; //splicing the same list

	Node* PrevNextElement = position.m_ptr->Next; //element next to position
	position.m_ptr->Next = other.before_head.m_ptr->Next; //attach other first element next to position
	
	//reset other
	other.before_head.m_ptr->Next = nullptr; 
	other.tail = iterator(nullptr);

	const_iterator it = position;
	while (it.m_ptr->Next != nullptr)
	{
		++it;
	}

	it.m_ptr->Next = PrevNextElement;
	if (position == tail)
	{
		tail.m_ptr = it.m_ptr; //new tail is last element of other
	}
}

template<class T>
void SList<T>::splice_after(const_iterator position, SList&& other)
{
	if (this == &other) return; //splicing the same list
	
	Node* PrevNextElement = position.m_ptr->Next; //element next to position
	position.m_ptr->Next = std::move(other.before_head.m_ptr->Next); //moving other first element next to position
	
	//reset other
	other.before_head.m_ptr->Next = nullptr;
	other.tail = nullptr;

	const_iterator it = position;
	while (it.m_ptr->Next != nullptr)
	{
		++it;
	}

	it.m_ptr->Next = PrevNextElement;
	if (position == tail)
	{
		tail.m_ptr = it.m_ptr; //new tail is last element of other
	}
}

/*
template<class T>
void SList<T>::splice_after(const_iterator position, SList& other, const_iterator i)
{
	Node* OtherElementToMove = i.m_ptr->Next;
	Node* PrevNextElement = position.m_ptr->Next;

	if(OtherElementToMove)
	{
		position.m_ptr->Next = OtherElementToMove; //attach other element at i
		i.m_ptr->Next = OtherElementToMove->Next;
		OtherElementToMove->Next = PrevNextElement;
	}
	else
	{
		i.m_ptr->Next = nullptr; //if OtherElementToMove was the last element in other, then i is the new last element
	}
}

template<class T>
void SList<T>::splice_after(const_iterator position, SList&& other, const_iterator i)
{
	Node* OtherElementToMove = i.m_ptr->Next;
	Node* PrevNextElement = position.m_ptr->Next;

	if (OtherElementToMove)
	{
		position.m_ptr->Next = OtherElementToMove; //attach other element at i
		i.m_ptr->Next = OtherElementToMove->Next;
		OtherElementToMove->Next = PrevNextElement;
	}
	else
	{
		i.m_ptr->Next = nullptr; //if OtherElementToMove was the last element in other, then i is the new last element
	}
}

template<class T>
void SList<T>::splice_after(const_iterator position, SList& other, const_iterator first, const_iterator last);
template<class T>
void SList<T>::splice_after(const_iterator position, SList&& other, const_iterator first, const_iterator last);
*/
template<class T>
void SList<T>::sort()
{
	sort(std::less<value_type>());
}

template<class T>
template<class Comparator>
void SList<T>::sort(Comparator comp)
{
	iterator NewBegin = begin();
	merge_sort(NewBegin, comp);
	before_head.m_ptr->Next = NewBegin.m_ptr;

	if(NewBegin.m_ptr)
	{
		tail = NewBegin;
		for (; tail.m_ptr->Next != nullptr;)
		{
			++tail;	
		}
	}
}


template<class T>
void SList<T>::merge(SList& other)
{
	splice_after(tail, other);
	sort();
}

template<class T>
void SList<T>::merge(SList&& other)
{
	splice_after(tail, std::forward<SList<value_type>>(other));
	sort();
}

template<class T>
template<class Comparator>
void SList<T>::merge(SList& other, Comparator comp)
{
	splice_after(tail, other);
	sort(comp);
}

template<class T>
template<class Comparator>
void SList<T>::merge(SList&& other, Comparator comp)
{
	splice_after(tail, std::forward<SList<value_type>>(other));
	sort(comp);
}