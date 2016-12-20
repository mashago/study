
#include <wchar.h>
#include <stdexcept>
#include <iostream>
#include <iterator>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <queue>
#include <stack>
#include <array>
#include <set>
#include <map>
#include <cmath>
#include <algorithm> // for_each()

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::deque;
using std::list;
using std::forward_list;
using std::queue;
using std::priority_queue;
using std::stack;
using std::array;
using std::for_each;
using std::set;
using std::map;
using std::pair;

// Function f cannot be a template
// template <typename T> void show_value(const T & t)
void show_value(const int & t)
{
	cout << " [" << t << "]";
}

// Function f cannot be a template
// template <typename T> void show_str(const T & t)
void show_str(const string & t)
{
	cout << " [" << t << "]";
}

void show_pair(const pair<int, string> & t)
{
	cout << " [" << t.first << ":" << t.second << "]";
}

struct sort_int_desc
{
	bool operator() (int d1, int d2) { std::cout<<"into sort_int_desc()"<<std::endl; return d1 > d2; }
};

int test_vector()
{

	// push_back(member);
	// erase(it);
	// erase(it1, it2);
	// insert(it, member);
	// insert(it, num, member);
	// insert(it, it_start, it_end);
	// pop_back();
	// clear();
	
	cout << "-----------------test_vector----------------" << endl;
	vector<int> v1;
	cout << "before v1.size()=" << v1.size() << " v1.capacity()=" << v1.capacity() << endl;

	// push_back
	v1.push_back(10);
	v1.push_back(11);
	v1.push_back(12);
	cout << "after push_back() v1.size()=" << v1.size() << " v1.capacity()=" << v1.capacity() << endl;

	cout << "-----------------------------" << endl;

	// erase
	v1.erase(v1.begin(), v1.begin() + 1);
	cout << "after erase() v1.size()=" << v1.size() << " v1.capacity()=" << v1.capacity() << endl;

	cout << "v1:";
	vector<int>::iterator it;
	for (it=v1.begin(); it<v1.end(); it++) {
		cout << " ["<< *it << "]";
	}
	cout << endl;

	cout << "-----------------------------" << endl;

	// insert
	v1.insert(v1.begin(), 19);
	cout << "after insert() v1:";
#ifdef CPP11
	for (auto m : v1) { // new for loop in c++11
		cout << " ["<< m << "]";
#else
	for (auto iter = v1.begin(); iter != v1.end(); iter++) {
		cout << " ["<< *iter << "]";
#endif
	}
	cout << endl;

	cout << "-----------------------------" << endl;

	vector<int> v2;
	v2 = {20, 21, 22};

	// insert(to_where, start_pos, end_pos);
	v1.insert(v1.end(), v2.begin(), v2.end());

	cout << "after insert() v1:";
	for_each(v1.begin(), v1.end(), show_value); // for_each cannot update container member
	cout << endl;

	cout << "-----------------------------" << endl;

	vector<int> v3;
	v3 = {30, 31, 32};

	// copy(start_pos, end_pos, to_where);
	// copy cannot resize container, so must make sure have enough space to do copy 
	copy(v3.begin(), v3.begin()+2, v1.begin());
	cout << "after copy() v1:";
	for_each(v1.begin(), v1.end(), show_value); 
	cout << endl;

	cout << "-----------------------------" << endl;


	cout << "ostream_iterator v1:";
	// ostream_iterator<in_type, out_type> name(ostream, "split")
	std::ostream_iterator<int, char> out_itor(cout, " ");
	copy(v1.begin(), v1.end(), out_itor);
	cout << endl;

	cout << "-----------------------------" << endl;

	cout << "reverse_iterator v1:";
	// rbegin(), rend() is reverse_iterator
	for_each(v1.rbegin(), v1.rend(), show_value); 
	cout << endl;

	cout << "-----------------------------" << endl;

	vector<int> v4;
	v4 = {40, 41, 42};

	// back_insert_iterator<type> name(who);
	std::back_insert_iterator<vector<int> > back_itor(v1);
	copy(v4.begin(), v4.end(), back_itor);
	cout << "back_insert_iterator v1:";
	for_each(v1.begin(), v1.end(), show_value); 
	cout << endl;

	cout << "-----------------------------" << endl;

	vector<int> v5;
	v5 = {50, 51, 52};

	// insert_iterator<type> name(who, where);
	std::insert_iterator<vector<int> > insert_itor(v1, v1.begin());
	copy(v5.begin(), v5.end(), insert_itor);
	cout << "insert_iterator v1:";
	for_each(v1.begin(), v1.end(), show_value); 
	cout << endl;

	cout << "-----------------------------" << endl;

	// v1.pop_front(); // vector push_front() is O(n)
	// v1.push_front(66); // vector push_front() is O(n)
	v1.pop_back();
	cout << "pop_back v1:";
	for_each(v1.begin(), v1.end(), show_value); 
	cout << endl;

	cout << "-----------------------------" << endl;

	cout << "v1[0]=" << v1[0] << endl;
	cout << "v1.at(0)=" << v1.at(0) << endl;
	try {
		// at() will check range, if out of range will throw out_of_range exception
		cout << "v1.at(100)=" << v1.at(100) << endl;
	} catch (std::out_of_range &e) {
		cout << "out_of_range exception" << endl;
	}

	cout << "*(v1.begin()+1)=" << *(v1.begin()+1) << endl; // support random access

	cout << "-----------------------------" << endl;

	vector<int> v6(10);
	cout << "v6.size()=" << v6.size() << endl;
	for (auto iter = v6.begin(); iter != v6.end(); iter++)
	{
		cout << " " << *iter;
	}
	cout << endl;

	cout << "-----------------------------" << endl;

	vector<int> v7;
	v7.push_back(10);
	cout << "v7.size()=" << v7.size() << endl;
	for (auto iter = v7.begin(); iter != v7.end(); iter++)
	{
		cout << " " << *iter;
	}
	cout << endl;
	std::sort(v7.begin(), v7.end(), sort_int_desc());

	cout << "-----------------------------" << endl;
	vector<int> v8;
	cout << "before reserve v8.capacity()=" << v8.capacity() << endl;
	v8.reserve(50);
	cout << "after reserve v8.capacity()=" << v8.capacity() << endl;
	cout << "v8.max_size()=" << v8.max_size() << endl;

	cout << "-----------------------------" << endl;
	// The difference of two sets is formed by the elements that are present in the first set, but not in the second one. The elements copied by the function come always from the first range, in the same order.
	std::vector<int> v9 = {3, 4, 5, 1, 2};	
	std::vector<int> v10 = {3, 4, 5, 6, 7};	
	std::vector<int> diff_vec(v9.size()+v10.size()); // difference vector size should be 

	// 1. sort 2 vector
	std::sort(v9.begin(), v9.end());
	std::sort(v10.begin(), v10.end());

	// 2.set_difference
	std::vector<int>::iterator iter;
	iter = std::set_difference(v9.begin(), v9.end(), v10.begin(), v10.end(), diff_vec.begin());

	// 3.resize by iter
	diff_vec.resize(iter - diff_vec.begin());
	cout << "diff_vec.size()=" << diff_vec.size() << endl;
	for (auto iter = diff_vec.begin(); iter != diff_vec.end(); iter++)
	{
		cout << " " << *iter;
	}
	cout << endl;


	cout << "-----------------------------" << endl;
	std::vector<char> v11(10);
	cout << "v11.size()=" << v11.size() << endl;
	std::vector<char> v12{10};
	cout << "v12.size()=" << v12.size() << endl;

	cout << "-----------------------------" << endl;

	struct Car
	{
		int num;
		Car(int n) : num(n) {};
	};

	std::vector<void *> v13;
	for (int i = 0; i < 5; i++)
	{
		Car *tmp = new Car(i);
		v13.push_back((void *)tmp);
	}

	std::cout << "v13, vector with void* :" << std::endl;
	for (auto iter = v13.begin(); iter != v13.end(); iter++)
	{
		std::cout << "(*iter)->num=" << ((Car *)(*iter))->num << std::endl;
	}

	return 0;
}

int test_deque()
{
	cout << endl << "-----------------test_deque----------------" << endl;

	// all vecotr + push_front() + pop_front()

	deque<int> d1;
	// deque is double-ended queue, support front and back operation
	d1.push_front(11);
	d1.push_front(10);
	d1.push_back(12);
	d1.push_back(13);

	cout << "d1:";
	for_each(d1.begin(), d1.end(), show_value); 
	cout << endl;

	cout << "*(d1.begin()+1)=" << *(d1.begin()+1) << endl; // support random access

	// deque is double-ended, do random access and insert, erase will slower then vector

	return 0;
}

int list_desc_insert(std::list<int> &my_list, int value)
{
	bool success = false;
	for (auto it = my_list.begin(); it != my_list.end(); it++)
	{
		if (*it <= value)
		{
			my_list.insert(it, value);
			success = true;
			break;
		}
	}
	if (!success)
	{
		my_list.insert(my_list.end(), value);
	}
	return 0;
}

int test_list()
{
	cout << endl << "-----------------test_list----------------" << endl;

	// == deque - random access
	// lista.merge(listb);  // two sorted list merge, add member in lista. listb will be empty
	// sort();
	// splice(it, listb); // add listb member move to it, listb will be empty
	// unique(); // nearby same member will merge


	int stuff[5] = {15, 12, 11, 14, 13};
	// list is doubly linked list
	list<int> l1;
	l1.insert(l1.begin(), stuff+1, stuff+5);

	cout << "l1:";
	for_each(l1.begin(), l1.end(), show_value); 
	cout << endl;

	auto iter = l1.begin();
	iter++; // this is ok
	cout << "*iter=" << *iter << endl;
	// iter += 2; // error: no viable overloaded '+='

	cout << "-----------------------------" << endl;

	l1.remove(13); // remove value, O(n)

	cout << "remove(13) l1:";
	for_each(l1.begin(), l1.end(), show_value); 
	cout << endl;

	cout << "-----------------------------" << endl;

	list<int> l2 = {19, 13, 19, 12};
	cout << "l2:";
	for_each(l2.begin(), l2.end(), show_value); 
	cout << endl;

	l1.splice(l1.begin(), l2); // move l2 to l1.pos, l2 will be empty
	cout << "splice(l2) l1:";
	for_each(l1.begin(), l1.end(), show_value); 
	cout << endl;

	cout << "after splice() l2.size()=" << l2.size() << endl;

	l1.unique(); // 
	cout << "unique() l1:"; // near same value will be merge
	for_each(l1.begin(), l1.end(), show_value); 
	cout << endl;

	l1.sort(); // use <, you can overload operator<
	cout << "sort() l1:"; // order by <
	for_each(l1.begin(), l1.end(), show_value); 
	cout << endl;

	l1.unique(); // 
	cout << "unique() l1:"; // near same value will be merge
	for_each(l1.begin(), l1.end(), show_value); 
	cout << endl;

	cout << "-----------------------------" << endl;

	list<int> l3 = {18, 14, 19, 13};
	cout << "l3:";
	for_each(l3.begin(), l3.end(), show_value); 
	cout << endl;

	l3.sort();
	cout << "sort() l3:";
	for_each(l3.begin(), l3.end(), show_value); 
	cout << endl;

	l1.merge(l3);
	cout << "merge(l3) l1:";  // must sort() before merge(), l3 will be empty
	for_each(l1.begin(), l1.end(), show_value); 
	cout << endl;

	cout << "reserve l1:";  // must sort() before merge(), l3 will be empty
	for_each(l1.rbegin(), l1.rend(), show_value); 
	cout << endl;

	cout << "after merge() l3.size()=" << l3.size() << endl;

	cout << "l1:";
	for_each(l1.begin(), l1.end(), show_value); 
	cout << endl;
	cout << "l1.size()=" << l1.size() << endl;

	// std::remove(), not member function remove(), cannot reset size
	list<int>::iterator last;
	last = std::remove(l1.begin(), l1.end(), 13); // will return last
	cout << "after std::remove() l1:";
	for_each(l1.begin(), l1.end(), show_value); 
	cout << endl;
	cout << "l1.size()=" << l1.size() << endl;
	l1.erase(last, l1.end());
	cout << "after erase() l1:";
	for_each(l1.begin(), l1.end(), show_value); 
	cout << endl;
	cout << "l1.size()=" << l1.size() << endl;

	cout << "*(++l1.begin())=" << *(++l1.begin()) << endl;
	// cout << "*(l1.begin()+1)=" << *(l1.begin()+1) << endl; // not support random access


	cout << "-----------------------------" << endl;

	list<int> l4 = {};
	list_desc_insert(l4, 10);
	list_desc_insert(l4, 11);
	list_desc_insert(l4, 9);
	list_desc_insert(l4, 12);
	list_desc_insert(l4, 8);
	cout << "l4.size()=" << l4.size() << endl;
	for_each(l4.begin(), l4.end(), show_value); 
	cout << endl;

	cout << "-----------------------------" << endl;

	list<int> l5;
	for (int i = 0; i < 10; i++)
	{
		l5.push_back(i);
	}
	for_each(l5.begin(), l5.end(), show_value); 
	cout << endl;
	
	auto iter1 = std::find(l5.begin(), l5.end(), 3);
	auto iter2 = std::find(l5.begin(), l5.end(), 8);
	if (iter1 != l5.end() && iter2 != l5.end())
	{
		cout << "*iter1 = " << *iter1 << " *iter2 = " << *iter2 << endl;

		// insert: iterators validity No changes.
		// return new iterator
		auto iter3 = l5.insert(iter1, 20);
		for_each(l5.begin(), l5.end(), show_value); 
		cout << endl;
		cout << "*iter1 = " << *iter1 << " *iter2 = " << *iter2 << " *iter3 = " << *iter3 << endl;

		// erase: Iterators, pointers and references referring to elements removed by the function are invalidated.
		// All other iterators, pointers and references keep their validity.
		// return next iterator
		auto iter4 = l5.erase(iter3); // iter3 is invalid !!!
		for_each(l5.begin(), l5.end(), show_value); 
		cout << endl;
		cout << "*iter1 = " << *iter1 << " *iter2 = " << *iter2 << " *iter3 = " << *iter3 << " *iter4 = " << *iter4 << endl;

	}

	cout << "-----------------------------" << endl;
	cout << "l6:" << endl;

	list<int> l6;
	for (int i = 10; i > 0; i--)
	{
		l6.push_back(i);
	}
	for_each(l6.begin(), l6.end(), show_value); 
	cout << endl;

	for (auto iter = l6.begin(); iter != l6.end(); iter++)
	{
		if (*iter == 10)
		{
			l6.insert(iter, 11);
			for (auto iter2 = iter; iter2 != l6.end(); iter2++)
			{
				if (*iter2 == 10)
				{
					l6.erase(iter2);	
					break;
				}
			}

			break;
		}
	}
	cout << "after insert and erase" << endl;
	for_each(l6.begin(), l6.end(), show_value); 
	cout << endl;
	
	cout << "-----------------------------" << endl;
	list<std::string> l7;
	std::string a = "abcd";
	std::string b = "1234";
	std::string &c = a;
	l7.push_back(a); // copy
	l7.push_back(std::move(b)); // move
	l7.push_back(c);

	cout << "l7 before:" << endl;
	for (auto iter = l7.begin(); iter != l7.end(); iter++)
	{
		cout << *iter << " ";
	}
	cout << endl;

	cout << "b = " << b << endl;

	cout << "l7 after:" << endl;
	for (auto iter = l7.begin(); iter != l7.end(); iter++)
	{
		cout << *iter << " ";
	}
	cout << endl;

	return 0;
}

int test_forward_list()
{
	cout << endl << "-----------------test_forward_list----------------" << endl;

	// == list - reverse_iterator, and cannot get back

	// forward_list is single way list, not support reserve_iterator
	forward_list<int> f1 = {10, 12, 14, 17, 12, 13};

	// f1.push_back(20); // not support push_back()
	// cout << "f1.back()=" << f1.back() << endl; // not support back()
	cout << "f1.front()=" << f1.front() << endl;

	cout << "f1:";
	forward_list<int>::iterator it;
	for (it = f1.begin(); it != f1.end(); it++) {
		cout << " " << *it;
	}
	cout << endl;
	
	cout << "f1:";
	for_each(f1.begin(), f1.end(), show_value); 
	cout << endl;

	// not support reserve_iterator
	// for_each(f1.rbegin(), f1.rend(), show_value); 

	f1.sort();
	cout << "sort() f1:";
	for_each(f1.begin(), f1.end(), show_value); 
	cout << endl;

	f1.unique();
	cout << "unique() f1:";
	for_each(f1.begin(), f1.end(), show_value); 
	cout << endl;

	return 0;
}

int test_queue()
{
	cout << endl << "-----------------test_queue----------------" << endl;

	// base on deque, only these function
	// empty();
	// size();
	// front();
	// back();
	// push();
	// pop();

	// queue is a special adatper class base on deque
	queue<int> q1;
	cout << "q1.empty()=" << q1.empty() << " q1.size()=" << q1.size() << endl;
	q1.push(10); // add to back
	q1.push(11);
	q1.push(12);
	q1.push(13); 
	cout << "after push() q1.empty()=" << q1.empty() << " q1.size()=" << q1.size() << endl;

	cout << "q1.front()=" << q1.front() << " q1.back()=" << q1.back() << endl;
	q1.pop(); // pop() not search data, only delete
	cout << "after pop() q1.front()=" << q1.front() << " q1.back()=" << q1.back() << endl;


	return 0;
}

struct node {
	int value;
	// operator< for priority_queue greater<>
	friend bool operator< (const node &n1, const node &n2)
	{
		return n1.value < n2.value;
	}
};
int test_priority_queue()
{
	cout << endl << "-----------------test_priority_queue----------------" << endl;
	// base on vector, only these function
	// empty();
	// size();
	// push();
	// pop();
	// top();

	// priority_queue is a special adatper class base on vector, like stack, but bigger value move to top
	priority_queue<int> q1;
	cout << "q1.empty()=" << q1.empty() << " q1.size()=" << q1.size() << endl;
	q1.push(12); // add to back
	q1.push(11);
	q1.push(10);
	q1.push(13); 
	cout << "after push() q1.empty()=" << q1.empty() << " q1.size()=" << q1.size() << endl;

	cout << "q1.top()=" << q1.top() << endl;
	q1.pop(); // pop() not search data, only delete
	cout << "after pop() q1.top()=" << q1.top() << endl;

	cout << "-----------------------------" << endl;

	// node should override operator< to sort
	priority_queue<node> q2;
	node nodes[3];
	nodes[0].value = 5;
	nodes[1].value = 15;
	nodes[2].value = 10;
	q2.push(nodes[0]);
	q2.push(nodes[1]);
	q2.push(nodes[2]);

	cout << "q2.top().value=" << q2.top().value << endl;
	q2.pop(); // pop() not search data, only delete
	cout << "q2.top().value=" << q2.top().value << endl;


	return 0;
}

int test_stack()
{
	cout << endl << "-----------------test_stack----------------" << endl;
	// base on vector, only these function
	// empty();
	// size();
	// push();
	// pop();
	// top();

	// stack is a special adatper class base on vector
	stack<int> s1;
	cout << "s1.empty()=" << s1.empty() << " s1.size()=" << s1.size() << endl;
	s1.push(12); // add to back
	s1.push(11);
	s1.push(10);
	s1.push(13); 
	cout << "after push() s1.empty()=" << s1.empty() << " s1.size()=" << s1.size() << endl;

	cout << "s1.top()=" << s1.top() << endl;
	s1.pop(); // pop() not search data, only delete
	cout << "after pop() s1.top()=" << s1.top() << endl;

	return 0;
}

int test_array()
{
	cout << endl << "-----------------test_array----------------" << endl;

	// like c-array

	// array is not a stl container, cannot reset size
	int nums[3] = {1, 2, 3};
	// array<typename, size>
	array<int, 10> a1;
	cout << "a1.size()=" << a1.size() << endl;
	cout << "a1:";
	for_each(a1.begin(), a1.end(), show_value); 
	cout << endl;

	// std::copy(nums, nums+3, std::begin(a1));
	std::copy(nums, nums+3, a1.begin());
	std::copy(nums, nums+3, &a1[3]);
	std::copy(nums, nums+3, &a1[0] + 6);

	cout << "after copy() a1.size()=" << a1.size() << endl;
	cout << "after copy() a1:";
	for_each(a1.begin(), a1.end(), show_value); 
	cout << endl;

	return 0;
}

int test_set()
{
	cout << endl << "-----------------test_set----------------" << endl;
	// key == value, key is unique
	// sort when insert memeber

	string strs[5] = {"aaa", "bbb", "ddd", "ccc", "bbb"};
	string strs2[5] = {"ddd", "fff", "ggg", "kkk", "eee"};
	string strs3[5] = {"jjj", "bbb", "hhh", "lll", "xxx"};
	// in set, key is value, key cannot duplicate
	set<string> s1(strs, strs+5); // default use less<> sort order
	cout << "s1.size()=" << s1.size() << endl;
	cout << "s1:";
	for_each(s1.begin(), s1.end(), show_str); 
	cout << endl;

#ifdef CPP11
	set<string> s2;
	std::insert_iterator<set<string> > it = std::insert_iterator<set<string> >(s2, s2.begin());
	copy(begin(strs2), end(strs2), it);
	cout << "s2.size()=" << s2.size() << endl;
	cout << "s2:";
	for_each(s2.begin(), s2.end(), show_str); 
	cout << endl;

	set<string> s3;
	std::insert_iterator<set<string> > it2 = std::insert_iterator<set<string> >(s3, s3.begin());
	// get s1 | s2
	set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), it2);
	cout << "s3.size()=" << s3.size() << endl;
	cout << "s3:";
	for_each(s3.begin(), s3.end(), show_str); 
	cout << endl;

	std::ostream_iterator<string, char> out = std::ostream_iterator<string, char> (cout, " ");
	cout << "set_intersection(s1, s2) :";
	// get s1 & s2
	set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), out);
	cout << endl;
	cout << "set_difference(s1, s2) :";
	set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), out);
	cout << endl;
	cout << "set_difference(s2, s1) :";
	set_difference(s2.begin(), s2.end(), s1.begin(), s1.end(), out);
	cout << endl;

	set<string> s4;
	s4.insert(begin(strs3), end(strs3)); // value is order in set, cannot insert into a pos
	cout << "s4.size()=" << s4.size() << endl;
	cout << "s4:";
	for_each(s4.begin(), s4.end(), show_str); 
	cout << endl;
