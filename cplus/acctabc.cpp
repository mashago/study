
#include <iostream>
#include "acctabc.h"

//// AcctABC method start ///// [
AcctABC::AcctABC(double bal) 
{
	balance = bal;
}

void AcctABC::update(double offset) // virtual method also can be defined
{
	balance += offset;
}
//// AcctABC method end ///// ]

//// Brass method start ///// [

void Brass::update(double offset)
{
	std::cout << "Brass::update:offset=" << offset << std::endl;
	if (offset + getBalance() >= 0) {
		AcctABC::update(offset);
	} else {
		std::cout << "Brass::update:balance_not_enough offset=" << offset << std::endl;
	}
}

void Brass::view() const
{
	using std::cout;
	using std::endl;

	cout << "Brass::view: balance=" << getBalance() << endl;
}


//// Brass method end ///// ]

//// BrassPlus method start ///// [
BrassPlus::BrassPlus(double bal, double ex) : AcctABC(bal)
{
	extra = ex;
}

void BrassPlus::update(double offset)
{
	std::cout << "BrassPlus::update:offset=" << offset << std::endl;
	double bal = getBalance();
	if (offset + bal >= 0) {
		AcctABC::update(offset);
	} else if (offset + bal + extra >= 0) {
		AcctABC::update(-bal);
		extra += offset + bal;
	} else {
		std::cout << "BrassPlus::update:balance_and_extra_not_enough offset=" << offset << std::endl;
	}
}

void BrassPlus::view() const
{
	using std::cout;
	using std::endl;

	cout << "BrassPlus::view: balance=" << getBalance() << " extra=" << extra << endl;
}

//// BrassPlus method end ///// ]

int main(int argc, char *argv[])
{
	using std::cout;
	using std::endl;
	// AcctABC m1; // cannot create abstract class member

	Brass m1;
	m1.view();
	cout << "-------------------------" << endl;

	Brass m2(20);
	m2.view();
	m2.update(-10);
	m2.view();
	m2.update(-20);
	m2.view();
	cout << "-------------------------" << endl;

	BrassPlus m3(20, 30);
	m3.view();
	m3.update(-10);
	m3.view();
	m3.update(-25);
	m3.view();
	m3.update(-25);
	m3.view();
	cout << "-------------------------" << endl;

	Brass m4(50);
	AcctABC & m5 = m4; // upcasting
	m5.view(); // call Brass::view()
	m5.update(-40); // call Brass::update()
	m5.view(); 
	m5.update(-40); // call Brass::update()
	m5.view(); 
	cout << "-------------------------" << endl;


	BrassPlus m6(70, 100);
	AcctABC & m7 = m6; // upcasting
	m7.view(); // call BrassPlus::view()
	m7.update(-60); // call BrassPlus::update()
	m7.view(); 
	m7.update(-60); // call BrassPlus::update()
	m7.view(); 
	cout << "-------------------------" << endl;

	Brass m8(90);
	AcctABC * m9 = &m8; // upcasting
	m9->view(); // call Brass::view()
	m9->update(-80); // call Brass::update()
	m9->view(); 
	m9->update(-80); // call Brass::update()
	m9->view(); 
	cout << "-------------------------" << endl;


	BrassPlus m10(110, 100);
	AcctABC * m11 = &m10; // upcasting
	m11->view(); // call BrassPlus::view()
	m11->update(-100); // call BrassPlus::update()
	m11->view(); 
	m11->update(-100); // call BrassPlus::update()
	m11->view(); 
	cout << "-------------------------" << endl;


	return 0;
}
