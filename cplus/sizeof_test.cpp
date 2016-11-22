#include <iostream>
#include <cstdio>

using namespace std;

typedef struct {
	int a;
	long b;
	int list[10];
} st;

int main()
{
	cout << "sizeof(int)=" << sizeof(int) << endl;
	cout << "sizeof(uint)=" << sizeof(unsigned int) << endl;
	cout << "sizeof(short)=" << sizeof(short) << endl;
	cout << "sizeof(long)=" << sizeof(long) << endl;
	cout << "sizeof(float)=" << sizeof(float) << endl;
	cout << "sizeof(double)=" << sizeof(double) << endl;
	cout << "sizeof(char)=" << sizeof(char) << endl;

	int *p_int;
	cout << "sizeof(int*)=" << sizeof(p_int) << endl;
	short *p_short;
	cout << "sizeof(short*)=" << sizeof(p_short) << endl;
	st *p_st;
	cout << "sizeof(st*)=" << sizeof(p_st) << endl;

	char buffer[5];
	sprintf(buffer, "%s", "12345");
	cout << "buffer=" << buffer << endl;
	// printf("buffer[5]=%d\n", buffer[5]-'\0'); // = 0



	return 0;
}
