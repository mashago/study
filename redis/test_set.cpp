
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
#define MY_SET_NAME "my_set"

// sadd : set add
int test0()
{
	int ret = 0;
	acl::string key;
	acl::string value; 
	std::vector<acl::string> values; 

	key = MY_SET_NAME;
	value = "a1";
	values.push_back(value);
	value = "a2";
	values.push_back(value);
	value = "a3";
	values.push_back(value);

	g_redis.clear();

	// return insert count, 0 for no change, -1 for error
	ret = g_redis.sadd(key.c_str(), values);
	printf("test0:sadd ret=%d\n", ret);
	if (ret == -1)
	{
		printf("test0:sadd fail, key[%s] %s", key.c_str(), g_redis.result_error());
		return -1;
	}

	return 0;
}

// smembers : set members
int test1()
{
	int ret = 0;
	acl::string key;
	std::vector<acl::string> values; 

	key = MY_SET_NAME;

	g_redis.clear();
	ret = g_redis.smembers(key.c_str(), &values);
	printf("test1:smembers ret=%d\n", ret);
	if (ret == -1)
	{
		printf("test1:key[%s] is not a set or error", key.c_str());
		return -1;
	}

	for (int i = 0; i < ret; i++)
	{
		printf("[%d]=%s\n", i, values[i].c_str());
	}
	
	return 0;
}

// srem : set remove member
int test2()
{
	int ret = 0;
	acl::string key;
	acl::string value; 
	std::vector<acl::string> values; 

	key = MY_SET_NAME;
	value = "a1";
	values.push_back(value);
	value = "a2";
	values.push_back(value);

	g_redis.clear();
	ret = g_redis.srem(key.c_str(), values);
	printf("test2:srem ret=%d\n", ret);
	if (ret == -1)
	{
		printf("test2:key[%s] is not a set", key.c_str());
		return -1;
	}

	// get members
	g_redis.clear();
	ret = g_redis.smembers(key.c_str(), &values);
	printf("test2:smembers ret=%d\n", ret);
	if (ret == -1)
	{
		printf("test2:key[%s] is not a set or error", key.c_str());
		return -1;
	}

	for (int i = 0; i < ret; i++)
	{
		printf("[%d]=%s\n", i, values[i].c_str());
	}

	return 0;
}


// sismember : check if member is in set
int test3()
{
	bool ret = true;
	acl::string key;
	acl::string value; 

	key = MY_SET_NAME;
	value = "a1";

	g_redis.clear();
	ret = g_redis.sismember(key.c_str(), value.c_str());
	if (ret == true)
	{
		printf("test3:value[%s] is in set\n", value.c_str());
	}
	else
	{
		printf("test3:value[%s] is not in set\n", value.c_str());
	}



	value = "a3";

	g_redis.clear();
	ret = g_redis.sismember(key.c_str(), value.c_str());
	if (ret == true)
	{
		printf("test3:value[%s] is in set\n", value.c_str());
	}
	else
	{
		printf("test3:value[%s] is not in set\n", value.c_str());
	}

	return 0;
}

typedef int (*testcase_t) ();
testcase_t test_list[] =
{
	test0
,	test1
,	test2
,	test3
};


int main(int argc, char **argv)
{
	printf("hello test_set\n");
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
