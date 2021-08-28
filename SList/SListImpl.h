#pragma once
#include "SList.h"
#include <algorithm>
#include <cassert>

#pragma region SLIST SORT UTILS
template<class T>
inline typename SList<T>::size_type SList<T>::list_size_from(const_iterator start) const
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
inline void SList<T>::merge_sort(iterator& it, Comparator comp)
{
	if(it.m_ptr == nullptr || it.m_ptr->Next == nullptr) return; //no sort can be applied

	size_type half_list_size = list_size_from(const_iterator(it.m_ptr)) / 2;

	iterator it_half_list = it; //last node in low half list
	for (size_type i = 0; i < (half_list_size - 1); ++i)
	{
		++it_half_list; 
	}

	const_iterator cit_low_half_list_end(it_half_list.m_ptr);
	++it_half_list;
	cit_low_half_list_end.m_ptr->Next = nullptr; //split half lists

	merge_sort(it, comp);
	merge_sort(it_half_list, comp);

	//merge sorted sub_lists
	iterator itSorted = comp(*it, *it_half_list) ? it++ : it_half_list++;

	iterator StartSortedList = itSorted;
	iterator end = nullptr;

	for (; it != end; ++itSorted)
	{
		if (it_half_list == end)
		{
			itSorted.m_ptr->Next = it.m_ptr;
			break;
		}
		if (comp(*it, *it_half_list))
		{
			itSorted.m_ptr->Next = it.m_ptr;
			++it;
		}
		else
		{
			itSorted.m_ptr->Next = it_half_list.m_ptr;
			++it_half_list;
		}
	}

	if(it == end)
	{
		itSorted.m_ptr->Next = it_half_list.m_ptr;
	}

	it = StartSortedList;
}

#pragma endregion

template<class T>
inline SList<T>::SList() : before_head(new Node())
{

}

template<class T>
inline SList<T>::~SList()
{
	if (!before_head.m_ptr || before_head.m_ptr->Next == nullptr) return;

	delete before_head.m_ptr;
}

template<class T>
inline SList<T>::SList(SList::size_type n) : SList()
{
	insert_after(cbefore_begin(), n, value_type());
}

template<class T>
inline SList<T>::SList(size_type n, const value_type& val) : SList()
{
	insert_after(cbefore_begin(), n, val);
}

template<class T>
template<class InputIterator>
inline SList<T>::SList(InputIterator first, InputIterator last) : SList()
{
	insert_after(cbefore_begin(), first, last);
}

template<class T>
inline SList<T>::SList(std::initializer_list<SList<T>::value_type> il) : SList()
{
	insert_after(cbefore_begin(), il);
}

template<class T>
inline SList<T>::SList(const SList& list) : SList()
{
	insert_after(cbefore_begin(), list.cbegin(), list.cend());
}

template<class T>
inline SList<T>::SList(SList&& list) : before_head()
{
	std::swap(before_head, list.before_head);
}

template<class T>
inline typename SList<T>::iterator SList<T>::before_begin() noexcept
{
	return before_head;
}

template<class T>
inline typename SList<T>::const_iterator SList<T>::cbefore_begin() const noexcept
{
	return const_iterator(before_head.m_ptr);
}

template<class T>
inline typename SList<T>::iterator SList<T>::begin() noexcept
{
	iterator begin = std::next(before_head);
	return begin;
}

template<class T>
inline typename SList<T>::const_iterator SList<T>::cbegin() const noexcept
{
	iterator Begin = std::next(before_head);
	return const_iterator(Begin.m_ptr);
}

template<class T>
inline typename SList<T>::iterator SList<T>::end() noexcept
{
	return nullptr;
}

template<class T>
inline typename SList<T>::const_iterator SList<T>::cend() const noexcept
{
	return nullptr;
}

template<class T>
inline typename SList<T>& SList<T>::operator=(const SList& other)
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
inline typename SList<T>& SList<T>::operator=(SList&& other)
{
	if (this != &other)
	{
		std::swap(before_head, other.before_head); //swap heads
	}

	return *this;
}

template<class T>
inline typename SList<T>& SList<T>::operator=(std::initializer_list<value_type> il)
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
inline bool SList<T>::empty() const noexcept
{
	return cbegin() == cend();
}

template<class T>
inline typename SList<T>::size_type SList<T>::max_size() const noexcept
{
	//total_size / el_size;
	return std::numeric_limits<SList<T>::size_type>::max() / sizeof(SList<T>::Node);
}

template<typename T>
inline typename SList<T>::reference SList<T>::front()
{
	return *begin();
}

template<class T>
inline typename SList<T>::const_reference SList<T>::front() const
{
	return *cbegin();
}

template<class T>
template<class InputIterator>
inline void SList<T>::assign(InputIterator first, InputIterator last)
{
	clear();
	insert_after(cbefore_begin(), first, last);
}

template<class T>
inline void SList<T>::assign(size_type n, const value_type& val)
{
	clear();
	insert_after(cbefore_begin(), n, val);
}

