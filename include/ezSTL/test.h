/*********************************************************************************************/
// class test:
// using to test ezSTL
// including some useful static functions:
//
// 1. test_vector();
// 2. test_list();
// 3. test_stack();
// 4. test_queue();
// 5. test_priority_queue();
// 6. test_rbtree();
// 7. test_set();
// 8. test_map();
// 9. test_smart_pointers();
// 10. test_set_map_efficiency();
// 11. test_algorithm();
// 12. test_algorithm_efficiency();

#ifndef EZSTL_TEST_H
#define EZSTL_TEST_H

#include "utility.h"
#include "vector.h"
#include "list.h"
#include "stack.h"
#include "queue.h"
#include "rbtree.h"
#include "set.h"
#include "map.h"
#include "unordered_map.h"
#include "memory.h"
#include "algorithm.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <random>

namespace ezSTL {

	class test {
	public:
		// ultility
		static clock_t startTime;
		static clock_t endTime;
		static void start_info(std::string s);
		static void finish_info(std::string s);
		static void test_info(std::string s);
		static void continue_next_test();
		static void begin_timer();
		static void end_timer(std::string str, int number);
		// vector test
		template<typename T>
		static void print_vector(vector<T>& v);
		static void test_vector();
		// list test
		template<typename T>
		static void print_list(list<T>& l);
		static void test_list();
		// stack test
		template<typename T>
		static void print_stack(stack<T>& s);
		static void test_stack();
		// queue test
		template<typename T>
		static void print_queue(queue<T>& q);
		static void test_queue();
		// priority_queue test
		template<typename T, typename Compare>
		static void print_priority_queue(priority_queue<T, Compare>& q);
		static void test_priority_queue();
		// rbtree test
		template<typename T, typename Compare>
		static void print_rbtree(rbtree<T, Compare>& rbt);
		static void test_rbtree();
		// set test
		template<typename T, typename Compare>
		static void print_set(set<T, Compare>& s);
		static void test_set();
		// map
		template<typename K, typename V>
		static void print_map(map<K, V>& m);
		static void test_map();
		// unordered_map test
		template<typename Key, typename Value>
		static void print_unordered_map(unordered_map<Key, Value>& um);
		static void test_unordered_map();
		// smart pointer test(shared_ptr, unique_ptr, weak_ptr)
		template<typename T>
		static void print_shared_ptr(shared_ptr<T>& ptr);
		template<typename T>
		static void print_unique_ptr(unique_ptr<T>& ptr);
		template<typename T>
		static void print_weak_ptr(weak_ptr<T>& ptr);
		static void test_smart_pointers();
		// efficiency(set and map) test
		template<typename T, typename Compare>
		static void print_stdset(std::set<T, Compare>& s);
		static void test_set_map_efficiency();
		// algorithm test
		static void test_algorithm();
		static void test_algorithm_efficiency();
	};

	clock_t test::startTime = 0;
	clock_t test::endTime = 0;

	void test::start_info(std::string s) {
		std::string asterisk = "********************************";
		std::cout << asterisk + asterisk << std::endl;
		std::cout << "Begin testing " + s + " :" << std::endl << std::endl;
	}

	void test::finish_info(std::string s) {
		std::cout << std::endl << "Finish testing " << s << std::endl << std::endl;
	}

	void test::test_info(std::string s) {
		std::cout << std::endl << s << std::endl;
	}

	void test::continue_next_test() {
		std::cout << "Press \"Enter\" to continue..." << std::endl;
		getchar();
	}

	void test::begin_timer() {
		startTime = clock();
	}

	void test::end_timer(std::string str, int max_number) {
		endTime = clock();
		std::cout << str << " " + std::to_string(max_number) + " total time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << std::endl;
	}

	template<typename T>
	void test::print_vector(vector<T>& v) {
		std::cout << "size: " << v.size();
		std::cout << " ; capacity: " << v.capacity();
		std::cout << " ; empty: " << v.empty();
		if(!v.empty()) std::cout << " ; front: " << v.front();
		if (!v.empty()) std::cout << " ; back: " << v.back();
		std::cout << std::endl << "vector: ";
		for (auto ite = v.begin(); ite != v.end(); ++ite)
			std::cout << *ite << " ";
		std::cout << std::endl;
	}

