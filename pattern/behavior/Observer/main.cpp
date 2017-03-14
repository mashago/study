
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>

class Subject;

class Observer
{
public:
	virtual ~Observer() {}
	virtual void Update(Subject *subject) = 0;
protected:
	Observer() {};
};

class Subject
{
public:
	virtual ~Subject() {}
	virtual void Attach(Observer *ob)
	{
		m_observers.insert(ob);
	}
	virtual void Detach(Observer *ob)
	{
		m_observers.erase(ob);
	}
	virtual void Notify()
	{
		for (auto iter : m_observers)
		{
			iter->Update(this);
		}
	}
protected:
	Subject() {};
private:
	std::set<Observer *> m_observers;
};

class ConcreteSubject : public Subject
{
public:
	ConcreteSubject() : Subject(), m_data(0) {}
	virtual ~ConcreteSubject() {}
	virtual void SetData(int d)
	{
		m_data = d;
		Notify();
	}
	virtual int GetData()
	{
		return m_data;
	}
private:
	int m_data;
};

class ConcreteObserver : public Observer
{
public:
	ConcreteObserver(ConcreteSubject *s) : Observer(), m_subject(s)
	{
		m_subject->Attach(this);
	}
	virtual ~ConcreteObserver()
	{
		m_subject->Detach(this);
	}
	virtual void Update(Subject *subject)
	{
		if (subject == m_subject)
		{
			Operation();
		}
	}
	virtual void Operation()
	{
		int data = m_subject->GetData();
		printf("ConcreteObserver::%s data=%d\n", __FUNCTION__, data);
	}
private:
	ConcreteSubject *m_subject;
};

int test0()
{
	ConcreteSubject s;
	ConcreteObserver o(&s);
	s.SetData(10);
	s.SetData(11);
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

