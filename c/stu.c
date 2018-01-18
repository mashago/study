#ifdef __linux__
#define _GNU_SOURCE // for strtok_r
#endif
#include <errno.h> // system location
#include <stdio.h> // system location
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h> // for isalpha()
#define NDEBUG // forbid assert()
#include <assert.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>

// #include "xxx.h" // local location

int test0()
{
	/*
	int ret;
	ret = 0;
	int input_int;
	char input_char[30];
	// scanf from stdin
	ret = scanf("%d %29s", &input_int, input_char);
	printf("test0:ret=%d input_int=%d input_char=%s\n"
	, ret, input_int, input_char);
	*/
	return 0;
}

int test1()
{
	int i = 0;
	char j = 'n';

	printf("test1:size %lu %lu\n", sizeof(i), sizeof(j));

	return 0;
}

int test2()
{

	int plusa;
	int bplus;
	int a = 0;
	int b = 0;

	plusa = ++a;
	bplus = b++;

	printf("test2:plusa=%d bplus=%d\n", plusa, bplus);

	return 0;
}

int test3()
{

	char ch;
	// same as scanf(%c, &ch);
	while ((ch = getchar()) != '\n') {
		// same as printf("%c", ch);
		putchar(ch);
	}
	putchar(ch);

	return 0;
}

int test4()
{
	int ret;
	ret = 0;

	char a = 'a';
	char b = '?';
	char c = '/';
	char d = '\\';

	ret = isalpha(a);
	printf("test4:isalpha(a)=%d\n", ret);

	ret = isalpha(b);
	printf("test4:isalpha(?)=%d\n", ret);

	ret = isalpha(c);
	printf("test4:isalpha(/)=%d\n", ret);

	ret = isalpha(d);
	printf("test4:isalpha(\\)=%d\n", ret);

	return 0;
}

int test5()
{

	int x;
	int y;
	y = 1;
	x = (y > 0) ? 0 : 1;
	printf("test5:x=%d\n", x);

	y = 0;
	x = (y > 0) ? 0 : 1;
	printf("test5:x=%d\n", x);

	return 0;
}

int test6()
{

	int x = 1;

	if (x) {
		printf("test6:1\n");
		if (x) {
			printf("test6:2\n");
			if (x) {
				goto goout;
				printf("test6:3\n");
				if (x) {
					printf("test6:4\n");
				}
			}
		}
	}

goout:
	printf("test6:5\n");

	return 0;
}

int pointer_test(int *n)
{
	*n = 20;

	int t = *n;
	t = 30;
	printf("t=%d\n", t);
	return *n;
}

int test7()
{

	int x = 10;
	int y;

	y = pointer_test(&x);
	printf("test7:x=%d y=%d\n", x, y);

	return 0;
}

int print_test8(int (*plist)[2])
{
	for (int i=0; i<2; i++) {
		for (int j=0; j<2; j++) {
			printf("test8:plist[%d][%d]=%d %p\n", i, j
			, *(plist[i] + j), plist[i]+j);
		}
	}
	return 0;
}

int test8()
{

	int list[2][2];
	printf("test8:sizeof(list)=%lu\n", sizeof(list));
	memset(list, 0, sizeof(list));
	list[0][0] = 1;
	list[0][1] = 2;
	list[1][0] = 3;
	list[1][1] = 4;

	for (int i=0; i<2; i++) {
		for (int j=0; j<2; j++) {
			printf("test8:list[%d][%d]=%p\n", i, j, &list[i][j]);
		}
	}

	int (*p)[2];
	p = list;
	printf("test8:p=%p\n", p);
	p = &list[0];
	printf("test8:p=%p\n", p);

	print_test8(list);

	int list2[4];
	memset(list2, 0, sizeof(list2));
	list2[0] = 1;
	list2[1] = 2;
	list2[2] = 3;
	list2[3] = 4;
	int *ptr = list2;
	printf("test8:ptr[0]=%d\n", ptr[0]);
	printf("test8:*ptr=%d\n", *ptr);
	printf("test8:*ptr=%d\n", *(ptr + 3));


	return 0;
}

int test9()
{

	char input[30];
	memset(input, 0, sizeof(input));

	// gets(input);
	// puts(input);

	// fgets(input, 20, stdin); // get input in stdin
	// fputs(input, stdout); // output in stdout

	// scanf("%s", input);

	// printf("test9:input=%s\n", input);

	return 0;
}

int test10()
{

	char aaa[50];
	char bbb[20];
	char *ptr;
	memset(aaa, 0, sizeof(aaa));
	memset(bbb, 0, sizeof(bbb));

	// strlen
	sprintf(aaa, "qwerty");
	sprintf(bbb, "123456");
	printf("test10:sizeof(aaa)=%lu\n", sizeof(aaa));
	printf("test10:strlen(aaa)=%lu\n", strlen(aaa));

	// strcat
	ptr = strcat(aaa, bbb);
	printf("test10:ptr1=%s\n", ptr);

	// strncat
	sprintf(aaa, "qwerty");
	sprintf(bbb, "123456");
	ptr = strncat(aaa, bbb, 2);
	printf("test10:ptr2=%s\n", ptr);

	// strcmp
	sprintf(aaa, "qwerty");
	sprintf(bbb, "123456");
	printf("test10:strcmp(aaa, bbb)1=%d\n", strcmp(aaa, bbb));
	sprintf(aaa, "qwerty");
	sprintf(bbb, "qwerty");
	printf("test10:strcmp(aaa, bbb)2=%d\n", strcmp(aaa, bbb));

	// strcpy
	sprintf(aaa, "qwerty");
	sprintf(bbb, "12345");
	ptr = strcpy(aaa, bbb);
	printf("test10:ptr3=%s aaa=%s\n", ptr, aaa);

	// strncpy
	sprintf(aaa, "qwerty");
	sprintf(bbb, "123456");
	ptr = strncpy(aaa, bbb, 3);
	printf("test10:ptr4=%s aaa=%s\n", ptr, aaa);



	return 0;
}

int test11()
{

	int aaa;
	char * ccc = "123";
	aaa = atoi(ccc);
	printf("test11:aaa1=%d\n", aaa);

	ccc = "12haha";
	aaa = atoi(ccc);
	printf("test11:aaa2=%d\n", aaa);

	return 0;
}

int test12_static() {
	int aaa = 0;
	static int bbb = 0;
	
	printf("test12:aaa=%d bbb=%d\n", aaa++, bbb++);

	return 0;
}

int test12()
{

	for (int i=0; i<5; i++) {
		test12_static();
	}

	return 0;
}

int test13_a = 1;

int test13()
{

	printf("test13:&test13_a=%p\n", &test13_a);

	// int test13_a;
	// printf("test13:&test13_a=%p\n", &test13_a);

	// same as int test13_a;
	// auto int test13_a;
	// printf("test13:&test13_a=%p\n", &test13_a);

	// not same as above
	// extern int test13_a;
	// printf("test13:&test13_a=%p\n", &test13_a);

	return 0;
}

int test14()
{

	// gcc -o stu stu.c empty.c
	// declare in empty.c
	// extern int EMPTY_INT;
	// printf("test14:EMPTY_INT=%d\n", EMPTY_INT);

	return 0;
}

int test15()
{

	char *ptr;
	ptr = malloc(30 * sizeof(char));

	printf("test15:size=%zu\n", sizeof(ptr));
	sprintf(ptr, "qwerty");
	printf("test15:ptr=%s\n", ptr);

	free(ptr);

	char buffer[100];
	printf("test15:size=%zu\n", sizeof(buffer));

	return 0;
}

