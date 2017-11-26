
#include <stdio.h>
#include "cat.h"

Cat::Cat(std::string name)
{
	m_name = name;
}

void Cat::Talk()
{
	printf("my name is %s\n", m_name.c_str());
}
