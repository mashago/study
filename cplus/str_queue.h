#include <mutex>

class StrQueue {
private:
	static const int MAX_QUEUE = 500;
	char *queue[MAX_QUEUE];
	unsigned int head; // index of using
	unsigned int tail; // index of free

	std::mutex mutex_lock;
	char * __copy_str(const char *str);
public:
	StrQueue();
	~StrQueue();

	int isempty() const;
	int size() const { return tail >= head ? (tail - head) : (MAX_QUEUE + tail - head);} // inline
	friend std::ostream & operator<<(std::ostream & os, const StrQueue &q);
	friend void show(const StrQueue &q);

	// operate function
	int add(const char *str);
	int operator<<(const char *str); // overload operator
	friend int operator>>(const char *str, StrQueue &q); // friend function, just for study
	char * get();
	char *& operator>>(char *(&str));

	StrQueue(const StrQueue&) = delete; // avoid user call this
	StrQueue &operator=(const StrQueue&) = delete;
};
