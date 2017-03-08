
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>

// use a composite as a component, handle composite inside component recursively

class Component
{
public:
	virtual ~Component() {}
	virtual void Operator() = 0;

	// default do nothing, interface for component
	virtual void Add(Component *p) {};
	virtual void Remove(Component *p) {};
};

class Leaf : public Component
{
public:
	virtual ~Leaf() {}
	virtual void Operator() override
	{
		printf("Leaf %s\n", __FUNCTION__);
	}
};

class Composite : public Component
{
public:
	virtual ~Composite() 
	{
		for (auto &leaf : m_contentList)
		{
			delete leaf;
		}
	}
	virtual void Operator() override
	{
		printf("Composite %s\n", __FUNCTION__);
		for (auto &leaf : m_contentList)
		{
			leaf->Operator();
		}
	}

	virtual void Add(Component *p) override
	{
		m_contentList.push_back(p);
	}

	virtual void Remove(Component *p) override
	{
		for (auto iter = m_contentList.begin(); iter != m_contentList.end(); iter++)
		{
			if (*iter == p)
			{
				m_contentList.erase(iter);
				break;
			}
		}
	}
private:
	std::list<Component *> m_contentList;
};

int test0()
{
	{
		Component *ptr1 = new Leaf();
		ptr1->Operator();
		delete ptr1;
		printf("\n");
	}

	{
		Component *ptr2 = new Composite();
		ptr2->Operator();
		delete ptr2;
		printf("\n");
	}

	{
		Component *ptr3 = new Composite();
		ptr3->Add(new Leaf());
		ptr3->Add(new Leaf());
		ptr3->Add(new Leaf());
		ptr3->Operator();
		delete ptr3;
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

