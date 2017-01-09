
#include <iostream>

template <typename T, typename V>
class Tpc
{
public:
	T t_value;
	V v_value;
	Tpc(T t, V v) : t_value(t), v_value(v) {};
	void print() { std::cout << "t_value=" << t_value << " v_value=" << v_value() << std::endl; }
};

int getKey()
{
	return 5;
}

int main(int argc, char **argv)
{
	typedef int (*GetV)();

	typedef Tpc<int, GetV> Xc;

	int a = 1;

	Xc c = Xc(a, getKey);
	
	c.print();
}
