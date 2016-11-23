
#include <iostream>
#include <sstream>
#include "json/json.h"

using namespace std;

int test0()
{
	std::cout << "--------------- test0 ---------------" << std::endl;
	// Json::Reader reader;
	Json::Reader reader(Json::Features::strictMode());
	Json::Value value;

	cout << "test1" << endl;
	string json1 = "{\"id\":20,\"name\":\"你好吗\"}";

	if (reader.parse(json1, value)) {
		if (!value["id"].isNull()) {
			cout << "value[\"id\"]=" << value["id"].asInt() << endl;
		} 
		if (!value["name"].isNull()) {
			cout << "value[\"name\"]=" << value["name"].asString() << endl;
		}
	}



	cout << "test2" << endl;
	string json2 = "[{\"id\":20,\"name\":\"masha\"},{\"id\":21,\"name\":\"jack\"}]";

	if (reader.parse(json2, value)) {
		int size = value.size();
		cout << "value.size()=" << size << endl;
		for (int i=0; i<size; i++) {
			cout << "id=" << value[i]["id"].asInt();
			cout << " name=" << value[i]["name"].asString() << endl;
		}
	}


	cout << "test3" << endl;
	string json3;
	Json::FastWriter writer;
	Json::Value root;
	Json::Value team;
	Json::Value person;

	person["id"] = 1990;
	person["name"] = "老陈";
	team.append(person);

	person["id"] = 2000;
	person["name"] = "zxcv";
	team.append(person);

	root["team"] = team;
	json3 = writer.write(root);

	cout << "json3=" << json3 << endl;

	root.clear();
	json3 = writer.write(root);

	cout << "json3=" << json3 << endl;
	return 0;
}

int test1()
{
	std::cout << "--------------- test1 ---------------" << std::endl;
	Json::Value root;

	root["code"] = 1;
	Json::Value d;
	d["result"] = "发货成功";
	d["orderno"] = "12345";
	root["data"] = d;

	std::ostringstream oss;
	oss << root;
	std::cout << "str=" << oss.str() << std::endl;

	std::cout << "root=" << root << std::endl;

	return 0;
}

int test2()
{
	std::cout << "--------------- test2 ---------------" << std::endl;
	Json::Value root;

	Json::Value s1;
	s1["id"] = 1;
	s1["ip"] = "192.168.1.1";
	s1["port"] = 40001;
	root.append(s1);

	s1["id"] = 2;
	s1["ip"] = "192.168.1.2";
	s1["port"] = 40002;
	root.append(s1);

	std::ostringstream oss;
	oss << root;
	std::cout << "str=" << oss.str() << std::endl;

	return 0;
}

int test3()
{
	std::cout << "--------------- test3 ---------------" << std::endl;
	Json::Reader reader(Json::Features::strictMode());
	Json::Value value;

	string json1 = "not a json";

	if (reader.parse(json1, value))
	{
		printf("test3:json1 is a json\n");
	}
	else
	{
		printf("test3:json1 is not a json\n");
	}


	return 0;
}

int main(int argc, char * argv[])
{
	test0();
	test1();
	test2();
	test3();

	return 0;
}
