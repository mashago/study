#include <limits.h>
#include <iostream>
#include <cstring> // type in c++
// #include <string.h> // type in c
#include <ctime>
#include <cmath>
#include <sys/time.h>
#include <unistd.h>
#include <malloc.h>
#include <stdarg.h>

#include <fstream>
#include <new>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <queue>
#include <list>

#include "stu.h"
#include "stock.h"
#include "counter.h"
#include "tc_a.h"
#include "util.h"


#if (defined __WIN32) || (defined __CYGWIN__)
#define bzero(a, b) memset(a, 0, b)
#endif

using namespace std; // namespace type 1 
int test0()
{
	// using namespace std; // namespace type 2
	// using std::cout; // namespace type 3
	// using std::endl;

	cout << "/*";
	cout << "*/";

	cout << "hello world";
	// std::cout << "hello world"; // namespace type 4
	cout << endl;

	int num = 10;
	// cin >> num;
	cout << "num=" << num << endl;
	return 0;
}

int test1() {

	char nnn = 'n';

	cout.put(nnn);
	cout.put('!');

	cout << endl;

	
	return 0;
}

int test2() {
	// int ret;

	/*
	char s1[50];
	char s2[50];

	// cin >> s1;
	cout << "input s1" << endl;
	cin.getline(s1, 49);
	cout << "test2:s1=" << s1 << endl;
	cout << "test2:strlen(s1)=" << strlen(s1) << endl;

	cout << "input s1" << endl;
	cin.getline(s1, 49);
	cout << "test2:s1=" << s1 << endl;
	cout << "test2:strlen(s1)=" << strlen(s1) << endl;

	cout << "input s1" << endl;
	cin.get(s1, 20).get();
	cout << "test2:s1=" << s1 << endl;
	cout << "test2:strlen(s1)=" << strlen(s1) << endl;

	cout << "input s2" << endl;
	cin.get(s2, 20);
	cout << "test2:s2=" << s2 << endl;
	cout << "test2:strlen(s2)=" << strlen(s2) << endl;
	*/


	// ret = 0;
	return 0;
}

int test3() {
	// int ret;

	string s1;
	string s2;
	string s3;
	string s4;

	/*
	cout << "input s1" << endl;
	cin >> s1;
	cout << "test3:s1=" <<  s1 << endl;
	cout << "test3:s1[1]=" <<  s1[1] << endl;
	*/

	s1 = "qwerty";
	s2 = s1;
	cout << "test3:1 s1=" <<  s1 << " s2=" << s2 << endl;

	s1 = "123456";
	cout << "test3:2 s1=" <<  s1 << " s2=" << s2 << endl;

	s3 = s1 + s2;
	cout << "test3:s3=" << s3 << endl;
	cout << "test3:s3.size()=" << s3.size() << endl;

	cout << "input s4" << endl;
	getline(cin, s4);
	cout << "test3:s4=" << s4 << endl;


	// ret = 0;
	return 0;
}

int test4() {
	// int ret;

	struct c_struct {
		int id;
		char name[20];
	};
	struct c_struct s1;
	s1.id = 20;
	sprintf(s1.name, "masha");
	cout << "test4:s1.id=" << s1.id << " s1.name=" << s1.name << endl;

	struct cpp_struct {
		int id;
		string name;
	};
	cpp_struct s2; // c++ can ignore struct
	s2.id = 30;
	s2.name = "john";
	cout << "test4:s2.id=" << s2.id << " s2.name=" << s2.name << endl;

	cpp_struct s3;
	s3 = s2; // work in c also
	cout << "test4:s3.id=" << s3.id << " s3.name=" << s3.name << endl;

	// ret = 0;
	return 0;
}

int test5() {
	// int ret;

	int * ptr1 = new int;
	*ptr1 = 10;
	cout << "test5:*ptr1=" << *ptr1 << endl;
	delete ptr1; // pair use "new" & "delete"

	int * ptr2 = new int[5];
	ptr2[0] = 10;
	int * ptr3 = ptr2+1;
	*ptr3 = 20;
	ptr2[2] = 30;
	cout << "test5:ptr2[0]=" << ptr2[0] 
	<< " ptr[1]=" << ptr2[1] 
	<< " ptr[2]=" << ptr2[2] 
	<< endl;
	
	delete [] ptr2;

	// ret = 0;
	return 0;
}

int test6() {
	// int ret;

	struct my_struct {
		int id;
		string name;
	};

	my_struct * ptr = new my_struct;
	ptr->id = 20;
	ptr->name = "masha";

	cout << "test6:id=" << ptr->id << " name=" << ptr->name << endl;

	delete ptr;

	// ret = 0;
	return 0;
}

int test7() {
	// int ret;

	float sec;
	cout << "test7: input sec" << endl;
	cin >> sec;
	clock_t delay;
	delay = sec * CLOCKS_PER_SEC;
	cout << "test7:delay=" << delay << endl;
	clock_t start = clock();
	while (clock() - start < delay) {	
		continue;
	}
	cout << "test7:done" << endl;

	// ret = 0;
	return 0;
}

int test8() {
	// int ret;

	char c;
	/*
	cin.get(c);
	while (c != EOF) {
		cout << c;
		cin.get(c);
	}
	*/

	c = cin.get();
	// while (c != EOF) {
	// while (c != '#') {
	// while (!cin.fail()) {
	while (cin) {
		cout << c;
		c = cin.get();
	}

	cout << endl;

	// ret = 0;
	return 0;
}

int test9() {
	// int ret;

	ofstream out_file;
	out_file.open("file.txt");

	out_file << "hello world" << endl;

	out_file.close();

	char c;
	ifstream in_file;
	in_file.open("file.txt");
	if (in_file.is_open()) {
		cout << "test9:in_file_open" << endl;
		// in_file >> c;
		c = in_file.get();
		while (in_file.good()) {
			cout << c;
			// in_file >> c;
			c = in_file.get();
		}
	}
	cout << endl;

	in_file.close();


	// ret = 0;
	return 0;
}

int sum_array(int array[][4], int size) 
{
	int ret;
	
	ret = 0;
	for (int i=0; i<size; i++) {
		for (int j=0; j<4; j++) {
			ret += array[i][j];
		}
	}
	return ret;
}
	

int test10() {
	int ret = 0;

	int array[2][4] = {{1, 1, 1, 1}, {2, 2, 2, 2}};

	ret = sum_array(array, 2);
	cout << "test10:ret=" << ret << endl;

	ret = 0;
	return 0;
}

int func11(int a)
{
	cout << "func11:a=" << a << endl;
	return a;
}

int test11() {
	// int ret;

	int (*pfunc)(int);
	pfunc = func11;
	pfunc(5);
	(*pfunc)(10); // the same


	// ret = 0;
	return 0;
}

inline int func12(int x) { return x * x;}
#define FUNC_SQUARE(x) x*x

int test12() {
	int ret;

	int x = 3;
	ret = func12(x);
	cout << "test12:ret1=" << ret << endl;
	
	x = 3;
	ret = FUNC_SQUARE(x);
	cout << "test12:ret2=" << ret << endl;


	ret = 0;
	return 0;
}

int func13(int & x) {
	x *= x;
	return x;
}

struct dog {
	int id;
	string name;
};

// dog & func13_dog(dog & d) {
const dog & func13_dog(dog & d) {
	d.id = 5;
	d.name = "small";
	return d;
}
int test13() {
	// int ret;

	int a;
	int & b = a;
	a = 10;
	cout << "test13:a=" << a << " b=" << b << endl;

	a = 20;
	cout << "test13:a=" << a << " b=" << b << endl;

	b = 30;
	cout << "test13:a=" << a << " b=" << b << endl;

	int x = 4;
	func13(x);
	cout << "test13:x=" << x << endl;

	/*
	// error
	long y = 4;
	func13(y);
	cout << "test13:y=" << y << endl;
	*/

	dog d1;
	d1.id = 10;
	d1.name = "bigbig";
	cout << "test13:d1 1 id=" << d1.id << " name=" << d1.name << endl;
	func13_dog(d1);
	cout << "test13:d1 2 id=" << d1.id << " name=" << d1.name << endl;

	dog d2 = func13_dog(d1);
	cout << "test13:d2 1 id=" << d2.id << " name=" << d2.name << endl;
	d2.id = 30;
	d2.name = "middle";
	// func13_dog(d1).id = 5; // func return const avoid this case
	cout << "test13:d1 3 id=" << d1.id << " name=" << d1.name << endl;
	cout << "test13:d2 3 id=" << d2.id << " name=" << d2.name << endl;


	// ret = 0;
	return 0;
}

int func14(int a, int b=10) {
	return a*b;
}

int test14() {
	int ret;

	int a;
	int b;
	a = 2;
	b = 3;

	ret = func14(a, b);
	cout << "test14:ret 1=" << ret << endl;

	ret = func14(a);
	cout << "test14:ret 2=" << ret << endl;

	ret = 0;
	return 0;
}

