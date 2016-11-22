#include <unistd.h>
#include <iostream>
#include <string>

using namespace std;

void add(long *v)
{
	(*v)++;
	cout << "v=" << *v << endl;
	sleep(5);	
}

void bug()
{
	int *p = NULL;
	// printf("%d", *p); 
	*p = 100; // will core dump in this line
}

int main(int argc, char **argv)
{
	cout << "hello gdb" << endl;

	bug();

	long value = 0;
	while (true)
	{
		add(&value);
	}
	return 0;
}
