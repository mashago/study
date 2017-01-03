#ifndef _CLASS_TEST_
#define _CLASS_TEST_

#include "string"

class Simple
{
public:
	int a = 10; // work since c++11

	const int b = 20; // work since c++11

	// static int c = 30; // error: non-const static data member must be initialized out of line
	static int c; // static member will uniqle in all class object and extend class object

	// static const int d = 40; // this is ok
	static const int d;

	static const long e = 50; // init static integer memeber in class is ok

	// static const double f = 60.1; // error: in-class initializer for static data member of type 'const double'
	static const double f;

	const std::string g = "hello"; // c++11 can init non-static const member in class
	// static const std::string str1 = "xxx"; // cannot init static member of non-integer in class 
	static const std::string str1;

	Simple() {};
	Simple(int val) : a(val) {};
	explicit Simple(std::string val) : g(val) {};

	// Simple(const Simple &s) = delete;
	Simple & operator=(const Simple &) = delete;

	void print_hello() const; 
	virtual void print_static();

	void func_delete() = delete;
protected:
	int h;	
};

class Middle : public Simple
{
public:
	int a1;
	static int a2;
	Middle(int v = 0) : a1(v) {};
	Middle(int v1, int v2) : Simple(v1), a1(v2) {};
	void print_hello();
	virtual void print_static();
};


class CarBase
{
public:
	CarBase() {};
	virtual ~CarBase() = 0;
	// ~CarBase() {};
};

#endif
