
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// warpper, decorator be a component and has a component inside, and transmit operation to inside component, therefore decorator can do some logic before or after component operation

class Component
{
public:
	virtual ~Component(){}
	virtual void Operation() = 0;
};

class ConcreteComponent : public Component
{
public:
	virtual ~ConcreteComponent(){}
	virtual void Operation() override
	{
		printf("ConcreteComponent %s\n", __FUNCTION__);
	}
};

class Decorator : public Component
{
public:
	Decorator(Component *p) : m_component(p) {}
	virtual ~Decorator()
	{
		printf("do delete\n");
		delete m_component;
	}
	virtual void Operation() override
	{
		printf("Decorator %s\n", __FUNCTION__);
		m_component->Operation();
	}
private:
	Component *m_component;
};

class ConcreteDecoratorA : public Decorator
{
public:
	ConcreteDecoratorA(Component *p) : Decorator(p) {}
	virtual ~ConcreteDecoratorA() { }
	virtual void Operation() override
	{
		printf("ConcreteDecoratorA %s\n", __FUNCTION__);
		Decorator::Operation();
		AdditionalOperation();
	}
private:
	void AdditionalOperation()
	{
		printf("ConcreteDecoratorA %s\n", __FUNCTION__);
	}
};

class ConcreteDecoratorB : public Decorator
{
public:
	ConcreteDecoratorB(Component *p) : Decorator(p) {}
	virtual ~ConcreteDecoratorB() { }
	virtual void Operation() override
	{
		printf("ConcreteDecoratorB %s\n", __FUNCTION__);
		AdditionalOperation();
		Decorator::Operation();
	}
private:
	void AdditionalOperation()
	{
		printf("ConcreteDecoratorB %s\n", __FUNCTION__);
	}
};

int test0()
{

	{
		Decorator *ptr = new ConcreteDecoratorA(new ConcreteComponent());	
		ptr->Operation();
		delete ptr;
		printf("\n");
	}

	{
		Decorator *ptr = new ConcreteDecoratorB(new ConcreteComponent());	
		ptr->Operation();
		delete ptr;
		printf("\n");
	}

	{
		Decorator *ptr = new ConcreteDecoratorA(new ConcreteDecoratorB(new ConcreteComponent()));
		ptr->Operation();
		delete ptr;
		printf("\n");
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

