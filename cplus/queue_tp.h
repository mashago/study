
template <typename T> class QueueTP {
private:
	class Node { // nested class
	public:
		T item;
		Node *next;
		Node (const T & i) : item(i), next(0) {};
	};
	Node *head;
	Node *tail;
	int size;
	QueueTP(const QueueTP & q) : size(0) {};
	QueueTP & operator=(const QueueTP & q) { return *this; };
public:
	QueueTP();
	~QueueTP();
	int add(const T &i);
	int get(T &i);
};
