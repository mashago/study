
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Abstract Factory is about create a set of products
// product set is confirmd, cannot create new product
// but can create different set of products

struct ProductA
{
	char mark;
	void Print()
	{
		printf("ProductA mark=%c\n", mark);
	}
};

struct ProductB
{
	char mark;
	void Print()
	{
		printf("ProductB mark=%c\n", mark);
	}
};

class AbstractFactory
{
public:
	virtual ~AbstractFactory() {}
	virtual ProductA CreateProductA() = 0;
	virtual ProductB CreateProductB() = 0;
};

class FactoryA : public AbstractFactory
{
public:
	const char MARK = 'A';
	virtual ProductA CreateProductA() override
	{
		ProductA p;
		p.mark = MARK;
		return p;
	}

	virtual ProductB CreateProductB() override
	{
		ProductB p;
		p.mark = MARK;
		return p;
	}
};

class FactoryB : public AbstractFactory
{
public:
	const char MARK = 'B';
	virtual ProductA CreateProductA() override
	{
		ProductA p;
		p.mark = MARK;
		return p;
	}

	virtual ProductB CreateProductB() override
	{
		ProductB p;
		p.mark = MARK;
		return p;
	}
};

void Make(AbstractFactory &factory)
{
	ProductA pa = factory.CreateProductA();
	pa.Print();

	ProductB pb = factory.CreateProductB();
	pb.Print();

	printf("\n");
}

int test0()
{

	FactoryA fa;
	Make(fa);

	FactoryB fb;
	Make(fb);


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

