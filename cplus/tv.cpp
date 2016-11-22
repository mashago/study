
#include <iostream>
#include "tv.h"


// Tv1 function
void Tv1::volup()
{
	if (volume < MaxVol) {
		volume++;
	}
}

void Tv1::voldown()
{
	if (volume > MinVol) {
		volume--;
	}
}

void Tv1::chanup()
{
	if (channel < maxchannel) {
		channel++;
	} else {
		channel = 1;
	}
}

void Tv1::chandown()
{
	if (channel > 1) {
		channel--;
	} else {
		channel = maxchannel;
	}
}

void Tv1::info() const
{
	using std::cout;
	using std::endl;

	if (state == On) {
		cout << "Volume=" << volume << endl;
		cout << "maxchannel=" << maxchannel << endl;
		cout << "channel=" << channel << endl;
		cout << "input=" << (input == TV ? "TV" : "DVD") << endl;
	}
}

// Tv2 function
void Tv2::volup()
{
	if (volume < MaxVol) {
		volume++;
	}
}

void Tv2::voldown()
{
	if (volume > MinVol) {
		volume--;
	}
}

void Tv2::chanup()
{
	if (channel < maxchannel) {
		channel++;
	} else {
		channel = 1;
	}
}

void Tv2::chandown()
{
	if (channel > 1) {
		channel--;
	} else {
		channel = maxchannel;
	}
}

void Tv2::info() const
{
	using std::cout;
	using std::endl;

	if (state == On) {
		cout << "Volume=" << volume << endl;
		cout << "maxchannel=" << maxchannel << endl;
		cout << "channel=" << channel << endl;
		cout << "input=" << (input == TV ? "TV" : "DVD") << endl;
	}
}

int main(int argc, char *argv[])
{
	
	Tv1 tv1(Tv1::Off, 100);
	Remote1 remote1;

	remote1.onoff(tv1);
	remote1.volup(tv1);
	remote1.set_chan(tv1, 81);
	remote1.set_input(tv1);
	remote1.info(tv1);

	std::cout << "---------------------------" << std::endl;

	Tv2 tv2(Tv1::Off, 200);
	Remote2 remote2;

	remote2.onoff(tv2);
	remote2.voldown(tv2);
	remote2.set_chan(tv2, 103);
	remote2.info(tv2);


	return 0;
}


