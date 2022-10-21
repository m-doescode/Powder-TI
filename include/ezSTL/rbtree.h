/*********************************************************************************************/
// class rbtree:
// an internal class, basic data structure of set and map
//
// 1. iterator begin() const;
// 2. iterator end() const;
// 3. size_type size() const;
// 4. bool empty() const;
// 5. iterator find(const T& v) const;
// 6. void insert(const T& v);
// 7. void erase(iterator pos);
// 8. void clear();
// only for debug:
// 9. bool isRBTree();

#ifndef EZSTL_RBTREE_H
#define EZSTL_RBTREE_H

#include "functional.h"
#include "iterator.h"
#include "memory.h"
#include "utility.h"

namespace ezSTL {

	// color type of RB Tree
	using __rbtree_color_type = bool;
	const __rbtree_color_type __rbtree_red = false;
	const __rbtree_color_type __rbtree_black = true;

	// node of RB Tree
	template<typename T>
	class __rbtree_node {
	public:
		__rbtree_node(const T& t) : color(__rbtree_red), parent(nullptr), left(nullptr), right(nullptr), data(t) {}
		// some type definition
		using color_type = __rbtree_color_type;
		using value_type = T;
		using pointer = __rbtree_node<value_type>*;

		// data members
		color_type color;
		pointer parent;
		pointer left;
		pointer right;
		value_type data;

		// find the minimum value
		static pointer minimum(pointer x) {
			while (x->left != nullptr)
				x = x->left;
			return x;
		}

		// find the maximum value
		static pointer maximum(pointer x) {
			while (x->right != nullptr)
				x = x->right;
			return x;
		}
	};

	// iterator of RB Tree
	template<typename T>
	class __rbtree_iterator : public iterator<bidirectional_iterator_tag, T> {
	public:
		using iterator = __rbtree_iterator<T>;
		using self = __rbtree_iterator<T>;
		using link_type = __rbtree_node<T>*;
		using pointer = T*;
		using reference = T&;
		using difference_type = int;

		// __rbtree_node pointer
		link_type node;

