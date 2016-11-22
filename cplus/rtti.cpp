
#include <iostream>
#include <typeinfo>

// runtime type identification

// base class
class Grand 
{
private:
	int hold;
public:
	Grand(int h=0) : hold(h) { };
	virtual void speak() const { std::cout << "I am Grand class" << std::endl; };
	virtual int value() const { return hold; };
};


// gen 2nd class
class Superb : public Grand 
{
public:
	Superb(int h=0) : Grand(h) { };
#ifdef CPP11
	void speak() const override { std::cout << "I am Superb class" << std::endl; };
#else
	void speak() const { std::cout << "I am Superb class" << std::endl; };
#endif
	virtual void say() const { std::cout << "Superb get int=" << value() << std::endl; };
};


// gen 3rd class
class Magnificent : public Superb 
{
private:
	char ch;
public:
	Magnificent(int h=0, char c='a') : Superb(h), ch(c) { };
	void speak() const { std::cout << "I am Magnificent class" << std::endl; };
	void say() const { std::cout << "Magnificent get int=" << value() << " and get char=" << ch << std::endl; };
};


int main(int argc, char *argv[])
{
	
	using std::cout;
	using std::endl;

	Grand g1(10);
	Superb s1(20);
	Magnificent m1(30, 'h');
	g1.speak();
	s1.speak();
	s1.say();
	m1.speak();
	m1.say();

	cout << "-------------------------" << endl;

	
	Grand * p1 = &g1;
	Grand * p2 = &s1;
	Grand * p3 = &m1;
	Superb * pp;

	// dynamic_cast for up type cast or same type cast
	// dynamic_cast avoid down type cast, return NULL
	// dynamic_cast<pointer*> fail will return NULL
	pp = dynamic_cast<Superb *>(p1);
	if (pp == NULL) {
		cout << "p1 dynamic_cast to Superb * fail" << endl;	
	} else {
		cout << "p1 dynamic_cast to Superb * success" << endl;	
	}

	pp = dynamic_cast<Superb *>(p2);
	if (pp == NULL) {
		cout << "p2 dynamic_cast to Superb * fail" << endl;	
	} else {
		cout << "p2 dynamic_cast to Superb * success" << endl;	
		pp->say();
	}

	pp = dynamic_cast<Superb *>(p3);
	if (pp == NULL) {
		cout << "p3 dynamic_cast to Superb * fail" << endl;	
	} else {
		cout << "p3 dynamic_cast to Superb * success" << endl;	
		pp->say();
	}

	cout << "-------------------------" << endl;

	// dynamic_cast<reference&> fail will throw std::bad_cast
	try {
		Grand g2(55);
		Grand *pg2 = &g2;
		Superb & s2 = dynamic_cast<Superb &>(*pg2);
		s2.say();
	} catch (std::bad_cast & e) {
		cout << "dynamic_cast reference e.what()=" << e.what() << endl;
	}

	cout << "-------------------------" << endl;

	// typeid(Typename).name()
	// typeid can get real type info
	cout << "typeid(Grand).name()=" << typeid(Grand).name() << endl;
	cout << "typeid(Superb).name()=" << typeid(Superb).name() << endl;
	cout << "typeid(Magnificent).name()=" << typeid(Magnificent).name() << endl;

	Superb * p4 = new Magnificent (60, 'e');
	cout << "typeid(*p4).name()=" << typeid(*p4).name() << endl;
	if (typeid(*p4) == typeid(Magnificent)) {
		p4->say();
	}
	
	cout << "-------------------------" << endl;

	// const_cast for same type remove const
	int v1 = 10;
	const int * pv1 = &v1;
	cout << "before v1=" << v1 << endl;
	int * pv2 = const_cast<int *>(pv1); // same type, remove const
	(*pv2)++;
	cout << "after v1=" << v1 << endl;

	cout << "-------------------------" << endl;

	// static_cast for up type cast and down type cast
	Grand g5(12);
	Magnificent m5(15, 'u');
	Grand * pg1 = static_cast<Grand *>(&m5);
	Magnificent * pm1 = static_cast<Magnificent *>(&g5);
	cout << "pg1->speak() :"; pg1->speak();
	cout << "pm1->speak() :"; pm1->speak();
	// cout << "pm1->say() :"; pm1->say(); // bug, pm1 really is a Grand *

	cout << "-------------------------" << endl;

	// reinterpret_cast for force cast type, not safe
	struct dat {short a; short b;};
	long value = 0xA224B118;
	dat *pd = reinterpret_cast<dat *> (&value);
	cout << std::hex << pd->a << endl;

	
	return 0;
}
