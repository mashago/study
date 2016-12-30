
#include <stdio.h>
#include <unistd.h>
#include <functional>

#include "TaskThread.h"

static void normal_func()
{
	printf("normal_func\n");
}

static void normal_func2()
{
	printf("normal_func2\n");
}

static void normal_func3(int n)
{
	printf("normal_func3 n=%d\n", n);
}

class MyClass
{
public:
	MyClass(int x = 1) : n(x)
	{
	}

	~MyClass()
	{
	}

	void print()
	{
		printf("MyClass: n=%d\n", n);
	}

	void print2(int offset)
	{
		printf("MyClass: n+offset=%d\n", n+offset);
	}
	
private:
	int n;
};

void testcase(TaskThread &tt, int counter)
{
	switch (counter)
	{
		case 0:
			{
				// post a global function pointer
				tt.Post(&normal_func);
				break;
			}
		case 1:
			{
				// post a function
				tt.Post(std::bind(normal_func2));
				break;
			}
		case 2:
			{
				// post a function with a param
				tt.Post(std::bind(normal_func3, counter));
				break;
			}
		case 3:
			{
				// post a lambda
				tt.Post([](){
							printf("post a lambda\n");			
						}
					);
				break;
			}
		case 4:
			{
				// post a class member function
				tt.Post(std::bind(&MyClass::print, std::make_shared<MyClass>(5)));
				break;
			}
		case 5:
			{
				// post a class member function
				tt.Post(std::bind(&MyClass::print2, std::make_shared<MyClass>(10), 20));
				break;
			}
	}
}

int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);

	TaskThread tt;

	tt.Start();

	int counter = 0;
	while (true)
	{
		sleep(1);
		printf("counter=%d\n", counter);
		testcase(tt, counter);
		counter++;
		if (counter > 5)
		{
			break;
		}
	}

	tt.Stop();

	return 0;
}

