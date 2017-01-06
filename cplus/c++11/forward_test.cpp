
#include <stdio.h>
#include <utility>

struct X
{
	int i = 10;
};

void ForwardFunc(int &m) { printf("lvalue\n"); }
void ForwardFunc(int &&m) { printf("rvalue\n"); }
void ForwardFunc(const int &m) { printf("const lvalue\n"); }
void ForwardFunc(const int &&m) { printf("const rvalue\n"); }

template <typename T>
void PerfectForward(T &&t) { ForwardFunc(std::forward<T>(t)); }

int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);

	{
	X a;
	X &&m1 = std::move(a);
	printf("i=%d\n", m1.i);

	// X f1 = std::forward(a);
	X f1 = std::forward<X>(a);
	printf("i=%d\n", f1.i);
	}
	printf("\n");

	////////////////
	
	{
	int m1 = 1;
	int m2 = 2;
	const int m3 = 3;
	const int m4 = 4;

	PerfectForward(m1);
	PerfectForward(std::move(m2));
	PerfectForward(m3);
	PerfectForward(std::move(m4));
	}

	return 0;
}
