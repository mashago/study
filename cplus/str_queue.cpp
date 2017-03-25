
extern "C" {
#include <string.h>
}
#include <iostream>
#include <thread>
#include "str_queue.h"

#if (defined __WIN32) || (defined __CYGWIN__)
#define bzero(a, b) memset(a, 0, b)
#endif

StrQueue::StrQueue()
{
	head = 0;
	tail = 0;
	bzero(queue, sizeof(queue));
}


StrQueue::~StrQueue()
{
	// clean queue
	for (int i=0; i<MAX_QUEUE; i++) {
		if (queue[i] != NULL) {
			// std::cout << "clean:" << i << std::endl;	
			free(queue[i]);
		}
	}
	head = 0;
	tail = 0;
}

int StrQueue::isempty() const
{
	for (int i=0; i<MAX_QUEUE; i++) {
		if (queue[i] != NULL) {
			return 0;
		}
	}
	return 1;
}

std::ostream & operator<<(std::ostream &out, const StrQueue &q)
{
	out << "q.head=" << q.head << " q.tail=" << q.tail 
	<< " q.size=" << q.size() << " q.isempty=" << q.isempty() << std::endl;
	return out;
}

void show(const StrQueue &q) // friend function
{
	std::cout << "q.head=" << q.head << " q.tail=" << q.tail 
	<< " q.size=" << q.size() << " q.isempty=" << q.isempty() << std::endl;
}

char * StrQueue::__copy_str(const char *str)
{
	// must free after get()
	// return strdup(str);
	size_t n = strlen(str) + 1;
	char *ptr = (char *)calloc(sizeof(char), n);
	strcpy(ptr, str);
	return ptr;
}

int StrQueue::add(const char * str)
{
	using std::cout;
	using std::endl;

	int ret = 0;
	int tmp_tail;
	char *cpy;

	if (NULL == str) {
		cout << "add:str_null" << endl;
		return -3;
	}
	if ('\0' == str[0]) {
		cout << "add:str_empty" << endl;
		return -13;
	}

	mutex_lock.lock();

	if (head == ((tail+1) % MAX_QUEUE)) {
		ret = -2;
		cout << "add:queue_full" << endl;
		goto cleanup;
	}

	cpy = __copy_str(str);
	queue[tail] = cpy;
	tail = (tail + 1) % MAX_QUEUE;
	tmp_tail = tail;

	if (NULL != queue[tmp_tail]) {
		ret = -7;
		cout << "add:new_tail_not_null" << endl;
		goto cleanup;
	}

	ret = 0;
cleanup:
	mutex_lock.unlock();
	return ret;
}

// operator<< to add
int StrQueue::operator<<(const char *str)
{
	return add(str);
}

// friend operator>> to add
int operator>>(const char *str, StrQueue &q)
{
	return q.add(str);		
}

char * StrQueue::get()
{
	using std::cout;
	using std::endl;

	char *str = NULL;
	if (head == tail) {
		// cout << "get:no_str" << endl;
		return NULL;
	}

	mutex_lock.lock();
	str = queue[head];
	queue[head] = NULL;
	head = (head + 1) % MAX_QUEUE;

	if (NULL == str) {
		str = NULL;
		cout << "get:str_null" << endl;
		goto cleanup;
	}

cleanup:
	mutex_lock.unlock();
	return str;
}

// operator>> to get
char *& StrQueue::operator>>(char *(&str))
{
	return (str=get());
}


///////////////////// TEST START ///////////////////////

void add_function(void *ptr, const char * str, const int size)
{
	StrQueue &q = *((StrQueue *)ptr);
	char buffer[30];
	for (int i=0; i<size; i++) {
		sprintf(buffer, "%s[%d]", str, i);
		q.add(buffer);	
	}
}

void add_function2(void *ptr, const char * str, const int size)
{
	StrQueue &q = *((StrQueue *)ptr);
	char buffer[30];
	for (int i=0; i<size; i++) {
		sprintf(buffer, "%s[%d]", str, i);
		q << buffer; // operator<<
	}
}

void add_function3(void *ptr, const char * str, const int size)
{
	StrQueue &q = *((StrQueue *)ptr);
	char buffer[30];
	for (int i=0; i<size; i++) {
		sprintf(buffer, "%s[%d]", str, i);
		buffer >> q; // friend operator>>, means operator>>(const char*,StrQueue)
	}
}

void get_function(void *ptr, const char *str, const int size)
{
	using std::cout;
	using std::endl;

	StrQueue &q = *((StrQueue *)ptr);
	char * out;
	int count = 0;
	while (count < size) {
		out = q.get();	
		if (out != NULL) {
			// cout << "out[ " << str << "]=" << out << endl;
			free(out);
			out = NULL;
			count ++;
		}
	}

	// show(q);
	cout << q;
}

void get_function2(void *ptr, const char *str, const int size)
{
	using std::cout;
	using std::endl;

	StrQueue &q = *((StrQueue *)ptr);
	char * out;
	int count = 0;
	while (count < size) {
		q >> out; // operator>>
		if (out != NULL) {
			// cout << "out[ " << str << "]=" << out << endl;
			free(out);
			out = NULL;
			count ++;
		}
	}

	// show(q);
	cout << q;
}

int test0()
{
	using std::cout;
	using std::endl;

	const char * str = "hhh";

	StrQueue sq;
	sq.add(str);
	char *out;
	out = sq.get();
	if (out != NULL) {
		cout << "get() out=" << out << endl;
	}
	cout << "sq.size=" << sq.size() << endl;
	return 0;
}

int test1()
{
	std::cout << "test1:for normal get,add" << std::endl;
	const int loop_size = 100;
	StrQueue sq;
	std::thread t1(get_function, (void*)(&sq), "t1", loop_size);
	std::thread t2(add_function, (void*)(&sq), "t2", loop_size);

	t1.join();
	t2.join();
	
	return 0;
}

int test2()
{
	std::cout << "test2:for normal get and 3 ways to add" << std::endl;
	const int loop_size = 100;
	StrQueue sq;

	std::thread t1(get_function, (void*)(&sq), "t1", loop_size * 3);

	std::thread t3(add_function, (void*)(&sq), "t3", loop_size);
	std::thread t4(add_function2, (void*)(&sq), "t4", loop_size);
	std::thread t5(add_function3, (void*)(&sq), "t5", loop_size);

	t1.join();
	t3.join();
	t4.join();
	t5.join();
	
	return 0;
}

int test3()
{
	std::cout << "test3:for operator>> get and 3 ways to add" << std::endl;
	const int loop_size = 10;
	StrQueue sq;

	std::thread t2(get_function2, (void*)(&sq), "t2", loop_size * 3);

	std::thread t3(add_function, (void*)(&sq), "t3", loop_size);
	std::thread t4(add_function2, (void*)(&sq), "t4", loop_size);
	std::thread t5(add_function3, (void*)(&sq), "t5", loop_size);

	t2.join();

	t3.join();
	t4.join();
	t5.join();
	
	return 0;
}

int main(int argc, char *argv[])
{
	
	// test0();
	// test1();
	// test2();
	test3();
	return 0;
}

