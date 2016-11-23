#include <stdio.h>
#include <string.h>
#include <fstream>
#include <string>
#include <iostream>

#include "masha.pb.h"

void print_person(MashaProto::Person &person)
{
	printf("person.ByteSize()=%d\n", person.ByteSize());
	printf("person.id()=%d\n", person.id());
	printf("person.name()=%s\n", person.name().c_str());
	if (person.has_age()) {
		printf("person.age=%d\n", person.age());
	} else {
		printf("person has no age\n");
	}
	int numlist_size = person.numlist_size();
	printf("person.numlist_size()=%d\n", numlist_size);
	for (int i=0; i<numlist_size; i++) {
		printf("person.numlist[%d]=%d\n", i, person.numlist(i));
	}
	int strlist_size = person.strlist_size();
	printf("person.strlist_size()=%d\n", strlist_size);
	for (int i=0; i<strlist_size; i++) {
		printf("person.strlist[%d]=%s\n", i, person.strlist(i).c_str());
	}
	printf("person.color()=%d\n", person.color());
	printf("\n");
	
}

void write_to_file(const char *file_name)
{
	std::cout << "write to file" << std::endl;
	MashaProto::Person p;
	p.set_id(20);
	p.set_name("Masha");
	p.set_age(26);
	p.add_numlist(1); // repeated int32 numList
	p.add_numlist(3);
	std::string *s1 = p.add_strlist(); // repeated string strList
	*s1 = "mmm";
	std::string *s2 = p.add_strlist(); 
	*s2 = "aaa";
	p.set_color(MashaProto::Color::BLACK);

	print_person(p);

	std::string buffer;
	p.SerializeToString(&buffer);

	std::ofstream fout;
	fout.open(file_name);
	if (!fout.is_open())
	{
		return;
	}

	fout << buffer;
	std::flush(fout);
}

void read_from_file(const char *file_name)
{
	std::cout << "read from file" << std::endl;
	std::ifstream fin;
	fin.open(file_name, std::ios::in | std::ios::binary);
	if (!fin.is_open())
	{
		return;
	}

	long start_pos = fin.tellg();
	fin.seekg(0, std::ios::end);
	long end_pos = fin.tellg();
	long len = end_pos - start_pos;

	char *buffer = new char[len];

	fin.seekg(0, std::ios::beg);
	fin.read(buffer, len);

	MashaProto::Person p;
	p.ParseFromArray(buffer, len);

	print_person(p);

	delete [] buffer;

}

int main(int argc, char **argv)
{
	printf("hello protobuf test2\n");
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	const char * file_name = "person_data.txt";
	// write_to_file(file_name);
	read_from_file(file_name);

	// must call in the end, avoid memory leek
	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}