int test16()
{
	int ret;
	ret = 0;
	
	char one;
	FILE * fp;
	FILE * out;
	fp = fopen("myfile.txt", "r");
	out = fopen("myfile2.txt", "a");
	// out = fopen("myfile2.txt", "w");
	if (fp == NULL || out == NULL) {
		fprintf(stderr, "test16:file_null\n");
		return -3;
	}

	// one = getc(fp);
	// printf("test16:one=%c\n", one);

	// while (one != EOF) {
	while ((one=getc(fp)) != EOF) {
		putc(one, out);
		// one = getc(fp);
	}



	ret = fclose(fp);
	printf("test16:ret1=%d\n", ret);
	ret = fclose(out);
	printf("test16:ret2=%d\n", ret);


	return 0;
}

int test17()
{
	int ret;
	ret = 0;
	
	char str[50];
	FILE * fp;
	fp = fopen("myfile.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "test17:file_null\n");
		return -3;
	}

	ret = fscanf(fp, "%48s", str);
	printf("test17:str=%s ret=%d\n", str, ret);

	fgets(str, 48, fp);
	printf("test17:str=%s\n", str);

	fgets(str, 48, fp);
	printf("test17:str=%s\n", str);

	ret = fclose(fp);
	printf("test17:ret1=%d\n", ret);
	return 0;
}

int test18()
{
	
	char one;
	long last;
	FILE * fp;
	fp = fopen("myfile.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "test17:file_null\n");
		return -3;
	}

	fseek(fp, 0L, SEEK_END); // to file end
	last = ftell(fp); // location
	printf("test18:last=%ld\n", last);

	for (long i=1L; i<=last; i++) {
		fseek(fp, -i, SEEK_END);
		one = getc(fp);
		putchar(one);
	}


	return 0;
}

int test19()
{
	
	long location;
	char str[50];
	FILE * fp;
	fp = fopen("myfile.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "test17:file_null\n");
		return -3;
	}


	while (!feof(fp)) {
		location = ftell(fp); // location
		printf("test19:location=%ld\n", location);
		fscanf(fp, "%48s", str);
		printf("test19:str=%s\n", str);
		location = ftell(fp); // location
		printf("test19:location=%ld\n\n", location);
	}

	rewind(fp); // move to start
	location = ftell(fp); // location
	printf("test19:location=%ld\n\n", location);


	return 0;
}

struct book_t {
	int id;
	char name[20];
	float price;
};

int test20()
{

	struct book_t book;
	// book_t book; // only work in c++
	book.id = 20;
	sprintf(book.name, "heha");
	book.price = 19.99;
	printf("test20:book id=%d name=%s price=%f\n"
	, book.id, book.name, book.price);

	struct book_t book2 = {
		10
		,"byebye"
		,29.99
	};
	printf("test20:book2 id=%d name=%s price=%f\n"
	, book2.id, book2.name, book2.price);

	struct book_t book3;
	book3 = book;	
	printf("test20:book3 id=%d name=%s price=%f\n"
	, book3.id, book3.name, book3.price);

	FILE * fp;
	fp = fopen("book.txt", "w");
	if (fp == NULL) {
		fprintf(stderr, "test20:file_null\n");
		return -3;
	}
	fwrite(&book3, sizeof(struct book_t), 1, fp);

	return 0;
}

union hold {
	int argv1;
	double argv2;
	char argv3[30];
};

int test21()
{

	union hold u1;
	u1.argv1 = 23;
	printf("test21:u1.argv1=%d\n", u1.argv1);
	u1.argv2 = 23.999;
	printf("test21:u1.argv1=%d\n", u1.argv1); // only 1 value can be saved
	sprintf(u1.argv3, "qwerty");
	printf("test21:u1.argv1=%d\n", u1.argv1);
	printf("test21:u1.argv3=%s\n", u1.argv3);


	return 0;
}

enum color {RED, BLUE, YELLOW};
enum color2 {RED2, BLUE2=10, YELLOW2};
int test22()
{

	printf("test22:RED=%d BLUE=%d YELLOW=%d\n", RED, BLUE, YELLOW);
	printf("test22:RED2=%d BLUE2=%d YELLOW2=%d\n", RED2, BLUE2, YELLOW2);

	return 0;
}

typedef unsigned int INT;
typedef struct {
	int id;
	char brand[20];
	double price;
} car_t;
int test23()
{

	INT i = 20;
	car_t car;
	car.id = 450;
	sprintf(car.brand, "BANZ");
	car.price = 2999.99;

	printf("test23:i=%d car id=%d brand=%s price=%lf\n"
	, i, car.id, car.brand, car.price);

	return 0;
}

int test24()
{

	int i = 1;
	i = ~i;
	printf("test24:i=%d\n", i);
	i = ~i;
	printf("test24:i=%d\n", i);

	int a, b, c;
	a = 1;
	b = 2;
	c = a & b;
	printf("test24:c1=%d\n", c);

	a = 1;
	b = 3;
	c = a & b;
	printf("test24:c2=%d\n", c);

	a = 1;
	b = 2;
	c = a | b;
	printf("test24:c3=%d\n", c);

	a = 1;
	b = 2;
	c = a ^ b;
	printf("test24:c4=%d\n", c);

	a = 1;
	b = 3;
	c = a ^ b;
	printf("test24:c5=%d\n", c);

	a = 1;
	c = a >> 2;
	printf("test24:c6=%d\n", c);
	c = a << 2;
	printf("test24:c6=%d\n", c);


	return 0;
}

#define PRINT_RET printf("print_return:ret=%d\n", ret)
#define PRINT(x) printf("print:x=%d\n", x)
#define PR(...) printf(__VA_ARGS__)
int test25()
{
	int ret;
	ret = 0;

	PRINT_RET;
	PRINT(ret);
	PR("pr:ret=%d\n", ret);

	return 0;
}

#define TTT 1

int test26()
{

#ifdef TTT
	printf("test26:define1 TTT\n");
#else
	printf("test26:undefine1 TTT\n");
#endif

#ifndef TTT
	printf("test26:undefine2 TTT\n");
#else
	printf("test26:define2 TTT\n");
#endif

	return 0;
}

int test27()
{

	printf("test27:date=%s\n", __DATE__);
	printf("test27:file=%s\n", __FILE__);
	printf("test27:line=%d\n", __LINE__);
	printf("test27:func=%s\n", __func__);
	printf("test27:version=%ld\n", __STDC_VERSION__);

	return 0;
}

int get_big(const void * v1, const void * v2)
{
	const int * a =  (const int *) v1;
	const int * b =  (const int *) v2;

	if (*a > *b) {
		return 1;
	}
	if (*a < *b) {
		return -1;
	}
	return 0;
}

int test28()
{

	int size = 5;

	int list[size];
	list[0] = 1;
	list[1] = 4;
	list[2] = 8;
	list[3] = 3;
	list[4] = 7;

	qsort(list, size, sizeof(int), get_big);

	for (int i=0; i<size; i++) {
		printf("test28:list[%d]=%d\n", i, list[i]);
	}

	return 0;
}

int test29()
{

	int i = 10;
	printf("test29:i=%d\n", i);

	assert(i == 9);
	// assert(i == 10);

	return 0;
}

int test30()
{

	char s1[20] = "hello";
	char s2[20] = "world";

	printf("test30:s1=%s s2=%s\n", s1, s2);

	memcpy(s2, s1, sizeof(char) * 2); // do not use in same memory
	printf("test30:s1=%s s2=%s\n", s1, s2);
	
	memmove(s2+4, s1, sizeof(char) * 5);
	printf("test30:s1=%s s2=%s\n", s1, s2);

	return 0;
}

