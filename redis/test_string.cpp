
extern "C" {
#include <stdio.h>
}

#include <iostream>
#include <map>
#include "lib_acl.h"
#include "acl_cpp/lib_acl.hpp"

using namespace std;

#define REDIS_IP "127.0.0.1"
#define REDIS_PORT 6379
#define REDIS_PASSWORD "masha"

// global redis operator
acl::redis g_redis;
#define MY_KEY_NAME1 "my_string_key1"
#define MY_KEY_NAME2 "my_string_key2"
#define MY_KEY_NAME3 "my_string_key3"

// set : set key and value
int test0()
{
	acl::string key;
	acl::string value; 

	key = MY_KEY_NAME1;
	value = "a1";
	printf("test0:key=%s value=%s\n", key.c_str(), value.c_str());

	g_redis.clear();
	if (g_redis.set(key.c_str(), value.c_str()) == false)
	{
		printf("test0:set key fail:key=[%s] error:%s\n", key.c_str(), g_redis.result_error());
		return -1;
	}
	printf("test0:set key[%s] ok\n", key.c_str());



	key = MY_KEY_NAME2;
	char buffer[10];
	buffer[0] = 'a';
	buffer[1] = '2';
	buffer[2] = '\0';
	buffer[3] = 'x';
	buffer[4] = '\0';
	value = acl::string(buffer, 4);

	g_redis.clear();
	if (g_redis.set(key.c_str(), key.size(), value.c_str(), value.size()) == false)
	{
		printf("test0:set key fail:key=[%s] error:%s\n", key.c_str(), g_redis.result_error());
		return -1;
	}
	printf("test0:set key[%s] ok\n", key.c_str());
	
	return 0;
}

// get : get value by key
int test1()
{
	acl::string key;
	acl::string value; 

	key = MY_KEY_NAME1;

	g_redis.clear();
	if (g_redis.get(key.c_str(), value) == false)
	{
		printf("test1:get key fail:key=[%s]\n", key.c_str());
		return -1;
	}
	printf("test1:get key ok,key=[%s] value=[%s] len=%d\n", key.c_str(), value.c_str(), (int)value.length());



	key = MY_KEY_NAME2;

	g_redis.clear();
	if (g_redis.get(key.c_str(), value) == false)
	{
		printf("test1:get key fail:key=[%s]\n", key.c_str());
		return -1;
	}
	printf("test1:get key ok,key=[%s] value=[%s] len=%d\n", key.c_str(), value.c_str(), (int)value.length());

	return 0;
}

// del : delete by key
// exists : check if key exists
int test2()
{
	std::string key;
	std::string value;
	acl::string result; // result must be acl::string

	key = MY_KEY_NAME2;
	value = "a3";
	printf("test2:key=%s value=%s\n", key.c_str(), value.c_str());

	g_redis.clear();
	if (g_redis.set(key.c_str(), value.c_str()) == false)
	{
		printf("test2:set key fail:key=[%s] error:%s\n", key.c_str(), g_redis.result_error());
		return -1;
	}
	printf("test2:set key[%s] ok\n", key.c_str());

	// check exist
	g_redis.clear();
	if (g_redis.exists(key.c_str()) == false)
	{
		printf("test2:not exists key:%s\n", key.c_str());
		return -1;
	}
	printf("test2:exists key:%s\n", key.c_str());

	// do delete
	g_redis.clear();
	int ret = g_redis.del_one(key.c_str());
	if (ret < 0)
	{
		printf("test2:delete key fail:key=[%s] error:%s\n", key.c_str(), g_redis.result_error());
		return -1;
	}
	printf("test2:delete key ok\n");

	// check exist
	g_redis.clear();
	if (g_redis.exists(key.c_str()) == true)
	{
		printf("test2:exists key:%s\n", key.c_str());
		return -1;
	}
	printf("test2:not exists key:%s\n", key.c_str());
	
	return 0;
}

typedef int (*testcase_t) ();
testcase_t test_list[] =
{
	test0
,	test1
,	test2
};


int main(int argc, char **argv)
{
	printf("hello test_string\n");
	printf("REDIS_IP=[%s] REDIS_PORT=[%d] REDIS_PASSWORD=[%s]\n", REDIS_IP, REDIS_PORT, REDIS_PASSWORD);

	// 1.init acl
	// 2.init client
	// 3.set client
	// 4.do auth

	// must do this
	acl::acl_cpp_init();
	acl::log::stdout_open(true);

	int conn_timeout = 10;
	int rw_timeout = 10;
	char addr_buffer[100];
	sprintf(addr_buffer, "%s:%d", REDIS_IP, REDIS_PORT);
	acl::string addr(addr_buffer);

	bool cluster_mode = false;

	// normal mode
	acl::redis_client client(addr.c_str(), conn_timeout, rw_timeout);

	// cluster mode
	acl::redis_client_cluster cluster(conn_timeout, rw_timeout);
	cluster.set(addr.c_str(), 100);

	// core logic, init redis_string, redis_key
	if (cluster_mode)
	{ 
		g_redis.set_cluster(&cluster, 100); 
	}
	else
	{
		g_redis.set_client(&client); 
	}

	// do auth
	std::string password(REDIS_PASSWORD);
	if (password != "")
	{
		if (g_redis.auth(REDIS_PASSWORD) == false)
		{
			printf("auth fail:error:%s\n", g_redis.result_error());
			return -1;
		}
	}

	///// test case logic

	int ret;
	int maxcase;
	int testcase;

	maxcase = sizeof(test_list) / sizeof(testcase_t);
	if (maxcase == 0)
	{
		return 0;
	}
	testcase = maxcase - 1;

	if (argc > 1)
	{
		if (!strcmp(argv[1], "all"))
		{
			printf("test:run all case\n");
			for (int i=0; i<maxcase; i++)
			{
				printf("\nRun test[%d]\n", i);
				ret = test_list[i]();
				if (ret != 0)
				{
					printf("test[%d] ERROR\n", i);
					return -1;
				}
			}
			return 0;	
		}

		testcase = atoi(argv[1]);
		if (testcase > maxcase - 1 || testcase < 0)
		{
			testcase = maxcase - 1;
		}
	}

	printf("\nRun test[%d]\n", testcase);
	ret = test_list[testcase]();
	if (ret != 0)
	{
		printf("test[%d] ERROR\n", testcase);
		return -1;
	}

	client.close();

	return 0;
}
