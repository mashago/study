#include <iostream>
#include "counter.h"

int Counter::flag = 0;

Counter::Counter()
{
	this->num = 0;
}

Counter::Counter(int num)
{
	this->num = num;
}

// copy constructor, deep copy
Counter::Counter(const Counter &c)
{
	std::cout << "Counter:use Counter(const Counter &c)" << std::endl;
	this->num = c.num;
}

void Counter::set_num(int num)
{
	this->num = num;
}


Counter Counter::sum(const Counter &c)
{
	Counter tmp;
	tmp.num = num + c.num;
	return tmp;
}


Counter Counter::operator+(const Counter &c)
{
	Counter tmp;
	tmp.num = num + c.num;
	return tmp;
}


// friend
Counter operator*(const Counter &c, int m)
{
	Counter tmp;
	tmp.num = c.num * m;
	return tmp;
}

// operator=, ==> {Counter x; x = y;}, please over write
Counter & Counter::operator=(const Counter &c)
{
	std::cout << "Counter:use operator=" << std::endl;
	if (this == &c) {
		return *this;
	}

	this->num = c.num;
	return *this;
}

Counter::operator int() const
{
	return num;
}

void Counter::show()
{
	using std::cout;
	using std::endl;
	using std::ios_base;
	ios_base::fmtflags orig = cout.setf(ios_base::fixed, ios_base::floatfield);
	std::streamsize prec = cout.precision(2);
	cout << "show: num=" << num << " flag=" << flag << endl;
	cout.setf(orig, ios_base::floatfield);
	cout.precision(prec);
}

int Counter::get_flag()
{
	return flag;
}


