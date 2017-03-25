
#include <stdio.h>
#include <vector>


template <typename T>
class X;

template <typename T>
class X
{
	// T is dependent name
	// vector<int>::iterator is qualified name
	
	int i;
	std::vector<int> ivec;
	std::vector<int>::iterator iter;

	T ti;
	std::vector<T> tvec;
	typename std::vector<T>::iterator titer; // qualified dependent name
};

template <typename T>
class Y
{
	// T::iterator is a qualified dependent name
	// must use "typename" at front
	// T::iterator iter; // error
	typename T::iterator iter;
	typedef typename T::iterator iterator;
};

template <typename T>
struct S
{
	typedef T value_type;
};

int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);

	typename S<int>::value_type n = 10;
	printf("n=%d\n", n);


	return 0;
}
