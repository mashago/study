extern "C"
{
#include <stdio.h>
#include <time.h>
}
#include <iostream>
#include <string>
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "boost/date_time.hpp"
#include "boost/format.hpp"

void bind_func(int a) {
	printf("a=%d\n", a);
}

int bind_func2(int a) {
	return a * 2;
}

int bind_func3() {
	return 1;
}

int bind_func4(int a, int b) {
	return a + b;
}

class MyClass {
public:
	MyClass(int x, int y) : a_(x), b_(y) {}
	int sum() {
		return a_ + b_;
	}
	int sum2(int x, int y) {
		return x + y;
	}
private:
	int a_;
	int b_;
};

int test_bind() {

	printf("---------- bind and function ----------\n");
	// http://www.boost.org/doc/libs/1_61_0/libs/bind/doc/html/bind.html

	int ret = 0;

	printf("bind normal function\n");

	// call bind function, function param is set in bind
	boost::bind(bind_func, 1)(); // 1

	// function param pass when call
	boost::bind(bind_func, _1)(2); // 2

	// bind return a function object
	auto func_a = boost::bind(bind_func, 3);
	func_a(); // 3
	auto func_b = boost::bind(bind_func, _1);
	func_b(4); // 4

	boost::function<void(int)> func_c = boost::bind(bind_func, _1);
	func_c(5);

	boost::function<int(int)> func_d = boost::bind(bind_func2, _1);
	ret = func_d(5);
	printf("1 ret = %d\n", ret);

	boost::function<int()> func_e = boost::bind(bind_func3);
	ret = func_e();
	printf("2 ret = %d\n", ret);

	boost::function<int(int, int)> func_f = boost::bind(bind_func4, _1, _2);
	ret = func_f(1, 2);
	printf("3 ret = %d\n", ret);

	// define parts of param in bind
	boost::function<int(int)> func_g = boost::bind(bind_func4, 3, _1);
	ret = func_g(2);
	printf("4 ret = %d\n", ret);


	printf("\nbind member function\n");

	// binding member
	MyClass c1(10, 20);

	// bind param 1 is class function address
	// param 2 is a class member
	auto func_c_a = boost::bind(&MyClass::sum, c1);
	ret = func_c_a(); // 30
	printf("1 ret = %d\n", ret);

	// bind set class member as a input param
	auto func_c_b = boost::bind(&MyClass::sum, _1);
	ret = func_c_b(c1);
	printf("2 ret = %d\n", ret);

	boost::function<int()> func_c_c = boost::bind(&MyClass::sum, c1); // (internal copy of c1).sum()
	ret = func_c_c();
	printf("3 ret = %d\n", ret);

	boost::function<int()> func_c_c2 = boost::bind(&MyClass::sum, boost::ref(c1)); // c1.sum()
	ret = func_c_c2();
	printf("3.1 ret = %d\n", ret);

	// define parts of param in bind
	boost::function<int(int)> func_c_d = boost::bind(&MyClass::sum2, c1, 1, _1);
	ret = func_c_d(2);
	printf("4 ret = %d\n", ret);

	// bind a class member function, not given class member.
	// so function param have to add a class member pointer or reference
	boost::function<int(MyClass&, int, int)> func_c_e = boost::bind(&MyClass::sum2, _1, _2, _3); // function recv a pointer
	ret = func_c_e(c1, 2, 3);
	printf("5 ret = %d\n", ret);

	boost::function<int(MyClass*, int, int)> func_c_f = boost::bind(&MyClass::sum2, _1, _2, _3); // function recv a reference
	ret = func_c_f(&c1, 2, 3);
	printf("6 ret = %d\n", ret);

	boost::function<int(MyClass&)> func_c_g = boost::bind(&MyClass::sum, _1);
	ret = func_c_g(c1);
	printf("7 ret = %d\n", ret);

	return 0;
}

