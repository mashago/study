#include <stdio.h>
#include <string>

int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);

	std::string buff("hello \\n world");
	printf("buff=[%s]\n", buff.c_str());
	printf("\n");

	// raw string literal
	// R"(...)"
	buff = R"(hello \n world)";
	printf("buff=[%s]\n", buff.c_str());
	printf("\n");

	const char *ptr = R"(hello \n world)";
	printf("ptr=[%s]\n", ptr);
	printf("\n");

	buff = R"(this
is
a
multi-line
words
!)";
	printf("buff=[%s]\n", buff.c_str());
	printf("\n");

	buff = R"(
first \n will not ignore
)";
	printf("buff=[%s]\n", buff.c_str());
	printf("\n");

	buff = R"(the \
		   will be ignore)";
	printf("buff=[%s]\n", buff.c_str());
	printf("\n");
		   

	return 0;
}