	void test::test_vector() {
		srand((unsigned int)time(NULL));
		start_info("vector");
		vector<int> v;
		print_vector(v);
		test_info("vector   push_back");
		for (int i = 0; i < 10; ++i) {
			v.push_back(rand());
			print_vector(v);
		}
		test_info("vector   pop_back");
		for (int i = 0; i < 2; ++i) {
			v.pop_back();
			print_vector(v);
		}
		test_info("vector   insert");
		for (int i = 0; i < 10; ++i) {
			int randn = rand() % 8;
			std::cout << "insert number " << -randn << " in index:" << randn;
			auto res_ite = v.insert(v.begin() + randn, -randn);
			std::cout << "  ;  inserted number(obtained by iterator): " << *res_ite << std::endl;
			print_vector(v);
		}
		test_info("vector   erase");
		for (int i = 0; i < 5; ++i) {
			int randn = rand() % 10;
			std::cout << "erase in index:" << randn;
			auto res_ite = v.erase(v.begin() + randn);
			std::cout << "  ;  number obtained by return iterator: " << *res_ite << std::endl;
			print_vector(v);
		}
		test_info("vector   copy constructor");
		vector<int> w(v);
		print_vector(w);
		test_info("vector   move constructor");
		vector<int> u(ezSTL::move(w));
		print_vector(u);
		print_vector(w);
		test_info("vector   copy assignment operator");
		vector<int> x;
		x = v;
		print_vector(x);
		test_info("vector   move assignment operator");
		vector<int> y;
		y = ezSTL::move(x);
		print_vector(y);
		print_vector(x);
		test_info("vector   clear");
		v.clear();
		print_vector(v);
		finish_info("vector");
		continue_next_test();
	}

	template<typename T>
	void test::print_list(list<T>& l) {
		std::cout << "size: " << l.size();
		std::cout << " ; empty: " << l.empty();
		if (!l.empty()) std::cout << " ; front: " << l.front();
		if (!l.empty()) std::cout << " ; back: " << l.back();
		std::cout << std::endl << "list: ";
		for (auto ite = l.begin(); ite != l.end(); ++ite)
			std::cout << *ite << " ";
		std::cout << std::endl;
	}

	void test::test_list() {
		srand((unsigned int)time(NULL));
		start_info("list");
		list<int> l;
		print_list(l);
		test_info("list   push_back");
		for (int i = 0; i < 5; ++i) {
			l.push_back(rand());
			print_list(l);
		}
		test_info("list   push_front");
		for (int i = 0; i < 5; ++i) {
			l.push_front(rand());
			print_list(l);
		}
		test_info("list   pop_back");
		for (int i = 0; i < 2; ++i) {
			l.pop_back();
			print_list(l);
		}
		test_info("list   pop_front");
		for (int i = 0; i < 2; ++i) {
			l.pop_front();
			print_list(l);
		}
		test_info("list   insert");
		for (int i = 0; i < 4; ++i) {
			int randn = rand() % 5;
			std::cout << "insert number " << -randn << " in index:" << randn;
			auto res_ite = l.insert(l.begin() + randn, -randn);
			std::cout << "  ;  inserted number(obtained by return iterator): " << *res_ite << std::endl;
			print_list(l);
		}
		test_info("list   erase");
		for (int i = 0; i < 4; ++i) {
			int randn = rand() % 5;
			std::cout << "erase in index:" << randn;
			auto res_ite = l.erase(l.begin() + randn);
			std::cout << "  ;  number obtained by return iterator: " << *res_ite << std::endl;
			print_list(l);
		}
		test_info("list   copy constructor");
		list<int> w(l);
		print_list(w);
		test_info("list   move constructor");
		list<int> u(ezSTL::move(w));
		print_list(u);
		test_info("list   copy assignment operator");
		list<int> x;
		x = l;
		print_list(x);
		test_info("list   move assignment operator");
		list<int> y;
		y = ezSTL::move(x);
		print_list(y);
		test_info("list   clear");
		l.clear();
		print_list(l);
		finish_info("list");
		continue_next_test();
	}

	template<typename T>
	void test::print_stack(stack<T>& s) {
		std::cout << "size: " << s.size();
		std::cout << " ; empty: " << s.empty();
		if (!s.empty()) std::cout << " ; top: " << s.top();
		std::cout << std::endl;
	}

