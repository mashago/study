
#include <iostream>

using std::cout;
using std::endl;

// declare template function
template <typename T> void f_tp(T t);
// define template function, implicit instantiation
template <typename T> void f_tp(T t)
{
	cout << "t=" << t << endl;	
}

template <> void f_tp(const char* t);
// explicit specialization
template <> void f_tp(const char* t)
{
	cout << "const char* t=" << t << endl;
}

// explicit specialization instantiation
template void f_tp<const char>(const char t);

int main(int argc, char *argv[])
{
	int a = 111;
	f_tp(a);

	const char * b = "hello";
	f_tp(b);

	char c = 'h';
	f_tp(c);

	return 0;
}
