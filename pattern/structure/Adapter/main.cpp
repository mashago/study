
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// wrapper, transmit request from one class to another, which have different interface

class Target
{
public:
	virtual ~Target() {}
	virtual void Request() = 0;
	virtual bool IsEmpty() = 0;
};

class Adaptee
{
public:
	Adaptee(int n) : num(n) {}
	virtual ~Adaptee() {}
	virtual void SpecificRequest()
	{
		printf("Adaptee SpecificRequest\n");
	}
	virtual bool IsEmpty()
	{
		return num == 0;
	}
private:
	int num;
};

class ClassAdapter : public Target, private Adaptee
{
public:
	ClassAdapter(int n) : Target(), Adaptee(n) {}
	~ClassAdapter() {}
	virtual void Request() override
	{
		SpecificRequest();
	}
	virtual bool IsEmpty() override
	{
		return Adaptee::IsEmpty();
	}
};

class MemberAdapter : public Target
{
public:
	MemberAdapter(Adaptee *p) : Target(), ptr(p) {}
	~MemberAdapter()
	{
		delete ptr;
	}
	virtual void Request() override
	{
		ptr->SpecificRequest();
	}
	virtual bool IsEmpty() override
	{
		return ptr->IsEmpty();
	}
private:
	Adaptee *ptr;
};

int test0()
{
	{
		Target * ptr = new ClassAdapter(10);
		ptr->Request();
		printf("IsEmpty=%d\n", ptr->IsEmpty());
		delete ptr;
	}
	printf("\n");

	{
		Adaptee * ap = new Adaptee(0);
		MemberAdapter *ptr = new MemberAdapter(ap);
		ptr->Request();
		printf("IsEmpty=%d\n", ptr->IsEmpty());
		delete ptr;
	}

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

