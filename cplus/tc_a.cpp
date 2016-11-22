#include <iostream>
#include "tc_a.h"

using namespace std;

TC_A::TC_A()
{
	this->id = 0;
	this->name = "no_name";
}

TC_A::TC_A(int id, string name)
{
	this->id = id;
	this->name = name;
}

TC_A::~TC_A()
{
	printf("~TC_A:destroy %s\n", this->name.c_str());
}

int TC_A::show()
{
	cout << "show:id=" << this->id << " name=" << this->name << endl;
	return 0;
}

int TC_A::useless()
{
	return 0;
}
