/*********************************************************************************************/
// class queue:
// using list as base container
// including some basic functions of queue:
//
// 1. bool empty() const;
// 2. size_type size() const;
// 3. reference front() const;
// 4. reference back() const;
// 5. void push(const_reference x);
// 6. void pop();
//
// class priority_queue:
// using vector as base container
// including some basic functions of priority_queue:
//
// 1. bool empty() const;
// 2. size_type size() const;
// 3. reference top() const;
// 4. void push(const_reference x);
// 5. void pop();

#ifndef EZSTL_QUEUE_H
#define EZSTL_QUEUE_H

#include "list.h"
#include "functional.h"

namespace ezSTL {

	// use list as base container
	template<typename T>
	class queue {
	protected:
		list<T, allocator<__list_node<T>>> base_list;
	public:
		using value_type = T;
		using size_type = unsigned int;
		using reference = T&;
		using const_reference = const T&;

		bool empty() const {
			return base_list.empty();
		}
		size_type size() const {
			return base_list.size();
		}
		reference front() const {
			return base_list.front();
		}
		reference back() const {
			return base_list.back();
		}
		void push(const_reference x) {
			base_list.push_back(x);
		}
		void pop() {
			base_list.pop_front();
		}
	};

	template<typename T, typename Compare = less<T>>
	class priority_queue {
	protected:
		vector<T, allocator<T>> base_vector;
		Compare cmp;
	public:
		using value_type = T;
		using size_type = unsigned int;
		using reference = T&;
		using const_reference = const T&;

		bool empty() const {
			return base_vector.empty();
		}
		size_type size() const {
			return base_vector.size();
		}
		reference top() const {
			return base_vector.front();
		}
		void push(const_reference x) {
			base_vector.push_back(x);
			// percolateUp
			for (size_type i = size() - 1; i > 0;) {
				// swap new inserted value and its parent
				if (cmp(base_vector[i], base_vector[(i - 1) / 2])) {
					ezSTL::swap(base_vector[i], base_vector[(i - 1) / 2]);
					i = (i - 1) / 2;
				}
				else break;
			}
		}
		void pop() {
			size_type i = 0;
			base_vector[0] = base_vector[size() - 1];
			base_vector.pop_back();
			// percolateDown
			// if it has both left child and right child
			while (i * 2 + 2 < size()) {
				if (cmp(base_vector[i * 2 + 1], base_vector[i]) &&
					!cmp(base_vector[i * 2 + 2], base_vector[i * 2 + 1])) {
					ezSTL::swap(base_vector[i], base_vector[i * 2 + 1]);
					i = i * 2 + 1;
				}
				else if (cmp(base_vector[i * 2 + 2], base_vector[i]) &&
					!cmp(base_vector[i * 2 + 1], base_vector[i * 2 + 2])) {
					ezSTL::swap(base_vector[i], base_vector[i * 2 + 2]);
					i = i * 2 + 2;
				}
				else break;
			}
			// if it has only left child
			if (i * 2 + 1 < size()) {
				if (cmp(base_vector[i * 2 + 1], base_vector[i]))
					ezSTL::swap(base_vector[i], base_vector[i * 2 + 1]);
			}
		}
	};
}

#endif // !EZSTL_QUEUE_H