#endif

	set<string> s5;
	s5.insert("hello");
	s5.insert("world");
	cout << "s5.size()=" << s5.size() << endl;
	cout << "s5:";
	for_each(s5.begin(), s5.end(), show_str); 
	cout << endl;

	return 0;
}

int test_map()
{
	cout << endl << "-----------------test_map----------------" << endl;
	// key and value, use pair

	pair<int, string> item1(1, "masha");
	cout << "item1.first=" << item1.first << " item1.second=" << item1.second << endl;

	map<int ,string> m1;
	m1.insert(item1);
	m1.insert(std::make_pair(4, "jake"));
	m1.insert(pair<int, string>(2, "kelton"));
	m1.insert(pair<int, string>(3, "peter"));
	// new compailer cannot make_pair with type
	// int id = 6;
	// m1.insert(std::make_pair<int, string>(id, "six"));

	m1[5] = "win";
	m1[4] = "jakes";

	cout << "m1.size()=" << m1.size() << endl;
	cout << "m1:";
	for_each(m1.begin(), m1.end(), show_pair);
	cout << endl;

#ifdef CPP11
	for (auto &it : m1)
	{
		it.second += "8";
	}
#else
	for (auto it = m1.begin(); it != m1.end(); it++)
	{
		it->second += "8";
	}
#endif

#ifdef CPP11
	for (auto it : m1)
	{
		cout << "it.first=" << it.first << " it.second=" << it.second << endl;
	}
#else
	for (auto it = m1.begin(); it != m1.end(); it++)
	{
		cout << "it->first=" << it->first << " it->second=" << it->second << endl;
	}
#endif

	cout << "m1.fine(3)" << endl;
	auto iter = m1.find(3);
	if (iter == m1.end())
	{
		cout << "m1.fine(3) == m1.end()" << endl;
	}
	else
	{
		cout << "first=" << iter->first << " second=" << iter->second << endl;
	}

	std::map<int32_t, int32_t> m2;
	m2.insert(std::make_pair(1, 10));
	m2.insert(std::make_pair(2, 20));
	m2.insert(std::make_pair(3, 30));
	std::map<int, int> m3;
	m3 = m2;
	cout << "m3.size()=" << m3.size() << endl;
	cout << "m3:";
	for (auto iter=m3.begin(); iter!=m3.end(); iter++)
	{
		cout << " " << iter->second;
	}
	cout << endl;

	m3.erase(4);
	cout << "m3.size()=" << m3.size() << endl;
	cout << "m3:";
	for (auto iter=m3.begin(); iter!=m3.end(); iter++)
	{
		cout << " " << iter->second;
	}
	cout << endl;

	cout << endl;
	std::map<int32_t, int32_t> m4;
	m4.insert(std::make_pair(1, 10));
	m4.insert(std::make_pair(2, 20));
	m4.insert(std::make_pair(3, 30));
	const auto &m5 = m4;
	cout << "m5.size()=" << m5.size() << endl;
	cout << "m5:";
	for (auto iter=m5.cbegin(); iter!=m5.cend(); iter++)
	// for (std::map<int32_t, int32_t>::const_iterator iter=m5.cbegin(); iter!=m5.cend(); iter++)
	{
		// iter->second += 1; // error
	}
	cout << endl;

	cout << "m5.size()=" << m5.size() << endl;
	cout << "m5:";
	for (auto iter=m5.begin(); iter!=m5.end(); iter++)
	// for (std::map<int32_t, int32_t>::const_iterator iter=m5.cbegin(); iter!=m5.cend(); iter++)
	{
		cout << " " << iter->second;
	}
	cout << endl;

	{
	cout << endl;
	std::map<int32_t, std::string> m6;
	m6.insert(std::make_pair(1, "a"));
	m6.insert(std::make_pair(3, "c"));
	// m6.insert(std::make_pair(5, "e"));
	m6.insert(std::make_pair(2, "b"));
	m6.insert(std::make_pair(4, "d"));
	m6.insert(std::make_pair(6, "f"));
	cout << "m6.size()=" << m6.size() << endl;
	cout << "m6:\n";
	for (auto iter=m6.begin(); iter!=m6.end(); iter++)
	{
		printf("key=%d value=%s\n", iter->first, iter->second.c_str());
	}
	cout << endl;
	int32_t key = 2;
	auto iter = m6.lower_bound(key);
	if (iter != m6.end() && iter->first == key)
	{
		printf("lower_bound(%d) found value=%s\n", key, iter->second.c_str());
	}
	else
	{
		printf("lower_bound(%d) not found\n", key);
		if (iter != m6.end())
		{
			printf("lower_bound(%d) value=%s\n", key, iter->second.c_str());
		}
	}

	key = 5;
	iter = m6.lower_bound(key);
	if (iter != m6.end() && iter->first == key)
	{
		printf("lower_bound(%d) found value=%s\n", key, iter->second.c_str());
	}
	else
	{
		printf("lower_bound(%d) not found\n", key);
		if (iter != m6.end())
		{
			printf("lower_bound(%d) value=%s\n", key, iter->second.c_str());
		}
	}

	key = 7;
	iter = m6.lower_bound(key);
	if (iter != m6.end() && iter->first == key)
	{
		printf("lower_bound(%d) found value=%s\n", key, iter->second.c_str());
	}
	else
	{
		printf("lower_bound(%d) not found\n", key);
		if (iter != m6.end())
		{
			printf("lower_bound(%d) value=%s\n", key, iter->second.c_str());
		}
	}


	}

	return 0;
}

