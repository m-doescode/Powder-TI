/*********************************************************************************************/
// class map:
// using rbtree as base container
// including some basic functions of map:
//
// 1. iterator begin() const;
// 2. iterator end() const;
// 3. size_type size() const;
// 4. bool empty() const;
// 5. iterator find(const T& v) const;
// 6. void insert(const T& v);
// 7. void erase(iterator pos);
// 8. void clear();

#ifndef EZSTL_MAP_H
#define EZSTL_MAP_H

#include "functional.h"
#include "memory.h"
#include "rbtree.h"
#include "utility.h"

namespace ezSTL {
	template<typename Key, typename Value, typename Compare = less<pair<Key, Value>>, 
		typename Alloc = allocator<__rbtree_node<pair<Key, Value>>>>
	class map {
	public:
		using size_type = unsigned int;
		using value_type = Value;
		using iterator = __rbtree_iterator<pair<Key, Value>>;
		using const_iterator = const iterator;
		using difference_type = int;
	protected:
		rbtree<pair<Key, Value>, Compare, Alloc> base_rbt;
	public:

		iterator begin() const {
			return base_rbt.begin();
		}
		iterator end() const {
			return base_rbt.end();
		}
		size_type size() const {
			return base_rbt.size();
		}
		bool empty() const {
			return base_rbt.empty();
		}
		// find a pair according a specific Key 
		iterator find(const Key& v) const {
			return base_rbt.find(make_pair(v, Value()));
		}
		// insert a pair
		void insert(const pair<Key, Value>& v) {
			base_rbt.insert(v);
		}
		// erase an iterator
		void erase(iterator pos) {
			base_rbt.erase(pos);
		}
		void clear() {
			base_rbt.clear();
		}
	};
}

#endif // !EZSTL_MAP_H