template <class Any>
int func15(Any &a, Any &b)
{

	Any tmp;
	tmp = a;
	a = b;
	b = tmp;
	return 0;
}

struct struct_15 {
	int id;
	string name;
};

template <> int func15<struct_15> (struct_15 &, struct_15 &);
template <> int func15<struct_15> (struct_15 &a, struct_15 &b)
{
	int tmp;
	tmp = a.id;
	a.id = b.id;
	b.id = tmp;
	return 0;
}

int test15() {
	// int ret;

	int a = 5;
	int b = 10;
	func15(a, b);
	printf("test15:a=%d b=%d\n", a, b);

	string c = "qwerty";
	string d = "123456";
	func15(c, d);
	printf("test15:c=%s d=%s\n", c.c_str(), d.c_str());

	struct struct_15 e;
	struct struct_15 f;

	e.id = 5;
	e.name = "masha";
	f.id = 10;
	f.name = "kelton";
	func15(e, f);
	printf("test15:e=%d %s f=%d %s\n", e.id, e.name.c_str()
	, f.id, f.name.c_str());

	// ret = 0;
	return 0;
}

int test16() {
	// int ret;

	// in stu2.cpp
	stu2_print();

	// ret = 0;
	return 0;
}

namespace MashaSpace {
	void masha_print() {
		cout << "hello mashaspace" << endl;
	}
}
void masha_print() {
	cout << "local masha print" << endl;
}

int test17() {
	// int ret;
	
	MashaSpace::masha_print();
	masha_print();
	
	// ret = 0;
	return 0;
}

int test18() {
	// int ret;

	const char * company1 = "PPC";
	const char * company2 = "ABC";
	const char * company3 = "PICC";
	const char * company4 = "QWE";
	const char * company5 = "RTY";
	const char * company6 = "MASHA";

	Stock s1;
	s1.init(company1, 1, 2.0, 5);
	s1.show();

	Stock s2 = Stock(company2, 2, 3.0, 6);
	s2.show();

	Stock *s3;
	s3 = new Stock(company3, 3, 4.0, 10);
	s3->show();
	delete s3;

	const Stock s4;
	s4.show_const();

	Stock stock_list1[2];
	stock_list1[0].show();

	Stock stock_list2[2] = 
	{
		Stock(company4, 1, 2.0, 5)
		, Stock(company5, 2, 3.0, 6)
	};
	stock_list2[0].show();

	Stock s5 = Stock(company6, 2, 3.0, 6);
	s5.show(s5);
	

	// ret = 0;
	return 0;
}


int test19() {
	// int ret;

	Counter c1;
	c1.show();
	c1.set_num(10);
	c1.show();

	Counter c2;
	c2.set_num(20);
	
	Counter c3 = c1.sum(c2);
	c3.show();
	
	Counter c4 = c1.operator+(c2);
	c4.show();
	
	// operator+
	Counter c5 = c1 + c2;
	c5.show();
	
	// friend
	Counter c6 = operator*(c1, 5);
	c6.show();

	// use Counter(int num);
	Counter c7 = 60;
	c7.show();

	Counter c8(70);
	c8.show();

	printf("\n");

	// operator int() const
	int num8 = c8;
	printf("num8=%d\n", num8);

	printf("use Counter(const Counter &c)\n");
	Counter c9 = c8;
	c9.show();

	printf("\n");

	printf("use operator=\n");
	Counter c10;
	c10 = c9;
	c10.show();


	printf("call static function\n");
	int flag;
	flag = Counter::get_flag();
	printf("flag1=%d\n", flag);
	Counter::flag = 10;
	flag = Counter::get_flag();
	printf("flag2=%d\n", flag);
	Counter c11;
	c11.flag = 20;
	flag = Counter::get_flag();
	printf("flag3=%d\n", flag);

	

	// ret = 0;
	return 0;
}

int test20() 
{
	// int ret;

	char *buffer = new char[512];

	printf("test20:sizeof(TC_A)=%zu sizeof(int)=%zu sizeof(string)=%zu\n"
	, sizeof(TC_A), sizeof(int), sizeof(string));

	TC_A * a1;
	TC_A * a2;
	TC_A * a3;
	TC_A * a4;
	a1 = new TC_A();
	a1->show();

	a2 = new (buffer) TC_A(20, "masha"); // new tc_a in buffer
	a2->show();

	printf("test20:address1 buffer=%p a1=%p a2=%p\n"
	, buffer, a1, a2);

	// a3 will cover a2 in buffer
	a3 = new (buffer) TC_A(30, "qwert");
	a3->show();
	a2->show();

	printf("test20:address2 buffer=%p a1=%p a2=%p a3=%p\n"
	, buffer, a1, a2, a3);

	a4 = new (buffer+sizeof(TC_A)) TC_A(40, "final");
	a4->show();
	a3->show();

	printf("test20:address3 buffer=%p a1=%p a2=%p a3=%p a4=%p\n"
	, buffer, a1, a2, a3, a4);
	printf("test20:len=%zu\n", a4 - a3);


	delete a1;
	a3->~TC_A();
	a4->~TC_A();
	delete [] buffer; // will not call ~TC_A()

	// ret = 0;
	return 0;
}

class Father_class
{
private:
	int id;
	string name;
public:
	virtual ~Father_class() {};
	Father_class();
	Father_class(int id, string name);
	int show();
	int update_id(int id);

	virtual void print1(); // son class will cover this function
	void print2();
};

class Son_class: public Father_class
{
private:
	int ext;
public:
	Son_class();
	Son_class(int ext, int id, string name);
	int set_ext(int ext);

	void print1();
	void print2();
};

Father_class::Father_class()
{
	cout << "call Father_class()" << endl;
	this->id = 0;
	this->name = "no_name";
}

Father_class::Father_class(int id, string name)
{
	cout << "call Father_class(id, name)" << endl;
	this->id = id;
	this->name = name;
}

int Father_class::show()
{
	cout << "show:id=" << this->id << " name=" << this->name << endl;
	return 0;
}

int Father_class::update_id(int id)
{
	this->id = id;
	return id;
}

void Father_class::print1()
{
	cout << "Father_class::print1" << endl;
}

void Father_class::print2()
{
	cout << "Father_class::print2" << endl;
}

Son_class::Son_class()
{
}

Son_class::Son_class(int ext, int id, string name): Father_class(id, name)
{
	this->ext = ext;
}

int Son_class::set_ext(int ext)
{
	this->ext = ext;
	return ext;
}

void Son_class::print1()
{
	cout << "Son_class::print1" << endl;
}

void Son_class::print2()
{
	cout << "Son_class::print2" << endl;
}


int test21() 
{
	// int ret;

	Father_class f1 = Father_class();
	f1.show();
	Father_class f2 = Father_class(10, "masha");
	f2.show();
	cout << endl;

	Son_class s1 = Son_class();
	s1.show();
	Son_class s2 = Son_class(1, 20, "hello");
	s2.show();
	cout << endl;

	Son_class s3 = Son_class(2, 30, "haha");
	s3.update_id(55);
	s3.show();
	Father_class *p1 = &s3;
	p1->update_id(44);
	p1->show();
	cout << endl;

	Father_class *p2 = new Father_class(); // 
	p2->print1();
	p2->print2();
	Son_class *p3 = new Son_class(); // 
	p3->print1();
	p3->print2();
	Father_class *p4 = new Son_class(); // 
	p4->print1(); // print1() is father virtual function, will call son print1()
	p4->print2();
	delete p2;
	delete p3;
	delete p4;



	// ret = 0;
	return 0;
}


class Abstract_father
{
private:
public:
	Abstract_father() {};
	virtual ~Abstract_father() {};

	virtual void func1() { printf("call Abstract_father::func1\n"); }; // normal virtual function
	virtual void func2() = 0; // pure virtual function, class become abstract class, cannot new this class, only can new its children class
	void func3() { printf("call Abstract_father::func3\n"); }; 
};

class Abstract_son: public Abstract_father
{
private:
public:
	Abstract_son() {};
	~Abstract_son() {};

	void func1() { printf("call Abstract_son::func1\n"); };
	void func2() { printf("call Abstract_son::func2\n"); };
	void func3() { printf("call Abstract_son::func3\n"); }; 
};
int test22() 
{
	// int ret;

	// Abstract_father af1; // this is a abstract class
	// Abstract_father af2 = new Abstract_father(); // abstract class cannot be create
	Abstract_father *af3 = new Abstract_son();
	af3->func1(); // call son func1
	af3->func2(); // call son func2
	af3->func3(); // call father func1

	Abstract_son *af4 = new Abstract_son();
	af4->func1(); // call son func
	af4->func2(); // call son func
	af4->func3(); // call son func

	delete af3;
	delete af4;


	// ret = 0;
	return 0;
}

int print_list(int *list, int size)
{
	for (int i=0; i<size; i++) {
		printf(" %d", list[i]);
	}
	printf("\n");
	return 0;
}

