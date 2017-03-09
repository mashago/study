
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

class Subject
{
public:
	virtual ~Subject(){}
	virtual void Operation() = 0;
};

class RealSubject : public Subject
{
public:
	virtual ~RealSubject() {}
	virtual void Operation()
	{
		printf("RealSubject %s\n", __FUNCTION__);
	}
};

class RemoteProxy : public Subject
{
public:
	virtual ~RemoteProxy() {}
	virtual void Operation()
	{
		printf("RemoteProxy %s\n", __FUNCTION__);
		// no subject inside, send request to remote subject to do operation
	}
};

class HeavySubject : public Subject
{
public:
	HeavySubject(std::string filename) : m_filename(filename)
	{
		Load();
	}
	virtual ~HeavySubject() {}
	virtual void Operation()
	{
		printf("HeavySubject %s\n", __FUNCTION__);
	}
private:
	void Load()
	{
		printf("HeavySubject %s filename=%s\n", __FUNCTION__, m_filename.c_str());
	}
	std::string m_filename;
};

class VirtualProxy : public Subject
{
public:
	VirtualProxy(std::string filename) : m_s(nullptr), m_filename(filename) { }
	virtual ~VirtualProxy()
	{
		delete m_s;
	}
	virtual void Operation()
	{
		printf("VirtualProxy %s\n", __FUNCTION__);
		if (m_s == nullptr)
		{
			m_s = new HeavySubject(m_filename);
		}
		m_s->Operation();
	}
private:
	HeavySubject *m_s;
	std::string m_filename;
};

int test0()
{
	{
		Subject *ptr = new RemoteProxy();
		ptr->Operation();
		delete ptr;
		printf("\n");
	}

	{
		Subject *ptr = new VirtualProxy("main.cpp");
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

