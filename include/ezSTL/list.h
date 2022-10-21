/*********************************************************************************************/
// class list:
// using bidirectional list as basic data structure
// including some basic functions of list:
//
// 1. iterator begin() const;
// 2. iterator end() const;
// 3. size_type size() const;
// 4. size_type capacity() const;
// 5. bool empty() const;
// 6. reference front() const;
// 7. reference back() const;
// 8. void push_back(const_reference x);
// 9. void push_front(const_reference x);
// 10. void pop_back();
// 11. void pop_front();
// 12. iterator insert(iterator pos, const_reference x);
// 13. iterator erase(iterator pos);
// 14. void clear();

#ifndef EZSTL_LIST_H
#define EZSTL_LIST_H

#include "iterator.h"
#include "memory.h"

namespace ezSTL {

	// list node
	template<typename T>
	class __list_node {
	public:
		using link_type = __list_node<T>*;
		link_type prev;
		link_type next;
		T data;
		// constructor for __list_node
		__list_node() = default;
		__list_node(const __list_node<T>& n) : prev(n.prev), next(n.next), data(n.data) {}
		explicit __list_node(const T& d): prev(nullptr), next(nullptr), data(d) {}
	};

	// list iterator
	template<typename T>
	class __list_iterator: public iterator<bidirectional_iterator_tag, T> {
	public:
		using iterator = __list_iterator<T>;
		using self = __list_iterator<T>;
		using link_type = __list_node<T>*;
		using pointer = T*;
		using reference = T&;
		using difference_type = int;

		// __list_node pointer
		link_type node;

		__list_iterator() = default;
		__list_iterator(link_type x) : node(x) {}
		__list_iterator(const iterator& x) : node(x.node) {}
		self& operator= (const iterator& x) {
			if (this != &x)
				node = x.node;
			return *this;
		}

		bool operator== (const self& x) const {
			return node == x.node;
		}
		bool operator!= (const self& x) const {
			return node != x.node;
		}
		reference operator*() const {
			return (*node).data;
		}
		pointer operator->() const {
			return &(operator*());
		}
		// go to next
		self& operator++() {
 			node = node->next;
			return *this;
		}
		self& operator++(int) {
			self temp = *this;
			++(*this);
			return temp;
		}
		// go to prev
		self& operator--() {
			node = node->prev;
			return *this;
		}
		self& operator--(int) {
			self temp = *this;
			--(*this);
			return temp;
		}

		self operator+ (difference_type n) const {
			self temp = *this;
			advance(temp, n);
			return temp;
		}

		self operator-(difference_type n) const {
			self temp = *this;
			advance(temp, -n);
			return temp;
		}
	};

	// class list
	template<typename T, typename Alloc = allocator<__list_node<T>>>
	class list {
	public:
		using list_node = __list_node<T>;
		using link_type = __list_node<T>*;
		using size_type = unsigned int;
		using value_type = T;
		using iterator = __list_iterator<T>;
		using const_iterator = const iterator;
		using reference = value_type&;
		using const_reference = const T&;
		using difference_type = int;

	protected:
		iterator list_iterator;
		size_type __size;

	public:
		// constructor
		list() : __size(0) {
			link_type new_node = Alloc().allocate();
			Alloc().construct(new_node, __list_node<T>());
			list_iterator.node = new_node;
			list_iterator.node->prev = list_iterator.node;
			list_iterator.node->next = list_iterator.node;
		}
		list(const list<T, Alloc>&);
		list<T, Alloc>& operator=(const list<T, Alloc>&);
		list(list<T, Alloc>&& other) noexcept: list_iterator(other.list_iterator), __size(other.__size) {
			other.__size = 0;
		}
		list<T, Alloc>& operator=(list<T, Alloc>&& other) noexcept {
			list_iterator = other.list_iterator;
			__size = other.__size;
			other.__size = 0;
			return *this;
		}
		// destructor
		~list() {
			if (!empty()) {
				clear();
				Alloc().destroy(list_iterator.node);
				Alloc().deallocate(list_iterator.node);
			}
		}

		// some common functions for list
		const_iterator begin() const {
			return list_iterator + 1;
		}
		const_iterator end() const {
			return list_iterator;
		}
		reference front() const {
			return *begin();
		}
		reference back() const {
			return *(end() - 1);
		}
		size_type size() const {
			return __size;
		}
		bool empty() const {
			return size() == 0;
		}
		void push_back(const_reference x);
		void push_front(const_reference x);
		void pop_back();
		void pop_front();
		iterator insert(iterator pos, const_reference x);
		iterator erase(iterator pos);
		void clear();
	};