int quick_sort(int *list, int left, int right)
{
	if (left >= right) {
		return 0;
	}

	int key = list[left];
	int low = left;
	int high = right;
	printf("start:key=[%d] low=[%d] high=[%d]\n", key, low, high);
	print_list(list, 5);
	while (low < high) {
		while (low < high && list[high] > key) {
			high--;
			printf("key=[%d] low=[%d] high=[%d]\n", key, low, high);
			print_list(list, 5);
		}
		list[low] = list[high];
		printf("key=[%d] low=[%d] high=[%d]\n", key, low, high);
		print_list(list, 5);
		while (low < high && list[low] < key) {
			low++;
			printf("key=[%d] low=[%d] high=[%d]\n", key, low, high);
			print_list(list, 5);
		}
		list[high] = list[low];
		printf("key=[%d] low=[%d] high=[%d]\n", key, low, high);
		print_list(list, 5);
	}
	list[low] = key;

	quick_sort(list, left, low-1);
	quick_sort(list, low+1, right);

	return 0;
}

int test23() 
{
	// int ret;

	int list[5];
	list[0] = 10;
	list[1] = 8;
	list[2] = 9;
	list[3] = 12;
	list[4] = 7;

	/*
	10 8 9 12 7
	->
	key = 10, low = 0, high = 4, list = 10 8 9 12 7
	->
	7 8 9 12 7, low = 0, high = 4
	->
	7 8 9 12 12, low = 3, high = 4
	->
	7 8 9 12 12, low = 3, high = 3
	->
	7 8 9 10 12, low = 3, high = 3
	*/

	quick_sort(list, 0, 4);
	for (int i=0; i<5; i++) {
		printf(" %d", list[i]);
	}
	printf("\n");

	// ret = 0;
	return 0;
}


int test24() 
{
	// int ret;
	const int size = 10;
	char input1[size] = {0};
	char c1 = '\0';

	char input2[size] = {0};
	char input3[size] = {0};
	char c3 = 'a';

	char input4[size] = {0};
	char c4 = 'a';

	string input5;

	cout << "write input1\n";
	cin >> input1;
	c1 = cin.get();
	cout << "input1=" << input1 << endl;
	cout << "c1=" << c1 << endl;

	cout << "write input2\n";
	cin.getline(input2, size);
	cout << "input2=" << input2 << endl;

	cout << "write input3\n";
	cin.get(input3, size);
	cin.get(c3);
	cout << "input3=" << input3 << endl;
	cout << "c3=" << c3 << endl;

	cout << "write input4\n";
	cin.get(input4, size);
	cin.get(c4);
	cout << "input4=" << input4 << endl;
	cout << "c4=" << c4 << endl;


	cout << "write input5\n";
	getline(cin, input5);
	cout << "input5=" << input5 << " size=" << input5.size()<< endl;

	// ret = 0;
	return 0;
}


typedef struct {
	int total;
} job_t;

// template <class T> // use typename or class
template <typename T> 
void Swap(T &a, T &b);

// template function
template <typename T> 
void Swap(T &a, T &b)
{
	cout << "use normal template function" << endl;
	T tmp;
	tmp = a;
	a = b;
	b = tmp;
}

// explicit specialization template function
template <> 
void Swap<job_t>(job_t &a, job_t &b)
{
	cout << "use explicit specialization template function" << endl;
	int tmp;
	tmp = a.total;
	a.total = b.total;
	b.total = tmp;
}

// explicit instantiation template
template void Swap<char>(char &, char &);	

int test25() 
{
	// int ret;

	int a1 = 20;
	int b1 = 30;
	cout << "before swap a1=" << a1 << " b1=" << b1 << endl;
	Swap(a1, b1); // core logic
	cout << "after swap a1=" << a1 << " b1=" << b1 << endl << endl;

	double a2 = 20;
	double b2 = 30;
	cout << "before swap a2=" << a2 << " b2=" << b2 << endl;
	Swap(a2, b2);
	cout << "after swap a2=" << a2 << " b2=" << b2 << endl << endl;

	job_t a3;
	a3.total = 5;
	job_t b3;
	b3.total = 10;
	cout << "before swap a3=" << a3.total << " b3=" << b3.total << endl;
	Swap(a3, b3);
	cout << "after swap a3=" << a3.total << " b3=" << b3.total << endl << endl;

	char a4 = 'a';
	char b4 = 'b';
	cout << "before swap a4=" << a4 << " b4=" << b4 << endl;
	Swap(a4, b4);
	cout << "after swap a4=" << a4 << " b4=" << b4 << endl << endl;

	// ret = 0;
	return 0;
}

int test26() 
{
	// int ret;

	// extern int eee; // error, symbols not found
	// int eee;
	// eee = 10;

	// ret = 0;
	return 0;
}

// placement new
int test27() 
{
	// int ret;

	const int N = 10;
	const int buffer_size = 100;
	char buffer[buffer_size];

	int *ptr;
	ptr = new (buffer) int[N]; // placement new

	cout << "buffer address:" << (void*) buffer << endl;
	cout << "ptr address:" << ptr << endl;

	// ret = 0;
	return 0;
}

namespace Jack {
	int jacka;
	double jackb;
	void jackprint() {
		cout << "jackprint:hahaha " << jacka << " " << jackb << endl;
	}
}

namespace Tim {
	int tima;
	double timb;
	void timprint() {
		cout << "timprint:yoyoyo " << tima<< endl;
	}
}

namespace Mike {
	int mikea;
	double mikeb;
	void mikeprint() {
		cout << "mikeprint:enenen " << mikea<< endl;
	}
}

using Jack::jackb; // put Jack::jackb to global namespace
using namespace Mike; // use Mike namespace, all param in Mike can use
int test28() 
{
	// int ret;

	Jack::jacka = 20;
	jackb = 100;
	Jack::jackprint();

	using Tim::tima; // put Tim::tima to local namespace
	tima = 10;
	using Tim::timprint; // put Tim::timprint to local namespace
	timprint();

	mikeprint();

	// ret = 0;
	return 0;
}


int shit = 1;
namespace Shit {
	int shit = 2;
}
int test29() 
{
	// int ret;
	cout << "first shit=" << shit << endl; // global shit
	using namespace Shit;

	// cout << "shit=" << shit << endl; // error, compiler don't know use which shit 

	int shit = 3; // local shit, hide other shit

	cout << "local shit=" << shit << endl; // local shit, hide other shit
	cout << "global ::shit=" << ::shit << endl; // global shit
	cout << "Shit::shit=" << Shit::shit << endl; // Shit::shit

	// ret = 0;
	return 0;
}

int test30() 
{
	// int ret;

	{
	Stock s1;
	s1.show();

	Stock s2 = Stock();
	s2.show();

	Stock s3;
	s3 = Stock("haha", 1, 2, 3);
	s3.show();

	}

	// ret = 0;
	return 0;
}

enum class egg {Small, Medium, Large};
enum class tee {Small, Medium, Large, Xlarge};
int test31() 
{
	// int ret;

	using std::cout;
	using std::endl;

	egg size = egg::Small;
	cout << "egg size=" << int(size) << endl;
	cout << "tee::Large=" << int(tee::Large) << endl;


	// ret = 0;
	return 0;
}

int test32() 
{
	using std::cout;
	using std::endl;
	// warning: ISO C++11 does not allow conversion from string literal to 'char *'
	// char * tmp = "abcdefg";
	const char * tmp = "abcdefg";

	cout << "tmp=" << tmp << endl;
	return 0;
}

int test33() 
{
	// int ret;

	printf("[%10d]:[%10d]\n", 11000, 2);

	// ret = 0;
	return 0;
}

int test34() 
{
	// int ret;

	struct timeval tv;    
	gettimeofday(&tv,NULL);    
	printf("1 utime=%ld\n", tv.tv_sec * 1000 + tv.tv_usec / 1000);
	sleep(1);
	gettimeofday(&tv,NULL);    
	printf("2 utime=%ld\n", tv.tv_sec * 1000 + tv.tv_usec / 1000);

	// ret = 0;
	return 0;
}

int test35() 
{
	// int ret;

	std::string s;
	int a = 111;
	long b = 222;
	double c = 333.333;
	s = to_string((long long int)a);
	printf("to_string(111)=%s\n", s.c_str());
	s = to_string((long long int)b);
	printf("to_string(222)=%s\n", s.c_str());
	s = to_string((long double)c);
	printf("to_string(333)=%s\n", s.c_str());




	// ret = 0;
	return 0;
}

int test36() 
{
	// int ret;

	std::vector<int> vec;
	vec.push_back(11);

	auto &vecRef = vec;
	vecRef.push_back(12);

	for (auto iter = vec.begin(); iter != vec.end(); iter++)
	{
		printf("*iter=%d\n", *iter);
	}

	auto a = 1;
	auto b = 2.2;
	decltype(a+b) c = 3.4;

	std::cout << "c=" << c << std::endl;

	int d = 5;
	int *ptr = &d;
	decltype(ptr) e; // e is int *
	e = &d;

	decltype(*ptr) f = d; // f is int &, should init

	d++;
	std::cout << "*e=" << *e << std::endl;
	std::cout << "f=" << f << std::endl;



	// ret = 0;
	return 0;
}

