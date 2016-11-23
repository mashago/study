
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
#define MY_HASH_NAME "my_hash"

// hset : set one pair in hash
int test0()
{
	int ret = 0;
	acl::string hash_key;
	acl::string key;
	acl::string value; 

	hash_key = MY_HASH_NAME;
	key = "a1";
	value = "b1";

	g_redis.clear();
	ret = g_redis.hset(hash_key.c_str(), key.c_str(), key.size(), value.c_str(), value.size());
	printf("test0:hset ret=%d\n", ret);
	if (ret == -1)
	{
		printf("test0:hset error hash_key[%s] %s\n", hash_key.c_str(), g_redis.result_error());
		return -1;
	}

	return 0;
}

// hget : get one pair in hash
int test1()
{
	bool ret = true;
	acl::string hash_key;
	acl::string key;
	acl::string value; 

	hash_key = MY_HASH_NAME;
	key = "a1";

	g_redis.clear();
	ret = g_redis.hget(hash_key.c_str(), key.c_str(), key.size(), value);
	if (ret == false)
	{
		printf("test1:hget error hash_key[%s] %s\n", hash_key.c_str(), g_redis.result_error());
		return -1;
	}
	printf("test1:hash_key=[%s] key=[%s] value=[%s]\n", hash_key.c_str(), key.c_str(), value.c_str());

	return 0;
}

// hmset : set pairs in hash
int test2()
{
	bool ret = true;
	acl::string hash_key;
	acl::string key;
	acl::string value;
	std::map<acl::string, acl::string> attrs; 

	hash_key = MY_HASH_NAME;
	key = "a2";
	value = "b2";
	attrs.insert(std::make_pair(key, value));

	key = "a3";
	value = "b3";
	attrs.insert(std::make_pair(key, value));

	key = "a4";
	value = "b4";
	attrs.insert(std::make_pair(key, value));

	g_redis.clear();
	ret = g_redis.hmset(hash_key.c_str(), attrs);
	if (ret == false)
	{
		printf("test2:hmset error hash_key[%s] %s\n", hash_key.c_str(), g_redis.result_error());
		return -1;
	}
	printf("test2:hmset success\n");

	return 0;
}

// hmget : get pairs in hash
int test3()
{
	bool ret = true;
	acl::string hash_key;
	acl::string key;
	std::vector<acl::string> names;
	std::vector<acl::string> result;

	hash_key = MY_HASH_NAME;
	key = "a2";
	names.push_back(key);

	key = "a3";
	names.push_back(key);

	key = "a5"; // no such key
	names.push_back(key);

	g_redis.clear();
	ret = g_redis.hmget(hash_key.c_str(), names, &result);
	if (ret == false)
	{
		printf("test3:hmget error hash_key[%s] %s\n", hash_key.c_str(), g_redis.result_error());
		return -1;
	}

	// if no such key in hash, result[i] == ""
	printf("test3:result.size()=%lu\n", result.size());
	for (size_t i = 0; i < result.size(); i++)
	{
		printf("[%lu] key=[%s] value=[%s]\n", i, names[i].c_str(), result[i].c_str());
	}

	return 0;
}

// hgetall : get all pairs in hash
int test4()
{
	bool ret = true;
	acl::string hash_key;
	std::map<acl::string, acl::string> result;

	hash_key = MY_HASH_NAME;

	g_redis.clear();
	ret = g_redis.hgetall(hash_key.c_str(), result);
	if (ret == false)
	{
		printf("test4:hgetall error hash_key[%s] %s\n", hash_key.c_str(), g_redis.result_error());
		return -1;
	}

	printf("test4:result.size()=%lu\n", result.size());
	for (auto iter = result.begin(); iter != result.end(); iter ++)
	{
		printf("key=[%s] value=[%s]\n", iter->first.c_str(), iter->first.c_str());
	}

	return 0;
}

// hkeys : get all keys in hash
int test5()
{
	bool ret = true;
	acl::string hash_key;
	std::vector<acl::string> keys;

	hash_key = MY_HASH_NAME;

	g_redis.clear();
	ret = g_redis.hkeys(hash_key.c_str(), keys);
	if (ret == false)
	{
		printf("test5:hkeys error hash_key[%s] %s\n", hash_key.c_str(), g_redis.result_error());
		return -1;
	}

	printf("test5:keys.size()=%lu\n", keys.size());
	for (auto iter = keys.begin(); iter != keys.end(); iter ++)
	{
		printf("key=[%s]\n", (*iter).c_str());
	}

	return 0;
}

// hdel : delete keys in hash
int test6()
{
	int ret = 0;
	acl::string hash_key;
	std::vector<acl::string> keys;

	hash_key = MY_HASH_NAME;
	keys.push_back(acl::string("a1"));
	keys.push_back(acl::string("a2"));

	g_redis.clear();
	ret = g_redis.hdel(hash_key.c_str(), keys);
	printf("test6:hdel ret = %d\n", ret);
	if (ret == -1)
	{
		printf("test6:hdel error hash_key[%s] %s\n", hash_key.c_str(), g_redis.result_error());
		return -1;
	}


	std::map<acl::string, acl::string> result;

	g_redis.clear();
	ret = g_redis.hgetall(hash_key.c_str(), result);
	if (ret == false)
	{
		printf("test6:hgetall error hash_key[%s] %s\n", hash_key.c_str(), g_redis.result_error());
		return -1;
	}

	printf("test6:result.size()=%lu\n", result.size());
	for (auto iter = result.begin(); iter != result.end(); iter ++)
	{
		printf("key=[%s] value=[%s]\n", iter->first.c_str(), iter->first.c_str());
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
,	test4
,	test5
,	test6
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