	void test::test_stack() {
		srand((unsigned int)time(NULL));
		start_info("stack");
		stack<int> s;
		print_stack(s);
		test_info("stack   push");
		for (int i = 0; i < 5; ++i) {
			s.push(rand());
			print_stack(s);
		}
		test_info("stack   pop");
		for (int i = 0; i < 5; ++i) {
			s.pop();
			print_stack(s);
		}
		finish_info("stack");
		continue_next_test();
	}

	template<typename T>
	void test::print_queue(queue<T>& q) {
		std::cout << "size: " << q.size();
		std::cout << " ; empty: " << q.empty();
		if (!q.empty()) std::cout << " ; front: " << q.front();
		if (!q.empty()) std::cout << " ; back: " << q.back();
		std::cout << std::endl;
	}

	void test::test_queue() {
		srand((unsigned int)time(NULL));
		start_info("queue");
		queue<int> q;
		print_queue(q);
		test_info("queue   push");
		for (int i = 0; i < 5; ++i) {
			q.push(rand());
			print_queue(q);
		}
		test_info("queue   pop");
		for (int i = 0; i < 5; ++i) {
			q.pop();
			print_queue(q);
		}
		finish_info("queue");
		continue_next_test();
	}

	template<typename T, typename Compare>
	void test::print_priority_queue(priority_queue<T, Compare>& pq) {
		std::cout << "size: " << pq.size();
		std::cout << " ; empty: " << pq.empty();
		if (!pq.empty()) std::cout << " ; top: " << pq.top();
		std::cout << std::endl;
	}
	void test::test_priority_queue() {
		const int max_iteration = 10;
		srand((unsigned int)time(NULL));
		start_info("priority_queue");
		priority_queue<int> pq;
		print_priority_queue(pq);
		test_info("priority_queue   push");
		for (int i = 0; i < max_iteration; ++i) {
			pq.push(rand());
			print_priority_queue(pq);
		}
		test_info("priority_queue   pop");
		print_priority_queue(pq);
		for (int i = 0; i < max_iteration; ++i) {
			pq.pop();
			print_priority_queue(pq);
		}
		finish_info("priority_queue");
		continue_next_test();
	}

	template<typename T, typename Compare>
	void test::print_rbtree(rbtree<T, Compare>& rbt) {
		std::cout << "size: " << rbt.size();
		std::cout << " ; empty: " << rbt.empty();
		std::cout << std::endl << "rbtree: ";
		for (auto ite = rbt.begin(); ite != rbt.end(); ++ite)
			std::cout << *ite << " ";
		/*for(auto ite = rbt.end(); ite != rbt.begin(); --ite)
			if(ite != rbt.end()) std::cout << *ite << " ";
		if(!empty()) std::cout << *rbt.begin() << " ";*/
		std::cout << std::endl;
		if(rbt.isRBTree())
			std::cout << "It is exactly an rbtree!" << std::endl;
		else
			std::cout << "It is not an rbtree." << std::endl;
	}

	void test::test_rbtree() {
		const int max_iteration = 10;
		srand((unsigned int)time(NULL));
		start_info("rbtree");
		rbtree<int> rbt;
		print_rbtree(rbt);
		test_info("rbtree   find and insert");
		for (int i = 0; i < max_iteration; ++i) {
			int number = rand() % 100;
			std::cout << "to insert " << number << " ; ";
			auto ite = rbt.find(number);
			if (ite == rbt.end()) std::cout << "not exists" << std::endl;
			else std::cout << "already exist : " << *ite << " (obtained by find iterator)" << std::endl;
			rbt.insert(number);
			print_rbtree(rbt);
		}
		test_info("rbtree   erase");
		for (int i = 0; i < max_iteration * 30; ++i) {
			int number = rand() % 100;
			auto ite = rbt.find(number);
			if (ite != rbt.end()) {
				std::cout << "to remove : " << number << std::endl;
				rbt.erase(ite);
				print_rbtree(rbt);
			}
		}
		test_info("rbtree   clear");
		rbt.clear();
		print_rbtree(rbt);
		finish_info("rbtree");
		continue_next_test();
	}