int test37() 
{
	// int ret;

	time_t now = time(NULL);
	std::cout << "now = " << now << std::endl;

	// struct tm :
	// int tm_sec;     /* seconds (0 - 60) */
	// int tm_min;     /* minutes (0 - 59) */
	// int tm_hour;    /* hours (0 - 23) */
	// int tm_mday;    /* day of month (1 - 31) */
	// int tm_mon;     /* month of year (0 - 11) */
	// int tm_year;    /* year - 1900 */
	// int tm_wday;    /* day of week (Sunday = 0) */
	// int tm_yday;    /* day of year (0 - 365) */
	// int tm_isdst;   /* is summer time in effect? */
	// char *tm_zone;  /* abbreviation of timezone name */
	// long tm_gmtoff; /* offset from UTC in seconds */
	// 

	struct tm detail;

	localtime_r(&now, &detail);

	std::cout << "tm_sec = " << detail.tm_sec << std::endl;
	std::cout << "tm_min = " << detail.tm_min << std::endl;
	std::cout << "tm_hour = " << detail.tm_hour << std::endl;
	std::cout << "tm_wday = " << detail.tm_wday << std::endl;

	// ret = 0;
	return 0;
}

int test38() 
{
	int count = 0;

	if (count == 0)
	{
		std::cout << "break1" << std::endl;
		// break; // cannot use break without a loop or switch case
	}

	std::cout << "break2" << std::endl;

	switch (count)
	{
		case 0:
		{
			if (count == 0)
			{
				std::cout << "break3" << std::endl;
				break;
				std::cout << "break4" << std::endl;
			}
			
			std::cout << "break5" << std::endl;
			break;
			std::cout << "break6" << std::endl;
		}
	}

	std::cout << "break7" << std::endl;

	return 0;
}

// functor
struct sort_asc_st
{
	bool operator() (int d1, int d2) { return d1 < d2; }
};

// callback
bool sort_desc_func(int d1, int d2)
{
	return d1 > d2;	
}

// about functor
int test39() 
{
	// int ret;
	
	std::vector<int> vec = {5, 3, 2, 1, 4};
	std::sort(vec.begin(), vec.end(), sort_asc_st());
	std::cout << "after functor sort\n";
	for (auto iter = vec.begin(); iter != vec.end(); iter++)
	{
		std::cout << *iter << " ";
	}
	std::cout << std::endl;

	std::sort(vec.begin(), vec.end(), sort_desc_func);
	std::cout << "after callback sort\n";
	for (auto iter = vec.begin(); iter != vec.end(); iter++)
	{
		std::cout << *iter << " ";
	}
	std::cout << std::endl;
	
	sort_asc_st s;
	std::sort(vec.begin(), vec.end(), s);
	std::cout << "after functor object sort\n";
	for (auto iter = vec.begin(); iter != vec.end(); iter++)
	{
		std::cout << *iter << " ";
	}
	std::cout << std::endl;

	// ret = 0;
	return 0;
}

void __str_to_int_vec(const std::string &input, std::vector<int> &output, const char seps = '|');
void __str_to_int_vec(const std::string &input, std::vector<int> &output, const char seps)
{
    unsigned int tokenStart = 0;
    for (unsigned int i = 0; i < input.size(); i++)
    {
        if (input[i] == seps)
        {
            std::string tmp = input.substr(tokenStart, i - tokenStart);
            int value = atoi(tmp.c_str());
            output.push_back(value);
            tokenStart = i + 1;
        }
    }
	std::string tmp = input.substr(tokenStart, input.size() - tokenStart + 1);
	int value = atoi(tmp.c_str());
	output.push_back(value);
}

int test40() 
{
	// int ret;

	std::string input("12|34");
	std::vector<int> output;
	__str_to_int_vec(input, output);

	std::cout << "output:" << std::endl;
	for (auto iter = output.begin(); iter != output.end(); iter++)
	{
		std::cout << *iter << " ";
	}
	std::cout << std::endl;

	// ret = 0;
	return 0;
}

int test41() 
{
	int a = 10;
	auto t1 = a;
	t1 = 5;
	std::cout << "a=" << a << " t1=" << t1 << std::endl;

	a = 10;
	auto &t2 = a;
	t2 = 5;
	std::cout << "a=" << a << " t2=" << t2 << std::endl;

	return 0;
}

template <typename It>
auto get_it(It iter) -> decltype(*iter)
{
	return *iter;
}

int test42() 
{
	std::vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	auto iter = v1.begin();
	int value;
	value = get_it(iter);
	std::cout << "value=" << value << std::endl;
	iter++;
	value = get_it(iter);
	std::cout << "value=" << value << std::endl;

	return 0;
}

int test43() 
{
	// int ret;

	int a = 10;
	int b = 20;
	int c = 100;

	int count = 0;
	count = a / b * c;
	std::cout << "count1=" << count << std::endl;

	count = (double)a / b * c;
	std::cout << "count2=" << count << std::endl;

	count = (double)a / (double)b * (double)c;
	std::cout << "count3=" << count << std::endl;

	// ret = 0;
	return 0;
}

int test44() 
{
	// int ret;
	std::string numStr = "";
	int32_t num = 0;
	double x = 0;

	numStr = "1000";
	num = atoi(numStr.c_str());
	std::cout << "numStr1=" << numStr << " num=" << num << std::endl;

	numStr = "-1000";
	num = atoi(numStr.c_str());
	std::cout << "numStr2=" << numStr << " num=" << num << std::endl;

	numStr = "1000abc";
	num = atoi(numStr.c_str());
	std::cout << "numStr3=" << numStr << " num=" << num << std::endl;

	numStr = "abc1000";
	num = atoi(numStr.c_str());
	std::cout << "numStr4=" << numStr << " num=" << num << std::endl;

	x = pow(2, 31) - 1;
	numStr = to_string((long double)x);
	num = atoi(numStr.c_str());
	std::cout << "numStr5=" << numStr << " num=" << num << std::endl;
	num += 1;
	std::cout << "num+=1 = " << num << std::endl;

	x = pow(2, 31);
	numStr = to_string((long double)x);
	num = atoi(numStr.c_str());
	std::cout << "numStr6=" << numStr << " num=" << num << std::endl;

	numStr = to_string((long long int)INT_MAX);
	num = atoi(numStr.c_str());
	std::cout << "numStr7=" << numStr << " num=" << num << std::endl;

	x = pow(2, 32);
	numStr = to_string((long double)x);
	num = atoi(numStr.c_str());
	std::cout << "numStr8=" << numStr << " num=" << num << std::endl;


	numStr = "000";
	num = atoi(numStr.c_str());
	std::cout << "numStr=" << numStr << " num=" << num << std::endl;

	numStr = "001";
	num = atoi(numStr.c_str());
	std::cout << "numStr=" << numStr << " num=" << num << std::endl;

	numStr = "010";
	num = atoi(numStr.c_str());
	std::cout << "numStr=" << numStr << " num=" << num << std::endl;

	// ret = 0;
	return 0;
}

int test45() 
{
	std::string content = std::string("act=recharge&account=5580449&gameid=51&sid=serivceName_&money=6.00&orderno=20160701161034564657&clientip=119.147.176.68&partnerid=65&other=other&ts=1467373202&sign=d20f5fed58771636c02a69d3e5b7bd0b");
	std::map<std::string, std::string> paramMap;

	param_to_map(content, paramMap);

	for (auto iter = paramMap.begin(); iter != paramMap.end(); iter++)
	{
		printf("[%s]:[%s]\n", iter->first.c_str(), iter->second.c_str());
	}

	return 0;
}

int test46() 
{
	std::string buffer = "123:456:789";
	std::string tmp = "";
	tmp = buffer.substr(1, 0);
	printf("tmp=%s\n", tmp.c_str());

	return 0;
}
  

