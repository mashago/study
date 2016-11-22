
#include <iostream>
#include <cstring>
#include "my_string.h"

int my_string::counter = 0;

my_string::my_string() 
{
	std::cout << "call my_string()" << std::endl;
	counter++;
	str = new char[1];
	str[0] = '\0';
}

my_string::my_string(const char *in) 
{
	std::cout << "call my_string(const char*)" << std::endl;
	counter++;
	int len = strlen(in);
	str = new char[len+1];
	strcpy(str, in);
}

my_string::my_string(const my_string &in) 
{
	std::cout << "call copy constructor" << std::endl;
	counter++;
	int len = strlen(in.str);
	str = new char[len+1];
	strcpy(str, in.str);
}

my_string::~my_string() 
{
	counter--;
	std::cout << "counter=" << counter << std::endl;
	delete []str;
}

my_string & my_string::operator=(const my_string & in) 
{
	std::cout << "call operator=(const my_string &)" << std::endl;
	if (this == &in) {
		return *this;
	}
	delete [] str;
	int len = strlen(in.str);
	str = new char[len+1];
	strcpy(str, in.str);
	return *this;
}

my_string & my_string::operator=(const char * in)
{
	std::cout << "call operator=(const char *)" << std::endl;
	delete [] str;
	int len = strlen(in);
	str = new char[len+1];
	strcpy(str, in);
	return *this;
}

my_string::operator const char*()
{
	return str;	
}

std::ostream & operator<<(std::ostream & os, const my_string & in)
{
	os << "str=" << in.str << std::endl;
	return os;
}

int my_string::get_counter()
{
	// only can access static member
	return counter;
}

int main(int argc, char *argv[])
{
	using std::cout;
	using std::endl;

	my_string s1;
	cout << "s1:" << s1;
	cout << "-----------------------" << endl;

	my_string s2 = "abc";
	cout << "s2:" << s2;
	cout << "-----------------------" << endl;

	my_string s3 = s2;
	cout << "s2:" << s2;
	cout << "s3:" << s3;
	cout << "-----------------------" << endl;

	my_string s4 = "123";
	cout << "s4:" << s4;
	s4 = s2;
	cout << "s2:" << s2;
	cout << "s4:" << s4;
	cout << "-----------------------" << endl;

	my_string s5;
	s5 = "aabb";
	cout << "s5:" << s5;
	cout << "-----------------------" << endl;

	my_string s6("conv");
	const char * tmp = (const char *)s6; // function add explicit
	cout << "s6:" << s6;
	cout << "tmp:" << tmp << endl;

	cout << "-----------------------" << endl;

	cout << "my_string::counter = " << my_string::get_counter() << endl;

	return 0;
}


