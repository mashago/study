
#include <iostream>
#include <cstring>
#include "stack_tp.h"

using std::cout;
using std::endl;

// size is const, must init in init list
template <typename T> Stack<T>::Stack(int n) : size(n), top(0)
{
	items = new T[size];
}

template <typename T> Stack<T>::~Stack()
{
	delete [] items;
}

template <typename T> int Stack<T>::push(const T &item)
{
	if (top < size) {
		items[top++] = item;
		return 0;
	} else {
		return -2;
	}
}

template <typename T> int Stack<T>::pop(T &item)
{
	if (top > 0) {
		item = items[--top];
		return 0;
	} else {
		return -2;
	}
}

int main(int argc, char * argv[])
{

	using std::string;
	Stack<string> s1(10);
	string a1 = "hello";
	string a2 = "haha";
	s1.push(a1);
	s1.push(a2);

	string a3;
	s1.pop(a3);
	cout << "a3=" << a3 << endl;
	s1.pop(a3);
	cout << "a3=" << a3 << endl;
	cout << "-------------------------------" << endl;

	Stack<char *> s2;
	char * c1 = new char[10];
	strcpy(c1, "masha");
	printf("c1=%p\n", c1);
	char * c2 = new char[10];
	strcpy(c2, "kelton");
	printf("c2=%p\n", c2);
	s2.push(c1);
	s2.push(c2);

	char * c3;
	s2.pop(c3);
	cout << "c3=" << c3 << endl;
	printf("c3=%p\n", c3);
	delete [] c3;
	s2.pop(c3);
	cout << "c3=" << c3 << endl;
	printf("c3=%p\n", c3);
	delete [] c3;
	

	return 0;
}
