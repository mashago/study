
#include "iostream"
#include "class_tp.h"

template <typename T> Base *Base::create(T t) {
	std::cout<<"t=" << t<< std::endl;		
	return new Mid<T>(5, t);
}