template<class T>
inline void SList<T>::assign(std::initializer_list<value_type> il)
{
	assign(il.begin(), il.end());
}

template<class T>
template<class... Args>
inline void SList<T>::emplace_front(Args&&... args)
{
	insert_after(cbefore_begin(), value_type(std::forward<Args>(args)...));
}

template<class T>
template <class... Args>
inline typename SList<T>::iterator SList<T>::emplace_after(const_iterator position, Args&&... args)
{
	return insert_after(position, value_type(std::forward<Args>(args)...));
}

template<class T>
inline void SList<T>::push_front(const value_type& val)
{
	insert_after(cbefore_begin(), val);
}

template<class T>
inline void SList<T>::push_front(value_type&& val)
{
	insert_after(cbefore_begin(), std::forward<value_type>(val));
}

template<class T>
inline void SList<T>::pop_front()
{
	erase_after(before_head);
}

template<class T>
inline typename SList<T>::iterator SList<T>::insert_after(const_iterator position, const value_type& val)
{
	iterator prevIt(position.m_ptr);
	const_iterator NextPos = std::next(position);

	Node* NewEl = new Node(val, NextPos.m_ptr);
	prevIt.m_ptr->Next = NewEl;

	return iterator(NewEl);
}

template<class T>
inline typename SList<T>::iterator SList<T>::insert_after(const_iterator position, value_type&& val)
{
	iterator prevIt = iterator(position.m_ptr);
	const_iterator NextPos = std::next(position);

	Node* NewEl = new Node();
	NewEl->info = std::move(val);
	NewEl->Next = NextPos.m_ptr;
	
	prevIt.m_ptr->Next = NewEl;

	return iterator(NewEl);
}

template<class T>
inline typename SList<T>::iterator SList<T>::insert_after(const_iterator position, size_type n, const value_type& val)
{
	for (size_type i = 0; i < n; ++i, ++position)
	{
		insert_after(position, val);
	}

	return iterator(position.m_ptr);
}

template<class T>
template <class InputIterator>
inline typename SList<T>::iterator SList<T>::insert_after(const_iterator position, InputIterator first, InputIterator last)
{
	for (; first != last; ++first, ++position)
	{
		insert_after(position, *first);
	}

	return iterator(position.m_ptr);
}

template<class T>
inline typename SList<T>::iterator SList<T>::insert_after(const_iterator position, std::initializer_list<value_type> il)
{
	return insert_after(position, il.begin(), il.end());
}

template<class T>
inline typename SList<T>::iterator SList<T>::erase_after(const_iterator position)
{
	const_iterator prevIt = position;
	const_iterator it = std::next(position);

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
inline typename SList<T>::iterator SList<T>::erase_after(const_iterator position, const_iterator last)
{
	iterator eraseResult(position.m_ptr);
	iterator  lastIterator(last.m_ptr);
	for (; eraseResult != lastIterator;)
	{
		eraseResult = erase_after(position);
	}

	assert(eraseResult == lastIterator);

	return eraseResult;
}

template<class T>
inline void SList<T>::swap(SList& other)
{
	std::swap(before_head, other.before_head);
}

template<class T>
inline void SList<T>::resize(size_type n)
{
	resize(n, value_type());
}

template<class T>
inline void SList<T>::resize(size_type n, const value_type& val)
{
	size_type list_size = 0;

	SList<T>::const_iterator it = cbegin();
	SList<T>::const_iterator lastIt = cbefore_begin();
	for (; it != cend() && list_size < n; ++it)
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
		erase_after(lastIt, cend());
	}
}

template<class T>
inline void SList<T>::clear() noexcept
{
	erase_after(cbefore_begin(), cend());
}

template<class T>
inline void SList<T>::remove(const value_type& value)
{
	SList<T>::iterator it = begin();
	SList<T>::const_iterator lastIt = cbefore_begin();
	for (; it != end();)
	{
		if (*it == value)
		{
			//erase element after lastIt
			//assign iterator to element after canceled element to it
			it = erase_after(lastIt);
		}
		else {
			lastIt.m_ptr = it.m_ptr;
			++it;
		}
	}
}

