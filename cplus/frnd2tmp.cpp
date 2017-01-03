
#include <iostream>

using std::cout;
using std::endl;

/////////////////////////////////////////////////
// 1.friend non-template function
template <typename T>
class HasFriend
{
private:
	T item;
public:
	HasFriend(const T &i) : item(i) {};
	~HasFriend(){};
	friend void show(const HasFriend<T> &); // non-template friend function
};

// non-template friend function, have to explicit specialization for all type you need
void show(const HasFriend<int> &f) 
{
	cout << "f.item=" << f.item << endl;
}

void show(const HasFriend<char> &f) 
{
	cout << "f.item=" << f.item << endl;
}

/////////////////////////////////////////////////

// 2.friend template function, same type with class, declare outside class
// must declare template function first
template <typename T>
void showT(const T &);

template <typename T>
class HasFriendT
{
private:
	T item;
public:
	HasFriendT(const T &i) : item(i) {};
	~HasFriendT(){};
	friend void showT<>(const HasFriendT<T> &); // template function, explicit specialization
};

// define template function
template <typename T>
void showT(const T &f) 
{
	cout << "f.item=" << f.item << endl;
}

/////////////////////////////////////////////////

// 3.friend template function, no need same type with class, declare inside class
template <typename T>
class ManyFriend
{
private:
	T item;
public:
	ManyFriend(const T &i) : item(i) {};
	~ManyFriend(){};
	template <typename X, typename Y>
	friend void showXY(const X &, const Y &);// template function, declare inside
};

template <typename X, typename Y>
void showXY(const X &x, const Y &y) 
{
	cout << "x.item=" << x.item << " y.item=" << y.item << endl;
}
/////////////////////////////////////////////////

int main(int argc, char *argv[])
{
	HasFriend<int> f1(50);
	show(f1);
	HasFriend<char> f2('h');
	show(f2);

	cout << "-----------------" << endl;

	HasFriendT<int> f3(100);
	showT(f3);

	HasFriendT<char> f4('j');
	showT(f4);

	cout << "-----------------" << endl;

	ManyFriend<int> f5(150);
	ManyFriend<char> f6('s');
	showXY(f5, f6);


	return 0;
}
