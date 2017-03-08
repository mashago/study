
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// split low level raw implementor and high level abstruction

class Implementor
{
public:
	virtual ~Implementor() {}	
	virtual void RawOperator1() = 0;
	virtual void RawOperator2() = 0;
};

class ConcreteImplementorA : public Implementor
{
public:
	virtual ~ConcreteImplementorA() {}	
	virtual void RawOperator1() override
	{
		printf("ConcreteImplementorA %s\n", __FUNCTION__);
	}
	virtual void RawOperator2() override
	{
		printf("ConcreteImplementorA %s\n", __FUNCTION__);
	}
};

class ConcreteImplementorB : public Implementor
{
public:
	virtual ~ConcreteImplementorB() {}	
	virtual void RawOperator1() override
	{
		printf("ConcreteImplementorB %s\n", __FUNCTION__);
	}
	virtual void RawOperator2() override
	{
		printf("ConcreteImplementorB %s\n", __FUNCTION__);
	}
};

////

class Factory
{
public:
	virtual Implementor *CreateImp() = 0;
};

class ConcreteFactoryA : public Factory
{
public:
	virtual Implementor *CreateImp() override
	{
		return new ConcreteImplementorA();
	}
};

class ConcreteFactoryB : public Factory
{
public:
	virtual Implementor *CreateImp() override
	{
		return new ConcreteImplementorB();
	}
};

////

class Abstruction
{
public:
	Abstruction(Implementor *p) : m_imp(p) {}
	virtual ~Abstruction()
	{
		delete m_imp;
	}
	virtual void Operator() = 0;
	Implementor *GetImp()
	{
		return m_imp;
	}
protected:
	Implementor *m_imp;
};

class ConcreteAbstructionA : public Abstruction
{
public:
	ConcreteAbstructionA(Implementor *p) : Abstruction(p) {}
	virtual ~ConcreteAbstructionA() {}
	virtual void Operator() override
	{
		printf("ConcreteAbstructionA %s\n", __FUNCTION__);
		// use raw operator, but donnot care how raw operartor work
		m_imp->RawOperator1();
		m_imp->RawOperator2();
	}
};

int test0()
{
	{
		ConcreteFactoryA factory;
		ConcreteAbstructionA conc(factory.CreateImp());
		conc.Operator();
	}
	printf("\n");

	{
		ConcreteFactoryB factory;
		ConcreteAbstructionA conc(factory.CreateImp());
		conc.Operator();
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

