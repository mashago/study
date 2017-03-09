
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

// a builder to create a complex product, by some part function

enum PART_ENUM
{
	PART_A = 0,
	PART_B = 1,
	PART_C = 2,
	PART_MAX = 3,
};
static const int MAX_PART = PART_ENUM::PART_MAX;

typedef std::vector<int> RESULT;

class Builder
{
public:
	Builder() : result(MAX_PART, 0)
	{
	}
	virtual void MakePartA()
	{
		result[PART_A] = 1;	
	}
	virtual void MakePartB()
	{
		result[PART_B] = 2;	
	}
	virtual void MakePartC()
	{
		result[PART_C] = 3;	
	}
	void Print()
	{
		printf("result=");
		for (int i = 0; i < MAX_PART; i++)
		{
			printf("[%d]", result[i]); 
		}
		printf("\n");
	}
	RESULT GetResult()
	{
		return result;
	}
protected:
	RESULT result;
};

class ConcreteBuilderA : public Builder
{
public:
	virtual void MakePartA() override
	{
		result[PART_A] = 11;	
	}
	virtual void MakePartC() override
	{
		result[PART_C] = 13;	
	}
};

class ConcreteBuilderB : public Builder
{
public:
	virtual void MakePartB() override
	{
		result[PART_B] = 22;	
	}
	virtual void MakePartC() override
	{
		result[PART_C] = 23;	
	}
};

RESULT Make(Builder &builder)
{
	builder.MakePartA();
	builder.MakePartB();
	builder.MakePartC();
	builder.Print();
	return builder.GetResult();
}

int test0()
{
	Builder b1;
	Make(b1);

	ConcreteBuilderA b2;
	Make(b2);

	ConcreteBuilderB b3;
	Make(b3);
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
// ,	test1
};

int main(int argc, char *argv[]) 
{
	int ret;
	int maxcase;
	int testcase;

	maxcase = sizeof(test_list) / sizeof(testcase_t);
	testcase = maxcase - 1;

	if (argc > 1) {
		if (!strcmp(argv[1], "all"))
		{
			printf("run all case\n");
			for (int i=0; i<maxcase; i++)
			{
				printf("run case[%d]\n", i);
				ret = test_list[i]();
				if (ret != 0) 
				{
					printf("case[%d] ret=%d\n", i, ret);
					return 0;
				}
			}
			return 0;
		}
		testcase = atoi(argv[1]);
		if (testcase < 0 || testcase >= maxcase) 
		{
			testcase = maxcase - 1;
		}
	}

	printf("run case[%d]\n", testcase);
	ret = test_list[testcase]();
	if (ret != 0) 
	{
		printf("case[%d] ret=%d\n", testcase, ret);
	}

	return 0;
}

