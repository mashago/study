
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

double get_time_ms()
{
	struct timeval tv;    
	gettimeofday(&tv,NULL);
	return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
}

void use_ifstream_read(const char *file)
{
	printf("%s\n", __FUNCTION__);
	double time1 = get_time_ms();

	// 1. in file stream open file
	// 2. get file size
	// 3. new a char buffer
	// 4. ifs read into buffer

	std::ifstream fin;
	fin.open(file); // default is ios_base::in
	if (!fin.is_open()) {
		printf("open file fail\n");
		return;
	}

	long start = fin.tellg();
	fin.seekg(0, std::ios::end);
	long end = fin.tellg();
	long len = end - start;
	char *buffer = new char[len];

	fin.seekg(0, std::ios::beg);

	fin.read(buffer, len);
	
	// std::cout << "buffer=" << buffer << std::endl;

	fin.close();

	std::string data(buffer, len);
	printf("data.size()=%lu\n", data.size());

	delete [] buffer;

	double time2 = get_time_ms();
	printf("%s:time=%lfms\n\n", __FUNCTION__, time2-time1);
}

void use_istreambuf_iterator(const char *file)
{
	printf("%s\n", __FUNCTION__);
	double time1 = get_time_ms();

	// 1. in file stream open file
	// 2. get file size
	// 3. reserve string
	// 4. string assign or append, from istreambuf_iterator

	std::ifstream fin;
	fin.open(file); // default is ios_base::in
	if (!fin.is_open()) {
		printf("open file fail\n");
		return;
	}

	fin.seekg(0, std::ios::end);
	long end = fin.tellg();

	fin.seekg(0, std::ios::beg);
	long start = fin.tellg();

	long len = end - start;

	std::string buffer;
	buffer.reserve(len);

	// very slow
	buffer.assign((std::istreambuf_iterator<char>(fin)), (std::istreambuf_iterator<char>()));
	// buffer.append((std::istreambuf_iterator<char>(fin)), (std::istreambuf_iterator<char>()));

	// std::cout << "buffer=" << buffer << std::endl;

	fin.close();

	printf("buffer.size()=%lu\n", buffer.size());
	double time2 = get_time_ms();
	printf("%s:time=%lfms\n\n", __FUNCTION__, time2-time1);
}

void use_istreambuf_iterator2(const char *file)
{
	printf("%s\n", __FUNCTION__);
	double time1 = get_time_ms();

	// 1. in file stream open file
	// 2. get file size
	// 3. init string by istreambuf_iterator

	std::ifstream fin;
	fin.open(file); // default is ios_base::in
	if (!fin.is_open()) {
		printf("open file fail\n");
		return;
	}

	// better before, but still slow
	std::string buffer((std::istreambuf_iterator<char>(fin)), (std::istreambuf_iterator<char>()));
	
	// std::cout << "buffer=" << buffer << std::endl;

	fin.close();
	printf("buffer.size()=%lu\n", buffer.size());

	double time2 = get_time_ms();
	printf("%s:time=%lfms\n\n", __FUNCTION__, time2-time1);
}

void use_stringstream(const char *file)
{
	printf("%s\n", __FUNCTION__);
	double time1 = get_time_ms();

	// 1. in file stream open file
	// 2. get file size
	// 3. init string by istreambuf_iterator

	std::ifstream fin;
	fin.open(file); // default is ios_base::in
	if (!fin.is_open()) {
		printf("open file fail\n");
		return;
	}

	std::stringstream sstream;
	sstream << fin.rdbuf();

	std::string buffer(sstream.str());
	
	// std::cout << "buffer=" << buffer << std::endl;

	fin.close();
	printf("buffer.size()=%lu\n", buffer.size());

	double time2 = get_time_ms();
	printf("%s:time=%lfms\n\n", __FUNCTION__, time2-time1);
}

void use_c_file(const char *file)
{
	printf("%s\n", __FUNCTION__);
	double time1 = get_time_ms();

	// 1. open FILE *
	// 2. new char buffer
	// 3. fread into buffer

	FILE *pfile;
	if ((pfile = fopen(file, "r")) == NULL)
	{
		printf("open file fail\n");
		return;
	}
	long len;
	fseek(pfile, 0, SEEK_END);
	len = ftell(pfile);
	char *buffer = new char[len];
	rewind(pfile);
	
	fread(buffer, sizeof(char), len, pfile);

	std::string data(buffer, len);
	printf("data.size()=%lu\n", data.size());

	delete [] buffer;

	double time2 = get_time_ms();
	printf("%s:time=%lfms\n\n", __FUNCTION__, time2-time1);
}

void read_line(const char *file)
{
	printf("read_line: use getline(ifstream)\n");
	double time1 = get_time_ms();

	std::ifstream fin;
	fin.open(file); // default is ios_base::in
	if (!fin.is_open()) {
		printf("open file fail\n");
		return;
	}

	std::string buffer;
	while (getline(fin, buffer))
	{
		// std::cout << "buffer=" << buffer << std::endl;
	}

	fin.close();
	printf("buffer.size()=%lu\n", buffer.size());

	double time2 = get_time_ms();
	printf("read_line:time=%lfms\n\n", time2-time1);
}


int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);

	const char *file = "read_file.cpp";
	use_ifstream_read(file);
	use_istreambuf_iterator(file);
	use_istreambuf_iterator2(file);
	use_stringstream(file);
	use_c_file(file);

	read_line(file);

	return 0;
}