int test47() 
{
	std::string origin_str = "";
	std::string final_str = "";

	origin_str = "%7B%22uid%22%3A%22554153860401662%22%2C%22sid%22%3A%221%22%7D";
	final_str = url_decode(origin_str);
	printf("1 origin_str=%s final_str=%s\n", origin_str.c_str(), final_str.c_str());

	origin_str = "{\"uid\":\"554153860401662\",\"sid\":\"1\"}";
	final_str = url_encode(origin_str);
	printf("2 origin_str=%s final_str=%s\n", origin_str.c_str(), final_str.c_str());

	origin_str = "%7b%22uid%22%3a%220%22%2c%22sid%22%3a%221%22%7d";
	final_str = url_decode(origin_str);
	printf("3 origin_str=%s final_str=%s\n", origin_str.c_str(), final_str.c_str());

	char buf[3];
	buf[0] = '7';
	buf[1] = 'b';
	buf[2] = 0;

	char ret = 0;

	ret = (char)strtoul(buf, NULL, 16);
	printf("ret = %c\n", ret);

	buf[0] = '3';
	buf[1] = '0';
	buf[2] = 0;
	ret = (char)strtoul(buf, NULL, 16);
	printf("ret = %c\n", ret);

	buf[0] = '4';
	buf[1] = '0';
	buf[2] = 0;
	ret = (char)strtoul(buf, NULL, 16);
	printf("ret = %c\n", ret);

	buf[0] = '4';
	buf[1] = '1';
	buf[2] = 0;
	ret = (char)strtoul(buf, NULL, 16);
	printf("ret = %c\n", ret);

	buf[0] = '4';
	buf[1] = '7';
	buf[2] = 0;
	ret = (char)strtoul(buf, NULL, 16);
	printf("ret = %c\n", ret);

	buf[0] = '6';
	buf[1] = '1';
	buf[2] = 0;
	ret = (char)strtoul(buf, NULL, 16);
	printf("ret = %c\n", ret);

	buf[0] = '6';
	buf[1] = '7';
	buf[2] = 0;
	ret = (char)strtoul(buf, NULL, 16);
	printf("ret = %c\n", ret);
	
	return 0;
}

int test48() 
{
	std::string content;
	std::string token;
	std::vector<std::string> data;

	content = "";
	token = "_";
	data.clear();
	split_to_vector(content, token, data);
	printf("content=%s token=%s\n", content.c_str(), token.c_str());
	printf("data.size()=%lu\n", data.size());
	for (auto iter = data.begin(); iter != data.end(); iter++)
	{
		printf("[%s]\n", (*iter).c_str());
	}
	printf("\n");

	content = "_";
	token = "_";
	data.clear();
	split_to_vector(content, token, data);
	printf("content=%s token=%s\n", content.c_str(), token.c_str());
	printf("data.size()=%lu\n", data.size());
	for (auto iter = data.begin(); iter != data.end(); iter++)
	{
		printf("[%s]\n", (*iter).c_str());
	}
	printf("\n");

	content = "496979255754754";
	token = "_";
	data.clear();
	split_to_vector(content, token, data);
	printf("content=%s token=%s\n", content.c_str(), token.c_str());
	printf("data.size()=%lu\n", data.size());
	for (auto iter = data.begin(); iter != data.end(); iter++)
	{
		printf("[%s]\n", (*iter).c_str());
	}
	printf("\n");

	content = "496979255754754_1";
	token = "_";
	data.clear();
	split_to_vector(content, token, data);
	printf("content=%s token=%s\n", content.c_str(), token.c_str());
	printf("data.size()=%lu\n", data.size());
	for (auto iter = data.begin(); iter != data.end(); iter++)
	{
		printf("[%s]\n", (*iter).c_str());
	}
	printf("\n");

	content = "_496979255754754_1";
	token = "_";
	data.clear();
	split_to_vector(content, token, data);
	printf("content=%s token=%s\n", content.c_str(), token.c_str());
	printf("data.size()=%lu\n", data.size());
	for (auto iter = data.begin(); iter != data.end(); iter++)
	{
		printf("[%s]\n", (*iter).c_str());
	}
	printf("\n");

	content = "496979255754754_1_";
	token = "_";
	data.clear();
	split_to_vector(content, token, data);
	printf("content=%s token=%s\n", content.c_str(), token.c_str());
	printf("data.size()=%lu\n", data.size());
	for (auto iter = data.begin(); iter != data.end(); iter++)
	{
		printf("[%s]\n", (*iter).c_str());
	}
	printf("\n");

	content = "496979255754754_1_abcd";
	token = "_";
	data.clear();
	split_to_vector(content, token, data);
	printf("content=%s token=%s\n", content.c_str(), token.c_str());
	printf("data.size()=%lu\n", data.size());
	for (auto iter = data.begin(); iter != data.end(); iter++)
	{
		printf("[%s]\n", (*iter).c_str());
	}
	printf("\n");

	content = "496979255754754__1__abcd";
	token = "__";
	data.clear();
	split_to_vector(content, token, data);
	printf("content=%s token=%s\n", content.c_str(), token.c_str());
	printf("data.size()=%lu\n", data.size());
	for (auto iter = data.begin(); iter != data.end(); iter++)
	{
		printf("[%s]\n", (*iter).c_str());
	}
	printf("\n");

	content = "496979255754754__1__abcd";
	token = "_";
	data.clear();
	split_to_vector(content, token, data);
	printf("content=%s token=%s\n", content.c_str(), token.c_str());
	printf("data.size()=%lu\n", data.size());
	for (auto iter = data.begin(); iter != data.end(); iter++)
	{
		printf("[%s]\n", (*iter).c_str());
	}
	printf("\n");
	
	return 0;
}

int test49() 
{
	std::string token;
	std::set<std::string> data;

	std::ifstream fin;
	fin.open("Illegal.txt");
	if (!fin.is_open())
	{
		printf("open file fail\n");
		return -1;
	}

	std::stringstream ss;
	ss << fin.rdbuf();
	std::string content(ss.str());
	fin.close();

	// content = "123,abc";
	token = ",";
	data.clear();

	double time1 = get_time_ms();
	
	split_to_set(content, token, data);
	// printf("content=%s token=%s\n", content.c_str(), token.c_str());
	printf("data.size()=%lu\n", data.size());
	for (auto iter = data.begin(); iter != data.end(); iter++)
	{
		printf("[%s]\n", (*iter).c_str());
	}
	printf("\n");
	double time2 = get_time_ms();

	printf("total time=%lf\n", time2-time1);

	return 0;
}

int test50() 
{
	std::string input;
	std::string output;

	input = "";
	output = trim_str(input);
	printf("input=[%s] output=[%s]\n", input.c_str(), output.c_str());

	input = " ";
	output = trim_str(input);
	printf("input=[%s] output=[%s]\n", input.c_str(), output.c_str());

	input = "  ";
	output = trim_str(input);
	printf("input=[%s] output=[%s]\n", input.c_str(), output.c_str());

	input = "a";
	output = trim_str(input);
	printf("input=[%s] output=[%s]\n", input.c_str(), output.c_str());

	input = " a";
	output = trim_str(input);
	printf("input=[%s] output=[%s]\n", input.c_str(), output.c_str());

	input = "a ";
	output = trim_str(input);
	printf("input=[%s] output=[%s]\n", input.c_str(), output.c_str());

	input = " a ";
	output = trim_str(input);
	printf("input=[%s] output=[%s]\n", input.c_str(), output.c_str());

	input = "  a ";
	output = trim_str(input);
	printf("input=[%s] output=[%s]\n", input.c_str(), output.c_str());

	input = " a  ";
	output = trim_str(input);
	printf("input=[%s] output=[%s]\n", input.c_str(), output.c_str());

	input = "  a  ";
	output = trim_str(input);
	printf("input=[%s] output=[%s]\n", input.c_str(), output.c_str());

	input = "ab";
	output = trim_str(input);
	printf("input=[%s] output=[%s]\n", input.c_str(), output.c_str());

	input = " ab";
	output = trim_str(input);
	printf("input=[%s] output=[%s]\n", input.c_str(), output.c_str());

	input = "ab ";
	output = trim_str(input);
	printf("input=[%s] output=[%s]\n", input.c_str(), output.c_str());

	input = " ab ";
	output = trim_str(input);
	printf("input=[%s] output=[%s]\n", input.c_str(), output.c_str());

	input = "  ab ";
	output = trim_str(input);
	printf("input=[%s] output=[%s]\n", input.c_str(), output.c_str());

	input = " ab  ";
	output = trim_str(input);
	printf("input=[%s] output=[%s]\n", input.c_str(), output.c_str());

	input = "  ab  ";
	output = trim_str(input);
	printf("input=[%s] output=[%s]\n", input.c_str(), output.c_str());

	input = " a b ";
	output = trim_str(input);
	printf("input=[%s] output=[%s]\n", input.c_str(), output.c_str());

	return 0;
}

int test51() 
{
	std::string buffer;

	buffer = "1";
	printf("buffer=[%s] buffer.size()=%lu\n", buffer.c_str(), buffer.size());

	buffer = "你好";
	printf("buffer=[%s] buffer.size()=%lu\n", buffer.c_str(), buffer.size());

	buffer = "你好1";
	printf("buffer=[%s] buffer.size()=%lu\n", buffer.c_str(), buffer.size());

	return 0;
}

