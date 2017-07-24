
#include <stdio.h>
#include "dog.h"

Dog::Dog(std::string name)
{
	m_name = name;
}

void Dog::Talk()
{
	printf("my name is %s\n", m_name.c_str());
}
