#ifndef _CLASS_SINGLETON_
#define _CLASS_SINGLETON_

#include <stdio.h>

template <typename T>
class ClassSingleton
{
private:
	ClassSingleton() {};
	ClassSingleton(const ClassSingleton<T> &) {};
	const ClassSingleton<T> & operator=(const ClassSingleton<T> &) { return *this; };
protected:
	static T *s_pInstance;
public:
	virtual ~ClassSingleton()
	{
		// will never call this because can not create object
		clear();
	}

	static T * instance()
	{
		if (s_pInstance != NULL)
		{
			// printf("s_pInstance not NULL\n");
			return s_pInstance;
		}
		printf("s_pInstance NULL\n");
		s_pInstance = new T;
		return s_pInstance;
	}

	static void clear()
	{
		printf("clear()\n");
		if (s_pInstance == NULL)
		{
			return;
		}
		delete s_pInstance;
		s_pInstance = NULL;
	}

};

// create space for static member
template <typename T>
T * ClassSingleton<T>::s_pInstance = NULL;

#endif