int test52() 
{
	double time1, time2;
	const char *words = "abcdefg";
	int len = strlen(words);
	int size = 50;
	std::string buffer = "";
	std::string buffer2 = "";

	printf("1 buffer.capacity()=%lu\n", buffer.capacity());

	time1 = get_time_ms();
	for (int i = 0; i < size; i++)
	{
		buffer += *(words + i%len);
	}
	time2 = get_time_ms();
	printf("1 size=%d total time=%lf\n", size, time2-time1);
	printf("1 buffer.capacity()=%lu\n", buffer.capacity());

	printf("\n");

	printf("2 buffer.capacity()=%lu\n", buffer2.capacity());
	time1 = get_time_ms();
	buffer2.reserve(size);
	for (int i = 0; i < size; i++)
	{
		buffer2 += *(words + i%len);
	}
	time2 = get_time_ms();
	printf("2 size=%d total time=%lf\n", size, time2-time1);
	printf("2 buffer.capacity()=%lu\n", buffer2.capacity());

	return 0;
}

int test53() 
{
	unsigned long x = 0;
	char buffer[50];

	x = 1470880955;
	sprintf(buffer, "%lx", x);
	printf("x=%ld buffer=[%s]\n", x, buffer);
	
	return 0;
}

int test54() 
{
	std::string buffer = "aa";
	for (size_t i = 0; i <= buffer.size(); i++)
	{
		printf("buffer[%lu]=%d\n", i, *(buffer.c_str()+i));
	}

	int a = 1;
	int b = 2;
	int c = 3;

	if (a == 1 || (b == 2 && c != 3))
	{
		printf("match\n");
	}
	else
	{
		printf("not match\n");
	}
	
	return 0;
}

struct cmp55_t
{
	bool operator()(int a, int b)
	{
		return a > b;
	}
};

struct cmp552_t
{
	bool operator()(int a, int b)
	{
		return a < b;
	}
};

int test55() 
{
	std::vector<int> nums = {2, 4, 6, 1, 3, 5};
	std::priority_queue<int, vector<int>, cmp55_t> heap;

	printf("nums=\n");
	for (int i = 0; i < (int)nums.size(); i++)
	{
		printf("nums[%d]=%d\n", i, nums[i]);
		heap.push(nums[i]);
	}
	printf("\n");

	printf("heap=\n");
	while (heap.size())
	{
		int r = heap.top();
		heap.pop();
		printf("r=%d\n", r);
	}
	printf("\n");

	std::priority_queue<int, vector<int>, cmp552_t> heap2;
	nums = {2, 4, 6, 1, 3, 5};
	printf("nums=\n");
	for (int i = 0; i < (int)nums.size(); i++)
	{
		printf("nums[%d]=%d\n", i, nums[i]);
		heap2.push(nums[i]);
	}
	printf("\n");

	printf("heap2=\n");
	while (heap2.size())
	{
		int r = heap2.top();
		heap2.pop();
		printf("r=%d\n", r);
	}
	printf("\n");


	return 0;
}

int test56() 
{
	int num_list[10];

	int *ptr = num_list + 5;
	printf("offset=%ld\n", ptr - num_list);

	struct tmp_t
	{
		int a;
		int b;
		int c;
	};

	tmp_t t_list[10];
	tmp_t *ptr2 = &t_list[5];
	printf("offset=%ld\n", ptr2 - t_list);

	return 0;
}

int test57() 
{
	int a = 0;
	if (a)
	{
		printf("(%d) is true\n", a);
	}
	else
	{
		printf("(%d) is false\n", a);
	}

	a = 1;
	if (a)
	{
		printf("(%d) is true\n", a);
	}
	else
	{
		printf("(%d) is false\n", a);
	}

	a = -1;
	if (a)
	{
		printf("(%d) is true\n", a);
	}
	else
	{
		printf("(%d) is false\n", a);
	}

	return 0;
}

struct a_t
{
	int a;
};

int test58() 
{
	
	printf("sizeof(a_t) = %zu\n", sizeof(a_t));
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

int test59() 
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

	printf("sizeof(struct item_a_t)=%lu\n", sizeof(struct item_a_t)); // 1
	printf("sizeof(struct item_b_t)=%lu\n", sizeof(struct item_b_t)); // 4
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
	return 0;
}

#define BITMASK(b) (1<<((b)%CHAR_BIT))//字节内偏移
#define BITSLOT(b) ((b)/CHAR_BIT)//将数据映射到所属字节内
#define BITSET(a,b) ((a)[BITSLOT(b)] |= BITMASK(b))//置位
#define BITCLEAR(a,b) ((a)[BITSLOT(b)] &= ~BITMASK(b))//清位
#define BITTEST(a,b) ((a)[BITSLOT(b)] & BITMASK(b))//查看相应位数据
#define BITNSLOTS(nb) ((nb + CHAR_BIT - 1) / CHAR_BIT)//计算所需字节数

int test60() 
{
	char buffer[4];
	bzero(&buffer, sizeof(buffer));

	int a = 0;
	a = BITTEST(buffer, 1);
	printf("a=%d\n", a);

	BITSET(buffer, 1);
	a = BITTEST(buffer, 1);
	printf("a=%d\n", a);
	return 0;
}

int is_little_endian()
{
	// little-endian:
	// we can call, little data in little memory path
	// memory path
	// low ------------ high
	// 0x00001 --------- 0x00004
	// data
	// 0x78, 0x56, 0x34, 0x12
	// *(char *)&i = 0x78
	int i=0x12345678;
	char *c=(char *)&i; 
	return(*c==0x78);
}

int is_little_endian2()
{
	union
	{
		short a;
		char b;
	}u;

	u.a = 1;
	return (u.b - '\0');
}
int test61() 
{
	printf("is_little_endian=%d\n", is_little_endian());
	printf("is_little_endian2=%d\n", is_little_endian2());
	return 0;
}

int test62() 
{
	std::string buffer = "CREATE TABLE `player` this is '0'";
	printf("buffer=%s\n", buffer.c_str());

	std::string delim = "'";

	int pos = buffer.find(delim);
	printf("pos=%d\n", pos);

	printf("buffer[pos]=%c\n", buffer[pos]);

	return 0;
}

int test63() 
{
	int32_t v = 100;
	int32_t percent = 23;
	v = (double)v * percent / 100;
	printf("v=%d\n", v);

	v = 50;
	percent = 23;
	v = (double)v * percent / 100;
	printf("v=%d\n", v);

	return 0;
}

int test64() 
{
	int v = 0;
	do
	{
		v++;
		if (v > 10)
		{
			break;
		}

		v++;
		if (v > 10)
		{
			break;
		}
		
	} while (false);

	printf("v=%d\n", v);

	return 0;
}

union my_union
{
	int32_t x;
	int64_t y;
	operator int32_t()
	{
		return x;
	}
};

struct my_struct
{
	int32_t x;
	int64_t y;
	bool operator ==(my_struct s)
	{
		return (x == s.x && y == s.y);
	}
};

int test65() 
{
	my_union u1;
	u1.x = 123;
	my_union u2;
	u2.x = 123;

	if (u1 == u2)
	{
		printf("u1 == u2\n");
	}
	else
	{
		printf("u1 != u2\n");
	}

	my_struct s1;
	s1.x = 123; s1.y = 456;
	my_struct s2;
	s2.x = 123; s2.y = 456;

	if (s1 == s2)
	{
		printf("s1 == s2\n");
	}
	else
	{
		printf("s1 != s2\n");
	}

	return 0;
}

int test66() 
{
	std::vector<int> v1;
	for (int i = 10; i > 0; i--)
	{
		v1.push_back(rand()%10);
	}

	// time_t now = time(NULL);
	// std::shuffle(v1.begin(), v1.end(), std::default_random_engine(now));
	
	printf("v1:");
	for (int i = 0; i < 10; i++)
	{
		printf(" %d", v1[i]);
	}
	printf("\n");

	nth_element(v1.begin(), v1.begin() + 5, v1.end());
	
	printf("v1:");
	for (int i = 0; i < 10; i++)
	{
		printf(" %d", v1[i]);
	}
	printf("\n");


	return 0;
}

struct KnowledgeRankNode
{
	int charId;
	int score;
	int time;
};

bool updateKnowledgeRank(std::list<KnowledgeRankNode> &rankList, KnowledgeRankNode &node, int &removeId)
{
	const int RANK_MAX = 10;
    bool isUpdate = false;
    removeId = 0;
    do
    {
        // 1. empty rank, just add
        if (rankList.size() == 0)
        {
            rankList.push_back(node);
            isUpdate = true;
            break;
        }

		// 2. score small then NO.Last, break
		if ((int32_t)rankList.size() >= RANK_MAX)
		{
			auto back = rankList.back();
			if (node.score < back.score)
			{
				break;
			}
		}

        auto iter = rankList.begin();
        // 3. insert or replace
        for (;iter != rankList.end();)
        {
			// find myself in rank
            if ((*iter).charId == node.charId)
            {
				if ((*iter).score >= node.score)
				{
					// old record score bigger, do nothing, just return
					return true;
				}
				// update old record, and return
				(*iter).score = node.score;
				(*iter).time = node.time;
				return true;
            }

			// big score, continue
            if ((*iter).score > node.score)
            {
                iter++;
                continue;
            }

			// same score, older record, continue
			if ((*iter).score == node.score && (*iter).time <= node.time)
			{
				iter++;
				continue;
			}

            // insert new record
            rankList.insert(iter, node);
            isUpdate = true;
			iter++;
            break;
        }

        // 4. remove duplicate
        for (;iter != rankList.end();)
        {
            if ((*iter).charId == node.charId)
            {
                // remove old duplicate record
                iter = rankList.erase(iter);
                continue;
            }
			iter++;
        }

        // 5. if size>max, pop last
        if ((int32_t)rankList.size() > RANK_MAX)
        {
            removeId = rankList.back().charId;
            rankList.pop_back();
			break;
        }

		// 6. if not add new record, and still has space in rank, add it
		if (!isUpdate && (int32_t)rankList.size() < RANK_MAX)
		{
			rankList.push_back(node);
			isUpdate = true;
		}

    } while (false);

	return isUpdate;
}

