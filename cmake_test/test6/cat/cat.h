
#pragma once

#include <string>

class Cat
{
public:
	Cat(std::string name);
	void Talk();
private:
	std::string m_name;
};
