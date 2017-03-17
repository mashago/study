#include <stdio.h>

enum
{
	ENUM_A = 1,
	ENUM_B = 2,
	ENUM_C = 3,
};

enum MyEnum2
{
	// ENUM_A = 11, // error, enum is global
	ENUM_A2 = 11,
	ENUM_B2 = 12,
	ENUM_C2 = 13,
};

namespace MyEnum3
{
	enum
	{
		ENUM_A = 21,
		ENUM_B = 22,
		ENUM_C = 23,
	};
}

enum class MyEnum4
{
	ENUM_A = 31,
	ENUM_B = 32,
	ENUM_C = 33,
	ENUM_D = 34,
};

int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);

	printf("ENUM_A=%d\n", ENUM_A);
	printf("ENUM_A2=%d\n", ENUM_A2);
	printf("MyEnum2::ENUM_A2=%d\n", MyEnum2::ENUM_A2);
	printf("MyEnum3::ENUM_A=%d\n", MyEnum3::ENUM_A);
	printf("MyEnum4::ENUM_A=%d\n", (int)MyEnum4::ENUM_A);
	// printf("ENUM_D=%d\n", ENUM_D); // error, enum class member is local

	return 0;
}
