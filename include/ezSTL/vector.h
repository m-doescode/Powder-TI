/*********************************************************************************************/
// class vector:
// using dynamic array as basic data structure
// including some basic functions of vector:
//
// 1. iterator begin() const;
// 2. iterator end() const;
// 3. size_type size() const;
// 4. size_type capacity() const;
// 5. bool empty() const;
// 6. reference operator[] (const size_type i);
// 7. const_reference operator[] (const size_type i) const;
// 8. reference front() const;
// 9. reference back() const;
// 10. void push_back(const_reference x);
// 11. void pop_back();
// 12. iterator insert(const_iterator pos, const_reference x);
// 13. iterator erase(iterator pos);
// 14. void resize(size_type n);
// 15. void clear();

#ifndef EZSTL_VECTOR_H
#define EZSTL_VECTOR_H

#include "iterator.h"
#include "memory.h"

namespace ezSTL {

	template<typename T, typename Alloc = allocator<T>>
	// TEMPLATE CLASS VECTOR
	class vector {
	public:
		using value_type = T;
		using iterator = value_type*;
		using const_iterator = const iterator;
		using reference = value_type&;
		using const_reference = const T&;
		using size_type = unsigned int;
		using difference_type = int;

		// constructor
		vector() :start(nullptr), finish(nullptr), end_of_storage(nullptr) {}
		vector(size_type n, const_reference value) {
			fill_initialize(n, value);
		}
		explicit vector(size_type n) {
			fill_initialize(n, value_type());
		}
		vector(const vector<value_type, Alloc>& v);
		vector<value_type, Alloc>& operator= (const vector<value_type, Alloc>& v);
		vector(vector<value_type, Alloc>&& v) noexcept;
		vector<value_type, Alloc>& operator= (vector<value_type, Alloc>&& v) noexcept;
		// destructor
		~vector() {
			destroy(start, finish);
			deallocate();
		}

		// some common functions of vector
		iterator begin() const {
			return start;
		}
		iterator end() const {
			return finish;
		}
		size_type size() const {
			return end() - begin();
		}
		size_type capacity() const {
			return end_of_storage - begin();
		}
		bool empty() const {
			return begin() == end();
		}
		// operator []
		reference operator[] (const size_type i) {
			return *(begin() + i);
		}
		const_reference operator[] (const size_type i) const {
			return *(begin() + i);
		}
		reference front() const {
			return *begin();
		}
		reference back() const {
			return *(end() - 1);
		}

		void push_back(const_reference x);
		void pop_back();
		iterator insert(const_iterator pos, const_reference x);
		iterator erase(iterator pos);
		void resize(size_type n);
		void clear();

	protected:
		// begining of space that has been used
		iterator start;
		// end of space that has been used
		iterator finish;
		// end of space that can be used
		iterator end_of_storage;
		// internal function, allocate n elements and fill with value
		iterator allocate_and_fill(size_type n, const_reference value) {
			iterator result = allocate(n);
			for (size_type i = 0; i < n; ++i)
				construct(result++, value);
			return result - n;
		}
		// internal function, fill n elements with value, and then adjust pointers
		void fill_initialize(size_type n, const_reference value) {
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}

		void construct(iterator ptr, const_reference value) {
			Alloc().construct(ptr, value);
		}

		void destroy(iterator ite) {
			Alloc().destroy(ite);
		}

		void destroy(iterator first, iterator last) {
			Alloc alloc = Alloc();
			while (first < last)
				alloc.destroy(first++);
		}

		iterator allocate(size_type n) {
			return Alloc().allocate(n);
		}

		void deallocate() {
			if (start)
				Alloc().deallocate(start);
		}
	};

	template<typename T, typename Alloc>
	vector<T, Alloc>::vector(const vector<T, Alloc>& v) {
		// copy from v to *this
		start = allocate(v.size());
		for (size_type i = 0; i < v.size(); ++i)
			construct(start + i, v[i]);
		finish = start + v.size();
		end_of_storage = finish;
	}