	template<typename T, typename Compare>
	void test::print_set(set<T, Compare>& s) {
		std::cout << "size: " << s.size();
		std::cout << " ; empty: " << s.empty();
		std::cout << std::endl << "set: ";
		for (auto ite = s.begin(); ite != s.end(); ++ite)
			std::cout << *ite << " ";
		std::cout << std::endl;
	}

	void test::test_set() {
		const int max_iteration = 10;
		srand((unsigned int)time(NULL));
		start_info("set");
		set<int> s;
		print_set(s);
		test_info("set   find and insert");
		for (int i = 0; i < max_iteration; ++i) {
			int number = rand() % 100;
			std::cout << "to insert " << number << " ; ";
			auto ite = s.find(number);
			if (ite == s.end()) std::cout << "not exists" << std::endl;
			else std::cout << "already exist : " << *ite << " (obtained by find iterator)" << std::endl;
			s.insert(number);
			print_set(s);
		}
		test_info("set   erase");
		for (int i = 0; i < max_iteration * 20; ++i) {
			int number = rand() % 100;
			auto ite = s.find(number);
			if (ite != s.end()) {
				std::cout << "to remove : " << number << std::endl;
				s.erase(ite);
				print_set(s);
			}
		}
		test_info("set   clear");
		s.clear();
		print_set(s);
		finish_info("set");
		continue_next_test();
	}

	template<typename K, typename V>
	void test::print_map(map<K, V>& m) {
		std::cout << "size: " << m.size();
		std::cout << " ; empty: " << m.empty();
		std::cout << std::endl << "map: ";
		for (auto ite = m.begin(); ite != m.end(); ++ite)
			std::cout << (*ite).first << ":" << (*ite).second << " ";
		std::cout << std::endl;
	}

	void test::test_map() {
		const int max_iteration = 10;
		srand((unsigned int)time(NULL));
		start_info("map");
		map<int, int> m;
		print_map(m);
		test_info("map   find and insert");
		for (int i = 0; i < max_iteration; ++i) {
			int number1 = rand() % 100;
			int number2 = rand() % 100;
			pair<int, int> p = make_pair(number1, number2);
			std::cout << "to insert " << p.first << ":" << p.second << " ; ";
			auto ite = m.find(p.first);
			if (ite == m.end()) std::cout << "not exists" << std::endl;
			std::cout << "already exist : " << p.first << ":" << p.second << " (obtained by find iterator)" << std::endl;
			m.insert(p);
			print_map(m);
		}
		test_info("map   erase");
		for (int i = 0; i < max_iteration; ++i) {
			int number = rand() % 100;
			auto ite = m.find(number);
			if (ite != m.end()) {
				std::cout << "to remove key: " << number << std::endl;
				m.erase(ite);
				print_map(m);
			}
		}

		test_info("map   clear");
		m.clear();
		print_map(m);
		finish_info("map");
		continue_next_test();
	}

	template<typename Key, typename Value>
	void test::print_unordered_map(unordered_map<Key, Value>& um) {
		std::cout << "size: " << um.size();
		std::cout << " ; bucket size: " << um.bucket_size();
		std::cout << " ; empty: " << um.empty();
		std::cout << std::endl << "unordered_map: ";
		for (auto ite = um.begin(); ite != um.end(); ++ite)
			std::cout << ite.get_key() << ":" << *ite << " ";
		std::cout << std::endl;
	}

	void test::test_unordered_map() {
		const int max_iteration = 10;
		srand((unsigned int)time(NULL));
		start_info("unordered_map");
		unordered_map<int, int> um;
		print_unordered_map(um);
		test_info("unordered_map   insert");
		for(int i = 0; i < max_iteration; ++i){
			pair<int, int> p(rand() % max_iteration, rand() % max_iteration);
			um.insert(p);
			std::cout << "insert key-value pair   " << p.first << ":" << p.second << std::endl;
			print_unordered_map(um);
		}
		test_info("unordered_map   erase");
		for (int i = 0; i < max_iteration; ++i) {
			int key = rand() % max_iteration;
			auto ite = um.find(key);
			if (ite != um.end()) um.erase(ite);
			std::cout << "deleting key   " << key << std::endl;
			print_unordered_map(um);
		}
		test_info("unordered_map   copy consructor");
		unordered_map<int, int> um1(um);
		print_unordered_map(um1);
		test_info("unordered_map   move consructor");
		unordered_map<int, int> um2(ezSTL::move(um1));
		print_unordered_map(um2);
		test_info("unordered_map   copy assignment operator");
		unordered_map<int, int> um3;
		um3 = um;
		print_unordered_map(um);
		print_unordered_map(um3);
		test_info("unordered_map   move assignment operator");
		unordered_map<int, int> um4;
		um4 = ezSTL::move(um2);
		print_unordered_map(um4);
		test_info("unordered_map   clear");
		um.clear();
		print_unordered_map(um);
		finish_info("unordered_map");
		continue_next_test();
	}

