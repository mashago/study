
#include "http_util.h"

void print_map(std::map<std::string, std::string> &my_map)
{
	for (auto iter = my_map.begin(); iter != my_map.end(); iter++)
	{
		printf("[%s]:[%s]\n", iter->first.c_str(), iter->second.c_str());
	}
}

int trim_test()
{
	printf("---- trim test ----\n");
	std::string buffer;

	buffer = "";
	trim_string(buffer);
	printf("1 buffer=[%s]\n", buffer.c_str());

	buffer = "a";
	trim_string(buffer);
	printf("2 buffer=[%s]\n", buffer.c_str());

	buffer = "ab";
	trim_string(buffer);
	printf("3 buffer=[%s]\n", buffer.c_str());

	buffer = " a";
	trim_string(buffer);
	printf("4 buffer=[%s]\n", buffer.c_str());

	buffer = "a ";
	trim_string(buffer);
	printf("5 buffer=[%s]\n", buffer.c_str());

	buffer = " ";
	trim_string(buffer);
	printf("6 buffer=[%s]\n", buffer.c_str());

	buffer = " ab";
	trim_string(buffer);
	printf("7 buffer=[%s]\n", buffer.c_str());

	buffer = "ab ";
	trim_string(buffer);
	printf("8 buffer=[%s]\n", buffer.c_str());

	buffer = " ab ";
	trim_string(buffer);
	printf("9 buffer=[%s]\n", buffer.c_str());

	buffer = "  ";
	trim_string(buffer);
	printf("10 buffer=[%s]\n", buffer.c_str());

	printf("\n");
	return 0;
}

int header_to_map_test()
{
	printf("---- header to map test ----\n");

	std::string buffer;
	std::map<std::string, std::string> my_map;

	buffer = "";
	my_map.clear();
	header_to_map(buffer, my_map);
	printf("1 my_map:\n");
	print_map(my_map);

	buffer = "aa:11";
	my_map.clear();
	header_to_map(buffer, my_map);
	printf("2 my_map:\n");
	print_map(my_map);

	buffer = "aa: 11";
	my_map.clear();
	header_to_map(buffer, my_map);
	printf("3 my_map:\n");
	print_map(my_map);

	buffer = " aa: 11 ";
	my_map.clear();
	header_to_map(buffer, my_map);
	printf("4 my_map:\n");
	print_map(my_map);

	buffer = "aa:";
	my_map.clear();
	header_to_map(buffer, my_map);
	printf("5 my_map:\n");
	print_map(my_map);

	buffer = ":";
	my_map.clear();
	header_to_map(buffer, my_map);
	printf("6 my_map:\n");
	print_map(my_map);

	buffer = ":bb";
	my_map.clear();
	header_to_map(buffer, my_map);
	printf("7 my_map:\n");
	print_map(my_map);

	buffer = " aa : bb ";
	my_map.clear();
	header_to_map(buffer, my_map);
	printf("8 my_map:\n");
	print_map(my_map);


	printf("\n");
	return 0;
}

int main(int argc, char **argv)
{
	trim_test();
	header_to_map_test();
	return 0;
}