	template<typename T, typename Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator= (const vector<T, Alloc>& v) {
		if (this == &v) return *this;
		else {
			// destroy and deallocate old vector
			destroy(begin(), end());
			deallocate();
			// allocate and construct new vector
			start = allocate(v.size());
			for (size_type i = 0; i < v.size(); ++i)
				construct(start + i, v[i]);
			finish = start + v.size();
			end_of_storage = finish;
			return *this;
		}
	}

	// move constructor
	template<typename T, typename Alloc>
	vector<T, Alloc>::vector(vector<T, Alloc>&& v) noexcept: start(v.start), finish(v.finish),
		end_of_storage(v.end_of_storage) {
		v.start = nullptr;
		v.finish = nullptr;
		v.end_of_storage = nullptr;
	}

	// move assignment operator
	template<typename T, typename Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator= (vector<T, Alloc>&& v) noexcept {
		if (this == &v) return *this;
		else {
			start = v.start;
			finish = v.finish;
			end_of_storage = v.end_of_storage;
			v.start = nullptr;
			v.finish = nullptr;
			v.end_of_storage = nullptr;
			return *this;
		}
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::push_back(const_reference x) {
		// if there is still extra space
		if (finish != end_of_storage) {
			construct(finish++, x);
		}
		else
		{
			size_type old_size = size();
			// calculate new size
			size_type new_size = old_size ? 2 * old_size : 1;
			iterator new_start = allocate(new_size);
			iterator temp = new_start;
			for (size_type i = 0; i < old_size; ++i)
				construct(temp++, (*this)[i]);
			destroy(start, finish);
			deallocate();
			construct(temp++, x);
			start = new_start;
			finish = temp;
			end_of_storage = start + new_size;
		}
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::pop_back() {
		destroy(--finish);
	}

	template<typename T, typename Alloc>
	typename vector<T, Alloc>::iterator
		vector<T, Alloc>::insert(const_iterator pos, const_reference x) {
		if (finish != end_of_storage) {
			for (auto ite = end(); ite != pos; --ite) {
				if (ite != end()) destroy(ite);
				*ite = *(ite - 1);
			}
			construct(pos, x);
			++finish;
			return pos;
		}
		else {
			size_type old_size = size();
			size_type new_size = old_size ? 2 * old_size : 1;
			// allocate new space
			iterator new_start = allocate(new_size);
			iterator temp = new_start;
			// copy values before pos
			for (auto ite = begin(); ite != pos; ++ite)
				construct(temp++, *ite);
			// insert new value x
			auto res = temp;
			construct(temp++, x);
			for (auto ite = pos; ite != end(); ++ite)
				construct(temp++, *ite);
			destroy(start, finish);
			deallocate();
			start = new_start;
			finish = temp;
			end_of_storage = start + new_size;
			return res;
		}
	}

	template<typename T, typename Alloc>
	typename vector<T, Alloc>::iterator
		vector<T, Alloc>::erase(iterator pos) {
		for (auto ite = pos; ite != end(); ++ite) {
			destroy(ite);
			if(ite + 1 != end())
				construct(ite, *(ite + 1));
		}
		--finish;
		return pos;
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::resize(size_type n) {
		size_type old_size = size();
		// if old size is bigger than n
		if (old_size >= n) {
			for (; old_size > n; --old_size) {
				pop_back();
			}
		}
		else {
			// allocate more space
			iterator new_start = allocate(n);
			for (size_type i = 0; i < n; ++i) {
				if(i < size()) construct(new_start + i, (*this)[i]);
				else construct(new_start + i, T());
			}
			clear();
			start = new_start;
			finish = start + n;
			end_of_storage = finish;
		}
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::clear() {
		destroy(begin(), end());
		finish = start;
	}
}

#endif // !EZSTL_VECTOR_H