template<class T>
template <class Predicate>
inline void SList<T>::remove_if(Predicate pred)
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
inline void SList<T>::reverse() noexcept
{
	iterator it = begin();

	if (it == end() || it.m_ptr->Next == nullptr) return; //there's nothing to reverse

	iterator prevIt = it++;
	prevIt.m_ptr->Next = nullptr;

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
inline void SList<T>::unique()
{
	SList<T>::iterator it = std::next(begin());
	SList<T>::const_iterator lastIt = cbegin();
	
	for (; it != end();)
	{
		if (*lastIt == *it)
		{
			it = erase_after(lastIt);
		}
		else
		{
			lastIt.m_ptr = it.m_ptr;
			++it;
		}
	}
}

template<class T>
template<class BinaryPredicate>
inline void SList<T>::unique(BinaryPredicate binary_pred)
{
	SList<T>::iterator it = std::next(begin());
	SList<T>::const_iterator lastIt = cbegin();

	for (; it != end();)
	{
		if (binary_pred(*it, *lastIt))
		{
			it = erase_after(lastIt);
		}
		else
		{
			lastIt.m_ptr = it.m_ptr;
			++it;
		}
	}
}

template<class T>
inline void SList<T>::splice_after(const_iterator position, SList& other)
{
	if (this == &other) return; //splicing the same list

	Node* PrevNextElement = position.m_ptr->Next; //element next to position
	position.m_ptr->Next = other.begin().m_ptr; //attach other first element next to position
	
	//reset other
	other.before_head.m_ptr->Next = nullptr; 

	const_iterator it = position;
	while (it.m_ptr->Next != nullptr)
	{
		++it;
	}

	it.m_ptr->Next = PrevNextElement;
}

template<class T>
inline void SList<T>::splice_after(const_iterator position, SList&& other)
{
	splice_after(position, other);
}

template<class T>
inline void SList<T>::splice_after(const_iterator position, SList& other, const_iterator i)
{
	const_iterator OtherElementToMove = std::next(i);

	if(position == cend() || OtherElementToMove == other.cend()) return; //if i is iterator to last element then we're done 

	const_iterator PrevNextElement = std::next(position);
	
	i.m_ptr->Next = OtherElementToMove.m_ptr->Next; //detach element following i from other

	position.m_ptr->Next = OtherElementToMove.m_ptr;
	OtherElementToMove.m_ptr->Next = PrevNextElement.m_ptr;
}

template<class T>
inline void SList<T>::splice_after(const_iterator position, SList&& other, const_iterator i)
{
	splice_after(position, other, i);
}

template<class T>
inline void SList<T>::splice_after(const_iterator position, SList& other, const_iterator first, const_iterator last)
{
	if(position == cend()) return; //invalid iterator
	const_iterator OriginalPositionNextElement = std::next(position);
	const_iterator cit = std::next(first);
	
	first.m_ptr->Next = last.m_ptr; //reconfigure other
	
	for (; cit != last; ++position, ++cit)
	{
		position.m_ptr->Next = cit.m_ptr;	
	}

	position.m_ptr->Next = OriginalPositionNextElement.m_ptr;
}

template<class T>
inline void SList<T>::splice_after(const_iterator position, SList&& other, const_iterator first, const_iterator last)
{
	splice_after(position, other, first, last);
}

template<class T>
inline void SList<T>::sort()
{
	sort(std::less<value_type>());
}

template<class T>
template<class Comparator>
inline void SList<T>::sort(Comparator comp)
{
	iterator NewBegin = begin();
	merge_sort(NewBegin, comp);
	before_head.m_ptr->Next = NewBegin.m_ptr;
}

template<class T>
inline void SList<T>::merge(SList& other)
{
	merge(other, std::less<value_type>());
}

template<class T>
inline void SList<T>::merge(SList&& other)
{
	merge(std::forward<SList<value_type>>(other), std::less<value_type>());
}

template<class T>
template<class Comparator>
inline void SList<T>::merge(SList& other, Comparator comp)
{
	const_iterator cit = cbegin();
	const_iterator cOtherIt = other.cbegin();

	//detach lists
	before_head.m_ptr->Next = nullptr;
	other.before_head.m_ptr->Next = nullptr;

	const_iterator citSorted = cbefore_begin();
	const_iterator cend = nullptr;

	for (; cit != cend; ++citSorted)
	{
		if (cOtherIt == cend) {
			citSorted.m_ptr->Next = cit.m_ptr;
			return;
		}
		if (comp(*cit, *cOtherIt))
		{
			citSorted.m_ptr->Next = cit.m_ptr;
			++cit;
		}
		else
		{
			citSorted.m_ptr->Next = cOtherIt.m_ptr;
			++cOtherIt;
		}
	}
	citSorted.m_ptr->Next = cOtherIt.m_ptr;
}

template<class T>
template<class Comparator>
inline void SList<T>::merge(SList&& other, Comparator comp)
{
	const_iterator cit = cbegin();
	const_iterator cOtherIt = other.cbegin();

	//detach lists
	before_head.m_ptr->Next = nullptr;
	other.before_head.m_ptr->Next = nullptr;

	const_iterator citSorted = cbefore_begin();
	const_iterator cend = nullptr;

	for (; cit != cend; ++citSorted)
	{
		if (cOtherIt == cend) {
			citSorted.m_ptr->Next = cit.m_ptr;
			return;
		}
		if (comp(*cit, *cOtherIt))
		{
			citSorted.m_ptr->Next = cit.m_ptr;
			++cit;
		}
		else
		{
			citSorted.m_ptr->Next = cOtherIt.m_ptr;
			++cOtherIt;
		}
	}
	citSorted.m_ptr->Next = cOtherIt.m_ptr;
}