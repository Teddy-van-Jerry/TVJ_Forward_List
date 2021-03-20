/*
 * File: TVJ_Forward_List.h
 * --------------------------
 * 
 * @author: Teddy van Jerry
 * @licence: The MIT Licence
 * @compiler: at least C++/11 (tested on MSVC and MinGW)
 *
 * @version 1.1 2021/03/20
 * - modidy functions
 * - add DEBUG check
 * 
 * @version 1.0 2021/03/19
 * - initial version
 * 
 */

#pragma once

#include <stdexcept>
#include <exception>
#include <iterator>
#include <vector>
#include <deque>
#include <list>

namespace tvj
{

#define ASCENDING  true
#define DESCENDING false

	// error throw code
	enum TVJ_FORWARD_LIST_EXCEPTION
	{
		TVJ_FORWARD_LIST_UNDERFLOW = 1100,
		TVJ_FORWARD_LIST_OVERFLOW,
		TVJ_FORWARD_LIST_TYPE_MISMATCH,
		TVJ_FORWARD_LIST_NULLPTR,
		TVJ_FORWARD_LIST_ITER_RANGE
	};

	/**
	 * @brief: throw error info
	 * @param: const char* and exception name (an enum type)
	 * @return: void
	 */
	inline void error_info(const char* text, const TVJ_FORWARD_LIST_EXCEPTION& exception_code)
	{
		switch (exception_code)
		{
		case TVJ_FORWARD_LIST_UNDERFLOW:
			throw std::underflow_error(text);
			break;
		case TVJ_FORWARD_LIST_OVERFLOW:
			throw std::overflow_error(text);
			break;
		case TVJ_FORWARD_LIST_TYPE_MISMATCH:
#ifdef _MSC_VER // MSVC compiler
			throw std::exception(text);
#else
			throw std::runtime_error(text);
#endif			
			break;
		case TVJ_FORWARD_LIST_NULLPTR:
			throw std::runtime_error(text);
			break;
		case TVJ_FORWARD_LIST_ITER_RANGE:
			throw std::range_error(text);
			break;
		default:
#ifdef _MSC_VER // MSVC compiler
			throw std::exception(text);
#else
			throw std::runtime_error(text);
#endif	
			break;
		}
	}

	// The tvj::forward_list class
	// that supports functions similar to the STL class.
	template<typename Elem>
	class forward_list
	{
	protected:
		// the class of the list node
		struct Node
		{
			Node();
			Node(Elem data_, Node* succ_ptr = nullptr);
			~Node();
			Elem data;               // tha data the node contains
			Node* succ = nullptr; // the pointer that points to the successor of the forward list
		};

	private:
		Node*  head = new Node;
		Node*  tail = head->succ = new Node;
		size_t size_ = 0;

		auto head_share()
		{
			return head;
		}

	public:
		class const_iterator
		{
			friend class forward_list<Elem>;

		protected:
			Node* node;
			const forward_list<Elem>* parent;

		public:
			const_iterator(Node* node_, const forward_list<Elem>* parent_);
		public:
			inline const Elem& operator*() const;
			inline const Elem* operator->() const;
			inline auto operator++();
			inline auto operator++(int);
			inline const_iterator operator+(const unsigned n) const;
			inline const_iterator operator+=(const unsigned n);
			inline bool operator==(const const_iterator& iter) const noexcept;
			inline bool operator!=(const const_iterator& iter) const noexcept;
		};

		class iterator : public const_iterator
		{
		public:
			// constructor declaration
			using const_iterator::const_iterator;
			inline Elem& operator*();
			inline Elem* operator->();
		};

	public:
		/**
		 * brief: constructor for empty constuctor list
		 * param: (void)
		 * return: --
		 */
		forward_list();

		/**
		 * brief: constructor for empty constuctor list
		 * param: (void)
		 * return: --
		 */
		explicit forward_list(const forward_list<Elem>& list_);

		/**
		 * brief: constructor for a container
		 * param: a container that supports iterators
		 * return: --
		 */
		template<typename Container_Type>
		forward_list(const Container_Type& container);

