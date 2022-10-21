/*********************************************************************************************/
// some common useful algorithms:
//
// using insertion sort/quick sort/heap sort, unstable sort:
// 1. void sort(Iterator First, Iterator Last, Compare cmp);
// 2. void sort(Iterator First, Iterator Last);
//
// using insertion sort/merge sort, stable sort:
// 3. void stable_sort(Iterator First, Iterator Last, Compare cmp);
// 4. void stable_sort(Iterator First, Iterator Last);

#ifndef EZSTL_ALGORITHM_H
#define EZSTL_ALGORITHM_H

#include "functional.h"
#include "iterator.h"
#include "utility.h"

namespace ezSTL {

	enum { insertion_threshold = 20 };

	// copy [Fisrt, Last) to [First + 1, Last + 1), used in insertion sort
	template<typename Iterator>
	inline void __copy_to_next(Iterator First, Iterator Last) {
		for (auto ite = Last; ite > First; --ite)
			*ite = *(ite - 1);
	}

	// insertion sort
	template<typename Iterator, typename Compare>
	inline void __insertion_sort(Iterator First, Iterator Last, Compare cmp) {
		auto next = First;
		auto value = *First;
		for (auto ite = First + 1; ite != Last; ++ite) {
			next = ite;
			value = *ite;
			if (cmp(value, *First)) {
				__copy_to_next(First, ite);
				*First = value;
			}
			else {
				// unguarded linear insertion
				while (cmp(value, *(next - 1))) {
					*next = *(next - 1);
					--next;
				}
				*next = value;
			}
		}
	}

	// unguarded insertion sort
	template<typename Iterator, typename Compare>
	inline void __unguarded_insertion_sort(Iterator First, Iterator Last, Compare cmp) {
		auto next = First;
		auto value = *First;
		for (auto ite = First + 1; ite != Last; ++ite) {
			next = ite;
			value = *ite;
			while (cmp(value, *(next - 1))) {
				*next = *(next - 1);
				--next;
			}
			*next = value;
		}
	}

	// quick sort partition, return medium iterator
	template<typename Iterator, typename Compare>
	inline Iterator __quick_sort_partition(Iterator First, Iterator Last, Compare cmp) {
		Iterator Medium = First + (Last - First) / 2;
		// get the medial value of First, Medium, Last - 1
		if ((cmp(*Medium, *First) && cmp(*(Last - 1), *Medium)) || (cmp(*Medium, *(Last - 1)) && cmp(*First, *Medium)))
			swap(*First, *Medium);
		else if ((cmp(*First, *(Last - 1)) && cmp(*(Last - 1), *Medium)) || (cmp(*Medium, *(Last - 1)) && cmp(*(Last - 1), *First)))
			swap(*First, *(Last - 1));
		auto pivot = *First;
		// partition according the value of pivot
		while (First < Last - 1) {
			while (First < Last - 1 && !cmp(*(Last - 1), pivot))
				--Last;
			*First = *(Last - 1);
			while (First < Last - 1 && !cmp(pivot, *First))
				++First;
			*(Last - 1) = *First;
		}
		*First = pivot;
		return First;
	}

	// heap sort
	template<typename Iterator, typename Compare>
	inline void __heap_sort(Iterator First, Iterator Last, Compare cmp) {
		Iterator now, left, right;
		// make heap
		for (Iterator ite = (Last - First - 1) / 2 + First; ite >= First; --ite) {
			now = ite;
			left = First + (now - First) * 2 + 1;
			right = First + (now - First) * 2 + 2;
			// if has both left and right children
			while (right < Last) {
				// exchange father and left
				if (!cmp(*left, *right) && cmp(*now, *left)) {
					swap(*left, *now);
					now = left;
					left = First + (now - First) * 2 + 1;
					right = First + (now - First) * 2 + 2;
				}
				// exchange father and right
				else if (!cmp(*right, *left) && cmp(*now, *right)) {
					swap(*right, *now);
					now = right;
					left = First + (now - First) * 2 + 1;
					right = First + (now - First) * 2 + 2;
				}
				else break;
			}
			// if only has left child
			if (right == Last && cmp(*now, *left))
				swap(*left, *now);
		}
		// heap sort
		for (Iterator ite = Last - 1; ite >= First; --ite) {
			swap(*First, *ite);
			now = First;
			left = First + 1;
			right = First + 2;
			// if has both left and right children
			while (right < ite) {
				// exchange father and left
				if (!cmp(*left, *right) && cmp(*now, *left)) {
					swap(*left, *now);
					now = left;
					left = First + (now - First) * 2 + 1;
					right = First + (now - First) * 2 + 2;
				}
				// exchange father and right
				else if (!cmp(*right, *left) && cmp(*now, *right)) {
					swap(*right, *now);
					now = right;
					left = First + (now - First) * 2 + 1;
					right = First + (now - First) * 2 + 2;
				}
				else break;
			}
			// if only has left child
			if (right == ite && cmp(*now, *left))
				swap(*left, *now);
		}
	}

