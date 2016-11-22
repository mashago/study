#include <iostream>
#include "worker.h"

using std::string;
using std::cout;
using std::endl;

// Worker function
Worker::Worker(string n) : name(n)
{
}

void Worker::data() const
{
	cout << "name=" << name << endl;
}

void Worker::show() const
{
	cout << "name=" << name << endl;
}

// Waiter function
Waiter::Waiter(string n, int id) : Worker(n), waiter_id(id)
{
}

Waiter::Waiter(const Worker &w, int id) : Worker(w), waiter_id(id)
{
}

void Waiter::data() const
{
	cout << "waiter_id=" << waiter_id << endl;
}

void Waiter::show() const
{
	Worker::data();
	cout << "waiter_id=" << waiter_id << endl;
}


// Singer function
Singer::Singer(string n, int id) : Worker(n), singer_id(id)
{
}

Singer::Singer(const Worker &w, int id) : Worker(w), singer_id(id)
{
}

void Singer::data() const
{
	cout << "singer_id=" << singer_id << endl;
}

void Singer::show() const
{
	Worker::data();
	cout << "singer_id=" << singer_id << endl;
}


// SingerWaiter function
// init all base class : Worker(), Waiter(), Singer()
// if not init grand base class, will call its default constructor, father class will not pass param to grand class
SingerWaiter::SingerWaiter(string n, int wid, int sid) : Worker(n), Waiter(n, wid), Singer(n, sid)
{
}

SingerWaiter::SingerWaiter(const Worker &w, int wid, int sid) : Worker(w), Waiter(w, wid), Singer(w, sid)
{
}

// grand virtual base class Worker will use default constructor, Waiter or Singer param will not pass to Worker
SingerWaiter::SingerWaiter(const Waiter &w, int sid) : Waiter(w), Singer(w, sid)
{
}

SingerWaiter::SingerWaiter(const Singer &w, int wid) : Waiter(w, wid), Singer(w)
{
}

void SingerWaiter::data() const
{
	Waiter::data();
	Singer::data();
}

void SingerWaiter::show() const
{
	Worker::data();
	data();
}

int main(int argc, char * argv[])
{
	Waiter w1 = Waiter("masha", 1);
	w1.show();
	Worker & w2 = w1;
	w2.show();
	cout << "------------------------------" << endl;

	Singer w3 = Singer("peter", 2);
	w3.show();
	Worker & w4 = w3;
	w4.show();
	cout << "------------------------------" << endl;

	SingerWaiter w5 = SingerWaiter("jake", 4, 5);
	w5.show();
	Worker & w6 = w5;
	w6.show();
	cout << "------------------------------" << endl;


	SingerWaiter w7 = SingerWaiter("tom", 1, 2);
	w7.show();
	w7.Singer::show();
	Worker & w8 = w7;
	// w8.Singer::show(); // error, not a member
	w8.Worker::show();

	cout << "------------------------------" << endl;




	return 0;
}
	
