#ifndef __ANIMAL_H__
#define __ANIMAL_H__

#include <stdio.h>

class animal
{
private:
	int height;
	int weight;
public:
	// no default constructor
	// extend class have to init parent in constructor init list
	animal(int h, int w) : height(h), weight(w)
	{
	}

	virtual ~animal()
	{
	}

	virtual void print()
	{
		printf("height=%d weight=%d\n", height, weight);
	}
};

class dog : public animal
{
private:
	int type;
public:
	/*
	// error, parent class has no default constructor, so have to init parent in dog constructor init list
	dog(int h, int w, int t)
	{
		height = h;
		weight = w;
		type = t;
	}
	*/

	dog(int h, int w, int t) : animal(h, w), type(t)
	{
	}

	~dog()
	{
	}

	void print()
	{
		// call parent function
		animal::print();
		printf("type=%d\n", type);
	}
};

class plant
{
private:
	int height;
	int weight;
public:
	plant() : height(0), weight(0)
	{
	}

	plant(int h, int w) : height(h), weight(w)
	{
	}

	virtual ~plant()
	{
	}

	virtual void print()
	{
		printf("height=%d weight=%d\n", height, weight);
	}
};

class rose : public plant
{
private:
	int type;
public:
	rose()
	{
		type = 0;
	}

	rose(int h, int w, int t) : plant(h, w), type(t)
	{
	}

	~rose()
	{
	}

	void print()
	{
		// call parent function
		plant::print();
		printf("type=%d\n", type);
	}
};

#endif
