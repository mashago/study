
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <vector>

using std::cout;
using std::endl;

enum { RANDOM_RANGE_MIN = 0, RANDOM_RANGE_MAX = 100 };

int randomNum()
{
	static std::default_random_engine e(time(NULL));
	std::uniform_int_distribution<unsigned> u(RANDOM_RANGE_MIN, RANDOM_RANGE_MAX); // [,]
	return u(e);
}

std::vector<int> getRandomVec()
{
	enum { MAX_SIZE = 100 };
	std::vector<int> nums(MAX_SIZE);
	std::generate(nums.begin(), nums.end(), randomNum);
	return nums;
}

bool isMod3(int n)
{
	return n % 3 == 0;
}

int test0()
{
	int count = 0;
	std::vector<int> nums = getRandomVec();

	// normal function
	count = std::count_if(nums.begin(), nums.end(), isMod3);
	cout << "normal function: count=" << count << endl;

	// functor
	// local class
	class ObjIsModX 
	{
	private:
		int x;
	public:
		ObjIsModX(int d) : x(d) {};
		bool operator() (int n) { return n % x == 0; };
	};
	ObjIsModX objIsMod3(3); // create a functor
	count = std::count_if(nums.begin(), nums.end(), objIsMod3);
	cout << "functor: count=" << count << endl;

	// bind
	// local class
	class ObjHandler
	{
	private:
		int x;
	public:
		ObjHandler(int d) : x(d) {}
		bool IsModX(int n) { return n % x == 0; }
	};
	ObjHandler objHandler(3);
	using namespace std::placeholders;
	count = std::count_if(nums.begin(), nums.end(), std::bind(&ObjHandler::IsModX, std::ref(objHandler), _1));
	cout << "bind: count=" << count << endl;

	// lambda
	// [capture_list](input) ->return_type {return logic;}
	// if logic is one single return, ->return_type can ignore
	// default is value capture
	int modx = 3;
	count = std::count_if(nums.begin(), nums.end(), [modx](int n){return n % modx == 0;});
	cout << "lambda1: count=" << count << endl;

	count = 0;
	// reference capture
	std::for_each(nums.begin(), nums.end(), [&count, modx](int n){if (n % modx == 0) ++count;});
	cout << "lambda2: count=" << count << endl;

	// give a name for lambda, ->bool is return
	auto lamIsModX = [modx](int n) ->bool {return n % modx == 0;};
	count = std::count_if(nums.begin(), nums.end(), lamIsModX);
	cout << "lambda3: count=" << count << endl;

	bool result = lamIsModX(4);
	cout << "result=" << result << endl;
	return 0;
}

int test1()
{
	return 0;
}

int test_notyet() 
{
	// int ret;

	// ret = 0;
	return 0;
}

typedef int (*testcase_t) ();
testcase_t test_list[] =
{
	test0
,	test1
};

int main(int argc, char *argv[]) 
{
	int ret;
	int maxcase;
	int testcase;

	maxcase = sizeof(test_list) / sizeof(testcase_t);
	testcase = maxcase - 1;

	if (argc > 1) {
		if (!strcmp(argv[1], "all")) {
			printf("run all case\n");
			for (int i=0; i<maxcase; i++) {
				printf("run case[%d]\n", i);
				ret = test_list[i]();
				if (ret != 0) {
					printf("case[%d] ret=%d\n", i, ret);
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

	printf("run case[%d]\n", testcase);
	ret = test_list[testcase]();
	if (ret != 0) {
		printf("case[%d] ret=%d\n", testcase, ret);
	}

	return 0;
}

