
#include <sys/time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

double get_time_ms()
{
	struct timeval tv;    
	gettimeofday(&tv,NULL);
	return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
}

int test_cout()
{
	using std::cout;
	using std::endl;

	cout << "---------------test_cout()------------" << endl;

	const char *tmp = "abcdefghijklmnopqrstuvwxyz";
	cout << "tmp=" << tmp << endl;

	// put(char);
	cout.put('W').put('X').put('Z');
	cout << endl;

	// write(const char *, streamsize n);
	cout.write(tmp, 20);
	cout << endl;

	cout << "flush tmp=" << tmp << std::flush;
	std::flush(cout); // or use this
	cout << endl;

	int num;
	num = 10;

	cout << "num(10)=" << num << endl;
	cout << std::hex;
	cout << "num(16)=" << num << endl;
	cout << std::oct;
	cout << "num(8)=" << num << endl;
	std::dec(cout);
	cout << "num(10)=" << num << endl;

	// width(size) only affect next cout
	cout.width(5);
	cout << num << endl;
	cout << num << endl;
	cout.width(10);
	cout << num << endl;

	// fill(char); will affect all cout, default is ' '
	cout.fill('*');
	cout.width(5);
	cout << num << endl;
	cout << num << endl;
	cout.width(10);
	cout << num << endl;
	cout.fill(' ');

	num++;

	double dd;
	dd = 10.123;
	cout << "dd=" << dd << endl;
	cout.precision(3);
	cout << "dd=" << dd << endl;
	cout << "dd=" << dd << endl;

	cout.precision(6);
	cout.setf(std::ios_base::showpoint);
	cout << "showpoint dd=" << dd << endl;

	std::ios_base::fmtflags old = cout.setf(std::ios_base::scientific, std::ios_base::floatfield);
	cout << "scientific dd=" << dd << endl;
	cout.setf(old, std::ios_base::floatfield);
	cout << "old dd=" << dd << endl;
	cout.unsetf(std::ios_base::floatfield);
	cout.unsetf(std::ios_base::basefield);
	cout << "unsetf dd=" << dd << endl;

	cout.width(20);
	cout << std::right; // call cout.setf(ios_base::right, ios_base::adjustfield);
	cout << "right dd=" << dd << endl;



	return 0;
}


int test_cin()
{
	using std::cin;
	using std::cout;
	using std::endl;
	cout << "---------------test_cin()------------" << endl;

	// cin >> xxx // get a word or a num, ignore space
	// char = cin.get() // will return EOF when to the end
	// istream& = cin.get(char) // will return false when to the end

	// if no char get, set failbit
	// istream& = cin.get(char *, int size, char split)  // leave '\n' in istream
	// istream& = cin.get(char *, int size); // default use '\n'
	
	// if no char get, except '\n', set failbit
	// if over max size, set failbit
	// istream& = cin.getline(char *, int size, char split)  // get '\n' and drop it
	// istream& = cin.getline(char *, int size); // default use '\n'
	// cin.ignore(int size, char ch) // ignore size of char or to ch

	int num;
	cout << "enter num:" << endl;
	while (cin >> num) {
		cout << num << endl;
	}

	char ch;
	if (cin.fail()) {
		cin.clear();
		while ((ch = cin.get()) != '\n') {
			continue;
		}
		cout << "enter num2:" << endl;
		while (cin >> num) {
			cout << num << endl;
		}
	}

	cin.clear();
	while ((ch = cin.get()) != '\n') {
		continue;
	}

	cout << "enter buffer" << endl;
	char buffer[30];
	cin.get(buffer, 30, '\n');
	cout << "buffer=" << buffer << endl;
	cout << "peek()=" << cin.peek()-0 << endl; // see next char is what
	// cin.get(); // get leave '\n'

	cin.putback('H'); // put a char in istream

	cout << "enter buffer2" << endl;
	cin.getline(buffer, 30, '\n');
	cout << "buffer2=" << buffer << endl;



	return 0;
}