void printKnowledgeRank(const std::list<KnowledgeRankNode> &rankList)
{
	printf("size=%zu\n", rankList.size());
	for (auto iter = rankList.begin(); iter != rankList.end(); iter++)
	{
		printf("c=%3d s=%3d t=%3d\n", (*iter).charId, (*iter).score, (*iter).time);
	}
	printf("\n");
}

int test67() 
{
	std::list<KnowledgeRankNode> rankList;

	KnowledgeRankNode node;
	int removeId = 0;

	node.charId = 1;
	node.score = 100;
	node.time = 0;
	updateKnowledgeRank(rankList, node, removeId);
	printKnowledgeRank(rankList);

	node.charId = 1;
	node.score = 101;
	node.time = 0;
	updateKnowledgeRank(rankList, node, removeId);
	printKnowledgeRank(rankList);

	node.charId = 3;
	node.score = 100;
	updateKnowledgeRank(rankList, node, removeId);

	node.charId = 2;
	node.score = 100;
	updateKnowledgeRank(rankList, node, removeId);

	node.charId = 2;
	node.score = 101;
	updateKnowledgeRank(rankList, node, removeId);

	printKnowledgeRank(rankList);

	printf("-----------------------------\n");

	rankList.clear();
	srand(time(NULL));
	const int MAX_RECORD = 30;
	const int CHARID_RANGE = 20;
	const int SCORE_RANGE = 20;
	const int TIME_RANGE = 3;
	std::list<KnowledgeRankNode> recordList;
	std::set<int> numSet;
	for (int i = 0; i < MAX_RECORD; i++)
	{
		node.charId = rand() % CHARID_RANGE + 1;
		node.score = rand() % SCORE_RANGE + 1;
		node.time = rand() % TIME_RANGE + 1;
		recordList.push_back(node);
		numSet.insert(node.charId);
	}
	printf("recordList: %zu\n", recordList.size());
	for (auto iter = recordList.begin(); iter != recordList.end(); iter++)
	{
		printf("c=%3d s=%3d t=%3d\n", iter->charId, iter->score, iter->time);
	}
	printf("numSet.size()=%zu\n\n", numSet.size());

	for (auto iter = recordList.begin(); iter != recordList.end(); iter++)
	{
		bool isUpdate = updateKnowledgeRank(rankList, *iter, removeId);
		if (isUpdate && removeId)
		{
			printf("removeId=%d\n", removeId);
		}
	}
	printKnowledgeRank(rankList);

	return 0;
}

struct guild
{
	int guildId;
	int createTime;
	int fakeId;
	friend bool operator< (const guild &g1, const guild &g2)
	{
		return g1.createTime < g2.createTime;
	}
};

int test68() 
{
	// int ret;
	
	srand(time(NULL));
	std::vector<guild> guildVec;
	const int MAX_GUILD = 100;
	for (int i = 1; i <= MAX_GUILD; i++)
	{
		guild g;
		g.guildId = i;
		g.createTime = rand() % 1000000;
		guildVec.push_back(g);
	}

	std::sort(guildVec.begin(), guildVec.end());

	int fakeId = 1;
	for (auto iter = guildVec.begin(); iter != guildVec.end(); iter++)
	{
		iter->fakeId = fakeId++;
		printf("guildId=%d createTime=%d fakeId=%d\n", iter->guildId, iter->createTime, iter->fakeId);
	}


	// ret = 0;
	return 0;
}

int test69() 
{
	char buffer[] = "abc";
	const char *buffer2 = "abcd";
	const char *empty_string = "";

	int result = strcmp(buffer, empty_string);
	printf("result=%d [%s] [%s]\n", result, buffer, empty_string);

	result = strcmp(buffer2, empty_string);
	printf("result=%d [%s] [%s]\n", result, buffer2, empty_string);

	result = -1;
	result = !result;
	printf("result=%d\n", result);

	return 0;
}

class MemoryPool
{
public:
	MemoryPool *next;
};

class MyNewObj
{
public:
	MyNewObj() : v1(0), v2(0) {};
	~MyNewObj() {};

	void * operator new(size_t size);
	void operator delete(void *p, size_t size);

	int v1;
	int v2;
private:
	static MemoryPool *memPool;
	static void expandMemoryPool();
};

MemoryPool * MyNewObj::memPool = NULL;

void MyNewObj::expandMemoryPool()
{
	size_t size = (sizeof(MyNewObj) > sizeof(MemoryPool *)) ? sizeof(MyNewObj) : sizeof(MemoryPool *);
	MemoryPool *runner = (MemoryPool *) new char[size];
	memPool = runner;
	enum { EXPAND_SIZE = 32 };
	for (int i = 0; i < EXPAND_SIZE; i++)
	{
		runner->next = (MemoryPool *) new char[size];
		runner = runner->next;
	}
	runner->next = NULL;
};

void * MyNewObj::operator new(size_t size)
{
	// TODO lock
	if (NULL == memPool)
	{
		expandMemoryPool();
	}
	MemoryPool *head = memPool;
	memPool = head->next;

	return head;
};

void MyNewObj::operator delete(void *p, size_t size)
{
	// TODO lock
	MemoryPool *head = (MemoryPool *)p;
	head->next = memPool;
	memPool = head;
};

class MyNewObj2
{
public:
	MyNewObj2() : v1(0), v2(0) {};
	~MyNewObj2() {};

	int v1;
	int v2;
private:
	static MemoryPool *memPool;
};

int test70() 
{
	enum { OBJ_COUNT = 1000 };
	enum { LOOP_NUM = 1000 };
	struct timeval startTime, endTime;

	{
		MyNewObj **objList = new MyNewObj*[OBJ_COUNT];
		gettimeofday(&startTime,NULL);
		for (int x = 0; x < LOOP_NUM; x++)
		{
			for (int i = 0; i < OBJ_COUNT; i++)
			{
				objList[i] = new MyNewObj();
			}
			for (int i = 0; i < OBJ_COUNT; i++)
			{
				delete objList[i];
			}
		}
		gettimeofday(&endTime,NULL);
		printf("use mempool cost=%lfms\n", endTime.tv_sec * 1000 + (double)endTime.tv_usec / 1000 - startTime.tv_sec * 1000 - (double)startTime.tv_usec / 1000);
		delete [] objList;
	}

	{
		MyNewObj2 **objList = new MyNewObj2*[OBJ_COUNT];
		gettimeofday(&startTime,NULL);
		for (int x = 0; x < LOOP_NUM; x++)
		{
			for (int i = 0; i < OBJ_COUNT; i++)
			{
				objList[i] = new MyNewObj2();
			}
			for (int i = 0; i < OBJ_COUNT; i++)
			{
				delete objList[i];
			}
		}
		gettimeofday(&endTime,NULL);
		printf("not use mempool cost=%lfms\n", endTime.tv_sec * 1000 + (double)endTime.tv_usec / 1000 - startTime.tv_sec * 1000 - (double)startTime.tv_usec / 1000);
		delete [] objList;
	}

	return 0;
}

class A
{
public:
	A()
	{
		value = 10;
	}

	int value;

};

class B
{
public:
	operator A&(){ return a; }
	A a;
};

int test71()
{
	B b;
	((A&)b).value = 1011;
	printf("value=%d\n", b.a.value);
	return 0;
}

int test72()
{
	printf("__func__ %s\n", __func__);
	printf("__FUNCTION__ %s\n", __FUNCTION__);
	printf("__FILE__ %s\n", __FILE__);
	printf("__LINE__ %d\n", __LINE__);
	printf("__TIME__ %s\n", __TIME__);
	printf("__DATE__ %s\n", __DATE__);
	return 0;
}

int funcForReturn(int x)
{
	printf("x=%d\n", x);
	return x;
}

// declare, trailing return type
auto funcReturnFuncPtr() -> int (*)(int);
// define
auto funcReturnFuncPtr() -> int (*)(int)
{
	return funcForReturn;
}

