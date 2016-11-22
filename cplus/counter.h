class Counter
{
	int num;
public:
	static int flag;
	Counter(); // default constructor, == {Counter x;}
	Counter(int num);
	Counter(const Counter &c); // copy constructor, == {Counter x = y;}, please over write
	void set_num(int num);
	Counter sum(const Counter &c);
	Counter operator+(const Counter &c);
	friend Counter operator*(const Counter &c, int m);
	Counter & operator=(const Counter &c); // operator=, == {Counter x; x = y;}, please over write
	operator int() const;
	void show();

	static int get_flag();
};
