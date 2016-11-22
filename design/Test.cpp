#include <stdio.h>
#include <vector>

#include "ClassFactory.h"
#include "ClassSingleton.h"

class Car
{
public:
	int m_Color;
	void Init()
	{
		m_Color = 10;
	}
	void Show()
	{
		printf("m_Color=%d\n", m_Color);
	}
};

int test_class_factory()
{
	printf("\n----------- test_class_factory() ------------\n");
	int total = 20;

	CClassFactory<Car>::GetThis()->Init(total);
	printf("before Factory size=%zu\n", CClassFactory<Car>::GetThis()->Size());

	
	// my space
	std::vector<Car*> my_cars;

	Car * pCar;
	pCar = CClassFactory<Car>::GetThis()->Pop();
	pCar->m_Color = 2;
	pCar->Show();
	my_cars.push_back(pCar);

	pCar = CClassFactory<Car>::GetThis()->Pop();
	pCar->m_Color = 3;
	pCar->Show();
	my_cars.push_back(pCar);

	printf("after Factory size=%zu\n", CClassFactory<Car>::GetThis()->Size());

	for (auto it=my_cars.begin(); it<my_cars.end();)
	{
		// memory recycle
		CClassFactory<Car>::GetThis()->Push(*it);
		my_cars.erase(it);
	}

	printf("end Factory size=%zu\n", CClassFactory<Car>::GetThis()->Size());

	return 0;
}

#define CarSingleton ClassSingleton<Car>::instance()

int test_class_singleton()
{
	printf("\n----------- test_class_singleton() ------------\n");

	ClassSingleton<Car>::instance()->Init();
	ClassSingleton<Car>::instance()->Show();
	ClassSingleton<Car>::instance()->m_Color = 7;

	CarSingleton->Show();

	ClassSingleton<Car>::clear();

	return 0;
}

int main(int argc, char ** argv)
{
	printf("hello cpp design\n");

	test_class_factory();
	test_class_singleton();
	test_class_singleton();

	return 0;
}