int get_args(int size, ...)
{
	int v1;
	// char v2;
	int v2;
	double v3;

	int x1;
	int x2;
	double x3;

	va_list list;
	va_list copy_list;
	va_start(list, size);
	va_copy(copy_list, list);

	v1 = va_arg(list, int);
	// v2 = va_arg(list, char); // second arg cannot be char, short, float
	v2 = va_arg(list, int);
	v3 = va_arg(list, double);

	x1 = va_arg(copy_list, int);
	x2 = va_arg(copy_list, int);
	x3 = va_arg(copy_list, double);
	
	printf("get_args:v1=%d v2=%c v3=%lf\n", v1, v2, v3);
	printf("get_args:x1=%d x2=%c x3=%lf\n", x1, x2, x3);

	va_end(copy_list);
	va_end(list);
	
	return 0;
}

int fmt_args(const char * fmt, ...)
{
	int size = 50;
	char buffer[size+1];

	va_list list;
	va_start(list, fmt);
	vsnprintf(buffer, size, fmt, list);
	va_end(list);

	printf("fmt_args:buffer=[%s]\n", buffer);

	return 0;
}

int fmt_args2(const char *fmt, ...)
{
	// 1. declare a va_list
	va_list ap;
	int d = 0;
	const char *s = NULL;
	char c = '_';

	// 2. init va_list, param2 is the last argument before variable argument list
	va_start(ap, fmt);

	while (*fmt)
	{
		switch (*fmt++)
		{
		case 'd':
			// 3. covert arg to real type
			d = va_arg(ap, int);
			printf("d=%d\n", d);
			break;
		case 's':
			s = va_arg(ap, const char *);
			printf("s=%s\n", s);
			break;
		case 'c':
			c = va_arg(ap, int); // char should use int type
			printf("c=%c\n", c);
			break;
		}
	}

	// 4. end
	va_end(ap);

	return 0;
}



int test31()
{

	int a = 20;
	char b = 'h';
	double c = 9.9;

	get_args(3, a, b, c);
	fmt_args("a=%d b=%c c=%lf", a, b, c);

	fmt_args2("dcsscd", 11, 'a', "hello", "world", 'k', 47);

	return 0;
}

int test32()
{

	time_t now;
	now = time(NULL);
	printf("test32:now=%ld\n", now);

	return 0;
}

int test33()
{
	char buffer[100];
	sprintf(buffer, "qwe\nxxx\n");
	printf("buffer=%s\n", buffer);

	char * tok;
	char * context;
	const char *sep = "\n";
	for (tok=strtok_r(buffer, sep, &context); tok!=NULL; tok=strtok_r(NULL, sep, &context)) {
		printf("tok=%s\n", tok);
	}

	return 0;
}

int test34()
{

	int a;
	int b;
	int c;
	int d;
	int e;

	a = 0; //  0
	b = 1; // 01
	c = 2; // 10
	d = 3; // 11

	e = a | b;
	if (e) {
		printf("test34:e1 true\n");
	} else {
		printf("test34:e1 false\n");
	}

	e = a | c;
	if (e) {
		printf("test34:e2 true\n");
	} else {
		printf("test34:e2 false\n");
	}

	e = b | c;
	if (e) {
		printf("test34:e3 true\n");
	} else {
		printf("test34:e3 false\n");
	}

	e = a & b;
	if (e) {
		printf("test34:e4 true\n");
	} else {
		printf("test34:e4 false\n");
	}

	e = b & c;
	if (e) {
		printf("test34:e5 true\n");
	} else {
		printf("test34:e5 false\n");
	}

	e = a & c;
	if (e) {
		printf("test34:e6 true\n");
	} else {
		printf("test34:e6 false\n");
	}

	e = b & d;
	if (e) {
		printf("test34:e7 true\n");
	} else {
		printf("test34:e7 false\n");
	}



	return 0;
}

#define swap(x,y) {x=x+y;y=x-y;x=x-y;}

int test35()
{

	int x=1;
	int y=2;
	printf("test35:before x=%d y=%d\n", x, y);
	swap(x, y);
	printf("test35:after x=%d y=%d\n", x, y);

	

	return 0;
}

struct item_a_t
{
};

struct item_b_t
{
	int a;
};

struct item_c_t
{
	int a;
	long b;
};

struct item_d_t
{
	long b;
	int a;
};

struct item_e_t
{
	int a;
	char buffer[4];
};

struct item_f_t
{
	int a;
	char buffer[5];
};

struct item_g_t
{
	int a;
	int b;
	int c;
};

struct item_h_t
{
	int a;
	long b;
	int c;
};

struct item_i_t
{
	int a;
	int b;
	long c;
};

struct item_j_t
{
	int a;
	char buffer[5];
	long c;
};

struct item_k_t
{
	char buffer[5];
};

struct item_l_t
{
	long a;
	char buffer[4];
};

int test36()
{

	printf("sizeof(char)=%lu\n", sizeof(char));						// 1
	printf("sizeof(int)=%lu\n", sizeof(int)); 						// 4
	printf("sizeof(unsigned int)=%lu\n", sizeof(unsigned int)); 	// 4
	printf("sizeof(short)=%lu\n", sizeof(short)); 					// 2
	printf("sizeof(long)=%lu\n", sizeof(long)); 					// 8
	printf("sizeof(float)=%lu\n", sizeof(float)); 					// 4
	printf("sizeof(double)=%lu\n", sizeof(double));					// 8

	printf("sizeof(char*)=%lu\n", sizeof(char*));					// 8
	printf("sizeof(int*)=%lu\n", sizeof(int*)); 					// 8
	printf("sizeof(void*)=%lu\n", sizeof(void*)); 					// 8


	char str1[10];
	printf("sizeof(char[10])=%lu\n", sizeof(str1));					// 10
	char str2[] = "hello";
	printf("sizeof(char str[]=\"hello\")=%lu\n", sizeof(str2));		// 6
	printf("strlen(str[]=\"hello\")=%lu\n", strlen(str2));			// 5
	const char * str3 = "hello";
	printf("sizeof(const char str3=\"hello\")=%lu\n", sizeof(str3)); // 8
	printf("sizeof(\"hello\")=%lu\n", sizeof("hello")); // 6

	printf("sizeof(struct item_a_t)=%lu\n", sizeof(struct item_a_t));	// 0
	printf("sizeof(struct item_b_t)=%lu\n", sizeof(struct item_b_t));	// 4
	printf("sizeof(struct item_c_t)=%lu\n", sizeof(struct item_c_t));	// 4(int) + 4(unuse) + 8(long) = 16
	printf("sizeof(struct item_d_t)=%lu\n", sizeof(struct item_d_t));	// 8(long) + 4(int) = 12, but size should be 8 times, so = 16
	printf("sizeof(struct item_e_t)=%lu\n", sizeof(struct item_e_t));	// 4(int) + 4(char[4]) = 8
	printf("sizeof(struct item_f_t)=%lu\n", sizeof(struct item_f_t));	// 4(int) + 4(char) + 1(char) = 9, but size should be 4 times, so = 12
	printf("sizeof(struct item_g_t)=%lu\n", sizeof(struct item_g_t));	// 4(int) * 3 = 12
	printf("sizeof(struct item_h_t)=%lu\n", sizeof(struct item_h_t));	// 4(int) + 4(unuse) + 8(long) + 4(int) = 20, but size should be times of max(sizeof(member)), so = 24
	printf("sizeof(struct item_i_t)=%lu\n", sizeof(struct item_i_t));	// 4(int) * 2 + 8(long) = 16
	printf("sizeof(struct item_j_t)=%lu\n", sizeof(struct item_j_t));	// 4(int) + 4(char[4]) + 1(char[1]) + 7(unuse) + 8(long) = 24
	printf("sizeof(struct item_k_t)=%lu\n", sizeof(struct item_k_t));	// 5(char) = 5
	printf("sizeof(struct item_l_t)=%lu\n", sizeof(struct item_l_t));	// 8(long) + 4(char[4] = 12, but 8 times, so = 16

	return 0;
}