	// get maximum recursive depth
	inline unsigned int recursive_depth(unsigned int n) {
		unsigned int depth = 0;
		for (; n > 1; n >>= 1, ++depth);
		return depth;
	}

	// intro sort
	template<typename Iterator, typename Compare>
	void __intro_sort_loop(Iterator First, Iterator Last, unsigned int depth, Compare cmp) {
		unsigned int count;
		while ((count = Last - First) > insertion_threshold && depth > 0) {
			--depth;
			Iterator Medium = __quick_sort_partition(First, Last, cmp);
			if (Medium - First < Last - Medium) {
				__intro_sort_loop(First, Medium, depth, cmp);
				First = Medium;
			}
			else {
				__intro_sort_loop(Medium, Last, depth, cmp);
				Last = Medium;
			}
		}
		// if exceed recursive depth limitation
		if (depth <= 0)
			__heap_sort(First, Last, cmp);
		// if < insertion_threshold, change to insertion sort
		else if (count >= 2)
			__insertion_sort(First, Last, cmp);
	}

	// outer interface, using insertion sort/quick sort/heap sort
	template<typename Iterator, typename Compare>
	inline void sort(Iterator First, Iterator Last, Compare cmp) {
		if (First < Last)
			__intro_sort_loop(First, Last, recursive_depth(Last - First), cmp);
	}

	// outer interface, using less as comparison
	template<typename Iterator>
	inline void sort(Iterator First, Iterator Last) {
		sort(First, Last, less<typename remove_reference<decltype(*First)>::type>());
	}

	// merge, used in merge srot
	template<typename Iterator, typename Compare>
	inline void __merge(Iterator First, Iterator Medium, Iterator Last, Compare cmp) {
		vector<typename remove_reference<decltype(*First)>::type> v(Medium - First);
		auto ite1 = First;
		auto ite2 = v.begin();
		// copy from {First, Medium) to vector v
		while (ite1 != Medium)
			*ite2++ = *ite1++;
		ite2 = v.begin();
		// merge
		for (auto ite = First; ite != Last; ++ite) {
			if (ite2 != v.end() && (ite1 == Last || !cmp(*ite1, *ite2)))
				*ite = *ite2++;
			else
				*ite = *ite1++;
		}
	}

	// merge sort
	template<typename Iterator, typename Compare>
	inline void __merge_sort(Iterator First, Iterator Last, Compare cmp) {
		unsigned int count = Last - First;
		if (count < 2)
			return;
		// if < insertion_threshold, use insertion sort
		else if (count <= insertion_threshold)
			__insertion_sort(First, Last, cmp);
		else {
			Iterator Medium = First + (Last - First) / 2;
			__merge_sort(First, Medium, cmp);
			__merge_sort(Medium, Last, cmp);
			__merge(First, Medium, Last, cmp);
		}
	}

	// outer interface, using insertion sort/merge sort
	template<typename Iterator, typename Compare>
	inline void stable_sort(Iterator First, Iterator Last, Compare cmp) {
		if (First < Last)
			__merge_sort(First, Last, cmp);
	}

	// outer interface, using less as comparison
	template<typename Iterator>
	inline void stable_sort(Iterator First, Iterator Last) {
		stable_sort(First, Last, less<typename remove_reference<decltype(*First)>::type>());
	}

