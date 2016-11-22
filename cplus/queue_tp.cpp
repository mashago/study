
#include <iostream>
#include <cstring>
#include "queue_tp.h"

template <typename T> QueueTP<T>::QueueTP()
{
	head = tail = 0;
	size = 0;
}

template <typename T> QueueTP<T>::~QueueTP()
{
	Node *tmp;
	while (head != NULL) {
		std::cout << "delete item:" << head->item << std::endl;
		tmp = head->next;
		delete head;
		head = tmp;
	}
}

template <typename T> int QueueTP<T>::add(const T &t)
{
	// Node is nested class
	Node * n = new Node(t);
	if (head == NULL) {
		head = n;
	} else {
		tail->next = n;
	}
	tail = n;

	return 0;
}

template <typename T> int QueueTP<T>::get(T &t)
{
	if (head == NULL) {
		return 0;
	}
	t = head->item;
	Node *tmp = head->next;
	delete head;
	head = tmp;
	if (head == NULL) {
		tail = NULL;
	}
	return 0;
}
		
	

int main(int argc, char * argv[])
{
	using std::cout;	
	using std::endl;	
	using std::string;	

	// cout << "hello queue" << endl;

	QueueTP<int> q1;
	int i1 = 200;
	int i2 = 300;
	int i3 = 400;

	q1.add(i1);
	q1.add(i2);
	q1.add(i3);

	int i4;
	q1.get(i4);
	cout << "i4=" << i4 << endl;
	q1.get(i4);
	cout << "i4=" << i4 << endl;
	q1.get(i4);
	cout << "i4=" << i4 << endl;
	cout << "--------------------------" << endl;

	QueueTP<string> q2;
	string s1 = "hello";
	string s2 = "world";
	string s3 = "mother";
	string s4 = "daddy";

	q2.add(s1);
	q2.add(s2);
	q2.add(s3);
	q2.add(s4);

	string s5;
	q2.get(s5);
	cout << "s5=" << s5 << endl;
	q2.get(s5);
	cout << "s5=" << s5 << endl;
	q2.get(s5);
	cout << "s5=" << s5 << endl;
	cout << "--------------------------" << endl;

	return 0;
}
