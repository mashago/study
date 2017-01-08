
#include <iostream>
#include <algorithm>
#include <vector>

bool f3(int x)
{
	return x % 3 == 0;
}

int main(int argc, char *argv[])
{
	using std::cout;
	using std::endl;
	
	const int size = 100;
	std::vector<int> nums(size);
	std::generate(nums.begin(), nums.end(), std::rand);

	int count = 0;

	// normal function
	count = std::count_if(nums.begin(), nums.end(), f3);
	cout << "normal function: count=" << count << endl;

	// functor
	class f_mod 
	{
	private:
		int v;
	public:
		f_mod(int d) : v(d) {};
		bool operator() (int x) { return x % v == 0; };
	};
	f_mod fm(3); // create a functor
	count = std::count_if(nums.begin(), nums.end(), fm);
	cout << "functor: count=" << count << endl;

	// lambda
	// [what_can_use_inside](int input)->return_type{return logic;}
	// if logic is one single return, ->return_type can ignore
	count = std::count_if(nums.begin(), nums.end(), [](int x){return x % 3 == 0;});
	cout << "lambda1: count=" << count << endl;

	count = 0;
	// std::for_each(nums.begin(), nums.end(), [&count](int x){count += (x%3==0);});
	// lambda can use count in logic
	std::for_each(nums.begin(), nums.end(), [&count](int x){if(x%3==0) count++;});
	cout << "lambda2: count=" << count << endl;

	// give a name for lambda, ->bool is return
	auto mod3 = [](int x) ->bool {return x % 3 == 0;};
	count = std::count_if(nums.begin(), nums.end(), mod3);
	cout << "lambda3: count=" << count << endl;

	bool result = mod3(4);
	cout << "result=" << result << endl;



	return 0;
}