		/**
		 * brief: constructor for iterators where the range of iterators are not checked during DEBUG
		 * param: two iterators
		 * return: --
		 */
		template<typename _Iter>
		forward_list(const _Iter& i_beg,
			typename std::enable_if<
			! std::is_same<std::decay<_Iter>, std::decay<typename std::vector<Elem>::const_iterator>>::value &&
			! std::is_same<std::decay<_Iter>, std::decay<typename std::vector<Elem>::iterator      >>::value &&
			! std::is_same<std::decay<_Iter>, std::decay<typename std::deque <Elem>::const_iterator>>::value &&
			! std::is_same<std::decay<_Iter>, std::decay<typename std::deque <Elem>::iterator      >>::value &&
			! std::is_same<std::decay<_Iter>, std::decay<typename std::list  <Elem>::const_iterator>>::value &&
			! std::is_same<std::decay<_Iter>, std::decay<typename std::list  <Elem>::iterator      >>::value,
			const _Iter&>::type i_end);

		/**
		 * brief: constructor for iterators where the range of iterators are checked during DEBUG
		 * param: two iterators
		 * return: --
		 */
		template<typename _Iter>
		forward_list(const _Iter& i_beg,
			typename std::enable_if<
			std::is_same<std::decay<_Iter>, std::decay<typename std::vector<Elem>::const_iterator>>::value ||
			std::is_same<std::decay<_Iter>, std::decay<typename std::vector<Elem>::iterator      >>::value ||
			std::is_same<std::decay<_Iter>, std::decay<typename std::deque <Elem>::const_iterator>>::value ||
			std::is_same<std::decay<_Iter>, std::decay<typename std::deque <Elem>::iterator      >>::value ||
			std::is_same<std::decay<_Iter>, std::decay<typename std::list  <Elem>::const_iterator>>::value ||
			std::is_same<std::decay<_Iter>, std::decay<typename std::list  <Elem>::iterator      >>::value,
			const _Iter&>::type i_end);

		/**
		 * brief: constructor two pointers
		 * param: two pointers
		 * return: --
		 */
		forward_list(const Elem* i_beg, const Elem* i_end);

		/**
		 * @brief: destructor
		 * @param: (void)
		 * @return: --
		 */
		~forward_list();

		/**
		 * brief: clear all elements in the list
		 * param: (void)
		 * return: void
		 */
		void clear();

		/**
		 * brief: the size (valid element number)
		 * param: (void)
		 * return: const size_t
		 */
		inline const auto& size() const noexcept;

		/**
		 * brief: if the list is empty
		 * param: (void)
		 * return: bool
		 */
		inline bool empty() const noexcept;

		/**
		 * brief: the iterator before begin()
		 * param: (void)
		 * return: iterator
		 */
		inline iterator before_begin() noexcept;

		/**
		 * brief: the iterator begin()
		 * param: (void)
		 * return: iterator
		 */
		inline iterator begin() noexcept;

		/**
		 * brief: the iterator front() which is the same to begin()
		 * param: (void)
		 * return: iterator
		 */
		inline iterator front() noexcept;

		/**
		 * brief: the iterator back() that is the last valid element
		 * param: (void)
		 * return: iterator
		 */
		inline iterator back() noexcept;

		/**
		 * brief: the iterator end() that is one past the last element
		 * param: (void)
		 * return: iterator
		 */
		inline iterator end() noexcept;

		/**
		 * brief: the iterator before begin()
		 * param: (void)
		 * return: const_iterator
		 */
		inline const_iterator before_begin() const noexcept;

		/**
		 * brief: the iterator begin()
		 * param: (void)
		 * return: const_iterator
		 */
		inline const_iterator begin() const noexcept;

		/**
		 * brief: the iterator front() which is the same to begin()
		 * param: (void)
		 * return: const_iterator
		 */
		inline const_iterator front() const noexcept;

		/**
		 * brief: the iterator back() that is the last valid element
		 * param: (void)
		 * return: const_iterator
		 */
		inline const_iterator back() const noexcept;

		/**
		 * brief: the iterator end() that is one past the last element
		 * param: (void)
		 * return: const_iterator
		 */
		inline const_iterator end() const noexcept;

		/**
		 * brief: the iterator before begin()
		 * param: (void)
		 * return: const_iterator
		 */
		inline const_iterator cbefore_begin() const noexcept;

		/**
		 * brief: the iterator begin()
		 * param: (void)
		 * return: const_iterator
		 */
		inline const_iterator cbegin() const noexcept;

		/**
		 * brief: the iterator end() that is one past the last element
		 * param: (void)
		 * return: iterator
		 */
		inline const_iterator cend() const noexcept;

		/**
		 * brief: find the element and return its iterator of first occurence,
		 *        if no result found, return cend()
		 * param: the element type
		 * return: const_iterator
		 */
		const_iterator find(const Elem& elem) const noexcept;