	template<typename T, typename Alloc>
	list<T, Alloc>::list(const list<T, Alloc>& other) {
		Alloc alloc = Alloc();
		link_type prev_node = alloc.allocate();
		alloc.construct(prev_node, __list_node<T>());
		list_iterator.node = prev_node;
		__size = 0;
		// copy each node of other
		for (iterator ite = other.begin(); ite != other.end(); ++ite) {
			link_type new_node = alloc.allocate();
			alloc.construct(new_node, __list_node<T>(*ite));
			if (ite == other.begin())
				list_iterator.node->next = new_node;
			prev_node->next = new_node;
			new_node->prev = prev_node;
			prev_node = new_node;
			++__size;
		}
		prev_node->next = list_iterator.node;
		list_iterator.node->prev = prev_node;
	}

	template<typename T, typename Alloc>
	list<T, Alloc>& list<T, Alloc>::operator=(const list<T, Alloc>& other) {
		if (this != &other) {
			Alloc alloc = Alloc();
			link_type prev_node = alloc.allocate();
			alloc.construct(prev_node, __list_node<T>());
			list_iterator.node = prev_node;
			__size = 0;
			// copy each node of other
			for (iterator ite = other.begin(); ite != other.end(); ++ite) {
				link_type new_node = alloc.allocate();
				alloc.construct(new_node, __list_node<T>(*ite));
				if (ite == other.begin())
					list_iterator.node->next = new_node;
				prev_node->next = new_node;
				new_node->prev = prev_node;
				prev_node = new_node;
				++__size;
			}
			prev_node->next = list_iterator.node;
			list_iterator.node->prev = prev_node;
		}
		return *this;
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::push_back(const_reference x){
		// alloacate space and construct
		link_type new_node = Alloc().allocate();
		Alloc().construct(new_node, __list_node<T>(x));
		// adjust pointers when list is empty or not
		new_node->next = list_iterator.node;
		new_node->prev = list_iterator.node->prev;
		list_iterator.node->prev->next = new_node;
		list_iterator.node->prev = new_node;
		++__size;
	}
	template<typename T, typename Alloc>
	void list<T, Alloc>::push_front(const_reference x){
		// alloacate space and construct
		link_type new_node = Alloc().allocate();
		Alloc().construct(new_node, __list_node<T>(x));
		// adjust pointers
		new_node->next = list_iterator.node->next;
		new_node->prev = list_iterator.node;
		list_iterator.node->next->prev = new_node;
		list_iterator.node->next = new_node;
		++__size;
	}
	template<typename T, typename Alloc>
	void list<T, Alloc>::pop_back(){
		iterator back_iterator = end() - 1;
		// adjust pointers
		list_iterator.node->prev = back_iterator.node->prev;
		back_iterator.node->prev->next = list_iterator.node;
		// destroy object and deallocate space
		Alloc().destroy(back_iterator.node);
		Alloc().deallocate(back_iterator.node);
		--__size;
	}
	template<typename T, typename Alloc>
	void list<T, Alloc>::pop_front(){
		iterator front_iterator = begin();
		// adjust pointers
		list_iterator.node->next = front_iterator.node->next;
		front_iterator.node->next->prev = list_iterator.node;
		// destroy object and deallocate space
		Alloc().destroy(front_iterator.node);
		Alloc().deallocate(front_iterator.node);
		--__size;
	}

	template<typename T, typename Alloc>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::insert(iterator pos, const_reference x) {
		// alloacate space and construct
		link_type new_node = Alloc().allocate();
		Alloc().construct(new_node, __list_node<T>(x));
		// adjust pointers
		new_node->next = pos.node;
		new_node->prev = pos.node->prev;
		pos.node->prev->next = new_node;
		pos.node->prev = new_node;
		++__size;
		return pos - 1;
	}

	template<typename T, typename Alloc>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::erase(iterator pos) {
		iterator before_iterator = pos - 1;
		// adjust pointers
		before_iterator.node->next = pos.node->next;
		pos.node->next->prev = before_iterator.node;
		// destroy object and deallocate space
		Alloc().destroy(pos.node);
		Alloc().deallocate(pos.node);
		--__size;
		return before_iterator;
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::clear() {
		Alloc alloc = Alloc();
		// clear all node
		for (iterator ite = begin(), next_iterator = begin(); ite != end(); ite = next_iterator) {
			next_iterator = ite + 1;
			alloc.destroy(ite.node);
			alloc.deallocate(ite.node);
		}
		list_iterator.node->prev = list_iterator.node;
		list_iterator.node->next = list_iterator.node;
		__size = 0;
	}
}

#endif // !EZSTL_LIST_H

