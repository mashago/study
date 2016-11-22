
#include <stdio.h>
#include "order_queue_tp.h"

/////////////////// for Dog
struct Dog
{
	int id;
	long value;
};

// both type are ok
// bool compareDog(const Dog &a, const Dog &b)
bool compareDog(Dog const &a, Dog const &b)
{
	if (a.value < b.value)
	{
		return false;
	}
	return true;
}

// both type are ok
// long getKeyDog(Dog const &a)
long getKeyDog(const Dog &a)
{
	return a.id;
}

void printQueueDog(OrderQueue<Dog, long> &queue)
{
	printf("-------------------------\n");
	list<Dog> &container = queue.getContainer();
	for (auto iter = container.begin(); iter != container.end(); iter++)
	{
		printf("id = %d value = %ld\n", (*iter).id, (*iter).value);
	}
}

/////////////////// for Cat
struct Cat
{
	int id;
	long value;
};

// although const Cat * &name == Cat * &name, but compiler cannot convert it, so "bool compareCat(const Cat * &a, const Cat * &b)" is error
bool compareCat(Cat * const &a, Cat * const &b)
{
	if (a->value < b->value)
	{
		return false;
	}
	return true;
}

long getKeyCat(Cat * const &a)
{
	return a->id;
}

void printQueueCat(OrderQueue<Cat *, long> &queue)
{
	printf("-------------------------\n");
	list<Cat *> &container = queue.getContainer();
	for (auto iter = container.begin(); iter != container.end(); iter++)
	{
		printf("id = %d value = %ld\n", (*iter)->id, (*iter)->value);
	}
}

int main(int argc, char **argv)
{
	// dog queue
	printf("dog queue\n");
	OrderQueue<Dog, long> queueDog = OrderQueue<Dog, long>(compareDog, getKeyDog);
	Dog d1;
	d1.id = 1;
	d1.value = 1;
	Dog d2;
	d2.id = 3;
	d2.value = 3;
	Dog d3;
	d3.id = 2;
	d3.value = 2;

	queueDog.insert(d1);
	queueDog.insert(d2);
	queueDog.insert(d3);

	printQueueDog(queueDog);

	d3.value = 5;
	queueDog.update(d3);
	printQueueDog(queueDog);

	queueDog.erase(d1.id);
	printQueueDog(queueDog);

	queueDog.insert(d1);
	printQueueDog(queueDog);

	printf("\n");

	// cat queue
	printf("cat queue\n");
	OrderQueue<Cat *, long> queueCat = OrderQueue<Cat *, long>(compareCat, getKeyCat);
	Cat c1;
	c1.id = 1;
	c1.value = 1;
	Cat c2;
	c2.id = 3;
	c2.value = 3;
	Cat c3;
	c3.id = 2;
	c3.value = 2;

	queueCat.insert(&c1);
	queueCat.insert(&c2);
	queueCat.insert(&c3);

	printQueueCat(queueCat);

	c3.value = 5;
	queueCat.update(&c3);
	printQueueCat(queueCat);

	queueCat.erase(c1.id);
	printQueueCat(queueCat);

	queueCat.insert(&c1);
	printQueueCat(queueCat);


	return 0;
}
