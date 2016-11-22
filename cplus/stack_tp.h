
template <typename T> class Stack {
private:
	T *items;
	const int size; // must init in construct init list
	int top;
	static const int SIZE = 10;
	// as private, cannot be call outside
	Stack(const Stack & st) : size(0) {};
public:
	Stack(int n = SIZE);
	~Stack();
	int push(const T &item);
	int pop(T &item);
	// or use delete
	Stack & operator=(const Stack &st) = delete;
};

template class Stack<std::string>; // explicit instantiation

// template <> class Stack<int> { ... }; // explicit specialization