	//  ********** SGI Version **********
	//// intro sort
	//template<typename Iterator, typename Compare>
	//void __intro_sort_loop(Iterator First, Iterator Last, unsigned int depth, Compare cmp) {
	//	unsigned int count;
	//	while (count = Last - First > insertion_threshold) {
	//		// if exceed recursive depth limitation
	//		if (depth == 0) {
	//			__heap_sort(First, Last, cmp);
	//			return;
	//		}
	//		--depth;
	//		Iterator Medium = __quick_sort_partition(First, Last, cmp);
	//		if (Medium - First < Last - Medium) {
	//			__intro_sort_loop(First, Medium, depth, cmp);
	//			First = Medium;
	//		}
	//		else {
	//			__intro_sort_loop(Medium, Last, depth, cmp);
	//			Last = Medium;
	//		}
	//	}
	//}
	//// outer interface, using insertion sort/quick sort/heap sort
	//template<typename Iterator, typename Compare>
	//inline void sort(Iterator First, Iterator Last, Compare cmp) {
	//	if (First < Last) {
	//		__intro_sort_loop(First, Last, recursive_depth(Last - First) * 2, cmp);
	//		if (Last - First > insertion_threshold) {
	//			__insertion_sort(First, First + insertion_threshold, cmp);
	//			__unguarded_insertion_sort(First + insertion_threshold, Last, cmp);
	//		}
	//		else
	//			__insertion_sort(First, Last, cmp);
	//	}
	//}

	//  ********** Version before optimization **********
	//// copy [Fisrt, Last) to [First + 1, Last + 1), used in insertion sort
	//template<typename Iterator>
	//inline void __copy_to_next(Iterator First, Iterator Last) {
	//	for (auto ite = Last - 1; ite >= First; --ite)
	//		*(ite + 1) = *ite;
	//}
	//// insertion sort
	//template<typename Iterator, typename Compare>
	//inline void __insertion_sort(Iterator First, Iterator Last, Compare cmp) {
	//	auto ite = First;
	//	for (auto Now = First + 1; Now < Last; ++Now) {
	//		ite = Now - 1;
	//		for (; ite >= First; --ite)
	//			if (!cmp(*Now, *ite)) break;
	//		auto temp = *Now;
	//		__copy_to_next(ite + 1, Now);
	//		*(ite + 1) = temp;
	//	}
	//}
	//// percolate down from iterator now in [First, Last), used in heap sort
	//template<typename Iterator, typename Compare>
	//inline void __percolateDown(Iterator now, Iterator First, Iterator Last, Compare cmp) {
	//	// if only has left child
	//	if (First + (now - First) * 2 + 2 == Last && cmp(*now, *(First + (now - First) * 2 + 1))) {
	//		swap(*(First + (now - First) * 2 + 1), *now);
	//		return;
	//	}
	//	// if has both left and right children
	//	while (First + (now - First) * 2 + 2 < Last) {
	//		if (!cmp(*(First + (now - First) * 2 + 1), *(First + (now - First) * 2 + 2)) && cmp(*now, *(First + (now - First) * 2 + 1))) {
	//			swap(*(First + (now - First) * 2 + 1), *now);
	//			now = First + (now - First) * 2 + 1;
	//		}
	//		else if (!cmp(*(First + (now - First) * 2 + 2), *(First + (now - First) * 2 + 1)) && cmp(*now, *(First + (now - First) * 2 + 2))) {
	//			swap(*(First + (now - First) * 2 + 2), *now);
	//			now = First + (now - First) * 2 + 2;
	//		}
	//		else break;
	//	}
	//}
	//// heap sort
	//template<typename Iterator, typename Compare>
	//inline void __heap_sort(Iterator First, Iterator Last, Compare cmp) {
	//	// make heap
	//	for (Iterator ite = (Last - First - 1) / 2 + First; ite >= First; --ite)
	//		__percolateDown(ite, First, Last, cmp);
	//	// heap sort
	//	for (Iterator ite = Last - 1; ite >= First; --ite) {
	//		swap(*First, *ite);
	//		__percolateDown(First, First, ite, cmp);
	//	}
	//}
}

#endif // !EZSTL_ALGORITHM_H
