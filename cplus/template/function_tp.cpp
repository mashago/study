
#include <iostream>

using std::cout;
using std::endl;

// declare template function
template <typename T>
void func_tp(T t);

// define template function, implicit instantiation
template <typename T>
void func_tp(T t)
{
	cout << "t=" << t << endl;	
}

// declare explicit specialization
template <>
void func_tp(const char* t);
// define explicit specialization
template <>
void func_tp(const char* t)
{
	cout << "const char* t=" << t << endl;
}

// explicit specialization instantiation
template
void func_tp<const char>(const char t);

int main(int argc, char *argv[])
{
	int a = 111;
	func_tp(a);

	const char * b = "hello";
	func_tp(b);

	char c = 'h';
	func_tp(c);

	return 0;
}
