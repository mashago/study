
#include <iostream>


// about macro
// 1. #EXP -> "($EXP)"
// 2. AAA##EXP -> AAA($EXP)

#define MY_MACRO(Class, Member) \
void Print##Class##Member(Class &p) \
{ \
	std::cout << p.Member << std::endl; \
	std::cout << #Member << std::endl; \
}

class MyClass
{
public:
	int M;
};

MY_MACRO(MyClass, M)

int main(int argc, char ** argv)
{

	MyClass t1;
	t1.M = 10;

	PrintMyClassM(t1);

#ifdef _MY_DEF_A
	std::cout << "_MY_DEF_A" << std::endl;
#elif _MY_DEF_B
	std::cout << "_MY_DEF_B" << std::endl;
#else
	std::cout << "not _MY_DEF_A or B" << std::endl;
#ifdef _MY_DEF_C
	std::cout << "_MY_DEF_C" << std::endl;
#else
	std::cout << "not _MY_DEF_A or B or C" << std::endl;
#endif
#endif
	
	return 0;
}