int test_date_time() {

	printf("\n---------- date_time ----------\n");

	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	std::cout << "now = " << now << std::endl;
	std::cout << "now.date() = " << now.date() << std::endl;
	std::cout << "now.time_of_day() = " << now.time_of_day() << std::endl;
	std::cout << "now - now.time_of_day() = " << now - now.time_of_day() << std::endl;
	std::string now_string = boost::posix_time::to_simple_string(now);
	std::cout << "now_string = " << now_string << std::endl;

	printf("\n");

	// ptime to time_t
	// time_t now_sec = boost::posix_time::to_time_t(now);
	// std::cout << "now_sec = " << now_sec << std::endl;

	time_t now_sec2 = time(NULL); // XXX no same with now_sec???
	std::cout << "now_sec2 = " << now_sec2 << std::endl;

	// string time to ptime
	boost::posix_time::ptime t1 = boost::posix_time::time_from_string("2008-09-01 08:10:30");
	std::cout << "t1 = " << t1 << std::endl;

	// iso string time to ptime
	boost::posix_time::ptime t2 = boost::posix_time::from_iso_string("20080902T182040");
	std::cout << "t2 = " << t2 << std::endl;

	return 0;
}

int test_format() {

	printf("\n---------- format ----------\n");

	std::string buffer("");

	// init format
	boost::format fmt1("%d %s world");
	fmt1 % 12;
	fmt1 % "hello";
	buffer = fmt1.str();
	std::cout << "1 buffer = " << buffer << std::endl;

	// only define order, not define type
	fmt1 = boost::format("%1% %2%");
	fmt1 % 12;
	fmt1 % "hello";
	buffer = fmt1.str();
	std::cout << "2 buffer = " << buffer << std::endl;

	// reuse param
	fmt1 = boost::format("%1% %2% %1% %2%");
	fmt1 % 12;
	fmt1 % "hello";
	buffer = fmt1.str();
	std::cout << "3 buffer = " << buffer << std::endl;

	// reuse format
	fmt1 % 34;
	fmt1 % "world";
	buffer = fmt1.str();
	std::cout << "4 buffer = " << buffer << std::endl;

	// order canbe exchange
	fmt1 = boost::format("%2% %1%");
	fmt1 % 12;
	fmt1 % "hello";
	buffer = fmt1.str();
	std::cout << "5 buffer = " << buffer << std::endl;

	// %%
	fmt1 = boost::format("%.2lf%%");
	fmt1 % 12.34;
	buffer = fmt1.str(); // 12.34%
	std::cout << "6 buffer = " << buffer << std::endl;

	// format convert to std::string
	buffer = boost::str(boost::format("%s %s") % "hello" % "format");
	std::cout << "7 buffer = " << buffer << std::endl;

	fmt1 = boost::format("%1d %2d");
	fmt1 % 12;
	try {
		// too few args, throw exception
		buffer = fmt1.str();
		std::cout << "8 buffer = " << buffer << std::endl;
	} catch (std::exception &err) {
		std::cout << err.what() << std::endl;
	}

	fmt1 = boost::format("%1% %2%");
	try {
		fmt1 % 12;
		fmt1 % 34;
		// too many args, throw exception
		fmt1 % 56;
	} catch (std::exception &err) {
		std::cout << err.what() << std::endl;
	}

	fmt1 = boost::format("_%1d_%2d_");
	// pre-set exceptions bit
	fmt1.exceptions(boost::io::all_error_bits ^ (boost::io::too_few_args_bit | boost::io::too_many_args_bit));
	fmt1 % 12;
	// too few args, but no throw exception
	buffer = fmt1.str();
	std::cout << "9 buffer = " << buffer << std::endl;

	fmt1 = boost::format("_%1d_%2d_");
	// pre-set exceptions bit
	fmt1.exceptions(boost::io::all_error_bits ^ (boost::io::too_few_args_bit | boost::io::too_many_args_bit));
	fmt1 % 12;
	fmt1 % 34;
	// too many args, but no throw exception
	fmt1 % 56;
	buffer = fmt1.str();
	std::cout << "10 buffer = " << buffer << std::endl;

	try {
		// bad format string
		fmt1 = boost::format("%2");
	} catch (std::exception &err) {
		std::cout << err.what() << std::endl;
	}

	return 0;
}

int main(int argc, char **argv) {
	printf("hello world\n");
	
	test_bind();
	test_date_time();
	test_format();

	std::cin.get();
	return 0;
}