int test_finout()
{
	using std::cin;
	using std::cout;
	using std::endl;
	using std::ofstream;
	using std::ifstream;
	using std::fstream;
	cout << "---------------test_finout()------------" << endl;

	// ios_base::app ? ios_base::ate : 
	// ios_base::app ONLY can add data to end
	// ios_base::ate just put pointer to end

	const char * file = "stream_file.txt";
	ofstream fout;
	fout.open(file); // default is ios_base::out | ios_base::trunc
	// same as ofstream fout("xxxxx");
	if (!fout.is_open()) {
		cout << "open out file fail" << endl;
		return 0;
	}
	fout << "hello ofstream" << endl;

	ifstream fin;
	fin.open(file); // default is ios_base::in
	if (!fin.is_open()) {
		cout << "open in file fail" << endl;
		return 0;
	}

	char data[80];
	fin.getline(data, 80);
	cout << "data=[" << data << "]" << endl;

	fin.seekg(0, std::ios_base::beg); // istream go to begin

	std::string data2;
	getline(fin, data2);
	cout << "data2=[" << data2 << "]" << endl;


	cout << endl;

	char buffer[100];
	fstream finout;
	// istream ostream at one fstream
	finout.open(file, std::ios_base::in | std::ios_base::out | std::ios_base::ate);
	if (finout.is_open()) {
		finout.seekg(0);
		finout.getline(buffer, 100);
		cout << "buffer=" << buffer << endl;

		finout.seekp(0);
		cout << "finout.tellp()=" << finout.tellp() << " finout.tellg()=" << finout.tellg() << endl;
		const char *tmp = "change";
		finout << tmp << endl;
		
		cout << "finout.tellp()=" << finout.tellp() << " finout.tellg()=" << finout.tellg() << endl;

		finout.seekg(0, std::ios_base::beg);
		finout.getline(buffer, 100);
		cout << "buffer=" << buffer << endl;

		finout.getline(buffer, 100);
		cout << "buffer=" << buffer << endl;

		finout.seekg(5, std::ios_base::beg);
		finout.getline(buffer, 100);
		cout << "buffer=" << buffer << endl;
	}

	return 0;

}

void read_ifstream1(const char *file)
{
	std::cout << "read 1: use ifstream read" << std::endl;	

	double time1 = get_time_ms();

	std::ifstream fin;
	fin.open(file); // default is ios_base::in
	if (!fin.is_open()) {
		std::cout << "open in file fail" << std::endl;
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

void read_ifstream2(const char *file)
{
	std::cout << "read 2: use istreambuf_iterator1" << std::endl;

	double time1 = get_time_ms();

	std::ifstream fin;
	fin.open(file); // default is ios_base::in
	if (!fin.is_open()) {
		std::cout << "open in file fail" << std::endl;
		return;
	}

	long start = fin.tellg();
	fin.seekg(0, std::ios::end);
	long end = fin.tellg();
	long len = end - start;

	std::string buffer;
	buffer.reserve(len);

	fin.seekg(0, std::ios::beg);

	// very slow
	buffer.assign((std::istreambuf_iterator<char>(fin)), (std::istreambuf_iterator<char>()));

	// std::cout << "buffer=" << buffer << std::endl;

	fin.close();

	printf("buffer.size()=%lu\n", buffer.size());
	double time2 = get_time_ms();
	printf("read2:time=%lfms\n\n", time2-time1);
}

void read_ifstream3(const char *file)
{
	std::cout << "read 3: use istreambuf_iterator2" << std::endl;
	double time1 = get_time_ms();

	std::ifstream fin;
	fin.open(file); // default is ios_base::in
	if (!fin.is_open()) {
		std::cout << "open in file fail" << std::endl;
		return;
	}

	std::string buffer((std::istreambuf_iterator<char>(fin)), (std::istreambuf_iterator<char>()));
	
	// std::cout << "buffer=" << buffer << std::endl;

	fin.close();
	printf("buffer.size()=%lu\n", buffer.size());

	double time2 = get_time_ms();
	printf("read3:time=%lfms\n\n", time2-time1);
}

void read_ifstream4(const char *file)
{
	std::cout << "read 4: use stringstream" << std::endl;	
	double time1 = get_time_ms();

	std::ifstream fin;
	fin.open(file); // default is ios_base::in
	if (!fin.is_open()) {
		std::cout << "open in file fail" << std::endl;
		return;
	}

	std::stringstream sstream;
	sstream << fin.rdbuf();

	std::string buffer(sstream.str());
	
	// std::cout << "buffer=" << buffer << std::endl;

	fin.close();
	printf("buffer.size()=%lu\n", buffer.size());

	double time2 = get_time_ms();
	printf("read3:time=%lfms\n\n", time2-time1);
}

int test_ifstream()
{

	std::cout << "---------------test_ifstream()------------" << std::endl;
	
	const char * file = "Illegal.txt";
	read_ifstream1(file);
	read_ifstream2(file);
	read_ifstream3(file);
	read_ifstream4(file);

	return 0;
}

int main(int argc, char *argv[])
{
	// test_cout();
	// test_cin();
	// test_finout();
	test_ifstream();
	return 0;
}
