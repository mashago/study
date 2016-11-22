
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <string>
#include <sstream>
#include <fstream>

double get_time_ms()
{
	struct timeval tv;    
	gettimeofday(&tv,NULL);
	return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
}

void read1(const char *file)
{
	printf("read 1: use ifstream.read\n");

	double time1 = get_time_ms();

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

	std::string data = std::string(buffer, len);
	printf("data.size()=%lu\n", data.size());

	delete [] buffer;

	double time2 = get_time_ms();
	printf("read1:time=%lfms\n\n", time2-time1);
}

void read2(const char *file)
{
	printf("read 2: use istreambuf_iterator1\n");

	double time1 = get_time_ms();

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

	// std::cout << "buffer=" << buffer << std::endl;

	fin.close();

	printf("buffer.size()=%lu\n", buffer.size());
	double time2 = get_time_ms();
	printf("read2:time=%lfms\n\n", time2-time1);
}

void read3(const char *file)
{
	printf("read 3: use istreambuf_iterator2\n");
	double time1 = get_time_ms();

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
	printf("read3:time=%lfms\n\n", time2-time1);
}

void read4(const char *file)
{
	printf("read 4: use stringstream\n");
	double time1 = get_time_ms();

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
	printf("read4:time=%lfms\n\n", time2-time1);
}

void read5(const char *file)
{
	printf("read 5: use c FILE, new char buffer\n");
	double time1 = get_time_ms();

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
	printf("read5:time=%lfms\n\n", time2-time1);
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
	printf("hello read_file\n");

	const char *file = "Illegal.txt";
	// const char *file = "Illegal2.txt";
	read1(file);
	read2(file);
	read3(file);
	read4(file);
	read5(file);
	read_line(file);

	return 0;
}
