#include <stdio.h>
#include "animal.h"

int main(int argc, char **argv)
{
	printf("hello animal\n");

	animal a1(50, 60);
	a1.print();

	dog d1(30, 40, 4);
	d1.print();

	animal *pd1 = &d1;
	pd1->print();

	printf("\n");

	plant p1(1, 2);
	p1.print();

	rose r1(1, 2, 3);
	r1.print();

	rose r2;
	r2.print();

	plant *pp1 = &r1;
	pp1->print();

	return 0;
}
