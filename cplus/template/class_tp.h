
class Base
{
private:
	int value;
public:
	Base(int v=0) : value(v) {};
	template <typename T> static Base * create(T t); 
};

template <typename Type> 
class Mid : public Base
{
private:
	Type ttt;
public:
	Mid(int v, Type t) : Base(v), ttt(t) {};
};