int test37()
{

	char c;

	// ctrl+d will case EOF
	while ((c = getchar()) != EOF) {
		putchar(c);
	}

	return 0;
}

int test38()
{
	int ret;
	ret = 0;

	int t;

	// ctrl+d will case EOF
	while ((ret = scanf("%d", &t)) != 1) {
		scanf("%*s"); // jump to next space char
	}
	printf("t=%d\n", t);

	return 0;
}

typedef struct {
	int a;
	int b;
} st39_t;

#define TEST39_SIZE	10
int test39()
{

	st39_t t1;
	t1.a = 10;
	t1.b = 20;

	st39_t t2 = t1;
	printf("t2.a=%d t2.b=%d\n", t2.a, t2.b);

	t1.a = 1;
	t1.b = 2;
	printf("t2.a=%d t2.b=%d\n", t2.a, t2.b);

	int list1[TEST39_SIZE] = {1, 0, 2};
	int list2[TEST39_SIZE];

	const int size = TEST39_SIZE;
	int list3[size];

	// list2 = list1; // error
	// list2 = {10, 2}; // error
	for (int i=0;i<TEST39_SIZE; i++) {
		list2[i] = list1[i]; // only way
		list3[i] = list1[i]; // only way
	}
	printf("list2[0]=%d\n", list2[0]);
	printf("list3[0]=%d\n", list3[0]);
	

	// list1++; // error
	int *ptr = list1;
	ptr++; // this is ok

	int *ptr2 = list1;
	printf("ptr-ptr2=%ld\n", ptr-ptr2);

	return 0;
}

int test40()
{

	char input[100];

	// gets(input); // should not se gets() ,fedora gcc will warn it
	fgets(input, 100, stdin);
	puts(input);

	char txt1[100] = "hello" "world"; // == "helloworld"
	puts(txt1);
	printf("strlen(txt1)=%ld\n", strlen(txt1)); // == 10


	return 0;
}

int test41()
{

	char txt1[] = "hello"; // {"hello"} is ok
	const char *ptr = "hello";

	printf("txt1=[%s]\n", txt1); // txt1 is static address
	printf("ptr=[%s]\n", ptr);
	printf("ptr=[%c]\n", *ptr);
	ptr++; // only ptr can do this
	printf("ptr=[%c]\n", *ptr);


	return 0;
}

// input test
int test42()
{
	const int size = 100;
	char input[size];
	char * ptr;


	// NOTE:
	// get from stdin
	// getc(FILE *stream); // this is marco
	// getchar(void); // get from stdin
	// gets(char *str); // get from stdin
	// scanf(const char *restrict format, ...);

	// get from file ptr
	// fgetc(FILE *stream);
	// fgets(char * restrict str, int size, FILE * restrict stream);
	// fscanf(FILE * restrict stream, const char *restrict format, ...);

	// 1.different between gets() fgets() and scanf():
	// gets() and fgets() get a string, scanf() only get a word
	// gets() and fgets() retrun char * for address, scanf() return scan count

	// 2.different between gets() and fgets()
	// fgets() add '\n' in buffer
	// fgets() need to set max size and i/o type
	// fgets() can check size, gets() may case buffer overflow


	printf("do gets() now\n");
	// ptr = gets(input); // fedora gcc will warn it
	ptr = fgets(input, size, stdin);
	if (ptr != NULL) {
		printf("gets() ptr=[%s]\n", ptr);
	}

	printf("do fgets() now\n");
	ptr = fgets(input, size, stdin); // will keep '\n' in buffer
	if (ptr != NULL) {
		printf("fgets() ptr=[%s]\n", ptr);
	}

	printf("do scanf() now\n");
	ptr = input;
	scanf("%10s", ptr); // scanf only get word, not a string. if stdin = "aaa bbb", scanf only get "aaa"
	printf("scanf() ptr=[%s]\n", ptr);

	return 0;
}

// out print test
int test43()
{
	const char *output = "hello world";

	printf("do puts() now\n");
	puts(output); // add '\n' in output end

	printf("do fputs() now\n");
	fputs(output, stdout); // not add '\n' in output end
	printf("\n");

	printf("do printf() now\n");
	printf("%s", output); // not add '\n' in output end
	printf("\n");

	return 0;
}

// str function test
int test44()
{
	int ret;
	ret = 0;

	// const int size = 20;
	// char buffer[size] = "hello world"; // not work in fedora gcc
	char buffer[] = "hello world";
	char *ptr;
	printf("buffer=[%s]\n", buffer);
	char cat[] = "cat";
	printf("cat=[%s]\n", cat);
	char nums[] = "1234567890123";
	printf("nums=[%s]\n", nums);

	// include <string.h>
	printf("strlen(buffer)=%lu\n", strlen(buffer));

	printf("\ndo strcat() now:\n");
	ptr = strcat(buffer, cat);
	printf("after strcat() buffer=[%s]\n", buffer);
	printf("ptr=[%s]\n", ptr);

	printf("\ndo strncat() now:\n");
	sprintf(buffer, "hello world");
	int len = sizeof(buffer) - strlen(buffer) - 1;
	ptr = strncat(buffer, nums, len);
	printf("after strncat() buffer=[%s]\n", buffer);
	printf("ptr=[%s]\n", ptr);


	const char * b1 = "helle";
	const char * b2 = "helne";
	const char * b3 = "helle";
	printf("\ndo strcmp() now:\n");
	printf("b1=[%s] b2=[%s] b3=[%s]\n", b1, b2, b3);
	printf("strcmp(b1, b2)=%d\n", strcmp(b1, b2));
	printf("strcmp(b1, b3)=%d\n", strcmp(b1, b3));

	printf("\ndo strncmp() now:\n");
	printf("b1=[%s] b2=[%s] b3=[%s]\n", b1, b2, b3);
	printf("strncmp(b1, b2, 3)=%d\n", strncmp(b1, b2, 3));
	printf("strncmp(b1, b2, 4)=%d\n", strncmp(b1, b2, 4));

	printf("\ndo strcpy() now:\n");
	// const int b_size = 10;
	// char buff[b_size] = "abcdefg";
	char buff[] = "abcdefg";
	const char *tmp = "123";
	printf("buff=[%s] tmp=[%s]\n", buff, tmp);
	ptr = strcpy(buff, tmp);
	printf("after strcpy() buffer=[%s] ptr=[%s]\n", buff, ptr);

	printf("\ndo strncpy() now:\n");
	// char buff2[b_size] = "abcdefg";
	char buff2[] = "abcdefg";
	const char *tmp2 = "123456789012";
	printf("buff2=[%s] tmp2=[%s]\n", buff2, tmp2);
	ptr = strncpy(buff2, tmp2, 7);
	buff2[7] = '\0'; // strncpy may have no '\0', need add this
	printf("after strncpy() buffer=[%s] ptr=[%s]\n", buff2, ptr);


	printf("\ndo strchr() now:\n");
	char buff3[] = "abcdefg";
	const char tmp3 = 'c';
	printf("buff3=[%s] tmp3=[%c]\n", buff3, tmp3);
	ptr = strchr(buff3, tmp3); // return ptr first time get second param char
	if (ptr != NULL) {
		printf("ptr=[%s]\n", ptr);
	}

	printf("\ndo strpbrk() now:\n");
	char buff4[] = "abcdefg";
	const char *tmp4 = "123ef";
	printf("buff4=[%s] tmp4=[%s]\n", buff4, tmp4);
	ptr = strpbrk(buff4, tmp4); // return ptr first time get any char in second param string
	if (ptr != NULL) {
		printf("*ptr=[%c] ptr=[%s]\n", *ptr, ptr);
	}

	printf("\ndo strrchr() now:\n");
	char buff5[] = "ab1de1fg";
	const char tmp5 = '1';
	printf("buff5=[%s] tmp5=[%c]\n", buff5, tmp5);
	ptr = strrchr(buff5, tmp5); // return ptr last time get second param char
	if (ptr != NULL) {
		printf("ptr=[%s]\n", ptr);
	}

	printf("\ndo strstr() now:\n");
	char buff6[] = "abcdefg";
	const char *tmp6 = "cde";
	printf("buff6=[%s] tmp6=[%s]\n", buff6, tmp6);
	ptr = strstr(buff6, tmp6); // return ptr first time get second param string
	if (ptr != NULL) {
		printf("*ptr=[%c] ptr=[%s]\n", *ptr, ptr);
		if (buff6 != strstr(buff6, tmp6)) {
			printf("tmp6 not first string in buff6\n");
		}
	}

	printf("\ndo atoi() now:\n");
	const char *tmp7 = "42aaa";
	printf("tmp7=[%s]\n", tmp7);
	ret = atoi(tmp7); // string to integer
	printf("atoi() ret=%d\n", ret);
	const char *tmp8 = "aaa42aa";
	printf("tmp8=[%s]\n", tmp8);
	ret = atoi(tmp8); // string to integer
	printf("atoi() ret=%d\n", ret);

	printf("\ndo strtol() now:\n");
	const char *tmp9 = "42aa";
	long value9 = 0;
	char *end;
	printf("tmp9=[%s]\n", tmp9);
	value9 = strtol(tmp9, &end, 10); // string to long, 10 is base format, if end != tmp, get long success
	if (end!=tmp9) {
		printf("strtol() value9=%ld\n", value9);
	}
	const char *tmp10 = "aa42";
	long value10 = 0;
	printf("tmp10=[%s]\n", tmp10);
	value10 = strtol(tmp10, &end, 10); // string to long
	if (end!=tmp10) {
		printf("strtol() value10=%ld\n", value10);
	}



	return 0;
}

