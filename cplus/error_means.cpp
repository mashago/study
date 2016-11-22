
#include <iostream>
#include <exception>
#include <stdexcept>

class bad_mean1 : public std::exception
{
public:
	// std::exception::what = 
	// virtual const char * what() const _NOEXCEPT;
#ifdef CPP11
	const char * what() const noexcept { return "bad_mean 1"; };
#endif
};

class bad_mean2 : public std::exception
{
public:
#ifdef CPP11
	const char * what() const noexcept { return "bad_mean 2"; };
#endif
};

class bad_mean3 : public bad_mean2
{
public:
#ifdef CPP11
	const char * what() const noexcept { return "bad_mean 3"; };
#endif
};

void bad_method(int type)
{
	if (type == 1) {
		throw bad_mean1();
	} else if (type == 2) {
		throw bad_mean2();
	} else if (type == 3) {
		throw bad_mean3();
	} else if (type == 4) {
		throw std::runtime_error("this is a runtime_error");
	} else {
		throw "other error";
	}
}

void bad_caller(int type)
{
	using std::cout;
	using std::endl;
	try 
	{
		bad_method(type);
	}
	catch (bad_mean1 & e) 
	{
		cout << e.what() << endl;
	} 
	catch (bad_mean3 & e) 
	{
		cout << e.what() << endl;
	} 
	catch (bad_mean2 & e) 
	{ 	
		// bad_mean2 & will catch bad_mean3
		cout << e.what() << endl;
	} 
	catch (std::runtime_error e) 
	{
		cout << e.what() << endl;
	} 
	catch (const char * e) 
	{
		cout << e << endl;
	}

}

int main(int argc, char * argv[])
{

	bad_caller(1);
	bad_caller(2);
	bad_caller(3);
	bad_caller(4);
	bad_caller(5);
	return 0;
}
