
#include <iostream>
#include <string>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::ostringstream;

void show_list() {};

// if only 1 param, will call this function
template<typename T>
void show_list(const T &t)
{
	cout << t << endl;
}

// variadic template
template<typename T, typename ... Args>
void show_list(T t, const Args &... args)
{
	cout << "sizeof...(Args) = " << sizeof...(Args) << endl;
	cout << "sizeof...(args) = " << sizeof...(args) << endl;
	cout << t << ",";
	show_list(args...);
}

template<typename T>
string debug_rep(const T &t)
{
	ostringstream ret;
	ret << t;
	cout << ret.str() << "," << endl;
	return ret.str();
}

template<typename ... Args>
void show_list2(const Args &... args)
{
	show_list(debug_rep(args)...); // every args call by debug_rep()
}

int main(int argc, char *argv[])
{
	cout << "list 1" << endl;
	show_list(0, 2, 2.4, "ssd", 22, "uuu", 5.789);
	cout << endl;

	cout << "list 2" << endl;
	show_list(1, 2, 3);
	cout << endl;

	cout << "list 3" << endl;
	show_list2(1, 2, 3);
	cout << endl;
	return 0;
}