// "static" means link type, not storage duration
int link_outside = 0; // external linkage
int link_outside2 = 0; // external linkage
static int link_inside = 0; // internal linkage
int test45()
{
	static int link_null = 0; // no linkage
	link_null++;
	link_inside++;

	extern int link_outside; // extern outside
	link_outside++;
	printf("link_outside=%d\n", link_outside);

	int link_outside2 = 20; // will hide outside
	link_outside2++;
	printf("link_outside2=%d\n", link_outside2);

	int x = 10;
	{
		int x = 20; // will hide outside define
		printf("inside x=%d\n", x);
		x++;
		printf("inside x=%d\n", x);
	}
	// back to outside define
	printf("outside x=%d\n", x);

	return 0;
}

int test46()
{

	FILE *fp;
	fp = fopen("read_file.txt", "r");
	if (fp == NULL) {
		printf("read_file.txt not found\n");
		return 0;
	}

	printf("start print read_file.txt\n");
	char ch;
	while ((ch = getc(fp)) != EOF) {
		putc(ch, stdout);
	}
	fclose(fp);	

	return 0;
}

int test47()
{

	char buff[50];
	sprintf(buff, "hello world %d", 123);
	fprintf(stdout, "buff=[%s]\n", buff);

	FILE *fp;
	fp = fopen("write_file.txt", "w+");
	if (fp == NULL) {
		printf("write_file.txt not found\n");
		return 0;
	}



	// write into file
	fprintf(fp, "%s", buff);

	// back to file start position
	rewind(fp);

	// scanf string
	fscanf(fp, "%49s", buff);
	fprintf(stdout, "fscanf() buff=[%s]\n", buff);
	
	fgets(buff, 49, fp);
	fprintf(stdout, "fgets() buff=[%s]\n", buff);

	long step = 0;
	fseek(fp, step, SEEK_SET); // go to start + step
	fseek(fp, step, SEEK_CUR); // go to current pos + step
	fseek(fp, step, SEEK_END); // go to end + step

	long pos = ftell(fp);
	fprintf(stdout, "ftell() pos=%ld\n", pos);

	fclose(fp);	

	return 0;
}

int test48()
{

	enum color_num {RED, YELLOW, BLACK, BLUE=7, GREEN};
	printf("RED=%d\n", RED);
	printf("YELLOW=%d\n", YELLOW);
	printf("BLACK=%d\n", BLACK);
	printf("BLUE=%d\n", BLUE);
	printf("GREEN=%d\n", GREEN);


	return 0;
}

// include <stdarg.h>
int sum_int_args(int n, ...) 
{
	int total = 0;
	va_list ap;
	va_start(ap, n);
	int arg = 0;
	for (int i=0; i<n; i++) {
		arg = va_arg(ap, int);
		total += arg;
	}
	va_end(ap);
	return total;
}

int buffer_args(const char *fmt, ...)
{
	const int size = 100;
	char buffer[size];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buffer, size, fmt, ap);
	va_end(ap);
	printf("buffer=[%s]\n", buffer);
	return 0;
}

int test49()
{

	int total = sum_int_args(5, 1, 3, 5, 7, 9);
	printf("total=%d\n", total);

	const char *fmt = "%s %d %s %d";
	buffer_args(fmt, "hello", 123, "world", 456);

	return 0;
}

int test50()
{

	int a = 0;

	a = 2 << 0;
	printf("a=%d\n", a);

	a = 2 << 1;
	printf("a=%d\n", a);

	a = 2 << 2;
	printf("a=%d\n", a);

	a = 2 << 3;
	printf("a=%d\n", a);

	return 0;
}

int test51()
{
	int ret;
	ret = (int)((1 - (double)77/10000) * 30);
	printf("test51:ret=%d\n", ret);

	return 0;
}


int test52()
{
	int ret;
	ret = 0;

	int array[5] = {1, 1, 2, 3, 3};

	for (int i=0; i<5; i++) {
		ret ^= array[i];	
	}
	printf("ret = %d\n", ret);

	int one, two, three;
	one = two = three = 0;
	int array2[5] = {1, 1, 1, 2, 2};
	for (int i=0; i<5; i++) {
		two = one & array2[i];
		one ^= array2[i];
		three = ~(one & two);
		one &= three;
		two &= three;
	}
	printf("one = %d two = %d\n", one, two);

	return 0;
}

void pointer_pointer_test(char **pptr)
{
	printf("*pptr=%s\n", *pptr);
	pptr++;
	printf("*pptr=%s\n", *pptr);
}

int test53()
{
	const int size = 10;

	char s1[size];
	snprintf(s1, size, "%s", "hello_world");
	printf("s1=%s\n", s1);

	char s2[size];
	snprintf(s2, size, "%s", "qwerty_12345");
	printf("s2=%s\n", s2);

	char *pptr[2];
	pptr[0] = s1;
	pptr[1] = s2;

	pointer_pointer_test(pptr);


	return 0;
}

