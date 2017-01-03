
#include <iostream>

class my_string{
private:
	char *str;
	static int counter;
public:
	my_string();
	my_string(const char*);
	my_string(const my_string &); // copy constructor
	~my_string();

	my_string & operator=(const my_string &); // operator=(const my_string &)
	my_string & operator=(const char *); // operator=(const char*), for my_string a = "abc", if not use this, have to call my_string(const char*), operator=(const my_string &), ~my_string()

	// conversion function, have no return
	explicit operator const char*(); // add explicit, have to explicit type case

	friend std::ostream & operator<<(std::ostream &, const my_string &in);
	static int get_counter(); // only can access static member
};
