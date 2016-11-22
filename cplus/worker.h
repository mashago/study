
class Worker {
private:
	std::string name;
protected:
	virtual void data() const; // can access by derived class member
public:
	Worker(std::string n="no_name");
	virtual ~Worker() {};
	virtual void show() const = 0;
};

// Worker is virtual base class
class Waiter : virtual public Worker {
private:
	int waiter_id;
protected:
	void data() const;
public:
	Waiter(std::string n="no_name", int id=0);
	Waiter(const Worker &w, int id);
	virtual ~Waiter() {};
	void show() const;
};

// Worker is virtual base class
class Singer : public virtual Worker {
private:
	int singer_id;
protected:
	void data() const;
public:
	Singer(std::string n="no_name", int id=0);
	Singer(const Worker &w, int id);
	virtual ~Singer() {};
	void show() const;
};

// only has 1 base class obj
class SingerWaiter : public Waiter, public Singer {
protected:
	void data() const;
public:
	SingerWaiter(std::string n="no_name", int wid=0, int sid=0);
	SingerWaiter(const Worker &w, int wid, int sid);
	SingerWaiter(const Singer &s, int wid);
	SingerWaiter(const Waiter &w, int sid);
	void show() const;
};