	template<typename T>
	void test::print_shared_ptr(shared_ptr<T>& ptr) {
		if (ptr)
			std::cout << "shared_ptr: value:" << *ptr << " ; use_count: " << ptr.use_count() << " ; unique: " << ptr.unique() << std::endl;
		else
			std::cout << "null object" << std::endl;
	}
	template<typename T>
	void test::print_unique_ptr(unique_ptr<T>& ptr) {
		if (ptr)
			std::cout << "unique_ptr: value:" << *ptr << std::endl;
		else
			std::cout << "null object" << std::endl;
	}
	template<typename T>
	void test::print_weak_ptr(weak_ptr<T>& ptr) {
		if (!ptr.expired())
			std::cout << "weak_ptr: shared_ptr use_count: " << ptr.use_count() << " ; expired: " << ptr.expired() << std::endl;
		else
			std::cout << "null object" << std::endl;
	}
	void test::test_smart_pointers() {
		start_info("shared_ptr:");
		shared_ptr<int> sh1(new int(8));
		print_shared_ptr(sh1);
		{
			test_info("shared_ptr   copy constructor");
			shared_ptr<int> sh2(sh1);
			print_shared_ptr(sh1);
			print_shared_ptr(sh2);
			shared_ptr<int> sh3(sh2);
			print_shared_ptr(sh1);
			print_shared_ptr(sh2);
			print_shared_ptr(sh3);
		}
		print_shared_ptr(sh1);
		shared_ptr<int> sh4;
		test_info("shared_ptr   copy assignment opeartor");
		sh4 = sh1;
		print_shared_ptr(sh1);
		print_shared_ptr(sh4);
		finish_info("shared_ptr");

		start_info("unique_ptr:");
		test_info("unique_ptr   constructor");
		unique_ptr<int> un1(new int(8));
		print_unique_ptr(un1);
		test_info("unique_ptr   move copy constructor");
		unique_ptr<int> un2(ezSTL::move(un1));
		print_unique_ptr(un1);
		print_unique_ptr(un2);
		finish_info("unique_ptr");

		start_info("weak_ptr:");
		test_info("weak_ptr   constructor");
		weak_ptr<int> we1(sh1);
		print_weak_ptr(we1);
		print_shared_ptr(sh1);
		{
			test_info("weak_ptr   lock");
			shared_ptr<int> sh = we1.lock();
			print_weak_ptr(we1);
			print_shared_ptr(sh);
		}
		print_weak_ptr(we1);
		test_info("weak_ptr   reset");
		we1.reset();
		print_weak_ptr(we1);
		finish_info("weak_ptr");
		continue_next_test();
	}
	template<typename T, typename Compare>
	void test::print_stdset(std::set<T, Compare>& s) {
		std::cout << "size: " << s.size();
		std::cout << " ; empty: " << s.empty();
		std::cout << std::endl << "set: ";
		for (auto ite = s.begin(); ite != s.end(); ++ite)
			std::cout << *ite << " ";
		std::cout << std::endl;
	}

