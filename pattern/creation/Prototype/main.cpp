
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// prototype do clone, can use for Abstract Factory

class ProductA
{
public:
	virtual ProductA * clone() = 0;
	virtual void Print() = 0;
};

class ProductB
{
public:
	virtual ProductB * clone() = 0;
	virtual void Print() = 0;
};

class ConcreteProductA1 : public ProductA
{
public:
	virtual ProductA * clone() override
	{
		return new ConcreteProductA1();
	}
	virtual void Print() override
	{
		printf("ConcreteProductA1 print\n");
	}
};

class ConcreteProductA2 : public ProductA
{
public:
	virtual ProductA * clone() override
	{
		return new ConcreteProductA2();
	}
	virtual void Print() override
	{
		printf("ConcreteProductA2 print\n");
	}
};

class ConcreteProductB1 : public ProductB
{
public:
	virtual ProductB * clone() override
	{
		return new ConcreteProductB1();
	}
	virtual void Print() override
	{
		printf("ConcreteProductB1 print\n");
	}
};

class ConcreteProductB2 : public ProductB
{
public:
	virtual ProductB * clone() override
	{
		return new ConcreteProductB2();
	}
	virtual void Print() override
	{
		printf("ConcreteProductB2 print\n");
	}
};

class Factory
{
public:
	Factory(ProductA *a, ProductB *b) : pa(a), pb(b)
	{
	}

	ProductA * MakeProductA()
	{
		return pa->clone();
	}

	ProductB * MakeProductB()
	{
		return pb->clone();
	}
	
private:
	ProductA *pa;
	ProductB *pb;
};

int test0()
{

	{
		Factory f(new ConcreteProductA1(), new ConcreteProductB1());
		ProductA * pa = f.MakeProductA();
		pa->Print();
		ProductB * pb = f.MakeProductB();
		pb->Print();
	}
	printf("\n");

	{
		Factory f(new ConcreteProductA2(), new ConcreteProductB1());
		ProductA * pa = f.MakeProductA();
		pa->Print();
		ProductB * pb = f.MakeProductB();
		pb->Print();
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

