
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// abstract define regular framework, concrete define change part

class AbstractClass
{
public:
	virtual ~AbstractClass(){}
	int Operation()
	{
		OperationPartA();
		OperationPartB();
		OperationPartC();
		return m_result;
	}
protected:
	AbstractClass() : m_result(0) {}
	virtual void OperationPartA()
	{
	}
	virtual void OperationPartB()
	{
	}
	virtual void OperationPartC()
	{
	}
	int m_result;
};

class ConcreteClassA : public AbstractClass
{
public:
	ConcreteClassA() : AbstractClass() {}
	virtual ~ConcreteClassA() {}
private:
	virtual void OperationPartA() override
	{
		m_result += 1;
	}
	virtual void OperationPartB() override
	{
		m_result += 1;
	}
	virtual void OperationPartC() override
	{
		m_result += 1;
	}
};

class ConcreteClassB : public AbstractClass
{
public:
	ConcreteClassB() : AbstractClass() {}
	virtual ~ConcreteClassB() {}
private:
	virtual void OperationPartA() override
	{
		m_result += 1;
	}
	virtual void OperationPartB() override
	{
		m_result += 2;
	}
	virtual void OperationPartC() override
	{
		m_result += 3;
	}
};

int test0()
{
	int result = 0;
	ConcreteClassA c1;
	result = c1.Operation();
	printf("result=%d\n", result);

	ConcreteClassB c2;
	result = c2.Operation();
	printf("result=%d\n", result);

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

