
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// new Factory for new Product

class Product
{
public:
	virtual void Print() = 0;
	virtual ~Product(){}
};

class ConcreteProductA : public Product
{
public:
	ConcreteProductA(int n) : x(n)
	{
	}
	virtual void Print() override
	{
		printf("ConcreteProductA x=%d\n", x);
	}
private:
	int x;
};

class ConcreteProductB : public Product
{
public:
	ConcreteProductB(double n) : y(n)
	{
	}
	virtual void Print() override
	{
		printf("ConcreteProductB y=%lf\n", y);
	}
private:
	double y;
};

////

class Factory
{
public:
	virtual Product * CreateProduct() = 0;
	virtual ~Factory(){}
};

class ConcreteFactoryA : public Factory
{
public:
	virtual Product * CreateProduct() override
	{
		ConcreteProductA *p = new ConcreteProductA(10);
		return p;
	}
};

class ConcreteFactoryB : public Factory
{
public:
	virtual Product * CreateProduct() override
	{
		ConcreteProductB *p = new ConcreteProductB(20.02);
		return p;
	}
};

int test0()
{
	Factory *f1 = new ConcreteFactoryA();
	Product *p1 = f1->CreateProduct();	
	p1->Print();
	delete f1;
	delete p1;

	Factory *f2 = new ConcreteFactoryB();
	Product *p2 = f2->CreateProduct();	
	p2->Print();
	delete f2;
	delete p2;

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

