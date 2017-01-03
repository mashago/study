
#include <iostream>

using std::cout;
using std::endl;

class Buff
{
private:
	int n; // real data size
	char * ptr; // real data ptr
public:
	Buff() : n(0), ptr(NULL)
	{
		cout << "call regular constructor 1" << endl;
	};

	Buff(int size)
	{
		cout << "call regular constructor 2" << endl;
		n = size+1;
		ptr = new char[n];
		ptr[n-1] = 0;
	};

	Buff(int size, char ch)
	{
		cout << "call regular constructor 3" << endl;
		n = size+1;
		ptr = new char[n];
		for (int i=0; i<n-1; i++) {
			ptr[i] = ch;	
		}
		ptr[n-1] = 0;
	};

	~Buff()
	{
		cout << "call ~Buff()" << endl;
		delete [] ptr;
	}

	// regular copy constructor
	Buff(const Buff &b)
	{
		cout << "call regular copy constructor" << endl;
		n = b.n;
		ptr = new char[n];
		for (int i=0; i<n; i++) {
			ptr[i] = b.ptr[i];	
		}
	};

	// regular operator=, last '&' means only left value can call this function
	Buff & operator=(const Buff &b) &
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

	// rvalue reference mostly for tmp object as a function param
	// in move function, we can pass tmp object member to real object, no need to new another
	// like 'steal' tmp object data
	// move constructor
	Buff(Buff &&tmp) : n(tmp.n), ptr(tmp.ptr)
	{
		cout << "call move constructor" << endl;
		// 1. steal tmp data ptr
		// 2. clean tmp ptr, so destuctor function will not clean original ptr
		tmp.n = 0;
		tmp.ptr = 0;
	};

	// move operator=
	Buff & operator=(Buff && tmp)
	{
		cout << "call move operator=()" << endl;
		if (this == &tmp) {
			return *this;
		}
		// 1. steal tmp data ptr
		n = tmp.n;
		ptr = tmp.ptr;

		// 2. clean tmp ptr, so destuctor function will not clean original ptr
		tmp.n = 0;
		tmp.ptr = 0;
		return *this;
	};

	Buff operator+(const Buff &b)
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

	void show() const
	{
		if (ptr==0) {
			cout << "ptr empty" << endl;
			return;
		}
		printf("this:%p ptr:%p %s\n", this, ptr, ptr);
	};
};


Buff getBuff(int size, char ch)
{
	Buff b(size, ch);
	b.show();
	return b;
}

template <class T>
void swap(T &a, T &b)
{
	T tmp(std::move(a));
	a = std::move(b);
	b = std::move(tmp);
}

/////////////////////////////////////////

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

	{
		printf("\n---------- regular ---------\n");
		Buff a(5, 'a');
		a.show();

		Buff n1(a);
		n1.show();

		Buff n2;
		n2 = n1;
		n2.show();
	}

	{
		printf("\n---------- move ---------\n");
		Buff s1(getBuff(5, 'c'));
		s1.show();

		printf("\n");

		Buff s2;
		s2 = getBuff(5, 'd');
		s2.show();
	}

	{
		printf("\n---------- std::move ---------\n");
		Buff a(5, 'a');
		a.show();

		Buff m;
		m = std::move(a); // forced move assignment. if no move operator=() in Buff, will call regular operator=()
		m.show();
		a.show(); // a will be empty after move operator=()
	}


	{
		printf("\n---------- rvalue, const lvalue ---------\n");
		Buff &&a = getBuff(10, 'a');
		a.show();

		const Buff &b = getBuff(10, 'b');
		b.show();
	}

	{
		printf("\n---------- swap ---------\n");
		Buff a(10, 'a');
		Buff b = Buff(10, 'b');
		a.show();
		b.show();

		printf("after swap\n");

		swap(a, b);
		a.show();
		b.show();
	}


	{
		printf("\n---------- foo ---------\n");
		Foo::retVal().show();
		Foo::retFoo().show();
	}
	
	return 0;
}