int test54()
{

	const char *buffer = "12345678901aaa";
	long value = 0;
	char *end;
	printf("buffer=[%s]\n", buffer);
	int base = 0; // base is radix, 2,8,10,16.... 0 in normal means 10, if head with 0x, means 16, head with 0, means 8
	value = strtol(buffer, &end, base);
	printf("base=%d value=%ld\n", base, value);

	base = 2;
	value = strtol(buffer, &end, base);
	printf("base=%d value=%ld\n", base, value);

	base = 8;
	value = strtol(buffer, &end, base);
	printf("base=%d value=%ld\n", base, value);

	base = 10;
	value = strtol(buffer, &end, base);
	printf("base=%d value=%ld\n", base, value);

	base = 16;
	value = strtol(buffer, &end, base);
	printf("base=%d value=%ld\n", base, value);

	const char *buffer2 = "abc123";
	base = 10;
	value = strtol(buffer2, &end, base);
	if (end == buffer2)
	{
		printf("strtol fail, buffer2=%s\n", buffer2);
	}
	else
	{
		printf("base=%d value=%ld\n", base, value);
	}


	return 0;
}

int test55()
{
	unsigned int a = 0x01;
	unsigned int b = 0x02;
	unsigned int c = 0x04;

	printf("a=%u, b=%u, c=%u\n", a, b, c);

	unsigned int tmp;
	unsigned int ret;

	tmp = 0x00;
	ret = tmp & a; 
	printf("ret1=%u\n", ret);

	tmp = 0x01;
	ret = tmp & a; 
	printf("ret2=%u\n", ret);

	tmp = 0x02;
	ret = tmp & a; 
	printf("ret3=%u\n", ret);

	tmp = 0x01;
	ret = tmp & b; 
	printf("ret4=%u\n", ret);

	tmp = a | b;
	ret = tmp & b; 
	printf("ret5=%u\n", ret);

	tmp = a | b;
	ret = tmp & c; 
	printf("ret6=%u\n", ret);

	ret = a ^ b;
	printf("ret7=%u\n", ret);

	tmp = 0x01;
	ret = (a | b) & ~tmp;
	printf("ret8=%u\n", ret);
	

	return 0;
}

typedef struct 
{
	int id;
} test56_t;

int test56()
{
	// calloc, param1 is obj num, param2 is each obj size
	test56_t *tmp = calloc(1, sizeof(test56_t));

	tmp->id = 5;

	int *ptr = &tmp->id;

	printf("*ptr=%d\n", *ptr);


	return 0;
}

void test57_va(int count, ...)
{
	va_list vl; // a
	va_start(vl, count); // b
	printf("value =");
	for (int i=0; i<count; i++)
	{
		int value = va_arg(vl, int); // c
		printf(" %d", value);
	}
	printf("\n");
	va_end(vl); // d
}

int test57()
{
	test57_va(5, 1, 3, 5, 7, 9);

	test57_va(0, 1, 3, 5, 7, 9);

	return 0;
}

int test58()
{
	const char *filename = "test58.txt";
	const char *filename2 = "test58x.txt";
	const char *filename3 = "test58y.txt";
	// const char *filename4 = "test58z.txt";
	FILE *pFile = NULL;
	if ((pFile = fopen(filename, "a")) == NULL)
	{
		perror("test58:fopen");
		return -1;
	}
	fprintf(pFile, "hello world\n");
	fclose(pFile);
	pFile = NULL;

	// stat
//	struct stat {
//	   dev_t     st_dev;     /* ID of device containing file */
//	   ino_t     st_ino;     /* inode number */
//	   mode_t    st_mode;    /* protection */
//	   nlink_t   st_nlink;   /* number of hard links */
//	   uid_t     st_uid;     /* user ID of owner */
//	   gid_t     st_gid;     /* group ID of owner */
//	   dev_t     st_rdev;    /* device ID (if special file) */
//	   off_t     st_size;    /* total size, in bytes */
//	   blksize_t st_blksize; /* blocksize for file system I/O */
//	   blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
//	   time_t    st_atime;   /* time of last access */
//	   time_t    st_mtime;   /* time of last modification */
//	   time_t    st_ctime;   /* time of last status change */
//	};
	struct stat file_stat;
	if (stat(filename, &file_stat) < 0)
	{
		perror("test58:stat1");
		return -1;
	}
	printf("1 file_stat.st_size=%lld\n", (long long)file_stat.st_size);

	if (file_stat.st_size > 30)
	{
		// rename
		if (rename(filename, filename2) < 0)
		{
			perror("test58:rename");
			return -1;
		}
		if (stat(filename2, &file_stat) < 0)
		{
			perror("test58:stat2");
			return -1;
		}
		printf("2 file_stat.st_size=%lld\n", (long long)file_stat.st_size);

		// remove
		if (remove(filename2) < 0)
		{
			perror("test58:remove");
			return -1;
		}
		if (stat(filename2, &file_stat) < 0)
		{
			perror("test58:stat3");
			return -1;
		}
		printf("3 file_stat.st_size=%lld\n", (long long)file_stat.st_size);
	}

	// access not exists file
	if (access(filename3, F_OK) == 0)
	{
		if (remove(filename3) < 0)
		{
			perror("test58:remove not exists");
			return -1;
		}
	}
	else
	{
		printf("access errno=%d\n", errno);
	}

	return 0;
}

int test59()
{
	int ret = system(NULL);
	if (ret != 0)
	{
		printf("do shell command\n");
		system("date >> date.txt; sleep 2");
	}
	return 0;
}

#define SEC_YEAR 60 * 60 * 24 * 365UL

int test60()
{
	unsigned long a = SEC_YEAR;
	printf("a=%lu\n", a);
	
	return 0;
}

int test61()
{
	int64_t uuid = 0;
	int32_t charId = 0;
	int32_t guildId = 0;

	charId = 3;
	guildId = 4;

	uuid = guildId;
	uuid = (uuid << 32);
	uuid += charId;

	// or
	// uuid = (((int64_t)guildId) << 32) + charId;
	
	printf("uuid=%lld\n", (long long)uuid);

	int32_t tmp = 0;
	tmp = 1 << 1;
	printf("tmp=%d\n", tmp);
	tmp = 1 << 22;
	printf("tmp=%d\n", tmp);
	tmp = 1 << 23;
	printf("tmp=%d\n", tmp);
	tmp = 1 << 24;
	printf("tmp=%d\n", tmp);
	tmp = (1 << 24) - 1;
	printf("tmp=%d\n", tmp);

	tmp = (1 << 8) + 1;
	printf("tmp=%d\n", tmp);
	tmp %= 256;
	printf("tmp=%d\n", tmp);

	tmp = (1 << 8) - 1;
	printf("tmp=%d\n", tmp);
	tmp %= 256;
	printf("tmp=%d\n", tmp);

	return 0;
}

#define BITS_PER_WORD (CHAR_BIT * sizeof(unsigned int))
#define I_WORD(i) ((unsigned int)(i) / BITS_PER_WORD)
#define I_BIT(i) (1 << ((unsigned int)(i) % BITS_PER_WORD))
typedef struct NumArray
{
	int size;
	unsigned int value[1];
} NumArray;

int get_bit(const NumArray *ptr, int index)
{
	if (index >= ptr->size)
	{
		return -1;
	}

	return ptr->value[I_WORD(index)] & I_BIT(index);
}

int set_bit(NumArray *ptr, int index, int flag)
{
	if (index >= ptr->size)
	{
		return -1;
	}

	if (flag)
	{
		ptr->value[I_WORD(index)] |= I_BIT(index);
	}
	else
	{
		ptr->value[I_WORD(index)] &= ~I_BIT(index);
	}
	return 0;
}

