
class AcctABC {
private:
	double balance;
public:
	AcctABC(double bal=0.0);
	virtual ~AcctABC() {};
	double getBalance() const {return balance;}
	virtual void update(double offset) = 0; // pure virtual function
	virtual void view() const = 0; // pure virtual function
};

class Brass : public AcctABC {
public:
	Brass(double bal=0.0) : AcctABC(bal) {};
	virtual ~Brass() {};
	virtual void update(double offset);
	virtual void view() const;
};

class BrassPlus : public AcctABC {
private:
	double extra;
public:
	BrassPlus(double bal=0.0, double ex=0.0);
	virtual ~BrassPlus() {};
	// override, final work since c++11
	virtual void update(double offset) override; // override means this method orerride base class virtual method
	virtual void view() const final; // final means this method cannot be override by its derived class
};
