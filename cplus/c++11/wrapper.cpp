

#include <iostream>
#include <functional>

using std::cout;
using std::endl;

template <typename T, typename F> T use_f(T t, F f)
{
	static int count = 0;
	count ++;
	cout << "use_f: count=" << count << endl;
	return f(t);
}

template <typename T, typename F> T use_f2(T t, F f)
{
	static int count = 0;
	count ++;
	cout << "use_f2: count=" << count << endl;
	return f(t);
}

template <typename T> T use_f3(T t, std::function<T(T)> f)
{
	static int count = 0;
	count ++;
	cout << "use_f3: count=" << count << endl;
	return f(t);
}


int add_f(int x) { return x + 5; };

class Add_c
{
private:
	static const int n = 10;
public:
	int operator()(int x) { return x+n; };
};

int main(int argc, char *argv[])
{
	
	int input = 10;

	// every F is different, will create 3 different template
	cout << "add_f: " << use_f(input, add_f) << endl;
	Add_c add_c;
	cout << "add_c: " << use_f(input, add_c) << endl;
	auto add_l = [](int x){return x+15;};
	cout << "add_l: " << use_f(input, add_l) << endl;


	// use function<output_type(input_type)>
	std::function<int(int)> ef1 = add_f;
	std::function<int(int)> ef2 = add_c;
	std::function<int(int)> ef3 = add_l;
	// every F is function<int(int)>, only create 1 template
	cout << "function add_f: " << use_f2(input, ef1) << endl;
	cout << "function add_c: " << use_f2(input, ef2) << endl;
	cout << "function add_l: " << use_f2(input, ef3) << endl;

	cout << "function2 add_f: " << use_f3<int>(input, ef1) << endl;
	cout << "function2 add_c: " << use_f3<int>(input, ef2) << endl;
	cout << "function2 add_l: " << use_f3<int>(input, ef3) << endl;
	


	return 0;
}
