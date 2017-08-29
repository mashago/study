
#pragma once

#include <string>

class Dog
{
public:
	Dog(std::string name);
	void Talk();
private:
	std::string m_name;
};
