
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "masha.pb.h"

int test0()
{
	// use package
	using namespace MashaProto;

	Bar bar;
	printf("bar.has_id()=%d\n", bar.has_id());
	bar.set_id(10);
	printf("bar.has_id()=%d\n", bar.has_id());
	printf("bar.id()=%d\n", bar.id());

	return 0;
}

int test1()
{
	using namespace MashaProto;
	{
	Foo foo;
	Bar &bar = *foo.mutable_bar();
	bar.set_id(100);
	printf("bar.id()=%d\n", bar.id());

	enum { MAX_SIZE = 10 };
	for (int i = 0; i < 10; i++)
	{
		Bar &bar = *foo.add_bars();
		bar.set_id(i);
	}
	printf("foo.bars_size()=%d\n", foo.bars_size());

	{
	const Bar &t = foo.bars(5);
	printf("t.id()=%d\n", t.id());

	const Bar *pt = foo.mutable_bars(6);
	printf("pt->id()=%d\n", pt->id());
	}

	/*
	{
	// out of bound
	// below will throw google::protobuf::FatalException
	// what():  CHECK failed: (index) < (size()):
	const Bar &t = foo.bars(MAX_SIZE);
	const Bar *pt = foo.mutable_bars(MAX_SIZE);
	}
	*/
	}
	printf("\n");

	{
	Foo foo;
	printf("foo.has_bar()=%d\n", foo.has_bar());

	const Bar &bar = foo.bar(); // will not set bar
	printf("foo.has_bar()=%d\n", foo.has_bar());

	Bar *pbar = foo.mutable_bar(); // will set bar if bar not exsits
	printf("foo.has_bar()=%d\n", foo.has_bar());
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

