#include <stdio.h>
#include "class_test.h"

int main(int argc, char **argv)
{
	printf("hello_class_test\n");

	Simple s1;
	printf("s1.a=%d\n", s1.a);
	// s1.b = 40; // error: read-only variable is not assignable
	printf("s1.b=%d\n", s1.b);

	// use static member directly
	printf("Simple::c=%d\n", Simple::c);
	printf("Simple::d=%d\n", Simple::d);

	printf("s1.c=%d\n", s1.c);
	printf("s1.d=%d\n", s1.d);

	printf("s1::e=%ld\n", s1.e);

	printf("s1::f=%lf\n", s1.f);

	printf("s1::str1=%s\n", s1.str1.c_str());

	Simple s2(5);
	printf("s2.a=%d\n", s2.a);
	printf("s2::str1=%s\n", s2.str1.c_str());

	Simple s3 = 20;
	printf("s3.a=%d\n", s3.a);

	// Simple s4 = "bye"; // explicit construct, cannot auto case
	Simple s4("bye");
	printf("s4.g=%s\n", s4.g.c_str());

	Simple s5(s4);
	printf("s5.g=%s\n", s5.g.c_str());

	// s5 = s4; // function is delete

	s5.print_hello();

	// s5.func_delete(); // function marked delete

	printf("s6:\n");
	Simple s6;
	s6.c = 12;
	s6.print_static();

	printf("m1:\n");
	Middle m1;
	m1.a1 = 13;
	m1.print_static();
	m1.print_hello();

	printf("m2:\n");
	Middle m2;
	m2.print_static();
	m2.print_hello();

	printf("pm2:\n");
	Simple *pm2 = &m2;
	pm2->print_static();
	pm2->print_hello();

	printf("rm2:\n");
	Simple &rm2 = *pm2;
	rm2.print_static();
	rm2.print_hello();

	/*
	printf("rm3:\n");
	Middle &rm3 = *pm2; // error
	rm3.print_static();
	*/

	printf("m3:\n");
	Middle m3;
	// m3.h = 10; // h is protected
	m3.print_hello();


	return 0;
}
