
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <functional>
#include <memory>

void funcNoParamNoReturn()
{
	printf("%s\n", __func__);
}

int funcNoParam()
{
	printf("%s\n", __func__);
	return 0;
}

int funcTwoParam(int x, int y)
{
	printf("%s\n", __func__);
	return x+y;
}

class FuncObj
{
public:
	FuncObj(int n = 0) : offset(n) {} 
	~FuncObj() { offset = 0; }
	double operator() (int x, int y)
	{
		printf("%s\n", __func__);
		return (double)x/y + offset;
	}

	double doDiv(int x, int y)
	{
		printf("%s\n", __func__);
		return (double)x/y + offset;
	}

	int offset;
};

int test0()
{
	funcNoParamNoReturn();
	funcNoParam();
	funcTwoParam(10, 20);

	FuncObj funcObj;
	funcObj(20, 40);
	funcObj(20, 40);

	return 0;
}

int test1()
{
	using namespace std::placeholders;
	double ret = 0;

	auto func1 = std::bind(funcNoParamNoReturn);
	func1();

	auto func2 = std::bind(&funcNoParamNoReturn);
	func2();

	std::function<void()> func3 = std::bind(funcNoParamNoReturn);
	func3();

	auto func4 = std::bind(funcNoParam);
	func4();

	auto func5 = std::bind(funcTwoParam, _1, _2);
	ret = func5(10, 20);
	printf("ret=%lf\n", ret);
	printf("\n");

	return 0;
}

int test2() 
{
	using namespace std::placeholders;
	double ret = 0;

	std::function<double(int,int)> func6;
	{
		FuncObj funcObj;
		// bind a copy obj
		func6 = std::bind(&FuncObj::doDiv, funcObj, _1, _2);
		ret = func6(10, 20);
		printf("ret=%lf\n", ret);

		// will not change bind obj
		funcObj.offset = 100;
		ret = func6(10, 20);
		printf("ret=%lf\n", ret);
	}
	// bind still work
	ret = func6(10, 20);
	printf("ret=%lf\n", ret);
	printf("\n");

	std::function<double(int,int)> func7;
	{
		FuncObj funcObj;
		// bind a ref obj
		func7 = std::bind(&FuncObj::doDiv, std::ref(funcObj), _1, _2);
		ret = func7(10, 20);
		printf("ret=%lf\n", ret);

		// will change bind obj
		funcObj.offset = 100;
		ret = func7(10, 20);
		printf("ret=%lf\n", ret);
	}
	// ref is destruct, call bind function is not a good idea
	// ret = func7(10, 20);
	printf("\n");

	{
		FuncObj *ptr = new FuncObj(10);
		// bind a obj pointer
		auto func = std::bind(&FuncObj::doDiv, ptr, _1, _2);
		ret = func(10, 20);
		printf("ret=%lf\n", ret);

		// will change bind obj
		ptr->offset = 100;
		ret = func(10, 20);
		printf("ret=%lf\n", ret);
		delete ptr;
	}
	printf("\n");

	{
		// bind a smart pointer
		auto func = std::bind(&FuncObj::doDiv, std::make_shared<FuncObj>(FuncObj(51)), _1, _2);
		ret = func(10, 20);
		printf("ret=%lf\n", ret);
	}
	printf("\n");


	{
		FuncObj funcObj;
		auto funcA = std::bind(&FuncObj::doDiv, std::ref(funcObj), _1, _2);
		ret = funcA(10, 20);
		printf("ret=%lf\n", ret);

		// reserve input param
		auto funcB = std::bind(&FuncObj::doDiv, std::ref(funcObj), _2, _1);
		ret = funcB(10, 20);
		printf("ret=%lf\n", ret);
	}
	printf("\n");

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
,	test2
};

int main(int argc, char *argv[]) 
{
	printf("hello %s\n", argv[0]);

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

