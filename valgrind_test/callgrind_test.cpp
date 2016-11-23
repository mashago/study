#include <iostream>

void func_c()
{
	std::cout << "call function func_c" << std::endl;
}

void func_b()
{
	std::cout << "call function func_b" << std::endl;
}

void func_a()
{
	std::cout << "call function func_a" << std::endl;
	func_b();
	func_c();
}

int main(int argc, char **argv)
{
	std::cout << "hello callgrind" << std::endl;

	func_a();
	
	return 0;
}
