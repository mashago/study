
#include <iostream>

using std::cout;
using std::endl;

class Buff
{
private:
	int n; // real data size
	char * ptr; // real data ptr
public:
	Buff();
	Buff(int size);
	Buff(int size, char ch);
	~Buff();

	Buff(const Buff &); // regular copy constructor
	Buff & operator=(const Buff &) &; // regular operator=, last '&' means only left value can call this function

	// rvalue reference mostly for tmp object as a function param
	// in move function, we can pass tmp object member to real object, no need to new another
	// like 'steal' tmp object data
	Buff(Buff &&); // move constructor
	Buff & operator=(Buff &&); // move operator=

	Buff operator+(const Buff &);

	void show() const;
	
};

Buff::Buff()
{
	n = 0;
	ptr = 0;
};

Buff::Buff(int size)
{
	n = size+1;
	ptr = new char[n];
	ptr[n-1] = 0;
};

Buff::Buff(int size, char ch)
{
	n = size+1;
	ptr = new char[n];
	for (int i=0; i<n-1; i++) {
		ptr[i] = ch;	
	}
	ptr[n-1] = 0;
};

Buff::~Buff()
{
	cout << "call ~Buff()" << endl;
	delete [] ptr;
}

// regular copy constructor
Buff::Buff(const Buff & b)
{
	cout << "call regular constructor" << endl;
	n = b.n;
	ptr = new char[n];
	for (int i=0; i<n; i++) {
		ptr[i] = b.ptr[i];	
	}
};

// regular operator=
Buff & Buff::operator=(const Buff & b) &
{
	cout << "call regular operator=()" << endl;
	if (this == &b) {
		return *this;
	}
	n = b.n;
	ptr = new char[n];
	for (int i=0; i<n; i++) {
		ptr[i] = b.ptr[i];	
	}
	return *this;
};

// move constructor
Buff::Buff(Buff && tmp)
{
	cout << "call move constructor" << endl;
	n = tmp.n;
	ptr = tmp.ptr;
	// clean tmp ptr, so destuctor function will not clean original ptr
	tmp.n = 0;
	tmp.ptr = 0;
};

// move operator=
Buff & Buff::operator=(Buff && tmp)
{
	cout << "call move operator=()" << endl;
	if (this == &tmp) {
		return *this;
	}
	n = tmp.n;
	ptr = tmp.ptr;
	// clean tmp ptr, so destuctor function will not clean original ptr
	tmp.n = 0;
	tmp.ptr = 0;
	return *this;
};

Buff Buff::operator+(const Buff &b)
{
	Buff tmp(n+b.n-1);
	int count = 0;
	for (int i=0; i<n-1; i++, count++) {
		tmp.ptr[count] = ptr[i];
	}
	for (int i=0; i<b.n-1; i++, count++) {
		tmp.ptr[count] = b.ptr[i];
	}
	return tmp;
}

void Buff::show() const
{
	if (ptr==0) {
		cout << "ptr empty" << endl;
		return;
	}
	cout << "ptr=" << ptr << endl; 
};


class Foo {
public:
	Foo show() &&;
	Foo show() &;
	static Foo retVal();
	static Foo &retFoo();
};

Foo Foo::show() &&
{
	std::cout << "show() rvalue" << std::endl;
	return *this;
}

Foo Foo::show() &
{
	std::cout << "show() lvalue" << std::endl;
	Foo ret;
	return ret;
}

Foo Foo::retVal()
{
	Foo ret;
	return ret;
}

Foo & Foo::retFoo()
{
	Foo *ret = new Foo();
	return *ret;
}



int main(int argc, char * argv[])
{

	cout << "---------- regular ---------" << endl;
	
	{
	Buff a(5, 'a');
	a.show();

	Buff n1(a);
	n1.show();

	Buff n2;
	n2 = n1;
	n2.show();
	}

	cout << "---------- move ---------" << endl;

	{
	Buff a(5, 'a');
	a.show();

	Buff b(5, 'b');
	b.show();

	Buff s1(a+b); // default will call move copy constructor, compiler may do it better
	s1.show();

	Buff s2;
	s2 = a + b; // call move operator=()
	s2.show();

	}

	cout << "---------- std::move() ---------" << endl;

	{
	Buff a(5, 'a');
	a.show();

	Buff m;
	m = std::move(a); // forced move assignment. if no move operator=() in Buff, will call regular operator=()
	m.show();
	a.show(); // a will be empty after move operator=()
	}


	cout << "---------- Foo ---------" << endl;
	{
	Foo::retVal().show();
	Foo::retFoo().show();
	}
	
	return 0;
}