	void test::test_set_map_efficiency() {
		const int max_number = 5000000;
		std::default_random_engine e((unsigned)time(0));
		std::uniform_int_distribution<unsigned> u(0, max_number);
		std::string cmpstl = "SGI STL";
		//std::string cmpstl = "PJ STL";
		start_info("efficiency of set and map");
		{
			test_info("set   insert");
			std::set<unsigned, less<unsigned>> std_set;
			ezSTL::set<unsigned, less<unsigned>> ezSTL_set;
			begin_timer();
			for (int i = 0; i < max_number; i++)
				ezSTL_set.insert(u(e));
			end_timer("ezSTL::set inserting", max_number);

			begin_timer();
			for (int i = 0; i < max_number; i++)
				std_set.insert(u(e));
			end_timer(cmpstl + "::set inserting", max_number);

			test_info("set   find and erase");
			auto ite1 = ezSTL_set.begin();
			unsigned int erase_count = 0;
			begin_timer();
			for (int i = 0; i < max_number; i++) {
				ite1 = ezSTL_set.find(u(e));
				if (ite1 != ezSTL_set.end()) {
					++erase_count;
					ezSTL_set.erase(ite1);
				}
			}
			end_timer("ezSTL::set finding and erasing", max_number);
			std::cout << "erasing number : " << std::to_string(erase_count) << std::endl;

			auto ite2 = std_set.begin();
			erase_count = 0;
			begin_timer();
			for (int i = 0; i < max_number; i++) {
				ite2 = std_set.find(u(e));
				if (ite2 != std_set.end()) {
					++erase_count;
					std_set.erase(ite2);
				}
			}
			end_timer(cmpstl + "::set finding and erasing", max_number);
			std::cout << "erasing number : " << std::to_string(erase_count) << std::endl;
		}
		{
			test_info("map   insert");
			std::map<unsigned, unsigned> std_map;
			ezSTL::map<unsigned, unsigned> ezSTL_map;
			begin_timer();
			for (int i = 0; i < max_number; i++)
				ezSTL_map.insert(make_pair(u(e), u(e)));
			end_timer("ezSTL::map inserting", max_number);

			begin_timer();
			for (int i = 0; i < max_number; i++)
				std_map.insert(std::make_pair(u(e), u(e)));
			end_timer(cmpstl + "::map inserting", max_number);

			test_info("map   find and erase");
			auto ite1 = ezSTL_map.begin();
			unsigned int erase_count = 0;
			begin_timer();
			for (int i = 0; i < max_number; i++) {
				ite1 = ezSTL_map.find(u(e));
				if (ite1 != ezSTL_map.end()) {
					++erase_count;
					ezSTL_map.erase(ite1);
				}
			}
			end_timer("ezSTL::map finding and erasing", max_number);
			std::cout << "erasing number : " << std::to_string(erase_count) << std::endl;

			auto ite2 = std_map.begin();
			erase_count = 0;
			begin_timer();
			for (int i = 0; i < max_number; i++) {
				ite2 = std_map.find(u(e));
				if (ite2 != std_map.end()) {
					++erase_count;
					std_map.erase(ite2);
				}
			}
			end_timer(cmpstl + "::map finding and erasing", max_number);
			std::cout << "erasing number : " << std::to_string(erase_count) << std::endl;
		}
		finish_info("efficiency of set and map");
		continue_next_test();
	}

	void test::test_algorithm() {
		const int max_iteration = 30;
		std::default_random_engine e((unsigned)time(0));
		std::uniform_int_distribution<unsigned> u(0, 20);
		start_info("algorithm");
		test_info("sort:");
		{
			vector<unsigned> v;
			for (int i = 0; i < max_iteration; ++i)
				v.push_back(u(e));
			print_vector(v);
			sort(v.begin(), v.end());
			print_vector(v);
		}
		test_info("stable_sort:");
		{
			vector<pair<unsigned, unsigned>> v;
			for (int i = 0; i < max_iteration; ++i)
				v.push_back(make_pair(u(e), u(e)));
			for (auto ite = v.begin(); ite != v.end(); ++ite)
				std::cout << (*ite).first << ":" << (*ite).second << " ";
			std::cout << std::endl;
			stable_sort(v.begin(), v.end(), less<pair<unsigned, unsigned>>());
			for (auto ite = v.begin(); ite != v.end(); ++ite)
				std::cout << (*ite).first << ":" << (*ite).second << " ";
			std::cout << std::endl;
		}
		finish_info("algorithm");
		continue_next_test();
	}

