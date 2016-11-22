
#include <iostream>
#include "tennis_player.h"

////////////////// TennisPlayer Start //////////////// [
TennisPlayer::TennisPlayer() : first_name("none"), last_name("none"), has_table(false) 
{
}

TennisPlayer::TennisPlayer(const std::string &fn, const std::string &ln, bool ht) : first_name(fn), last_name(ln), has_table(ht) 
{
}

void TennisPlayer::show()
{
	std::cout << "first_name=" << first_name << " last_name=" << last_name << " has_table=" << has_table << std::endl;
}

void TennisPlayer::reset_table(bool flag)
{
	has_table = flag;
}
////////////////// TennisPlayer END ///////////////// ]

////////////////// RatedPlayer START /////////////// [
/*
// this will call default TennisPlayer constructor
RatedPlayer::RatedPlayer(int r, const std::string &fn, const std::string &ln, bool ht) {
	...
}

// same as :
RatedPlayer::RatedPlayer(int r, const std::string &fn, const std::string &ln, bool ht) : TennisPlayer() {
	...
}
*/

// must init TennisPlayer first
RatedPlayer::RatedPlayer(int r, const std::string &fn, const std::string &ln, bool ht) : TennisPlayer(fn, ln, ht)
{
	rate = r;
}

// must init TennisPlayer first
// TennisPlayer(tp) will call default copy constructor
RatedPlayer::RatedPlayer(int r, const TennisPlayer &tp) : TennisPlayer(tp), rate(r)
{
}

// void RatedPlayer::reset_rate(int r) : rate(r) // error, only constructors take base initializers
void RatedPlayer::reset_rate(int r) 
{
	rate = r;
}

int RatedPlayer::get_rate()
{
	return rate;
}

void RatedPlayer::reset_name(const std::string &fn, const std::string &ln)
{
	// error, derived method cannot get base private member, only base method can get base private member
	// first_name = fn;
	// last_name = ln;
}

////////////////// RatedPlayer END ///////////////// ]

int main(int argc, char *argv[])
{
	using std::cout;
	using std::endl;
	TennisPlayer p1;
	p1.show();

	RatedPlayer p2;
	p2.show();
	cout << "p2.rate=" << p2.get_rate() << endl;

	RatedPlayer p3 = RatedPlayer(1, "masha", "liu", true);
	p3.show();
	cout << "p3.rate=" << p3.get_rate() << endl;

	RatedPlayer p4(2, "kelton", "xian", false);
	p4.show();
	cout << "p4.rate=" << p4.get_rate() << endl;

	RatedPlayer p5(3, "aaa", "bbb", true);
	TennisPlayer &p6 = p5;
	p6.show();
	// p6.get_rate(); // error, base object cannot call derived not base class virtual method


	return 0;
}
