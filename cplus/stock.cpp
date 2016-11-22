#include <iostream>
#include "stock.h"

Stock::Stock()
{
	std::cout << "Sotck_create1" << std::endl;
	sprintf(company, "empty");
	shares = 0;
	price = 0;
	total = 0;
}

Stock::~Stock()
{
	// show();
	printf("bye\n");
}

Stock::Stock(const char *company, int shares, double price, double total)
{
	std::cout << "Sotck_create2" << std::endl;
	// this is pointer
	sprintf(this->company, "%s", company);
	this->shares = shares;
	this->price = price;
	this->total = total;
}

void Stock::init(const char *company, int shares, double price, double total)
{
	sprintf(this->company, "%s", company);
	this->shares = shares;
	this->price = price;
	this->total = total;
}

void Stock::show() const
{
	using std::cout;
	using std::endl;
	using std::ios_base;
	ios_base::fmtflags orig = cout.setf(ios_base::fixed, ios_base::floatfield);
	std::streamsize prec = cout.precision(2);
	cout << "show:company=" << company << " shares=" << shares << " price=" << price << " total=" << total << endl;
	cout.setf(orig, ios_base::floatfield);
	cout.precision(prec);
}

void Stock::show(const Stock s)
{
	printf("show: company=%s shares=%d price=%lf total=%lf\n"
	, s.company, s.shares, s.price, s.total);
}

void Stock::show_const() const
{
	printf("show_const: company=%s shares=%d price=%lf total=%lf\n"
	, company, shares, price, total);
}
