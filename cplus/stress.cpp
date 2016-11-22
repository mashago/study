#include <sys/time.h>
#include <iostream>
#include <map>
#include <vector>
#include <deque>
#include <list>
#include <string.h>

using namespace std;

long long get_utime()
{
	struct timeval t;
	gettimeofday(&t, NULL);

	return t.tv_sec * 1000000 + t.tv_usec;
}

// map stress
int test0() 
{
	long long t1;
	t1 = get_utime();

	// write map 1000000 about 650ms
	map<int, int> tmp_map;
	for (int i = 0; i < 1000000; i++)
	{
		tmp_map[i] = i;	
	}

	long long t2;
	t2 = get_utime();

	cout << "map write 1000000 = " << t2 - t1 << "us" << endl;

	// read about 60ms
	for (auto iter = tmp_map.begin(); iter != tmp_map.end(); iter++)
	{
	}

	long long t3;
	t3 = get_utime();
	cout << "map read 1000000 = " << t3 - t2 << "us"<< endl;

	return 0;
}

// vector stress
int test1() 
{
	long long t1;
	t1 = get_utime();

	// write vector 1000000 about 40ms
	vector<int> tmp_vec;
	for (int i = 0; i < 1000000; i++)
	{
		tmp_vec.push_back(i);
	}

	long long t2;
	t2 = get_utime();

	cout << "vector write 1000000 = " << t2 - t1 << "us" << endl;

	// read about 15ms
	for (auto iter = tmp_vec.begin(); iter != tmp_vec.end(); iter++)
	{
	}

	long long t3;
	t3 = get_utime();
	cout << "vector read 1000000 = " << t3 - t2 << "us"<< endl;

	return 0;
}

// deque stress
int test2() 
{
	long long t1;
	t1 = get_utime();

	// write deque 1000000 about 55ms
	deque<int> tmp_deq;
	for (int i = 0; i < 1000000; i++)
	{
		tmp_deq.push_back(i);
	}

	long long t2;
	t2 = get_utime();

	cout << "deque write 1000000 = " << t2 - t1 << "us" << endl;

	// read about 30ms
	for (auto iter = tmp_deq.begin(); iter != tmp_deq.end(); iter++)
	{
	}

	long long t3;
	t3 = get_utime();
	cout << "deque read 1000000 = " << t3 - t2 << "us"<< endl;

	return 0;
}

// list stress
int test3() 
{
	long long t1;
	t1 = get_utime();

	// write list 1000000 about 55ms
	list<int> tmp_list;
	for (int i = 0; i < 1000000; i++)
	{
		tmp_list.push_back(i);
	}

	long long t2;
	t2 = get_utime();

	cout << "list write 1000000 = " << t2 - t1 << "us" << endl;

	// read about 30ms
	for (auto iter = tmp_list.begin(); iter != tmp_list.end(); iter++)
	{
	}

	long long t3;
	t3 = get_utime();
	cout << "list read 1000000 = " << t3 - t2 << "us"<< endl;

	return 0;
}

// list stress
int test4() 
{
	int MAX_SIZE = 1000000;

	{
	list<int> l1;
	for (int i = 0; i < MAX_SIZE; i++)
	{
		l1.push_back(i);
	}

	int value = 2000000;
	l1.push_back(value);

	long long t1;
	t1 = get_utime();

	// type 1
	// 0. find new pos
	// 1. insert
	// 2. update add
	// 3. erase

	auto newIter = l1.begin();
	auto tmpIter = l1.rbegin();
	for (; tmpIter != l1.rend(); tmpIter++)
	{
		if (*tmpIter > value)
		{
			break;
		}
	}

	newIter = l1.insert(l1.begin(), 1);
	newIter++;
	for (auto iter = newIter; iter != l1.end(); iter++)
	{
		if (*iter == value)
		{
			l1.erase(iter);
			break;
		}
		(*iter)++;
	}

	long long t2;
	t2 = get_utime();

	cout << "type 1 [" << MAX_SIZE << "] = " << t2 - t1 << "us" << endl;

	}

	{

	list<int> l1;
	for (int i = 0; i < MAX_SIZE; i++)
	{
		l1.push_back(i);
	}

	int value = 0;
	l1.push_back(value);

	long long t1;
	t1 = get_utime();

	// type 2
	// 1. compare with forward item, update

	int tmp = 0;
	auto nextIter = l1.rbegin();
	nextIter++;
	auto iter = l1.rbegin();

	for (; nextIter != l1.rend(); nextIter++, iter++)
	{
		if (*iter < *nextIter)
		{
			tmp = *iter;
			*iter = *nextIter;
			*nextIter = tmp;
		}
	}

	long long t2;
	t2 = get_utime();

	cout << "type 2 [" << MAX_SIZE << "] = " << t2 - t1 << "us" << endl;
	}

	return 0;
}

typedef int (*testcase_t) ();
testcase_t test_list[] =
{
	test0
,	test1
,	test2
,	test3
,	test4
};

int main(int argc, char **argv)
{
	cout << "hello stress" << endl;

	int ret;
	int maxcase;
	int testcase;

	maxcase = sizeof(test_list) / sizeof(testcase_t);
	testcase = maxcase - 1;

	if (argc > 1) {
		if (!strcmp(argv[1], "all")) {
			cout << "run all case" << endl;
			for (int i=0; i<maxcase; i++) {
				cout << "RUN case[" << i << "]" << endl;
				ret = test_list[i]();
				if (ret != 0) {
					cout << "case[" << i << "] " << ret << endl;
					return 0;
				}
			}
			return 0;
		}
		testcase = atoi(argv[1]);
		if (testcase < 0 || testcase >= maxcase) {
			testcase = maxcase - 1;
		}
	}

	cout << "RUN case[" << testcase << "]" << endl;
	ret = test_list[testcase]();
	if (ret != 0) {
		cout << "stu:case[" << testcase << "] " << ret << endl;
	}

	return 0;
}

