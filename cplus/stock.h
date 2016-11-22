class Stock
{
private:
	char company[30];
	int shares;
	double price;
	double total;
	void set_tot(double total) { this->total = total; }
public:
	Stock();
	~Stock();
	Stock(const char *company, int shares, double price, double total);
	void init(const char *company, int shares, double price, double total);
	void sell(int num, double price);
	void show() const; // let it const
	void show(const Stock s);
	void show_const() const;
};
