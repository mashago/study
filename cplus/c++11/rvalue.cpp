
#include <iostream>
#include <list>

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

////////////////////////////////////////

int getInt() { return 0; }

void RunCode(int &m) { cout << "lvalue ref" << endl; }
void RunCode(int &&m) { cout << "rvalue ref" << endl; }
void RunCode(const int &m) { cout << "const lvalue ref" << endl; }
void RunCode(const int &&m) { cout << "const rvalue ref" << endl; }

template <typename T>
void PerfectForward(T &&t)
{
	// RunCode(std::forward<T>(t));
	RunCode(static_cast<T &&>(t));
}

////////////////////////////////////////


void getBuffListRef(std::list<Buff> &ret)
{
	ret.push_back(Buff());
	ret.push_back(Buff(10, 'a'));
	printf("end getBuffListRef ---\n");
}

// use NRVO
std::list<Buff> getBuffList()
{
	std::list<Buff> tmp;
	tmp.push_back(Buff());
	tmp.push_back(Buff(10, 'a'));
	printf("end getBuffList ---\n");
	return tmp;
}

std::list<Buff> getBuffListMove()
{
	std::list<Buff> tmp;
	tmp.push_back(Buff());
	tmp.push_back(Buff(10, 'a'));
	printf("end getBuffList ---\n");
	// LLVM warning: moving a local object in a return statement prevents copy elision
	return std::move(tmp);
}
////////////////////////////////////////

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

	{
		printf("\n---------- perfect forward ---------\n");
		int a = 1;
		int b = 2;
		const int c = 3;
		const int d = 4;

		PerfectForward(a);
		PerfectForward(std::move(b));
		PerfectForward(c);
		PerfectForward(std::move(d));
		PerfectForward(getInt());
	}

	{
		printf("\n---------- move vec ---------\n");
		{
		printf("ref param\n");
		std::list<Buff> v0;
		getBuffListRef(v0);
		printf("size()=%zu\n", v0.size());
		}
		printf("\n");

		{
		printf("without move1\n");
		std::list<Buff> v1 = getBuffList();
		printf("size()=%zu\n", v1.size());
		}
		printf("\n");

		{
		printf("without move2\n");
		std::list<Buff> v1;
		v1 = getBuffList();
		printf("size()=%zu\n", v1.size());
		}
		printf("\n");

		{
		printf("without move3\n");
		std::list<Buff> &&v1 = getBuffList();
		printf("size()=%zu\n", v1.size());
		}
		printf("\n");

		{
		printf("with move1\n");
		// LLVM warning: moving a temporary object prevents copy elision
		std::list<Buff> v2 = std::move(getBuffList());
		printf("size()=%zu\n", v2.size());
		}
		printf("\n");

		{
		printf("with move2\n");
		std::list<Buff> v2;
		// LLVM warning: moving a temporary object prevents copy elision
		v2= std::move(getBuffList());
		printf("size()=%zu\n", v2.size());
		}
		printf("\n");

		/*
		{
		printf("with move3\n");
		std::list<Buff> &&v2 = std::move(getBuffList()); // error
		printf("size()=%zu\n", v2.size());
		}
		printf("\n");
		*/

		{
		printf("get move\n");
		std::list<Buff> &&v3 = getBuffListMove();
		printf("size()=%zu\n", v3.size());
		}
		printf("\n");
	}
	
	return 0;
}