		/**
		 * brief: find the element and return its iterator of the last element that is less tan or equal to it
		 * param: the element type and the sorting order (default as ASCENDING, otherwise DESCENDING)
		 * return: const_iterator
		 */
		const_iterator search(const Elem& elem, bool is_ascending = ASCENDING) const noexcept;

		/**
		 * brief: check whether the list is sorted
		 * param: the sorting order (default as ASCENDING, otherwise DESCENDING)
		 * return: bool
		 */
		bool sorted(bool is_ascending = ASCENDING) const noexcept;

		/**
		 * brief: check whether the list contains the element
		 * param: the element type
		 * return: bool
		 */
		bool contains(const Elem& elem) const noexcept;

		/**
		 * brief: count the occurence of the element
		 * param: the element type
		 * return: size_t
		 */
		size_t count(const Elem& elem) const noexcept;

		/**
		 * brief: assign the value to an iterator
		 * param: the iterator and the element
		 * return: size_t
		 */
		inline void assign(const const_iterator& iter, const Elem& elem);

		/**
		 * brief: insert a new element after the iterator
		 * param: the iterator and the element
		 * return: void
		 */
		inline void insert_after(const const_iterator& iter, const Elem& elem);

		/**
		 * brief: insert new elements after the iterator
		 * param: the iterator, the element, number of elements
		 * return: void
		 */
		inline void insert_after(const const_iterator& iter, const Elem& elem, size_t n);

		/**
		 * brief: insert a new element at the end
		 * param: the element type
		 * return: void
		 */
		inline void push_back(const Elem& elem);

		/**
		 * brief: insert a new element before the first element
		 * param: the element type
		 * return: void
		 */
		inline void push_front(const Elem& elem);

		/**
		 * brief: remove the last element
		 * param: (void)
		 * return: void
		 */
		inline void pop_back();

		/**
		 * brief: remove the first element
		 * param: (void)
		 * return: void
		 */
		inline void pop_front();

		/**
		 * brief: remove the element at a certain location and return its value,
		 *        argument 2 indicates whether the action is successful
		 * param: the element type, a pointer to bool
		 * return: the element type
		 */
		Elem remove_at(const const_iterator& iter, bool* ok = nullptr);

		/**
		 * brief: erase all elements from the iterator (including itself)
		 * param: iterator
		 * return: void
		 */
		void erase_after(const const_iterator& iter1);

		/**
		 * brief: erase all elements in the range [iter1, iter2).
		 * param: two iterators
		 * return: void
		 */
		void erase_after(const const_iterator& iter1, const const_iterator& iter2);

		/**
		 * brief: make the elements unique in a sorted list
		 * param: (void)
		 * return: void
		 */
		void unique();

		/**
		 * brief: link the list to *this
		 * param: another list with the same element type
		 * return: void
		 */
		void link(const forward_list& list_);

		/**
		 * brief: merged two sorted lists (use the old nodes)
		 * param: another list with the same element type, the sorting order (default as ASCENDING, otherwise DESCENDING)
		 * return: void
		 */
		void merge(const forward_list& list_, bool is_ascending = ASCENDING);

	protected:
		// merge the two parts in order
		// Range 1: (first_, mid_]
		// Range 2: (mid_, last_]
		Node* _inplace_merge(Node* first_, Node* mid_, Node* last_, bool is_ascending);
		
		// sort two elements
		inline Node* _sort2(Node* first, bool is_ascending);

		// the merge sort taking pointers and boundary
		Node* _sort(Node* first, size_t bound, bool is_ascending);

	public:
		/**
		 * brief: the merge sort
		 * param: another list with the same element type, the sorting order (default as ASCENDING, otherwise DESCENDING)
		 * return: void
		 */
		void sort(bool is_ascending = ASCENDING);

	};

	template<typename Elem>
	forward_list<Elem>::Node::Node() { }

	template<typename Elem>
	forward_list<Elem>::Node::Node(Elem data_, Node* succ_ptr) : data(data_), succ(succ_ptr) { }

	template<typename Elem>
	forward_list<Elem>::Node::~Node() { }

	template<typename Elem>
	forward_list<Elem>::const_iterator::const_iterator(Node* node_, const forward_list<Elem>* parent_) : node(node_), parent(parent_) { }

