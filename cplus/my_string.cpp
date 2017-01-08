
#include <iostream>
#include <cstring>
#include "my_string.h"

// declare static member
int my_string::counter = 0;

my_string::my_string() : ptr(nullptr), _size(0)
{
	std::cout << "call my_string()" << std::endl;
	counter++;
}

my_string::my_string(const char *str) 
{
	std::cout << "call my_string(const char*)" << std::endl;
	_size = strlen(str);
	ptr = new char[_size+1];
	strcpy(ptr, str);
	ptr[_size] = '\0';
	counter++;
}

my_string::my_string(const my_string &str) 
{
	std::cout << "call copy constructor" << std::endl;
	_size = str._size;
	ptr = new char[_size+1];
	memcpy(ptr, str.ptr, _size);
	counter++;
}

my_string::my_string(my_string &&str) : ptr(str.ptr), _size(str._size)
{
	std::cout << "call move constructor" << std::endl;
	str._size = 0;
	str.ptr = nullptr;
	counter++;
}

my_string::~my_string() 
{
	counter--;
	std::cout << "counter=" << counter << std::endl;
	delete []ptr;
}

my_string & my_string::operator=(const char * str)
{
	std::cout << "call operator=(const char *)" << std::endl;
	delete [] ptr;
	_size = strlen(str);
	ptr = new char[_size+1];
	strcpy(ptr, str);
	return *this;
}

my_string & my_string::operator=(const my_string &str) &
{
	std::cout << "call copy assignment operator" << std::endl;
	char *newptr = new char[str._size+1];
	memcpy(newptr, str.ptr, str._size);
	delete [] ptr;
	ptr = newptr;
	_size = str._size;
	return *this;
}

my_string & my_string::operator=(my_string &&str) &
{
	std::cout << "call move assignment operator" << std::endl;
	if (this == &str) {
		return *this;
	}
	ptr = str.ptr;
	_size = str._size;

	str.ptr = nullptr;
	str._size = 0;
	return *this;
}

my_string::operator const char*() const
{
	if (ptr == nullptr)
	{
		return "";
	}
	return ptr;	
}

std::ostream & operator<<(std::ostream & os, const my_string & str)
{
	os << "ptr=" << (const char *)str;
	return os;
}

int my_string::get_counter()
{
	// only can access static member
	return counter;
}

my_string create_my_string(const char *ptr)
{
	return my_string(ptr);
}

int main(int argc, char *argv[])
{
	using std::cout;
	using std::endl;

	{
	my_string s1;
	cout << "s1." << s1 << endl;
	}
	cout << endl;

	{
	my_string s2 = "abc";
	cout << "s2." << s2 << endl;
	}
	cout << endl;

	{
	my_string s2 = "abc";
	my_string s3 = s2;
	cout << "s2." << s2 << endl;
	cout << "s3." << s3 << endl;
	}
	cout << endl;

	{
	my_string s2 = "abc";
	my_string s4 = "123";
	cout << "s4." << s4 << endl;
	s4 = s2;
	cout << "s2." << s2 << endl;
	cout << "s4." << s4 << endl;
	}
	cout << endl;

	{
	my_string s5;
	s5 = "aabb";
	cout << "s5." << s5 << endl;
	}
	cout << endl;

	{
	my_string s6("conv");
	const char * tmp = (const char *)s6; // function add explicit
	cout << "s6." << s6 << endl;
	cout << "tmp." << tmp << endl;
	}
	cout << endl;

	{
	my_string s7 = create_my_string("abcd");
	cout << "s7." << s7 << endl;
	}
	cout << endl;

	{
	my_string s8;
	s8 = create_my_string("abcd");
	cout << "s8." << s8 << endl;
	}
	cout << endl;

	{
	my_string str("abcd");
	my_string str2(std::move(str));
	cout << "str." << str << endl;
	cout << "str2." << str2 << endl;
	}
	cout << endl;


	cout << "my_string::counter = " << my_string::get_counter() << endl;

	return 0;
}


