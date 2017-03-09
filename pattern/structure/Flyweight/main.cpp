
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// share object

struct InnerState
{
	int x;
	int y;
	void Print()
	{
		printf("x=%d y=%d\n", x, y);
	}
};

class Flyweight
{
public:
	virtual ~Flyweight() {}
	virtual InnerState *GetState() = 0;
};

class ConcreteFlyweight : public Flyweight
{
public:
	ConcreteFlyweight()
	{
		m_state.x = 10;
		m_state.y = 20;
	}
	virtual ~ConcreteFlyweight() {}
	virtual InnerState *GetState()
	{
		return &m_state;
	}
private:
	InnerState m_state;
};

class Factory
{
public:
	Flyweight *Create()
	{
		if (m_f == nullptr)
		{
			m_f = new ConcreteFlyweight();
		}
		return m_f;
	}
	~Factory()
	{
		delete m_f;
	}
private:
	Flyweight *m_f;
};

int test0()
{
	Factory f;
	Flyweight * p1 = f.Create();
	Flyweight * p2 = f.Create();
	p1->GetState()->Print();
	p2->GetState()->Print();
	if (p1 == p2)
	{
		printf("same node\n");
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