	template<typename Elem>
	const Elem& forward_list<Elem>::const_iterator::operator*() const
	{
#ifndef NDEBUG
		if (!node)                error_info("Null pointer in operator * of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_NULLPTR);
		if (node == parent->head) error_info("Underflow in operator * of const_iterator of tvj::forward_list.",    TVJ_FORWARD_LIST_UNDERFLOW);
		if (node == parent->tail) error_info("Overflow in operator * of const_iterator of tvj::forward_list.",     TVJ_FORWARD_LIST_OVERFLOW);
#endif
		return node->data;
	}

	template<typename Elem>
	const Elem* forward_list<Elem>::const_iterator::operator->() const
	{
#ifndef NDEBUG
		if (!node)                error_info("Null pointer in operator -> of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_NULLPTR);
		if (node == parent->head) error_info("Underflow in operator -> of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_UNDERFLOW);
		if (node == parent->tail) error_info("Overflow in operator -> of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_OVERFLOW);
#endif
		return &node->data;
	}

	template<typename Elem>
	auto forward_list<Elem>::const_iterator::operator++()
	{
#ifndef NDEBUG
		if (!node)                error_info("Null pointer in operator ++ of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_NULLPTR);
//		if (node == parent->head) error_info("Underflow in operator ++ of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_UNDERFLOW);
		if (node == parent->tail) error_info("Overflow in operator ++ of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_OVERFLOW);
#endif
		node = node->succ;
		return *this;
	}

	template<typename Elem>
	auto forward_list<Elem>::const_iterator::operator++(int)
	{
#ifndef NDEBUG
		if (!node)                error_info("Null pointer in operator ++ of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_NULLPTR);
//		if (node == parent->head) error_info("Underflow in operator ++ of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_UNDERFLOW);
		if (node == parent->tail) error_info("Overflow in operator ++ of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_OVERFLOW);
#endif
		auto ret = *this;
		node = node->succ;
		return ret;
	}

	template<typename Elem>
	typename forward_list<Elem>::const_iterator forward_list<Elem>::const_iterator::operator+(const unsigned n) const
	{
#ifndef NDEBUG
		if (!node)                error_info("Null pointer in operator + of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_NULLPTR);
//		if (node == parent->head) error_info("Underflow in operator + of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_UNDERFLOW);
		if (node == parent->tail) error_info("Overflow in operator + of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_OVERFLOW);
#endif
		auto _node = node;
		for (unsigned i = 0; i != n; i++, _node = _node->succ)
		{
#ifndef NDEBUG
			if (!node)
				error_info("Null pointer in operator + of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_NULLPTR);
			if(node == parent->tail)
				error_info("Overflow in operator + of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_OVERFLOW);
#endif
		}
		return const_iterator(_node, parent);
	}

	template<typename Elem>
	typename forward_list<Elem>::const_iterator forward_list<Elem>::const_iterator::operator+=(const unsigned n)
	{
#ifndef NDEBUG
		if (!node)                error_info("Null pointer in operator += of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_NULLPTR);
//		if (node == parent->head) error_info("Underflow in operator += of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_UNDERFLOW);
		if (node == parent->tail) error_info("Overflow in operator += of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_OVERFLOW);
#endif
		for (unsigned i = 0; i != n; i++, node = node->succ)
		{
#ifndef NDEBUG
			if (!node)
				error_info("Null pointer in operator += of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_NULLPTR);
			if (node == parent->tail)
				error_info("Overflow in operator += of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_OVERFLOW);
#endif
		}
		return *this;
	}

	template<typename Elem>
	bool forward_list<Elem>::const_iterator::operator==(const const_iterator& iter) const noexcept
	{
#ifndef NDEBUG
//		if (!node)                error_info("Null pointer in operator == of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_NULLPTR);
//		if (node == parent->head) error_info("Underflow in operator == of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_UNDERFLOW);
//		if (node == parent->tail) error_info("Overflow in operator == of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_OVERFLOW);
#endif
		return this->node == iter.node;
	}

	template<typename Elem>
	bool forward_list<Elem>::const_iterator::operator!=(const const_iterator& iter) const noexcept
	{
#ifndef NDEBUG
//		if (!node)                error_info("Null pointer in operator != of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_NULLPTR);
//		if (node == parent->head) error_info("Underflow in operator != of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_UNDERFLOW);
//		if (node == parent->tail) error_info("Overflow in operator != of const_iterator of tvj::forward_list.", TVJ_FORWARD_LIST_OVERFLOW);
#endif
		return this->node != iter.node;
	}

	template<typename Elem>
	Elem& forward_list<Elem>::iterator::operator*()
	{
#ifndef NDEBUG
		if (!const_iterator::node)                                
			error_info("Null pointer in operator * of iterator of tvj::forward_list.", TVJ_FORWARD_LIST_NULLPTR);
		if (const_iterator::node == const_iterator::parent->head)
		    error_info("Underflow in operator * of iterator of tvj::forward_list.", TVJ_FORWARD_LIST_UNDERFLOW);
		if (const_iterator::node == const_iterator::parent->tail)
			error_info("Overflow in operator * of iterator of tvj::forward_list.", TVJ_FORWARD_LIST_OVERFLOW);
#endif
		return const_iterator::node->data;
	}

	template<typename Elem>
	Elem* forward_list<Elem>::iterator::operator->()
	{
#ifndef NDEBUG
		if (!const_iterator::node)
			error_info("Null pointer in operator -> of iterator of tvj::forward_list.", TVJ_FORWARD_LIST_NULLPTR);
		if (const_iterator::node == const_iterator::parent->head)
			error_info("Underflow in operator -> of iterator of tvj::forward_list.", TVJ_FORWARD_LIST_UNDERFLOW);
		if (const_iterator::node == const_iterator::parent->tail)
			error_info("Overflow in operator -> of iterator of tvj::forward_list.", TVJ_FORWARD_LIST_OVERFLOW);
#endif
		return &const_iterator::node->data;
	}

	template<typename Elem>
	forward_list<Elem>::forward_list() { }

	template<typename Elem>
	forward_list<Elem>::forward_list(const forward_list<Elem>& list_)
	{
		for (const auto& elem : list_)
		{
			this->push_back(elem);
		}
	}

	template<typename Elem> template<typename Container_Type>
	forward_list<Elem>::forward_list(const Container_Type& container)
	{
		for (const auto& elem : container)
		{
			this->push_back(elem);
		}
	}

	template<typename Elem> template<typename _Iter>
	forward_list<Elem>::forward_list(const _Iter& i_beg,
		typename std::enable_if<
		! std::is_same<std::decay<_Iter>, std::decay<typename std::vector<Elem>::const_iterator>>::value &&
		! std::is_same<std::decay<_Iter>, std::decay<typename std::vector<Elem>::iterator      >>::value &&
		! std::is_same<std::decay<_Iter>, std::decay<typename std::deque <Elem>::const_iterator>>::value &&
		! std::is_same<std::decay<_Iter>, std::decay<typename std::deque <Elem>::iterator      >>::value &&
		! std::is_same<std::decay<_Iter>, std::decay<typename std::list  <Elem>::const_iterator>>::value &&
		! std::is_same<std::decay<_Iter>, std::decay<typename std::list  <Elem>::iterator      >>::value,
		const _Iter&>::type i_end)
	{
#ifndef NDEBUG
		if (!std::is_class<_Iter>::value)
		{
			error_info("Constructor iterator type mismatch.", TVJ_FORWARD_LIST_TYPE_MISMATCH);
		}
#endif
		for (auto iter = i_beg; iter != i_end; ++iter)
		{
			this->push_back(*iter);
		}
	}

	template<typename Elem> template<typename _Iter>
	forward_list<Elem>::forward_list(const _Iter& i_beg,
		typename std::enable_if<
		std::is_same<std::decay<_Iter>, std::decay<typename std::vector<Elem>::const_iterator>>::value ||
	    std::is_same<std::decay<_Iter>, std::decay<typename std::vector<Elem>::iterator      >>::value ||
	    std::is_same<std::decay<_Iter>, std::decay<typename std::deque <Elem>::const_iterator>>::value ||
	    std::is_same<std::decay<_Iter>, std::decay<typename std::deque <Elem>::iterator      >>::value ||
		std::is_same<std::decay<_Iter>, std::decay<typename std::list  <Elem>::const_iterator>>::value ||
		std::is_same<std::decay<_Iter>, std::decay<typename std::list  <Elem>::iterator      >>::value,
		const _Iter&>::type i_end)
	{
#ifndef NDEBUG
		if (!std::is_class<_Iter>::value)
		{
			error_info("Constructor iterator type mismatch.", TVJ_FORWARD_LIST_TYPE_MISMATCH);
		}
		if (i_end - i_beg < 0)
		{
			error_info("Constructor iterators range error: 'end' before 'begin'", TVJ_FORWARD_LIST_ITER_RANGE);
		}
#endif
		for (auto iter = i_beg; iter != i_end; ++iter)
		{
			this->push_back(*iter);
		}
	}

	template<typename Elem>
	forward_list<Elem>::forward_list(const Elem* i_beg, const Elem* i_end)
	{
#ifndef NDEBUG
		if (!i_beg) error_info("The constructor for tvj::forward_list has pointer i_beg to be a nullptr.", TVJ_FORWARD_LIST_NULLPTR);
		if (!i_end) error_info("The constructor for tvj::forward_list has pointer i_end to be a nullptr.", TVJ_FORWARD_LIST_NULLPTR);
#endif
		for (auto iter = i_beg; iter != i_end; ++iter)
		{
			this->push_back(*iter);
		}
	}

	template<typename Elem>
	forward_list<Elem>::~forward_list()
	{
		erase_after(cbegin());
#ifdef NDEBUG
		if (head) delete head;
		if (tail) delete tail;
#endif
	}

	template<typename Elem>
	void forward_list<Elem>::clear()
	{
		erase_after(cbegin());
	}

	template<typename Elem>
	const auto& forward_list<Elem>::size() const noexcept
	{
		return size_;
	}

	template<typename Elem>
	bool forward_list<Elem>::empty() const noexcept
	{
		return !size_;
	}

	template<typename Elem>
	typename forward_list<Elem>::iterator forward_list<Elem>::before_begin() noexcept
	{
		return iterator(head, this);
	}

	template<typename Elem>
	typename forward_list<Elem>::iterator forward_list<Elem>::begin() noexcept
	{
		return iterator(head->succ, this);
	}

	template<typename Elem>
	typename forward_list<Elem>::iterator forward_list<Elem>::front() noexcept
	{
		return iterator(head->succ, this);
	}

	template<typename Elem>
	typename forward_list<Elem>::iterator forward_list<Elem>::back() noexcept
	{
		auto i = before_begin();
		while (i + 1 != end()) i++;
		return i;
	}

	template<typename Elem>
	typename forward_list<Elem>::iterator forward_list<Elem>::end() noexcept
	{
		return iterator(tail, this);
	}

	template<typename Elem>
	typename forward_list<Elem>::const_iterator forward_list<Elem>::before_begin() const noexcept
	{
		return const_iterator(head, this);
	}

	template<typename Elem>
	typename forward_list<Elem>::const_iterator forward_list<Elem>::begin() const noexcept
	{
		return const_iterator(head->succ, this);
	}

	template<typename Elem>
	typename forward_list<Elem>::const_iterator forward_list<Elem>::front() const noexcept
	{
		return iterator(head->succ, this);
	}

	template<typename Elem>
	typename forward_list<Elem>::const_iterator forward_list<Elem>::back() const noexcept
	{
		return iterator(head->succ, this);
	}

	template<typename Elem>
	typename forward_list<Elem>::const_iterator forward_list<Elem>::end() const noexcept
	{
		return const_iterator(tail, this);
	}

	template<typename Elem>
	typename forward_list<Elem>::const_iterator forward_list<Elem>::cbefore_begin() const noexcept
	{
		return const_iterator(head, this);
	}

	template<typename Elem>
	typename forward_list<Elem>::const_iterator forward_list<Elem>::cbegin() const noexcept
	{
		return const_iterator(head->succ, this);
	}

	template<typename Elem>
	typename forward_list<Elem>::const_iterator forward_list<Elem>::cend() const noexcept
	{
		return const_iterator(tail, this);
	}

	template<typename Elem>
	typename forward_list<Elem>::const_iterator forward_list<Elem>::find(const Elem& elem) const noexcept
	{
		for (auto iter = begin(); iter != end(); iter++)
		{
			if (*iter == elem) return iter;
		}
		return end();
	}

	template<typename Elem>
	typename forward_list<Elem>::const_iterator forward_list<Elem>::search(const Elem& elem, bool is_ascending) const noexcept
	{
		auto iter = before_begin();
		if (is_ascending ? *(iter + 1) < elem : *(iter + 1) > elem) return before_begin();
		while (iter + 1 != cend() && (is_ascending ? *(iter + 1) >= elem : *(iter + 1) <= elem)) ++iter;
		return iter;
	}

	template<typename Elem>
	bool forward_list<Elem>::sorted(bool is_ascending) const noexcept
	{
		if (size_ < 2) return true;
		for (auto iter = cbegin(); iter + 1 != cend(); ++iter)
		{
			if (*iter == *(iter + 1)) continue;
			if ((*iter < *(iter + 1)) ^ is_ascending) return false;
		}
		return true;
	}

	template<typename Elem>
	bool forward_list<Elem>::contains(const Elem& elem) const noexcept
	{
		for (const auto elem_ : *this)
		{
			if (elem == elem_) return true;
		}
		return false;
	}

	template<typename Elem>
	size_t forward_list<Elem>::count(const Elem& elem) const noexcept
	{
		size_t count = 0;
		for (const auto elem_ : *this)
		{
			if (elem == elem_) count++;
		}
		return count;
	}

	template<typename Elem>
	void forward_list<Elem>::assign(const const_iterator& iter, const Elem& elem)
	{
#ifndef NDEBUG
		if (!iter.node)        error_info("Null pointer of 'iter' in function assign of tvj::forward_list.", TVJ_FORWARD_LIST_NULLPTR);
		if (iter.node == head) error_info("Underflow of 'iter' in function assign of tvj::forward_list.", TVJ_FORWARD_LIST_UNDERFLOW);
		if (iter.node == tail) error_info("Overflow of 'iter' in function assign of tvj::forward_list.", TVJ_FORWARD_LIST_OVERFLOW);
#endif
		iter.node->data = elem;
	}

	template<typename Elem>
	void forward_list<Elem>::insert_after(const const_iterator& iter, const Elem& elem)
	{
		insert_after(iter, elem, 1);
	}

	template<typename Elem>
	void forward_list<Elem>::insert_after(const const_iterator& iter, const Elem& elem, size_t n)
	{
		if (n == 0) return;
		for (const_iterator i = cbefore_begin(); (i + 1) != cend(); i++)
		{
			if (i == iter)
			{
				for (size_t j = 0; j != n; j++)
				{
					Node* new_node = new Node(elem, i.node->succ);
					i.node->succ = new_node;
					i++;
					size_++;
				}
				return;
			}
		}
		// add after back()
		for (size_t j = 0; j != n; j++) push_back(elem);
	}

	template<typename Elem>
	void forward_list<Elem>::push_back(const Elem& elem)
	{
		tail->data = elem;
		tail->succ = new Node;
		tail = tail->succ;
		tail->succ = nullptr;
		size_++;
	}

	template<typename Elem>
	void forward_list<Elem>::push_front(const Elem& elem)
	{
		insert_after(const_iterator(head, this), elem);
	}

	template<typename Elem>
	void forward_list<Elem>::pop_back()
	{
		auto i = cbefore_begin();
		if (empty()) return;
		while (i + 2 != cend())
		{
			i++;
		}
		auto tmp = i.node->succ;
		i.node->succ = tail;
		if (tmp) delete tmp;
		size_--;
	}

	template<typename Elem>
	void forward_list<Elem>::pop_front()
	{
		if (empty()) return;
		head = head->succ;
		size_--;
	}

	template<typename Elem>
	Elem forward_list<Elem>::remove_at(const const_iterator& iter, bool* ok)
	{
		if (ok) *ok = false;
		for (auto i = cbefore_begin(); (i + 1) != cend(); i++)
		{
			if (i + 1 != iter) continue;
			Elem ret = *(i + 1);
			i.node->succ = (i + 1).node->succ;
			size_--;
			if (ok) *ok = true;
			return ret;
		}
		return tail->data;
	}

	template<typename Elem>
	void forward_list<Elem>::erase_after(const const_iterator& iter1)
	{
		if (empty()) return;
		const_iterator iter2 = cend();
		for (auto i = cbefore_begin(); i + 1 != cend(); i++)
		{
			if (i + 1 != iter1) continue;
			size_t tmp_size = 0;
			auto j = i + 1;
			while (j.node && j != iter2)
			{
				auto to_delete = j.node;
				++j;
#ifdef NDEBUG
				if (to_delete) delete to_delete;
#endif
				tmp_size++;
			}
			size_ -= tmp_size;
			i.node->succ = j.node;
			return;
		}
	}

	template<typename Elem>
	void forward_list<Elem>::erase_after(const const_iterator& iter1, const const_iterator& iter2)
	{
		if (empty()) return;
		for (auto i = cbefore_begin(); i + 1 != cend(); i++)
		{
			if (i.node && i + 1!= iter1) continue;
			size_t tmp_size = 0;
			auto j = i + 1;
			while (j != iter2 && j != cend())
			{
				auto to_delete = j.node;
				++j;
#ifdef NDEBUG
				if (to_delete) delete to_delete;
#endif
				tmp_size++;
			}
			size_ -= tmp_size;
			i.node->succ = j.node;
			return;
		}
	}

	template<typename Elem>
	void forward_list<Elem>::unique()
	{
		if (size_ < 2) return;

#ifndef NDEBUG
		if (!sorted()) sort();
#endif

		auto i = cbegin();
		for (; i != cend() && i + 1 != cend() && i + 2 != cend(); i++)
		{
			while (i != cend() && *i == *(i + 1))
			{
				auto tmp = (i + 1).node;
				i.node->succ = (i + 2).node->succ;
				delete tmp;
				size_--;
			}
		}
		if (*i == *(i + 1)) pop_back();
	}

	template<typename Elem>
	void forward_list<Elem>::link(const forward_list& list_)
	{
		if (list_.empty()) return;

		// copy the list first otherwise it uses nodes in list_ which is unsafe
		forward_list<Elem> new_list = list_;

		size_ += list_.size_;
		this->tail->data = new_list.head->succ->data;
		this->tail->succ = new_list.head->succ->succ;
		this->tail = new_list.tail;
	}

	template<typename Elem>
	void forward_list<Elem>::merge(const forward_list& list_, bool is_ascending)
	{
		if (list_.empty()) return;

		// copy the list first otherwise it uses nodes in list_ which is unsafe
		forward_list<Elem> new_list = list_;

#ifndef NDEBUG		
		if (!this->sorted(is_ascending))    this->sort(is_ascending);
		if (!list_.sorted(is_ascending)) new_list.sort(is_ascending);
#endif

		auto first_1 = this->head;
		auto end_1   = this->tail;
		auto first_2 = new_list.head;
		auto end_2   = new_list.tail;

		auto i = first_1->succ;
		auto j = first_2->succ;
		auto h = this->head;

		bool label = true; // true for i, false for j
		size_t new_size = 0;

		while (i && i != end_1 && j && j != end_2)
		{
			if (i->data == j->data)
			{
				h = h->succ = i;
				i = i->succ;
				j = j->succ;
				label = true;
			}
			else if ((i->data < j->data) ^ !is_ascending)
			{
				h = h->succ = i;
				i = i->succ;
				label = true;
			}
			else
			{
				h = h->succ = j;
				j = j->succ;
			}
			new_size++;
		}
		while (i && i != end_1)
		{
			h = h->succ = i;
			i = i->succ;
			label = true;
			new_size++;
		}
		while (j && j != end_2)
		{
			h = h->succ = j;
			j = j->succ;
			label = false;
			new_size++;
		}
		tail = h->succ;
		size_ = new_size;
	}

	template<typename Elem>
	typename forward_list<Elem>::Node* forward_list<Elem>::_inplace_merge(Node* first_, Node* mid_, Node* last_, bool is_ascending)
	{
		auto	 i = first_->succ;
		auto j = mid_->succ;
		auto pre_end = last_->succ;
		auto h = first_;
		while (i && i != mid_->succ && j && j != last_->succ)
		{
			if ((i->data < j->data) ^ !is_ascending)
			{
				h = h->succ = i;
				i = i->succ;
			}
			else
			{
				h = h->succ = j;
				j = j->succ;
			}
		}
		while (i && i != mid_->succ)
		{
			h = h->succ = i;
			i = i->succ;
		}
		while (j && j != last_->succ)
		{
			h = h->succ = j;
			j = j->succ;
		}
		h = pre_end;
		return (last_->data < mid_->data) ^ !is_ascending ? mid_ : last_;
	}

	template<typename Elem>
	typename forward_list<Elem>::Node* forward_list<Elem>::_sort2(Node* first, bool is_ascending)
	{
		if (is_ascending ^ (first->succ->data < first->succ->succ->data))
		{
			auto tmp = first->succ->data;
			first->succ->data = first->succ->succ->data;
			first->succ->succ->data = tmp;
		}
		return first->succ->succ;
	}

	template<typename Elem>
	typename forward_list<Elem>::Node* forward_list<Elem>::_sort(Node* first, size_t bound, bool is_ascending)
	{
		if (bound == 0) return nullptr;
		if (bound == 1) return first->succ;
		if (bound == 2) return _sort2(first, is_ascending);
		const auto half_bound = bound / 2;
		const auto mid_node = _sort(first, half_bound, is_ascending);
		const auto last_node = _sort(mid_node, bound - half_bound, is_ascending);
		return _inplace_merge(first, mid_node, last_node, is_ascending);
	}

	template<typename Elem>
	void forward_list<Elem>::sort(bool is_ascending)
	{
		_sort(head, size_, is_ascending);
	}
};

// ALL RIGHTS RESERVED (C) 2021 Teddy van Jerry