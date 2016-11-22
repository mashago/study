

// 1. class Remote1 is Tv1 friend member
class Tv1 {
private:
	int state;
	int volume;
	int maxchannel;
	int channel;
	int input;
public:
	friend class Remote1; // friend class
	enum {Off, On};
	enum {MinVol, MaxVol = 20};
	enum {TV, DVD};

	Tv1(int s=Off, int mc=125) : state(s), volume(5), maxchannel(mc), channel(2), input(TV) {};
	void onoff() {state ^= 1;};
	void volup();
	void voldown();
	void chanup();
	void chandown();
	void set_input() { input = ((input == TV) ? DVD : TV); }
	void info() const;
};

// friend class can access class private member
class Remote1 {
public:
	void onoff(Tv1 &tv) { tv.onoff(); };
	void volup(Tv1 &tv) { tv.volup(); };
	void voldown(Tv1 &tv) { tv.voldown(); };
	void chanup(Tv1 &tv) { tv.chanup(); };
	void chandown(Tv1 &tv) { tv.chandown(); };
	void set_chan(Tv1 &tv, int chan) { tv.channel = chan; }; // update Tv1 private member
	void set_input(Tv1 &tv) { tv.set_input(); };
	void info(Tv1 &tv) const { tv.info(); };
};


// 2. Remote2::set_chan() is Tv2 friend function

// forward declaration, so Remote2 can use Tv2
class Tv2;

class Remote2 {
public:
	void onoff(Tv2 &tv); // prototype only
	void volup(Tv2 &tv);
	void voldown(Tv2 &tv);
	void chanup(Tv2 &tv);
	void chandown(Tv2 &tv);
	void set_chan(Tv2 &tv, int chan);
	void set_input(Tv2 &tv);
	void info(Tv2 &tv) const;
};

class Tv2 {
private:
	int state;
	int volume;
	int maxchannel;
	int channel;
	int input;
public:
	enum {Off, On};
	enum {MinVol, MaxVol = 20};
	enum {TV, DVD};

	Tv2(int s=Off, int mc=125) : state(s), volume(5), maxchannel(mc), channel(2), input(TV) {};
	void onoff() {state ^= 1;};
	void volup();
	void voldown();
	void chanup();
	void chandown();
	void set_input() { input = ((input == TV) ? DVD : TV); }
	friend void Remote2::set_chan(Tv2 &tv, int chan); // Remote2::set_chan() is friend method
	void info() const;
};

// Remote2 methods as inline functions
inline void Remote2::onoff(Tv2 &tv) { tv.onoff(); };
inline void Remote2::volup(Tv2 &tv) { tv.volup(); };
inline void Remote2::voldown(Tv2 &tv) { tv.voldown(); };
inline void Remote2::chanup(Tv2 &tv) { tv.chanup(); };
inline void Remote2::chandown(Tv2 &tv) { tv.chandown(); };
inline void Remote2::set_chan(Tv2 &tv, int chan) { tv.channel = chan; }; // update Tv2 private member
inline void Remote2::set_input(Tv2 &tv) { tv.set_input(); };
inline void Remote2::info(Tv2 &tv) const { tv.info(); };

