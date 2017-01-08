
#include <iostream>

class my_string{
public:
	my_string();
	my_string(const char*);
	my_string(const my_string &); // copy constructor
	my_string(my_string &&); // move constructor

	~my_string(); // destructor

	my_string & operator=(const char *); //
	my_string & operator=(const my_string &) &; // copy-assignment
	my_string & operator=(my_string &&) &; // move-assignment

	// conversion function, have no return
	explicit operator const char*() const; // add explicit, have to explicit type case
	unsigned int size() { return _size; }

	static int get_counter(); // only can access static member
private:
	char *ptr;
	unsigned int _size;
	static int counter;
};

// define non-member function
std::ostream & operator<<(std::ostream &, const my_string &in);

