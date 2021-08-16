#pragma once
#include <memory>

template<class T>
class SList
{
public:
	using value_type = T;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const pointer;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	
	struct Iterator;
	using iterator = Iterator;
	using const_iterator = const Iterator;
private:
	struct Node
	{
		value_type info;
		Node* Next;

		Node() : Next(nullptr) {}
		Node(value_type _info, Node* _Next) : info(_info), Next(_Next) {}
		virtual ~Node()
		{
			if (Next)
			{
				delete Next;
			}
		}
	};
	
	iterator before_head;
	static constexpr const_iterator tail = nullptr;

	size_type size() const;
public:
	struct Iterator {
		template<class T>
		friend class SList<T>;
		using iterator_category = std::forward_iterator_tag;
		using it_pointer = Node*;

		Iterator(it_pointer ptr) : m_ptr(ptr) {}

		reference operator*() const { 
			return (*m_ptr).info; 
		}
		
		pointer operator->() {
			return &(m_ptr->info); 
		}

		// Prefix increment
		Iterator& operator++() { 
			m_ptr = m_ptr->Next; 
			return *this; 
		}
		// Postfix increment
		Iterator operator++(int) {
			Iterator tmp = *this; 
			++(*this); 
			return tmp; 
		}

		friend inline bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
		friend inline bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

	private:
		it_pointer m_ptr;
	};

	explicit SList();
	explicit SList(size_type n);
	explicit SList(size_type n, const value_type& val);
	template<class InputInteractor>
	SList(InputInteractor first, InputInteractor last);
	SList(const SList& list);
	SList(SList&& list):
	//SList(initializer_list<value_type> il):
	
	//DESTRUCTOR
	virtual ~SList();

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
	//TODO SList& operator=(initializer_list<value_type> il);

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
	//void assign(initializer_list<value_type> il);

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
	//iterator insert_after(const_iterator position, initializer_list<value_type> il);

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

	void sort();
	template<class Comparator>
	void sort(Comparator comp);

	void merge(SList& other);
	void merge(SList&& other);
	template<class Comparator>
	void merge(SList& other, Comparator comp);
	template<class Comparator>
	void merge(SList&& other, Comparator comp);
};

#include "SListImpl.h"