int test62()
{
	printf("BITS_PER_WORD=%lu\n", BITS_PER_WORD);

	const int MAX_SIZE = 64;
	int mem_size = sizeof(NumArray) + I_WORD(MAX_SIZE-1) + sizeof(unsigned int);
	NumArray *ptr = calloc(1, mem_size);
	ptr->size = MAX_SIZE;

	for (int i = 0; i < ptr->size; i++)
	{
		printf("%c", get_bit(ptr, i) ? '1' : '0');
	}
	printf("\n");

	int index = 0;
	int flag = 0;

	index = 0;
	flag = 1;
	set_bit(ptr, index, flag);

	index = 1;
	flag = 1;
	set_bit(ptr, index, flag);

	index = 0;
	flag = get_bit(ptr, index);
	printf("index=%d flag=%c\n", index, flag ? '1' : '0');
	
	index = 1;
	flag = get_bit(ptr, index);
	printf("index=%d flag=%c\n", index, flag ? '1' : '0');
	
	index = 2;
	flag = get_bit(ptr, index);
	printf("index=%d flag=%c\n", index, flag ? '1' : '0');
	
	index = 1;
	flag = 0;
	set_bit(ptr, index, flag);
	
	index = MAX_SIZE-1;
	flag = 1;
	set_bit(ptr, index, flag);
	
	index = MAX_SIZE;
	flag = 1;
	set_bit(ptr, index, flag);

	for (int i = 0; i < ptr->size; i++)
	{
		printf("%c", get_bit(ptr, i) ? '1' : '0');
	}
	printf("\n");


	free(ptr);

	return 0;
}

int get_power(int flag, int type, int tmax)
{
	return flag / (int)pow(10, tmax - 1 - type) % 2;
}

int test63()
{

	int flag = 0;
	int power = 0;
	int type = 1; 
	int tmax = 4;

	flag = 0;
	printf("flag=%.3d\n", flag);
	for (int i = 1; i < tmax; i++)
	{
		power = get_power(flag, i, tmax);
		printf("type=%d power=%d\n", i, power);
	}

	flag = 1;
	printf("flag=%.3d\n", flag);
	for (int i = 1; i < tmax; i++)
	{
		power = get_power(flag, i, tmax);
		printf("type=%d power=%d\n", i, power);
	}

	flag = 10;
	printf("flag=%.3d\n", flag);
	for (int i = 1; i < tmax; i++)
	{
		power = get_power(flag, i, tmax);
		printf("type=%d power=%d\n", i, power);
	}

	flag = 100;
	printf("flag=%.3d\n", flag);
	for (int i = 1; i < tmax; i++)
	{
		power = get_power(flag, i, tmax);
		printf("type=%d power=%d\n", i, power);
	}

	flag = 11;
	printf("flag=%.3d\n", flag);
	for (int i = 1; i < tmax; i++)
	{
		power = get_power(flag, i, tmax);
		printf("type=%d power=%d\n", i, power);
	}

	flag = 100;
	printf("flag=%.3d\n", flag);
	for (int i = 1; i < tmax; i++)
	{
		power = get_power(flag, i, tmax);
		printf("type=%d power=%d\n", i, power);
	}

	flag = 101;
	printf("flag=%.3d\n", flag);
	for (int i = 1; i < tmax; i++)
	{
		power = get_power(flag, i, tmax);
		printf("type=%d power=%d\n", i, power);
	}

	flag = 110;
	printf("flag=%.3d\n", flag);
	for (int i = 1; i < tmax; i++)
	{
		power = get_power(flag, i, tmax);
		printf("type=%d power=%d\n", i, power);
	}

	flag = 111;
	printf("flag=%.3d\n", flag);
	for (int i = 1; i < tmax; i++)
	{
		power = get_power(flag, i, tmax);
		printf("type=%d power=%d\n", i, power);
	}

	return 0;
}

int test64()
{
	{
		char msg[] = "12345";
		printf("strlen=%zu size=%zu\n", strlen(msg), sizeof(msg));
	}
	{
		char *msg = "12345"; // "12345" in data segement
		printf("strlen=%zu size=%zu\n", strlen(msg), sizeof(msg));
		// msg[0] = 'a'; // error, change data segement const value
	}

	return 0;
}

void LogDebug(const char *fmt, ...)
{
	enum {MAX_LOG_BUFFER_SIZE = 1024};
	char buffer[MAX_LOG_BUFFER_SIZE];

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buffer, MAX_LOG_BUFFER_SIZE, fmt, ap);
	va_end(ap);

	printf("%s\n", buffer);
}

int test65()
{
	LogDebug("%s hello %d", __FUNCTION__, 10);
	return 0;
}


/*
void print_func_info(const char *head, int line, const char *fmt, va_list ap)
{
	enum {MAX_LOG_BUFFER_SIZE = 1024};
	char buffer[MAX_LOG_BUFFER_SIZE];
	vsnprintf(buffer, MAX_LOG_BUFFER_SIZE, fmt, ap);

	printf("%s[%d]:%s\n", head, line, buffer);
}
*/

void print_func_info(const char *head, int line, const char *fmt, ...)
{
	enum {MAX_LOG_BUFFER_SIZE = 1024};
	char buffer[MAX_LOG_BUFFER_SIZE] = {0};

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buffer, MAX_LOG_BUFFER_SIZE, fmt, ap);
	va_end(ap);

	printf("%s[%d]:%s\n", head, line, buffer);
}

// #define PRINT_FUNC_INFO(fmt, arg...) print_func_info(__FUNCTION__, __LINE__, fmt, ##arg)
#define PRINT_FUNC_INFO(fmt, ...) print_func_info(__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

int test66()
{

	PRINT_FUNC_INFO("hello");
	PRINT_FUNC_INFO("hello %d", 123);

	return 0;
}

int test67()
{
	/*
	int ret;
	ret = 0;
	*/

	printf("\033[0;32;31m RED \033[m \n");
	printf("\033[1;31m LIGHT_RED \033[m \n");
	printf("\033[0;32;32m GREEN \033[m \n");
	printf("\033[1;32m LIGHT_GREEN \033[m \n");
	printf("\033[1;33m YELLOW \033[m \n");

	return 0;
}

void reserver_str(char *str)
{
	int len = strlen(str);
	for (int i=0; i<len/2; i++)
	{
		char c = str[i];
		str[i] = str[len-1-i];
		str[len-1-i] = c;
	}
}