// use decltype
decltype(funcForReturn) *funcReturnFuncPtr2()
{
	return funcForReturn;
}

int test73()
{
	auto fp1 = funcReturnFuncPtr();
	fp1(10);

	int (*fp2)(int) = funcReturnFuncPtr();
	fp2(20);

	{
	using FP = int (*)(int);
	FP fp3 = funcReturnFuncPtr();
	fp3(30);
	}

	{
	typedef int (*FP)(int);
	FP fp4 = funcReturnFuncPtr();
	fp4(40);
	}

	auto fp5 = funcReturnFuncPtr2();
	fp5(50);

	return 0;
}

class TestX
{
public:
	virtual ~TestX() {};
	int a, b, c;
};

class TestY
{
public:
	int a, b, c;
};

int test74()
{
#define OPEN_TEST74 0
#if OPEN_TEST74
	{
		TestX x;
		printf("sizeof(TestX)=%lu\n", sizeof(TestX));
		printf("sizeof(x)=%lu\n", sizeof(x));
		printf("&TestX::a=%p\n", &TestX::a);
		printf("&TestX::b=%p\n", &TestX::b);
		printf("&TestX::c=%p\n", &TestX::c);
		printf("&x=%p\n", &x);
		printf("&x.a=%p\n", &x.a);
		x.a = 10;
		printf("&x.a=%p\n", &x.a);
		printf("x.a=%d\n", (&x)->*(&TestX::a));
	}

	printf("\n");

	{
		TestY y;
		printf("sizeof(TestY)=%lu\n", sizeof(TestY));
		printf("sizeof(y)=%lu\n", sizeof(y));
		printf("&TestY::a=%p\n", &TestY::a);
		printf("&TestY::b=%p\n", &TestY::b);
		printf("&TestY::c=%p\n", &TestY::c);
		int TestY::*p1 = 0;
		int TestY::*p2 = &TestY::a;
		printf("p1=%p\n", p1);
		printf("p2=%p\n", p2);
		printf("&y=%p\n", &y);
		printf("&y.a=%p\n", &y.a);
	}
#endif 

	return 0;
}

int test75()
{

	std::vector<int32_t> v1;
	for (int i = 0; i < 10; i++)
	{
		v1.push_back(i);
	}
	printf("v1:");
	for (auto v : v1)
	{
		printf("%d ", v);
	}
	printf("\n");

	std::vector<int32_t> v2;
	for (int i = 5; i < 15; i++)
	{
		v2.push_back(i);
	}
	printf("v2:");
	for (auto v : v2)
	{
		printf("%d ", v);
	}
	printf("\n");

	{
	std::vector<int32_t> result(v1.size() + v2.size());
	auto iter = std::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());
	result.resize(iter - result.begin());
	printf("v1 diff v2: result.size=%lu\n", result.size());
	for (auto v : result)
	{
		printf("%d ", v);
	}
	printf("\n");
	}

	{
	std::vector<int32_t> result(v1.size() + v2.size());
	auto iter = std::set_difference(v2.begin(), v2.end(), v1.begin(), v1.end(), result.begin());
	result.resize(iter - result.begin());
	printf("v2 diff v1: result.size=%lu\n", result.size());
	for (auto v : result)
	{
		printf("%d ", v);
	}
	printf("\n");
	}

	{
	std::vector<int32_t> result(v1.size() + v2.size());
	auto iter = std::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());
	result.resize(iter - result.begin());
	printf("v1 union v2: result.size=%lu\n", result.size());
	for (auto v : result)
	{
		printf("%d ", v);
	}
	printf("\n");
	}

	{
	std::vector<int32_t> result(v1.size() + v2.size());
	auto iter = std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());
	result.resize(iter - result.begin());
	printf("v1 intersection v2: result.size=%lu\n", result.size());
	for (auto v : result)
	{
		printf("%d ", v);
	}
	printf("\n");
	}

	return 0;
}

int test76()
{
#define XXX_TEST_DEFINE_A 1
#define XXX_TEST_DEFINE_B 0

#ifdef XXX_TEST_DEFINE_A 
	printf("ifdef XXX_TEST_DEFINE_A\n");
#endif

#if XXX_TEST_DEFINE_A 
	printf("if XXX_TEST_DEFINE_A\n");
#endif

#undef XXX_TEST_DEFINE_A 
#if XXX_TEST_DEFINE_A 
	printf("after if XXX_TEST_DEFINE_A\n");
#endif

#ifdef XXX_TEST_DEFINE_B
	printf("ifdef XXX_TEST_DEFINE_B\n");
#endif

#if XXX_TEST_DEFINE_B
	printf("if XXX_TEST_DEFINE_B\n");
#endif

#if XXX_TEST_DEFINE_B == 0
	printf("if XXX_TEST_DEFINE_B == 0\n");
#endif

#if XXX_TEST_DEFINE_C
	printf("if XXX_TEST_DEFINE_C\n");
#endif

	return 0;
}

int test77()
{
	auto print_malloc = [](const char *msg)
	{
		printf("---- %s\n", msg);
		malloc_stats();
		printf("\n");
	};

	print_malloc("start");
	{
		// malloc and free
		void *ptr = malloc(100);
		print_malloc("after malloc");

		free(ptr);
		print_malloc("after free");
	}

	print_malloc("start2");
	{
		// malloc no free
		char *ptr = (char *)malloc(100);
		print_malloc("after malloc");
		ptr[0] = 'a';
	}

	print_malloc("no free start");
	{
		// new and delete
		char *ptr = new char[100];
		print_malloc("after new");
		ptr[0] = 'a';

		delete [] ptr;
		print_malloc("after delete");
	}

	print_malloc("start3");
	{
		// new and delete
		char *ptr = new char[100];
		print_malloc("after new");
		ptr[0] = 'a';
	}
	print_malloc("no delete");


	return 0;
}

int test78()
{
	{
		typedef int I32;
		I32 a = 10;
		printf("a=%d\n", a);

		typedef std::vector<int> VI32;
		VI32 b = {10, 12};
		b.clear();
	}

	{
		using I32 = int;
		I32 a = 11;
		printf("a=%d\n", a);

		using VI32 = std::vector<int>;
		VI32 b = {10, 12};
		b.clear();
	}

	return 0;
}

int test79()
{
	{
		char msg[] = "12345";
		printf("strlen=%zu size=%zu\n", strlen(msg), sizeof(msg));
	}
	{
		// char *msg = "12345"; // "12345" in data segement
		const char *msg = "12345"; // "12345" in data segement
		printf("strlen=%zu size=%zu\n", strlen(msg), sizeof(msg));
		// msg[0] = 'a'; // error, change data segement const value
	}


	return 0;
}

void logger(const char *tag, const char *funcname, int line, const char *fmt, ...)
{
	enum {MAX_LOG_BUFFER_SIZE = 1024};
	char buffer[MAX_LOG_BUFFER_SIZE] = {0};

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buffer, MAX_LOG_BUFFER_SIZE, fmt, ap);
	va_end(ap);

	printf("%s %s[%d]: %s\n", tag, funcname, line, buffer);
}

// work in gcc and vs
#define LOG_DEBUG(fmt, ...) logger("DEBUG", __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
// only work in gcc
// #define LOG_DEBUG(fmt, arg...) logger("DEBUG", __FUNCTION__, __LINE__, fmt, ##arg)

int test80()
{

	LOG_DEBUG("hello");
	LOG_DEBUG("hello %d", 123);

	return 0;
}

int test_notyet() 
{
	// int ret;

	// ret = 0;
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
,	test69
,	test70
,	test71
,	test72
,	test73
,	test74
,	test75
,	test76
,	test77
,	test78
,	test79
,	test80
};

int main(int argc, char *argv[]) 
{

	/*
	int opt;
	// optstring 'x' is flag, 'x:' is with value
	while ((opt = getopt(argc, argv, "nc:t:")) != -1)
	{
		switch (opt)
		{
		case 'n' :
			printf("getopt: get 'n'\n");
			break;
		case 'c' :
			printf("getopt: get 'c' value=[%s]\n", optarg);
			break;
		case 't' :
			printf("getopt: get 't' value=[%d]\n", atoi(optarg));
			break;
		}
	}
	*/


	int ret;
	int maxcase;
	int testcase;

	maxcase = sizeof(test_list) / sizeof(testcase_t);
	testcase = maxcase - 1;

	if (argc > 1) {
		if (!strcmp(argv[1], "all")) {
			cout << "stu:run_all_case" << endl;
			for (int i=0; i<maxcase; i++) {
				cout << "stu:run_case[" << i << "]" << endl;
				ret = test_list[i]();
				if (ret != 0) {
					cout << "stu:case[" << i << "] " << ret << endl;
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

	cout << "stu:RUN case[" << testcase << "]" << endl;
	ret = test_list[testcase]();
	if (ret != 0) {
		cout << "stu:case[" << testcase << "] " << ret << endl;
	}

	return 0;
}

