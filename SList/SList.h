#pragma once
#include <memory>

/**
 * Single Linked List 
 * 
 * A custom forward linked list data structure. It is based on an underlying implementation of a Node that knows only the related info and what node comes next.
 *
 *@author Nicola Cisternino
 *
 */
template<class T>
class SList
{
public:
	using value_type = T;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

private:
	/** Underlying list element. This structure is almost exclusively manager by the iterators  */
	struct Node
	{
		value_type info;
		Node* Next;

		Node() : Next(nullptr) {};
		Node(value_type _info, Node* _Next) : info(_info), Next(_Next) {};
		virtual ~Node()
		{
			if (Next)
			{
				delete Next;
			}
		};
	};

public:
	/** Iterator data structure. It is configurable to be either const or non const */
	template<bool IsConst = false>
	struct Iterator {
		friend class SList;

		using iterator_category = std::forward_iterator_tag;
		using value_type = SList::value_type;
		using difference_type = SList::difference_type;
		using pointer = typename std::conditional_t< IsConst, value_type const*, value_type*>;
		using reference = typename std::conditional_t< IsConst, value_type const&, value_type&>;

		Iterator() : m_ptr(nullptr) {};
		Iterator(Node* ptr) : m_ptr(ptr) {};
		Iterator(const Iterator<IsConst>& other) : m_ptr(other.m_ptr) {};

		Iterator& operator=(const Iterator& other) 
		{
			if (this != &other)
			{
				m_ptr = other.m_ptr;
			}

			return *this;
		}

		friend void swap(Iterator& lhs, Iterator& rhs)
		{
			std::swap(lhs.m_ptr, rhs.m_ptr);
		}

		//USE SFINAE TO FILTER OPERATOR SIGNATURES
		template< bool _Const = IsConst >
		std::enable_if_t< _Const, reference > operator*() const { return (*m_ptr).info; };
		template< bool _Const = IsConst >
		std::enable_if_t< !_Const, reference > operator*() { return (*m_ptr).info; };
		template< bool _Const = IsConst >
		std::enable_if_t< _Const, pointer > operator->() const { return &(m_ptr->info); };
		template< bool _Const = IsConst >
		std::enable_if_t< !_Const, pointer > operator->() { return &(m_ptr->info); };

		// Prefix increment
		Iterator& operator++()
		{
			if (m_ptr)
			{
				m_ptr = m_ptr->Next;
			}
			return *this;
		}

		// Postfix increment
		Iterator operator++(int)
		{
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}

		friend inline bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
		friend inline bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

	private:
		Node* m_ptr;
	};

	using iterator = Iterator<false>;
	using const_iterator = Iterator<true>;

public:

	explicit SList();
	explicit SList(size_type n);
	explicit SList(size_type n, const value_type& val);
	template<class InputIterator>
	SList(InputIterator first, InputIterator last);
	SList(const SList& list);
	SList(SList&& list);
	SList(std::initializer_list<value_type> il);

	//DESTRUCTOR
	~SList();

	//ITERATORS
	iterator before_begin() noexcept;
	const_iterator cbefore_begin() const noexcept;

	iterator begin() noexcept;
	const_iterator cbegin() const noexcept;

	iterator end() noexcept;
	const_iterator cend() const noexcept;

	//MEMBER FUNCTIONS
	SList& operator=(const SList& other);
	SList& operator=(SList&& other);
	SList& operator=(std::initializer_list<value_type> il);

	//CAPACITY
	bool empty() const noexcept;
	size_type max_size() const noexcept;

	//ELEMENT ACCESS
	reference front();
	const_reference front() const;

	//MODIFIERS
	template<class InputIterator>
	void assign(InputIterator first, InputIterator last);
	void assign(size_type n, const value_type& val);
	void assign(std::initializer_list<value_type> il);

	template<class... Args>
	void emplace_front(Args&&... args);
	template <class... Args>
	iterator emplace_after(const_iterator position, Args&&... args);

	void push_front(const value_type& val);
	void push_front(value_type&& val);

	void pop_front();

	iterator insert_after(const_iterator position, const value_type& val);
	iterator insert_after(const_iterator position, value_type&& val);
	iterator insert_after(const_iterator position, size_type n, const value_type& val);
	template <class InputIterator>
	iterator insert_after(const_iterator position, InputIterator first, InputIterator last);
	iterator insert_after(const_iterator position, std::initializer_list<value_type> il);

	iterator erase_after(const_iterator position);
	iterator erase_after(const_iterator position, const_iterator last);

	void swap(SList& other);

	void resize(size_type n);
	void resize(size_type n, const value_type& val);

	void clear() noexcept;

	//OPERATIONS
	void remove(const value_type& value);
	template <class Predicate>
	void remove_if(Predicate pred);

	void reverse() noexcept;

	void unique();
	template<class BinaryPredicate>
	void unique(BinaryPredicate binary_pred);

	void splice_after(const_iterator position, SList& other);
	void splice_after(const_iterator position, SList&& other);
	void splice_after(const_iterator position, SList& other, const_iterator i);
	void splice_after(const_iterator position, SList&& other, const_iterator i);
	void splice_after(const_iterator position, SList& other, const_iterator first, const_iterator last);
	void splice_after(const_iterator position, SList&& other, const_iterator first, const_iterator last);

	void sort();
	template<class Comparator>
	void sort(Comparator comp);

	void merge(SList& other);
	void merge(SList&& other);
	template<class Comparator>
	void merge(SList& other, Comparator comp);
	template<class Comparator>
	void merge(SList&& other, Comparator comp);
private:
	iterator before_head; //iterator to the element in list that precedes the first

	/** Compute the size of a sublist starting from to the end of the list */
	SList::size_type list_size_from(const_iterator start) const;
	/** [it] is an iterator to the new begin element of sorted list. O(nlogn)*/
	template<class Comparator>
	void merge_sort(iterator& it, Comparator comp);
};

#include "SListImpl.h"
