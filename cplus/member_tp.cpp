#include <iostream>
#include <string>

using std::cout;
using std::endl;

template <typename T> class Beta
{
private:
	template <typename V> class Holder;  // nested template class
	Holder<T> h; // template class member
	Holder<int> n;
	static int a;
public:
	Beta(T a, int v) : h(a), n(v) {};
	~Beta() { cout << "~Beta()" << endl; };
	void show() { h.show(); n.show(); };
};

template <typename T>
int Beta<T>::a = 10;

template <typename T> template <typename V> class Beta<T>::Holder
{
private:
	V value;
public:
	Holder(V a) : value(a) {};
	~Holder() {};
	void show() { cout << "holder.value=" << value << endl; };
};

class Alpha
{
private:
	int value;
public:
	Alpha(int a) : value(a) {};
	~Alpha() {};
	template <typename X> void show(X); // template function in a non-template class
};

template <typename X> void Alpha::show(X x) // when function be called, X will be define
{
	cout << "alpha.value=" << value << " x=" << x << endl;
};

int main (int argc, char *argv[])
{
	Beta<const char*> b1("hello", 10);
	b1.show();

	Beta<std::string> b2("world", 20);
	b2.show();

	Beta<int> b3(555, 30);
	b3.show();

	Alpha a1(666);
	a1.show(777);
	a1.show("masha");


	
	return 0;
}
