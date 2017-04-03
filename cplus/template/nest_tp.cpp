
#include <malloc.h>
#include <memory>
#include <list>

template <typename T, template <typename E, typename ALLOC = std::allocator<E>> class Container>
void ClearContainer(Container<T> &c)
{
	while (!c.empty())
	{
		auto iter = c.begin();
		delete *iter;
		*iter = nullptr;
		c.erase(iter);
	}
}

struct st_a
{
	int x;
};

void test0()
{
	auto print_malloc = [](const char *msg)
	{
		printf("---- %s\n", msg);
		malloc_stats();
		printf("\n");
	};

	std::list<st_a *> c;
	print_malloc("1");
	for (int i = 0; i < 10; ++i)
	{
		st_a *t = new st_a;
		c.push_back(t);
	}
	printf("c.size=%zu\n", c.size());
	print_malloc("2");

	ClearContainer(c);
	printf("c.size=%zu\n", c.size());
	print_malloc("3");

}

template <typename T, template <typename K> class X>
void myFunc(X<T> x)
{
	printf("%s\n", __FUNCTION__);
}

template <typename T>
class cl_a
{
public:
	T a;
};

template <typename T>
struct st_b
{
public:
	T a;
};

void test1()
{
	{
		cl_a<int> t;
		myFunc(t);
	}
	{
		st_b<int> t;
		myFunc(t);
	}
}

int main(int argc, char **argv)
{
	test0();
	test1();

	return 0;
}
