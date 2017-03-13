
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// an OO way callback

class Command
{
public:
	virtual ~Command() {}
	virtual void Execute() = 0;
};

class Button
{
public:
	Button(Command *cmd) : m_cmd(cmd) {}
	~Button()
	{
		delete m_cmd;
	}
	void Click()
	{
		if (m_cmd)
		{
			m_cmd->Execute();
		}
	}
private:
	Command *m_cmd;
};

class Receiver
{
public:
	void Action()
	{
		printf("Receiver::%s\n", __FUNCTION__);
	}
};

class ConcreteCommand : public Command
{
public:
	ConcreteCommand(Receiver *recv) : Command(), m_recv(recv) {}
	virtual ~ConcreteCommand() {}
	virtual void Execute() override
	{
		m_recv->Action();
	}
private:
	Receiver *m_recv;
};

template<typename T>
class SimpleCommand : public Command
{
public:
	typedef void (T::*Action)();
	SimpleCommand(T *r, Action a) : Command(), m_recv(r), m_action(a) {}
	virtual ~SimpleCommand() {}
	virtual void Execute() override
	{
		(m_recv->*m_action)();
	}
private:
	Receiver *m_recv;
	Action m_action;
};

int test0()
{
	{
		Receiver receiver;
		Button btn(new ConcreteCommand(&receiver));
		btn.Click();
		printf("\n");
	}

	{
		Receiver receiver;
		Button btn(new SimpleCommand<Receiver>(&receiver, &Receiver::Action));
		btn.Click();
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

