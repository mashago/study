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
	ENUM_A2 = 12,
	ENUM_B2 = 22,
	ENUM_C2 = 32,
};

enum class MyEnum
{
	ENUM_A = 11,
	ENUM_B = 22,
	ENUM_C = 33,
	ENUM_D = 44,
};

int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);

	printf("ENUM_A=%d\n", ENUM_A);
	printf("ENUM_A2=%d\n", ENUM_A2);
	printf("MyEnum::ENUM_A=%d\n", MyEnum::ENUM_A);
	// printf("ENUM_D=%d\n", ENUM_D); // error, enum class member is local

	return 0;
}
