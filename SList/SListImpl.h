#pragma once
#include "SList.h"
#include <algorithm>
#include <cassert>

#pragma region Node
template<class T>
SList<T>::Node::Node() : Next(nullptr) {}

template<class T>
SList<T>::Node::Node(value_type _info, Node* _Next) : info(_info), Next(_Next) {}

template<class T>
SList<T>::Node::~Node()
{
	if (Next)
	{
		delete Next;
	}
}
#pragma endregion
#pragma region Iterator
template<class T>
SList<T>::Iterator::Iterator(it_pointer ptr) : m_ptr(ptr) {}

template<class T>
SList<T>::reference SList<T>::Iterator::operator*() const {
	return (*m_ptr).info;
}

template<class T>
SList<T>::pointer SList<T>::Iterator::operator->() {
	return &(m_ptr->info);
}

template<class T>
SList<T>::Iterator& SList<T>::Iterator::operator++() {
	m_ptr = m_ptr->Next;
	return *this;
}
template<class T>
SList<T>::Iterator SList<T>::Iterator::operator++(int) {
	Iterator tmp = *this;
	++(*this);
	return tmp;
}

#pragma endregion

template<class T>
SList<T>::SList() 
{
	before_head = Iterator(new Node());
}

template<class T>
SList<T>::~SList()
{
	if (!head) return;

	delete head;
}

template<class T>
SList<T>::SList(SList::size_type n) : SList()
{
	const value_type default();
	insert_after(before_head, n, default);
}

template<class T>
SList<T>::SList(size_type n, const value_type& val) : SList()
{
	insert_after(before_head, n, val);
}

template<class T>
template<class InputInteractor>
SList<T>::SList(InputInteractor first, InputInteractor last) : SList()
{
	insert_after(before_head, first, last);
}

template<class T>
SList<T>::SList(const SList& list) : SList()
{
	insert_after(before_head, list.begin(), list.end());
}

template<class T>
SList<T>::SList(SList&& list)
{
	before_head = std::move(list.before_head);
	other.before_head = SList<T>::tail;
}

template<class T>
SList<T>::iterator SList<T>::before_begin() noexcept
{
	return before_head;
}

template<class T>
SList<T>::const_iterator SList<T>::cbefore_begin() const noexcept
{
	return before_head;
}

template<class T>
SList<T>::iterator SList<T>::begin() noexcept
{
	iterator begin = before_head;
	++begin;
	return begin;
}

template<class T>
SList<T>::const_iterator SList<T>::cbegin() const noexcept
{
	const_iterator cbegin = begin();
	return cbegin;
}

template<class T>
SList<T>::iterator SList<T>::end() noexcept
{
	return tail;
}

template<class T>
SList<T>::const_iterator SList<T>::cend() const noexcept
{
	return tail;
}

template<class T>
SList<T>& SList<T>::operator=(const SList<T>& other)
{
	if (this != &other)
	{
		iterator lastUpdateIt = before_head;
		iterator it = before_head;
		iterator oIt = other.before_head;

		//copy the available elements from other in this
		//if either other or this equals to end stop
		for (; it != tail && oIt != other.end(); ++it, ++oIt)
		{
			*it = *oIt;
			lastUpdateIt = it;
		}

		//this has less elements than other, add the remainings in other
		if (it == tail && oIt != other.end())
		{
			insert_after(lastUpdateIt, oIt, other.end());
		}
		//this has more elems, delete the remainings in this
		else if (it != tail && oIt == other.end()) 
		{
			erase_after(lastUpdateIt, tail);
		}
	}

	return *this;
}

template<class T>
SList<T>& SList<T>::operator=(SList<T>&& other)
{
	if (this != &other)
	{
		std::swap(before_head, other.before_head); //swap only heads
	}

	return *this;
}

template<class T>
bool SList<T>::empty() const noexcept
{
	iterator it = before_head;
	++it;
	return it == tail;
}

template<class T>
SList<T>::size_type SList<T>::max_size() const noexcept
{
	//total_size / el_size;
	return std::numeric_limits<SList<T>::size_type>::max() / sizeof(SList<T>::Node);
}

template<class T>
SList<T>::reference SList<T>::front()
{
	iterator it = before_head;
	++it;
	return *it;
}

