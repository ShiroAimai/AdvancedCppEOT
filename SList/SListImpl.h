#pragma once
#include "SList.h"
#include <algorithm>
#include <cassert>

template<class T>
typename SList<T>::iterator SList<T>::tail(nullptr);

template<class T>
SList<T>::SList() : before_head(new Node())
{

}

template<class T>
SList<T>::~SList()
{
	if (before_head == tail) return;

	delete before_head.m_ptr;
}

template<class T>
SList<T>::SList(SList::size_type n) : SList()
{
	insert_after(cbefore_begin(), n, value_type());
}

template<class T>
SList<T>::SList(size_type n, const value_type& val) : SList()
{
	insert_after(cbefore_begin(), n, val);
}

template<class T>
template<class InputIterator>
SList<T>::SList(InputIterator first, InputIterator last) : SList()
{
	insert_after(cbefore_begin(), first, last);
}

template<class T>
SList<T>::SList(std::initializer_list<SList<T>::value_type> il) : SList()
{
	insert_after(cbefore_begin(), il);
}

template<class T>
SList<T>::SList(const SList& list) : SList()
{
	insert_after(cbefore_begin(), list.cbegin(), list.cend());
}

template<class T>
SList<T>::SList(SList&& list) : before_head(nullptr)
{
	std::swap(before_head, list.before_head);
	list.before_head = tail;
}

template<class T>
typename SList<T>::iterator SList<T>::before_begin() noexcept
{
	return before_head;
}

template<class T>
typename SList<T>::const_iterator SList<T>::cbefore_begin() const noexcept
{
	return const_iterator(before_head.m_ptr);
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
	const_iterator cit(before_head);
	++cit;
	return cit;
}

template<class T>
typename SList<T>::iterator SList<T>::end() noexcept
{
	return tail;
}

template<class T>
typename SList<T>::const_iterator SList<T>::cend() const noexcept
{
	return const_iterator(tail);
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
		for (; it != tail && oIt != other.cend(); ++it, ++oIt)
		{
			*it = *oIt;
			lastUpdateIt = it;
		}

		//this has less elements than other, add the remainings in other
		if (it == tail && oIt != other.cend())
		{
			insert_after(lastUpdateIt, oIt, other.cend());
		}
		//this has more elems, delete the remainings in this
		else if (it != tail && oIt == other.cend())
		{
			erase_after(lastUpdateIt, tail);
		}
	}

	return *this;
}

template<class T>
typename SList<T>& SList<T>::operator=(SList&& other)
{
	if (this != &other)
	{
		std::swap(before_head, other.before_head); //swap only heads
	}

	return *this;
}

template<class T>
typename SList<T>& SList<T>::operator=(std::initializer_list<value_type> il)
{

	iterator it = begin();
	iterator lastUpdateIt = before_head;
	typename std::initializer_list<value_type>::iterator ilIt = il.begin();

	//copy the available elements from other in this
	//if either other or this equals to end stop
	for (; it != tail && ilIt != il.end(); ++it, ++ilIt)
	{
		*it = *ilIt;
		lastUpdateIt = it;
	}

	//this has less elements than other, add the remainings in other
	if (it == tail && ilIt != il.end())
	{
		insert_after(lastUpdateIt, ilIt, il.end());
	}
	//this has more elems, delete the remainings in this
	else if (it != tail && ilIt == il.end())
	{
		erase_after(lastUpdateIt, tail);
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
	insert_after(before_head, T(std::forward<Args>(args)...));
}

template<class T>
template <class... Args>
typename SList<T>::iterator SList<T>::emplace_after(const_iterator position, Args&&... args)
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
	insert_after(cbefore_begin(), std::move(val));
}

template<class T>
void SList<T>::pop_front()
{
	erase_after(before_head);
}

template<class T>
typename SList<T>::iterator SList<T>::insert_after(const_iterator position, const value_type& val)
{
	iterator prevIt = iterator(position.m_ptr);
	++position;

	Node* NewEl = new Node(val, position.m_ptr);
	prevIt.m_ptr->Next = NewEl;

	return iterator(NewEl);
}

template<class T>
typename SList<T>::iterator SList<T>::insert_after(const_iterator position, value_type&& val)
{
	iterator prevIt = iterator(position.m_ptr);
	++position;

	Node* NewEl = new Node();
	NewEl->info = std::move(val);
	NewEl->Next = position.m_ptr;
	
	prevIt.m_ptr->Next = NewEl;

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

	if (it != tail)
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

	if (it == tail) return; //there's nothing to reverse

	iterator prevIt = it;
	++it;
	prevIt.m_ptr->Next = tail;

	if (it == tail) return; //it means the list is composed of only one element

	before_head.m_ptr->Next = nullptr;
	iterator nextIt = it.m_ptr->Next;
	for (; it != tail;)
	{
		it.m_ptr->Next = prevIt;
		prevIt = it;
		it = nextIt;
		if (it != tail)
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
void SList<T>::splice_after(const_iterator position, SList& other)
{
	if (this == &other) return; //splicing the same list
	Node* PrevNextElement = position.m_ptr->Next;
	position.m_ptr->Next = other.before_head->Next; //attach other
	other.before_head->Next = nullptr; //reset other

	iterator it = position;
	while (it.m_ptr->Next != nullptr)
	{
		++it;
	}

	it.m_ptr->Next = PrevNextElement;
}

template<class T>
void SList<T>::splice_after(const_iterator position, SList&& other)
{
	if (this == &other) return; //splicing the same list
	Node* PrevNextElement = position.m_ptr->Next;
	position.m_ptr->Next = std::move(other.before_head->Next); //moving other
	other.before_head->Next = nullptr; //reset other after move

	iterator it = position;
	while (it.m_ptr->Next != nullptr)
	{
		++it;
	}

	it.m_ptr->Next = PrevNextElement;
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

template<class T>
void SList<T>::sort()
{
	//TODO
}

template<class T>
template<class Comparator>
void SList<T>::sort(Comparator comp)
{
	//TODO
}


template<class T>
void SList<T>::merge(SList& other);
template<class T>
void SList<T>::merge(SList&& other);
template<class T>
template<class Comparator>
void SList<T>::merge(SList& other, Comparator comp);
template<class T>
template<class Comparator>
void SList<T>::merge(SList&& other, Comparator comp);*/