char * sum_str(const char *in1, const char *in2)
{
	if (!in1 || !in2)
	{
		return NULL;
	}

	char *str1 = calloc(strlen(in1)+1, sizeof(char));
	char *str2 = calloc(strlen(in2)+1, sizeof(char));
	strcpy(str1, in1);
	strcpy(str2, in2);

	char *abs_str1 = str1;
	char *abs_str2 = str2;

	// check op
	char *a = NULL;
	char *b = NULL;
	// a must >= b
	bool do_add = true;		
	bool is_minus = false;
	bool is_zero = false;

	if (str1[0] != '-' && str2[0] != '-') 
	{
		do_add = true;		
		is_minus = false;
		a = str1;
		b = str2;
		if (strlen(a) < strlen(b))
		{
			char *t = a;
			a = b;
			b = t;
		}
	}
	else if (str1[0] == '-' && str2[0] != '-') 
	{
		a = str1+1;
		b = str2;
		if (strlen(a) < strlen(b))
		{
			char *t = a;
			a = b;
			b = t;
			do_add = false;
			is_minus = false;
		}
		else if (strcmp(a, b) == 0)
		{
			is_zero = true;
		}
		else if (strcmp(a, b) > 0)
		{
			do_add = false;
			is_minus = true;
		}
		else if (strcmp(a, b) < 0)
		{
			char *t = a;
			a = b;
			b = t;
			do_add = false;
			is_minus = false;
		}

	}
	else if (str1[0] != '-' && str2[0] == '-')
	{
		a = str1;
		b = str2+1;
		if (strlen(a) < strlen(b))
		{
			char *t = a;
			a = b;
			b = t;
			do_add = false;
			is_minus = true;
		}
		else if (strcmp(a, b) == 0)
		{
			is_zero = true;
		}
		else if (strcmp(a, b) > 0)
		{
			do_add = false;
			is_minus = false;
		}
		else if (strcmp(a, b) < 0)
		{
			char *t = a;
			a = b;
			b = t;
			do_add = false;
			is_minus = true;
		}
	}
	else if (str1[0] == '-' && str2[0] == '-')
	{
		do_add = true;
		is_minus = true;
		a = str1+1;
		b = str2+1;
		if (strlen(a) < strlen(b))
		{
			char *t = a;
			a = b;
			b = t;
		}
	}


	if (is_zero)
	{
		char *out_str = calloc(2, sizeof(char));
		out_str[0] = '0';
		return out_str;
	}

	printf("a=%s\n", a);
	printf("b=%s\n", b);
	printf("do_add=%d\n", do_add);
	printf("is_minus=%d\n", is_minus);

	int a_len = strlen(a);
	int b_len = strlen(b);

	reserver_str(a);
	reserver_str(b);


	// do merge a and b to out_str
	char *out_str = calloc(a_len+3, sizeof(char));
	short offset = 0;
	int i = 0;
	for (; i < b_len; i++)
	{
		if (do_add)
		{
			short t = 0;
			if (offset == 1)
			{
				t = a[i] + b[i] - '0' - '0' + 1;
				offset = 0;
			}
			else
			{
				t = a[i] + b[i] - '0' - '0';
			}
			if (t > 9)
			{
				offset = 1;
				t = t - 10;
			}
			out_str[i] = t + '0';
		}
		else
		{
			short t = 0;
			if (offset == -1)
			{
				t = a[i] - b[i] - 1;
				offset = 0;
			}
			else
			{
				t = a[i] - b[i];
			}
			if (t < 0)
			{
				offset = -1;
				t = t + 10;
			}
			out_str[i] = t + '0';
		}
	}

	// do merge a to out_str
	for (; i < a_len; i++)
	{
		if (do_add)
		{
			short t = 0;
			if (offset == 1)
			{
				t = a[i] - '0' + 1;
				offset = 0;
			}
			else
			{
				t = a[i] - '0';
			}
			if (t > 9)
			{
				offset = 1;
				t = t - 10;
			}
			out_str[i] = t + '0';
		}
		else
		{
			short t = 0;
			if (offset == -1)
			{
				t = a[i] - '0' - 1;
				offset = 0;
			}
			else
			{
				t = a[i] - '0';
			}
			if (t < 0)
			{
				offset = -1;
				t = t + 10;
			}
			out_str[i] = t + '0';
		}
	}

	// this is impossible
	assert(offset != -1);

	// fix head num and flag
	if (offset == 1)
	{
		out_str[i] = '1';
		++i;
	}
	if (is_minus)
	{
		out_str[i] = '-';	
	}

	reserver_str(out_str);

	free(str1);
	free(str2);
	return out_str;
}

int test68()
{
	{
		char str1[] = "123";
		char str2[] = "456";
		char *out = sum_str(str1, str2);
		printf("str1=%s str2=%s out=%s\n", str1, str2, out);
		free(out);
		printf("\n");
	}
	{
		char str1[] = "1234";
		char str2[] = "456";
		char *out = sum_str(str1, str2);
		printf("str1=%s str2=%s out=%s\n", str1, str2, out);
		free(out);
		printf("\n");
	}
	{
		char str1[] = "345";
		char str2[] = "12345";
		char *out = sum_str(str1, str2);
		printf("str1=%s str2=%s out=%s\n", str1, str2, out);
		free(out);
		printf("\n");
	}

	{
		char str1[] = "-456";
		char str2[] = "12345";
		char *out = sum_str(str1, str2);
		printf("str1=%s str2=%s out=%s\n", str1, str2, out);
		free(out);
		printf("\n");
	}

	{
		char str1[] = "-456";
		char str2[] = "-12345";
		char *out = sum_str(str1, str2);
		printf("str1=%s str2=%s out=%s\n", str1, str2, out);
		free(out);
		printf("\n");
	}

	{
		char str1[] = "-456";
		char str2[] = "-999";
		char *out = sum_str(str1, str2);
		printf("str1=%s str2=%s out=%s\n", str1, str2, out);
		free(out);
		printf("\n");
	}

	{
		char str1[] = "-123";
		char str2[] = "123";
		char *out = sum_str(str1, str2);
		printf("str1=%s str2=%s out=%s\n", str1, str2, out);
		free(out);
		printf("\n");
	}

	{
		char str1[] = "-122";
		char str2[] = "123";
		char *out = sum_str(str1, str2);
		printf("str1=%s str2=%s out=%s\n", str1, str2, out);
		free(out);
		printf("\n");
	}

	{
		char str1[] = "-124";
		char str2[] = "123";
		char *out = sum_str(str1, str2);
		printf("str1=%s str2=%s out=%s\n", str1, str2, out);
		free(out);
		printf("\n");
	}

	return 0;
}

int test_tmp()
{
	/*
	int ret;
	ret = 0;
	*/

	return 0;
}

typedef int (*testcase_t) ();
testcase_t test_list[] = 
{
	test0
,	test1
,	test2
,	test3
,	test4
,	test5
,	test6
,	test7
,	test8
,	test9
,	test10
,	test11
,	test12
,	test13
,	test14
,	test15
,	test16
,	test17
,	test18
,	test19
,	test20
,	test21
,	test22
,	test23
,	test24
,	test25
,	test26
,	test27
,	test28
,	test29
,	test30
,	test31
,	test32
,	test33
,	test34
,	test35
,	test36
,	test37
,	test38
,	test39
,	test40
,	test41
,	test42
,	test43
,	test44
,	test45
,	test46
,	test47
,	test48
,	test49
,	test50
,	test51
,	test52
,	test53
,	test54
,	test55
,	test56
,	test57
,	test58
,	test59
,	test60
,	test61
,	test62
,	test63
,	test64
,	test65
,	test66
,	test67
,	test68
};

int main(int argc, char * argv[]) 
{
	int ret;
	int testcase;
	int maxcase;

	ret = 0;

	maxcase = sizeof(test_list) / sizeof(testcase_t);
	testcase = maxcase - 1;
	if (argc > 1) {
		// run all test case
		if (!strcmp(argv[1], "all")) {
			printf("stu:run_all_case\n");
			for (int i=0; i<maxcase; i++) {
				printf("stu:RUN testcase[%d]\n", i);
				ret = test_list[i]();
				if (ret != 0) {
					printf("stu:case[%d] %d\n", i, ret);	
					return 0;
				}
			}
			return 0;
		}

		testcase = atoi(argv[1]);
		if (testcase < 0 || testcase >= maxcase) {
			testcase = maxcase - 1;
		}
	}

	printf("stu:RUN testcase[%d]\n", testcase);
	ret = test_list[testcase]();
	if (ret != 0) {
		printf("stu:case[%d] %d\n", testcase, ret);	
	}

	return ret;
}