template<class T>
SList<T>::const_reference SList<T>::front() const
{
	iterator it = before_head;
	++it;
	return *it;
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
template<class... Args>
void SList<T>::emplace_front(Args&&... args)
{
	insert_after(before_head, T(std::forward<Args>(args)...))
}

template<class T>
template <class... Args>
SList<T>::iterator SList<T>::emplace_after(const_iterator position, Args&&... args)
{
	return insert_after(position, T(std::forward<Args>(args)...));
}

template<class T>
void SList<T>::push_front(const value_type& val)
{
	insert_after(before_head, val);
}

template<class T>
void SList<T>::push_front(value_type&& val)
{
	insert_after(before_head, std::move(val));
}

template<class T>
void SList<T>::pop_front()
{
	erase_after(before_head);
}

template<class T>
SList<T>::iterator SList<T>::insert_after(const_iterator position, const value_type& val)
{
	iterator prevIt = position;
	iterator nextIt = position;
	++nextIt;
	
	Node* NewEl = new Node(val, nextIt.m_ptr);
	prevIt.m_ptr->Next = NewEl;

	return iterator(NewEl);
}

template<class T>
SList<T>::iterator SList<T>::insert_after(const_iterator position, value_type&& val)
{
	iterator prevIt = position;
	iterator nextIt = position;
	++nextIt;

	Node* NewEl = new Node();
	NewEl->info = std::move(val);
	NewEl->Next = nextIt.m_ptr;

	prevIt.m_ptr->Next = NewEl;

	return iterator(NewEl);
}

template<class T>
SList<T>::iterator SList<T>::insert_after(const_iterator position, size_type n, const value_type& val)
{
	iterator prevIt = position;

	for (size_type i = 0; i < n; ++i, ++prevIt)
	{
		insert_after(prevIt, val);
	}

	return prevIt;
}

template<class T>
template <class InputIterator>
SList<T>::iterator SList<T>::insert_after(const_iterator position, InputIterator first, InputIterator last)
{
	iterator prevIt = position;

	for (iterator startIt = first; first != last; ++first. ++prevIt)
	{
		insert_after(prevIt, *startIt);
	}

	return prevIt;
}

template<class T>
SList<T>::iterator SList<T>::erase_after(const_iterator position)
{
	iterator prevIt = position;
	iterator it = position;
	++it;

	if(it != tail)
	{
		prevIt.m_ptr->Next = it.m_ptr->Next;
		Node* NodeToErase = it.m_ptr;
		
		++it;

		NodeToErase->Next = nullptr;
		delete NodeToErase;
	}

	return it;
}

template<class T>
SList<T>::iterator SList<T>::erase_after(const_iterator position, const_iterator last)
{
	iterator prevIt = position;
	
	for (; prevIt != last; ++prevIt)
	{
		erase_after(prevIt);
	}

	assert(prevIt == last);

	return prevIt;
}

template<class T>
void SList<T>::swap(SList<T>& other)
{
	std::swap(before_head, other.before_head);
}

template<class T>
SList<T>::size_type SList<T>::size() const
{
	size_type list_size = 0;
	for (SList<T>::iterator it = begin(); it != tail; ++it)
	{
		++list_size;
	}
	
	return list_size;
}

template<class T>
void SList<T>::resize(size_type n)
{
	const value_type default();

	resize(n, default);
}

template<class T>
void SList<T>::resize(size_type n, const value_type& val)
{
	size_type list_size = 0;
	
	SList<T>::iterator it = begin();
	SList<T>::iterator lastIt = before_head;
	for (; it != tail && list_size < n; ++it)
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
		erase_after(lastIt, tail);
	}
}

template<class T>
void SList<T>::clear() noexcept
{
	erase_after(before_head, tail);
}

template<class T>
void SList<T>::remove(const value_type& value)
{
	SList<T>::iterator it = begin();
	SList<T>::iterator lastIt = before_head;
	for (; it != tail;)
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
	for (; it != tail;)
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
	
	if(it == tail) return; //there's nothing to reverse

	iterator prevIt = it;
	++it;
	prevIt.m_ptr->Next = tail;
	
	if(it == tail) return; //it means the list is composed of only one element
	
	before_head.m_ptr->Next = nullptr;
	iterator nextIt = it.m_ptr->Next;
	for (; it != tail;)
	{
		it.m_ptr->Next = prevIt;
		prevIt = it;
		it = nextIt;
		if(it != tail)
		{
			nextIt = it.m_ptr->Next;
		}
	}

	before_head.m_ptr->Next = prevIt;
}

template<class T>
void SList<T>::unique()
{

	SList<T>::iterator it = begin();
	SList<T>::iterator lastIt = it;
	++it;
	for (; it != tail;)
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

	for (; it != tail;)
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
void SList<T>::sort()
{

}

template<class T>
template<class Comparator>
void SList<T>::sort(Comparator comp)
{

}
