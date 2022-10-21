/*********************************************************************************************/
// some common useful tools:
// 1. remove_reference class
// 2. move function
// 3. swap function
// 4. pair class
// 5. make_pair function
//
// the above classes of funcitons are frequently used in containers, adapters, algorithm, etc

#ifndef EZSTL_UTILITY_H
#define EZSTL_UTILITY_H

namespace ezSTL {

	// remove_reference traits
	// if the real type is T or T& or T&&, then type = T
	template<typename T>
	class remove_reference {
	public:
		using type = T;
	};
	template<typename T>
	class remove_reference<T&> {
	public:
		using type = T;
	};

	template<typename T>
	class remove_reference<T&&> {
	public:
		using type = T;
	};

	// move
	// convert a reference from lvalue of rvalue to rvalue
	template<typename T>
	inline typename remove_reference<T>::type&& move(T&& arg) noexcept {
		return (static_cast<typename remove_reference<T>::type&&>(arg));
	}

	// swap
	// swap two arguments
	template<typename T>
	inline void swap(T& lhs, T& rhs) noexcept {
		T temp = ezSTL::move(lhs);
		lhs = ezSTL::move(rhs);
		rhs = ezSTL::move(temp);
	}

	// pair class
	template<typename FIRST, typename SECOND>
	class pair {
	public:
		FIRST first;
		SECOND second;
	public:
		pair() = default;
		pair(const pair<FIRST, SECOND>& p) : first(p.first), second(p.second) {}
		pair(const FIRST& p1, const SECOND& p2) : first(p1), second(p2) {}
		pair<FIRST, SECOND>& operator=(const pair<FIRST, SECOND>& p) {
			if (this != &p)
			{
				first = p.first;
				second = p.second;
			}
			return *this;
		}
	public:
		inline friend bool operator== (const pair<FIRST, SECOND>& lhs, const pair<FIRST, SECOND>& rhs) {
			return lhs.first == rhs.first && lhs.second == rhs.second;
		}
		inline friend bool operator!= (const pair<FIRST, SECOND>& lhs, const pair<FIRST, SECOND>& rhs) {
			return !(lhs == rhs);
		}
		inline friend bool operator<  (const pair<FIRST, SECOND>& lhs, const pair<FIRST, SECOND>& rhs) {
			// return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
			return lhs.first < rhs.first;
		}
		inline friend bool operator<= (const pair<FIRST, SECOND>& lhs, const pair<FIRST, SECOND>& rhs) {
			return !(rhs < lhs);
		}
		inline friend bool operator>  (const pair<FIRST, SECOND>& lhs, const pair<FIRST, SECOND>& rhs) {
			return rhs < lhs;
		}
		inline friend bool operator>= (const pair<FIRST, SECOND>& lhs, const pair<FIRST, SECOND>& rhs) {
			return !(lhs < rhs);
		}
	};

	// make_pair
	// using constructor of class pair
	template<typename FIRST, typename SECOND>
	inline pair<FIRST, SECOND> make_pair(const FIRST& a, const SECOND& b) {
		return pair<FIRST, SECOND>(a, b);
	}
}

#endif // !EZSTL_UTILITY_H