		__rbtree_iterator() = default;
		__rbtree_iterator(link_type x) : node(x) {}
		__rbtree_iterator(const iterator& x) : node(x.node) {}
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
		self& operator++() {
			// if it has right node
			if (node->right != nullptr) {
				node = node->right;
				while (node->left != nullptr)
					node = node->left;
			}
			else {
				link_type parent_node = node->parent;
				while (node == parent_node->right) {
					node = parent_node;
					parent_node = parent_node->parent;
				}
				if (node->right != parent_node)
					node = parent_node;
			}
			return *this;
		}
		self& operator++(int) {
			self temp = *this;
			++(*this);
			return temp;
		}
		self& operator--() {
			// present node is header
			if (node->color == __rbtree_red && node->parent->parent == node)
				node = node->right;
			// if node has left child
			else if (node->left != nullptr) {
				node = node->left;
				while (node->right != nullptr)
					node = node->right;
			}
			else {
				link_type parent_node = node->parent;
				while (node == parent_node->left) {
					node = parent_node;
					parent_node = parent_node->parent;
				}
				node = parent_node;
			}
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

	// RB Tree class
	template<typename T, typename Compare = less<T>, typename Alloc = allocator<__rbtree_node<T>>>
	class rbtree {
	
	public:
		// some type definition for rbtree
		using rbtree_node = __rbtree_node<T>;
		using link_type = __rbtree_node<T>*;
		using size_type = unsigned int;
		using value_type = T;
		using iterator = __rbtree_iterator<T>;
		using const_iterator = const iterator;
		using reference = value_type&;
		using const_reference = const T&;
		using difference_type = int;

	protected:
		// data members of rbtree
		size_type __size;
		iterator header;
		Compare cmp;

	public:
		// constructor
		rbtree() : __size(0) {
			header.node = Alloc().allocate();
			header.node->color = __rbtree_red;
			header.node->parent = nullptr;
			header.node->left = header.node;
			header.node->right = header.node;
		}
		rbtree(const rbtree<T, Compare, Alloc>& t) {}
		rbtree(rbtree<T, Compare, Alloc>&& t) noexcept {}
		rbtree<T, Compare, Alloc>& operator=(const rbtree<T, Compare, Alloc>& t) {}
		rbtree<T, Compare, Alloc>& operator=(rbtree<T, Compare, Alloc>&& t) noexcept {}
		~rbtree() {
			clear();
		}

	protected:
		// some common functions for header
		link_type root() const {
			return header.node->parent;
		}
		link_type leftmost() const {
			return header.node->left;
		}
		link_type rightmost() const {
			return header.node->right;
		}
		// rotate
		void left_rotate(link_type z);
		void right_rotate(link_type z);
		// fixup after inserting a new value
		void insert_fixup(link_type z);
		// fixup after deleting a value
		void delete_fixup(link_type father, link_type son);
		void dfs_clear(link_type root);
		bool dfs_isRBTree(link_type root, unsigned int count_of_black_now, const unsigned int count_of_black);

	public:
		// some outer interfaces
		iterator begin() const {
			return leftmost();
		}
		iterator end() const {
			return header;
		}
		bool empty() const {
			return __size == 0;
		}
		size_type size() const {
			return __size;
		}

		iterator find(const value_type& v) const;
		void insert(const value_type& v);
		void erase(iterator pos);
		void clear();

		// interface for debug
		// judge if the present tree is a RBTree
		bool isRBTree();
	};

	// find a value, O(lgn)
	template<typename T, typename Compare, typename Alloc>
	typename rbtree<T, Compare, Alloc>::iterator rbtree<T, Compare, Alloc>::find(const value_type& v) const {
		link_type x = root();
		while (x != nullptr) {
			if (cmp(v, x->data))
				x = x->left;
			else if (cmp(x->data, v))
				x = x->right;
			else
				return iterator(x);
		}
		return end();
	}

	template<typename T, typename Compare, typename Alloc>
	void rbtree<T, Compare, Alloc>::insert(const value_type& v) {
		// z : node to be inserted
		link_type z = Alloc().allocate();
		Alloc().construct(z, v);
		// y : parent node of z
		link_type y = nullptr;
		// x : node used to find proper position
		link_type x = root();
		// find the proper position to insert
		while (x != nullptr) {
			y = x;
			if (cmp(v, x->data))
				x = x->left;
			else if(cmp(x->data, v))
				x = x->right;
			// if v == x->data , renew it
			else {
				x->data = v;
				return;
			}
		}
		z->parent = y;
		// if it is an empty tree
		if (y == nullptr) {
			header.node->parent = z;
			header.node->left = z;
			header.node->right = z;
			z->parent = header.node;
		}
		else if (cmp(v, y->data))
			y->left = z;
		else y->right = z;
		// fix up
		insert_fixup(z);
		// adjust header: left and right
		if (empty() || cmp(v, leftmost()->data))
			header.node->left = z;
		if (empty() || cmp(rightmost()->data, v))
			header.node->right = z;
		++__size;
	}

	template<typename T, typename Compare, typename Alloc>
	void rbtree<T, Compare, Alloc>::erase(iterator pos) {
		// adjust leftmost and rightmost
		if (pos == begin())
			header.node->left = (pos + 1).node;
		if(pos == (end() - 1))
			header.node->right = (pos - 1).node;
		link_type todelete = nullptr;
		if (pos.node->left == nullptr || pos.node->right == nullptr)
			todelete = pos.node;
		else
			todelete = (pos + 1).node;
		
		// son: son of node todelete, father: parent of son
		link_type son = nullptr, father = nullptr;
		if (todelete->left != nullptr)
			son = todelete->left;
		else
			son = todelete->right;
		if (son)
			son->parent = todelete->parent;
		father = todelete->parent;
		if (todelete == father->left)
			father->left = son;
		else if(todelete == father->right)
			father->right = son;
		if (todelete == root())
			header.node->parent = son;
		// copy data from node to be deleted to pos
		if (todelete != pos.node)
			pos.node->data = todelete->data;
		// if the deleted node is black, then fix up
		if (todelete->color == __rbtree_black)
			delete_fixup(father, son);
		// deallocate node
		Alloc().destroy(todelete);
		Alloc().deallocate(todelete);
		--__size;
	}

	template<typename T, typename Compare, typename Alloc>
	void rbtree<T, Compare, Alloc>::clear() {
		// dfs clear
		if (root())
			dfs_clear(root());
		header.node->left = header.node;
		header.node->right = header.node->right;
		header.node->parent = nullptr;
		header.node->color = __rbtree_red;
		__size = 0;
	}

	template<typename T, typename Compare, typename Alloc>
	void rbtree<T, Compare, Alloc>::left_rotate(link_type z) {
		link_type y = z->right;
		z->right = y->left;
		if (y->left)
			y->left->parent = z;
		y->parent = z->parent;
		// change root
		if (z == root())
			header.node->parent = y;
		else if (z == z->parent->left)
			z->parent->left = y;
		else if (z == z->parent->right)
			z->parent->right = y;
		y->left = z;
		z->parent = y;
	}

	template<typename T, typename Compare, typename Alloc>
	void rbtree<T, Compare, Alloc>::right_rotate(link_type z) {
		link_type y = z->left;
		z->left = y->right;
		if (y->right)
			y->right->parent = z;
		y->parent = z->parent;
		// change root
		if (z == root())
			header.node->parent = y;
		else if (z == z->parent->left)
			z->parent->left = y;
		else if (z == z->parent->right)
			z->parent->right = y;
		y->right = z;
		z->parent = y;
	}

	template<typename T, typename Compare, typename Alloc>
	void rbtree<T, Compare, Alloc>::insert_fixup(link_type now) {
		while (now != root() && now->parent->color == __rbtree_red) {
			if (now->parent == now->parent->parent->left) {
				link_type uncle = now->parent->parent->right;
				// if  parent and uncle are both red, case 1
				if (uncle && uncle->color == __rbtree_red) {
					now->parent->color = __rbtree_black;
					uncle->color = __rbtree_black;
					now->parent->parent->color = __rbtree_red;
					now = now->parent->parent;
				}
				// if no uncle or uncle is black
				else {
					// if now is a right child, case 2
					if (now == now->parent->right) {
						now = now->parent;
						left_rotate(now);
					}
					// if now is a left child, case 3
					now->parent->color = __rbtree_black;
					now->parent->parent->color = __rbtree_red;
					right_rotate(now->parent->parent);
				}
			}
			else {
				link_type uncle = now->parent->parent->left;
				// if  parent and uncle are both red
				if (uncle && uncle->color == __rbtree_red) {
					now->parent->color = __rbtree_black;
					uncle->color = __rbtree_black;
					now->parent->parent->color = __rbtree_red;
					now = now->parent->parent;
				}
				// if no uncle or uncle is black
				else {
					// if now is a left child
					if (now == now->parent->left) {
						now = now->parent;
						right_rotate(now);
					}
					now->parent->color = __rbtree_black;
					now->parent->parent->color = __rbtree_red;
					left_rotate(now->parent->parent);
				}
			}
		}
		// root is always black
		root()->color = __rbtree_black;
	}

	template<typename T, typename Compare, typename Alloc>
	void rbtree<T, Compare, Alloc>::delete_fixup(link_type father, link_type son) {
		while (son != root() && (!son || son->color == __rbtree_black)) {
			link_type brother = nullptr;
			if (son == father->left) {
				brother = father->right;
				// case 1: color of brother is red
				if (brother->color == __rbtree_red) {
					brother->color = __rbtree_black;
					father->color = __rbtree_red;
					left_rotate(father);
					brother = father->right;
				}
				// case 2: color of brother's sons are both black
				if ((!brother->left || brother->left->color == __rbtree_black) &&
					(!brother->right || brother->right->color == __rbtree_black)) {
					brother->color = __rbtree_red;
					son = father;
					father = father->parent;
				}
				else {
					// case 3: color of brother's right son is black
					if (!brother->right || brother->right->color == __rbtree_black) {
						brother->color = __rbtree_red;
						if (brother->left)
							brother->left->color = __rbtree_black;
						right_rotate(brother);
						brother = father->right;
					}
					// case 4: color of brother's left son is black
					brother->color = father->color;
					father->color = __rbtree_black;
					if (brother->right)
						brother->right->color = __rbtree_black;
					left_rotate(father);
					break;
				}
			}
			// similar as above, only change left to right, change right to left
			else {
				brother = father->left;
				if (brother->color == __rbtree_red) {
					brother->color = __rbtree_black;
					father->color = __rbtree_red;
					right_rotate(father);
					brother = father->left;
				}
				if ((!brother->right || brother->right->color == __rbtree_black) &&
					(!brother->left || brother->left->color == __rbtree_black)) {
					brother->color = __rbtree_red;
					son = father;
					father = father->parent;
				}
				else {
					if (!brother->left || brother->left->color == __rbtree_black) {
						brother->color = __rbtree_red;
						if (brother->right)
							brother->right->color = __rbtree_black;
						left_rotate(brother);
						brother = father->left;
					}
					brother->color = father->color;
					father->color = __rbtree_black;
					if (brother->left)
						brother->left->color = __rbtree_black;
					right_rotate(father);
					break;
				}
			}
		}
		if (son)
			son->color = __rbtree_black;
	}

	template<typename T, typename Compare, typename Alloc>
	void rbtree<T, Compare, Alloc>::dfs_clear(link_type root) {
		// dfs clear left and right
		if (root->left)
			dfs_clear(root->left);
		if (root->right)
			dfs_clear(root->right);
		Alloc().destroy(root);
		Alloc().deallocate(root);
	}

	// interface for debug
	template<typename T, typename Compare, typename Alloc>
	bool rbtree<T, Compare, Alloc>::isRBTree() {
		if (empty()) return true;
		// if root is not black, return false
		if (root()->color != __rbtree_black)
			return false;
		// if a node is red and its left or right is not black, return false
		for (auto ite = begin(); ite != end(); ++ite) {
			if (ite.node->color == __rbtree_red) {
				if (ite.node->left != nullptr && ite.node->left->color != __rbtree_black)
					return false;
				if (ite.node->right != nullptr && ite.node->right->color != __rbtree_black)
					return false;
			}
				
		}
		link_type x = root();
		unsigned int count_of_black = 0;
		// count the number of black nodes in a path
		while (x != nullptr) {
			if (x->color == __rbtree_black)
				++count_of_black;
			x = x->left;
		}
		// NIL node is black
		++count_of_black;
		// if the number of black nodes in any path are different, return false
		return dfs_isRBTree(root(), 0, count_of_black);
	}

	template<typename T, typename Compare, typename Alloc>
	bool rbtree<T, Compare, Alloc>::dfs_isRBTree(link_type root, unsigned int count_of_black_now, const unsigned int count_of_black) {
		// if root is black, count plus 1
		if (root->color == __rbtree_black)
			++count_of_black_now;
		// left or right is nullptr
		if (root->left == nullptr && root->right == nullptr) {
			++count_of_black_now;
			if (count_of_black_now != count_of_black)
				return false;
			else return true;
		}
		// left and right are not nullptr
		else {
			if (root->left == nullptr)
				return count_of_black_now + 1 == count_of_black && dfs_isRBTree(root->right, count_of_black_now, count_of_black);
			else if (root->right == nullptr)
				return count_of_black_now + 1 == count_of_black && dfs_isRBTree(root->left, count_of_black_now, count_of_black);
			else
				return dfs_isRBTree(root->left, count_of_black_now, count_of_black) && dfs_isRBTree(root->right, count_of_black_now, count_of_black);
		}
	}
}

#endif // !EZSTL_RBTREE_H