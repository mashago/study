
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>

// connect lots of objects by one mediator, objects only has a ptr to mediator

enum
{
	TYPE_INPUT = 1,
	TYPE_OUTPUT = 2,
};

class Colleague;

class Mediator
{
public:
	virtual ~Mediator(){}
	virtual bool RegisterColleague(Colleague *c) = 0;
	virtual void ColleagueChange(Colleague *c) = 0;
protected:
	Mediator(){}
};

class Colleague
{
public:
	virtual ~Colleague() {}
	virtual int GetType()
	{
		return m_type;
	}
	virtual int GetData()
	{
		return m_data;
	}
	virtual void SetMediator(Mediator *m)
	{
		m_mediator = m;
	}
	virtual void Change()
	{
		if (m_mediator)
		{
			m_mediator->ColleagueChange(this);
		}
	}
	virtual void UpdateData(int d) = 0;
	virtual void Print()
	{
		printf("m_type=%d m_data=%d\n", m_type, m_data);
	}

protected:
	Colleague(int t) : m_type(t), m_data(0), m_mediator(nullptr) {}
	int m_type;
	int m_data;
	Mediator *m_mediator;
};

class ConcreteColleagueA : public Colleague
{
public:
	ConcreteColleagueA(int t) : Colleague(t) {}
	virtual ~ConcreteColleagueA() {}
	virtual void UpdateData(int d)
	{
		m_data = d;
		Change();
	}
};

class ConcreteColleagueB : public Colleague
{
public:
	ConcreteColleagueB(int t) : Colleague(t) {}
	virtual ~ConcreteColleagueB() {}
	virtual void UpdateData(int d)
	{
		m_data += d;
		Change();
	}
};

class ConcreteMediator : public Mediator
{
public:
	ConcreteMediator() : Mediator() {}
	virtual ~ConcreteMediator() {}
	virtual bool RegisterColleague(Colleague *c) override
	{
		m_members.insert(c);
		c->SetMediator(this);
		return true;
	}
	virtual void ColleagueChange(Colleague *c) override
	{
		switch (c->GetType())
		{
			case TYPE_INPUT:
			{
				for (auto ptr : m_members)
				{
					if (ptr->GetType() == TYPE_OUTPUT)
					{
						ptr->UpdateData(c->GetData());
					}
				}
				break;
			}
			case TYPE_OUTPUT:
			{
				c->Print();
				break;
			}
		}
	}
private:
	std::set<Colleague*> m_members;
};

int test0()
{
	ConcreteMediator mediator;
	ConcreteColleagueA c1(TYPE_INPUT);
	mediator.RegisterColleague(&c1);
	ConcreteColleagueA c2(TYPE_INPUT);
	mediator.RegisterColleague(&c2);
	ConcreteColleagueB c3(TYPE_OUTPUT);
	mediator.RegisterColleague(&c3);
	ConcreteColleagueB c4(TYPE_OUTPUT);
	mediator.RegisterColleague(&c4);

	c1.Print();
	c2.Print();
	c3.Print();
	c4.Print();
	printf("\n");

	c1.UpdateData(10);
	printf("\n");

	c1.Print();
	c2.Print();
	c3.Print();
	c4.Print();
	printf("\n");

	c2.UpdateData(11);
	printf("\n");

	c1.Print();
	c2.Print();
	c3.Print();
	c4.Print();


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