template <typename T> class Show
{
public:
	Show(){};
	~Show(){};
	// overload operator()(input)
	void operator() (const T &t) { cout << " " <<  t; };
};

template <typename T> class Less
{
public:
	Less(){};
	~Less(){};
	// overload operator()(input1, input2)
	bool operator() (const T &t1, const T &t2) { return t1<t2; };
};

template <typename T> bool less2(const T &t1, const T &t2) {return t1 < t2;};


int test_functor()
{

	cout << endl << "-----------------test_functor----------------" << endl;

	// STL function
	// for_each(it_start, it_end, functor_2_param);
	// sort(it_start, it_end, functor);
	// copy(it_start, it_end, it_to);
	// transform(it_start, it_end, it_to, functor_1_param);

	list<int> l1 = {3, 2, 1, 5, 8, 6};
	Show<int> s;
	cout << "l1:";
	for_each(l1.begin(), l1.end(), s);
	cout << endl;

	vector<int> v1 = {3, 2, 1, 5, 8, 6};
	Less<int> le;
	sort(v1.begin(), v1.end(), le);
	// or use a function
	// sort(v1.begin(), v1.end(), less2<int>);
	cout << "v1:";
	for_each(v1.begin(), v1.end(), s);
	cout << endl;

	std::ostream_iterator<int, char> out(cout, " ");

	cout << "sqrt v1:";
	// from v1.begin to v1.end() use sqrt() output to out
	transform(v1.begin(), v1.end(), out, sqrt);
	cout << endl;

	cout << "bind1st(plus<int>, 10) v1:";
	// from v1.begin to v1.end() use bind1st() output to out
	transform(v1.begin(), v1.end(), out, std::bind1st(std::plus<int>(), 10));
	cout << endl;


	vector<int> v2 = {3, 2, 1, 5, 8, 6};
	cout << "v2:";
	for_each(v2.begin(), v2.end(), s);
	cout << endl;
	cout << "add v1,v2:";
	transform(v1.begin(), v1.end(), v2.begin(), out, std::plus<int>());
	cout << endl;
	


	return 0;
}


int main(int argc, char *argv[])
{
	cout << "hello container" << endl;
	test_vector();
	test_deque();
	test_list();
	test_forward_list();
	test_queue();
	test_priority_queue();
	test_stack();
	test_array();
	test_set();
	test_map();
	test_functor();


	return 0;
}