	void test::test_algorithm_efficiency() {
		const int max_number = 20000000;
		std::string cmpstl = "SGI STL";
		//std::string cmpstl = "PJ STL";
		std::uniform_real_distribution<double> u(0.0, 1.0);
		std::normal_distribution<double> n(0.0, 1.0);
		std::default_random_engine e((unsigned)time(0));
		start_info("efficiency of algorithm sort and stable_sort");
		test_info("uniform distribution of unsigned int:");
		{
			ezSTL::vector<unsigned> ezSTL_v;
			for (int i = 0; i < max_number; i++)
				ezSTL_v.push_back(e());
			begin_timer();
			sort(ezSTL_v.begin(), ezSTL_v.end(), less<unsigned>());
			end_timer("ezSTL::sort", max_number);
		}
		{
			std::vector<unsigned> std_v;
			for (int i = 0; i < max_number; i++)
				std_v.push_back(e());
			begin_timer();
			std::sort(std_v.begin(), std_v.end(), std::less<unsigned>());
			end_timer(cmpstl + "::sort", max_number);
		}
		test_info("uniform distribution of double(0, 1):");
		{
			ezSTL::vector<double> ezSTL_v;
			for (int i = 0; i < max_number; i++)
				ezSTL_v.push_back(u(e));
			begin_timer();
			sort(ezSTL_v.begin(), ezSTL_v.end(), less<double>());
			end_timer("ezSTL::sort", max_number);
		}
		{
			std::vector<double> std_v;
			for (int i = 0; i < max_number; i++)
				std_v.push_back(u(e));
			begin_timer();
			std::sort(std_v.begin(), std_v.end(), std::less<double>());
			end_timer(cmpstl + "::sort", max_number);
		}
		test_info("normal distribution double(mean value 0, standard deviation 1):");
		{
			ezSTL::vector<double> ezSTL_v;
			for (int i = 0; i < max_number; i++)
				ezSTL_v.push_back(n(e));
			begin_timer();
			sort(ezSTL_v.begin(), ezSTL_v.end(), less<double>());
			end_timer("ezSTL::sort", max_number);
		}
		{
			std::vector<double> std_v;
			for (int i = 0; i < max_number; i++)
				std_v.push_back(n(e));
			begin_timer();
			std::sort(std_v.begin(), std_v.end(), std::less<double>());
			end_timer(cmpstl + "::sort", max_number);
		}
		start_info("efficiency of algorithm stable_sort");
		test_info("uniform distribution of unsigned int:");
		{
			ezSTL::vector<unsigned> ezSTL_v;
			for (int i = 0; i < max_number; i++)
				ezSTL_v.push_back(e());
			begin_timer();
			stable_sort(ezSTL_v.begin(), ezSTL_v.end(), less<unsigned>());
			end_timer("ezSTL::stable_sort", max_number);
		}
		{
			std::vector<unsigned> std_v;
			for (int i = 0; i < max_number; i++)
				std_v.push_back(e());
			begin_timer();
			std::stable_sort(std_v.begin(), std_v.end(), std::less<unsigned>());
			end_timer(cmpstl + "::stable_sort", max_number);
		}
		test_info("uniform distribution of double(0, 1):");
		{
			ezSTL::vector<double> ezSTL_v;
			for (int i = 0; i < max_number; i++)
				ezSTL_v.push_back(u(e));
			begin_timer();
			stable_sort(ezSTL_v.begin(), ezSTL_v.end(), less<double>());
			end_timer("ezSTL::stable_sort", max_number);
		}
		{
			std::vector<double> std_v;
			for (int i = 0; i < max_number; i++)
				std_v.push_back(u(e));
			begin_timer();
			std::stable_sort(std_v.begin(), std_v.end(), std::less<double>());
			end_timer(cmpstl + "::stable_sort", max_number);
		}
		test_info("normal distribution double(mean value 0, standard deviation 1):");
		{
			ezSTL::vector<double> ezSTL_v;
			for (int i = 0; i < max_number; i++)
				ezSTL_v.push_back(n(e));
			begin_timer();
			stable_sort(ezSTL_v.begin(), ezSTL_v.end(), less<double>());
			end_timer("ezSTL::stable_sort", max_number);
		}
		{
			std::vector<double> std_v;
			for (int i = 0; i < max_number; i++)
				std_v.push_back(n(e));
			begin_timer();
			std::stable_sort(std_v.begin(), std_v.end(), std::less<double>());
			end_timer(cmpstl + "::stable_sort", max_number);
		}
		finish_info("efficiency of algorithm sort and stable_sort");
		continue_next_test();
	}
}

#endif // !EZSTL_TEST_H

