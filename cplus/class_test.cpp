#include <stdio.h>
#include "class_test.h"

// const int Simple::b = 40; // error: non-static data member defined out-of-line
int Simple::c = 30; // must init non-const static member
const int Simple::d = 40;
const double Simple::f = 60.1;
const std::string Simple::str1 = "xxx1";

int Middle::a2 = 10;

void Simple::print_hello() const
{
	printf("Simple::print_hello\n");
	// h = 10; // error: cannot assign to non-static data member within const member function
}

void Simple::print_static()
{
	printf("Simple::print_static: c=%d\n", c);
}

/*
void Simple::func_delete()
{
	printf("Simple::func_delete\n");
}
*/

void Middle::print_hello()
{
	printf("Middle::print_hello\n");
}

inline void Middle::print_static()
{
	Simple::print_static();
	printf("Middle::print_static: a2=%d\n", a2);
}

CarBase::~CarBase()
{
}

