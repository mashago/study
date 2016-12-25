
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

// global redis string operator
// acl::redis_string g_redis;
acl::redis g_redis;
// global redis key operator
// acl::redis_key g_key;
acl::redis g_key;
// global redis hash operator
// acl::redis_hash g_hash;
acl::redis g_hash;

// set and get
int test0()
{
	acl::string key;
	acl::string value; 

	key = "test0_key";
	value = "v0";
	printf("key=%s value=%s\n", key.c_str(), value.c_str());

	g_redis.clear();
	if (g_redis.set(key.c_str(), value.c_str()) == false)
	{
		printf("set key fail:key=[%s] error:%s\n", key.c_str(), g_redis.result_error());
		return -1;
	}
	printf("set key ok\n");


	// before do next operation, need clear redis
	g_redis.clear();
	value.clear();
	if (g_redis.get(key.c_str(), value) == false)
	{
		printf("get key fail:key=[%s]\n", key.c_str());
		return -1;
	}
	printf("get key ok:value=[%s] len=%d\n", value.c_str(), (int)value.length());
	
	return 0;
}

// set and get
int test1()
{
	std::string key;
	std::string value;
	acl::string result; // result must be acl::string

	key = "test1_key";
	value = "v1";
	printf("key=%s value=%s\n", key.c_str(), value.c_str());

	g_redis.clear();
	if (g_redis.set(key.c_str(), key.length(), value.c_str(), value.length()) == false)
	{
		printf("set key fail:key=[%s] error:%s\n", key.c_str(), g_redis.result_error());
		return -1;
	}
	printf("set key ok\n");


	g_redis.clear();
	result.clear();
	if (g_redis.get(key.c_str(), key.length(), result) == false)
	{
		printf("get key fail:key=[%s]\n", key.c_str());
		return -1;
	}
	printf("get key ok:result=[%s] len=%d\n", result.c_str(), (int)result.length());
	
	return 0;
}

// delete, by redis_key
int test2()
{
	std::string key;
	std::string value;
	acl::string result; // result must be acl::string

	// do set
	key = "test2_key";
	value = "v2";
	printf("key=%s value=%s\n", key.c_str(), value.c_str());

	g_redis.clear();
	if (g_redis.set(key.c_str(), key.length(), value.c_str(), value.length()) == false)
	{
		printf("set key fail:key=[%s] error:%s\n", key.c_str(), g_redis.result_error());
		return -1;
	}
	printf("set key ok\n");

	// check exist
	g_key.clear();
	if (g_key.exists(key.c_str()) == false)
	{
		printf("no exists key:%s\n", key.c_str());
		return -1;
	}
	printf("exists key:%s\n", key.c_str());

	// do delete
	g_key.clear();
	int ret = g_key.del_one(key.c_str());
	if (ret < 0)
	{
		printf("delete key fail:key=[%s] error:%s\n", key.c_str(), g_key.result_error());
		return -1;
	}
	printf("delete key ok\n");

	// check exist
	g_key.clear();
	if (g_key.exists(key.c_str()) == true)
	{
		printf("exists key:%s\n", key.c_str());
		return -1;
	}
	printf("not exists key:%s\n", key.c_str());

	// try get
	g_redis.clear();
	result.clear();
	if (g_redis.get(key.c_str(), key.length(), result) == false)
	{
		printf("get key fail:key=[%s]\n", key.c_str());
		return 0;
	}
	printf("get key ok:result=[%s] len=%d\n", result.c_str(), (int)result.length());
	
	return 0;
}

// hash
int test3()
{
	acl::string key;
	key = "test3_key";
	std::map<acl::string, acl::string> attrs;
	std::map<acl::string, acl::string> result;
	char tmp_buffer[50];
	bzero(tmp_buffer, 50);
	tmp_buffer[0] = 'a';
	tmp_buffer[1] = 'b';
	tmp_buffer[2] = 'c';
	tmp_buffer[3] = '\0';
	tmp_buffer[4] = 'a';
	tmp_buffer[5] = 'b';
	tmp_buffer[6] = 'c';
	tmp_buffer[7] = '\0';

	//////////////////// do get all [
	{
	printf("-------- hgetall start\n");
	g_hash.clear();
	result.clear();
	// core logic
    /**
     * 从 redis 哈希表中获取某个 key 对象的所有域字段的值
     * get all the fields and values in hash stored at key
     * @param key {const char*} key 键值
     *  the hash key
     * @param result {std::map<string, string>&} 存储域字段名-值查询结果集
     *  store the result of all the fileds and values
     * @return {bool} 操作是否成功，含义：
     *  if ok, show below:
     *  true -- 操作成功，当该域不存在时也返回成功，需要检查 result 内容是否变化，
     *          比如可以通过检查 result.size() 的变化来表明是否查询到结果
     *          successful if the key is a hash key or the key not exists
     *  false -- 操作失败或该 key 对象非哈希对象
     *           error happened or the key isn't a hash key
     */
	if ((g_hash.hgetall(key.c_str(), result)) == false)
	{
		printf("hgetall error:%s\n", g_hash.result_error());
		return -1;
	}
	for (auto iter=result.begin(); iter!=result.end(); iter++)
	{
		// printf("key=[%s] value=[%s]\n", iter->first.c_str(), iter->second.c_str());
		std::string k = std::string(iter->first.c_str());
		std::string v = std::string(iter->second.c_str());
		// printf("key=[%s] value=[%s]\n", k.c_str(), v.c_str());
		printf("key=[%s] value=[%s] value.size()=%zu\n", iter->first.c_str(), iter->second.c_str(), iter->second.size());
		
	}
	printf("-------- hgetall end\n");
	}
	//////////////////// do get all ]

	printf("\n");

	//////////////////// do set all [
	{
	printf("-------- hmset start\n");

	for (int i=0; i<3; i++)
	{
		acl::string attr;
		acl::string value;
		attr.format("attr%d", i);
		value.format("value%d", i);
		attrs[attr] = value;
	}
	acl::string attr = "attr4";
	acl::string value = acl::string(tmp_buffer, 7);
	attrs[attr] = value;
	g_hash.clear();

	// core logic
	if ((g_hash.hmset(key.c_str(), attrs)) == false)
	{
		printf("hmset error:%s\n", g_hash.result_error());
		return -1;
	}
	printf("-------- hmset end\n");
	}
	//////////////////// do set all ]

	printf("\n");

	//////////////////// do get all [
	{
	printf("-------- hgetall start\n");
	g_hash.clear();
	result.clear();
	// core logic
	if ((g_hash.hgetall(key.c_str(), result)) == false)
	{
		printf("hgetall error:%s\n", g_hash.result_error());
		return -1;
	}
	for (auto iter=result.begin(); iter!=result.end(); iter++)
	{
		// printf("key=[%s] value=[%s]\n", iter->first.c_str(), iter->second.c_str());
		std::string k = std::string(iter->first.c_str());
		std::string v = std::string(iter->second.c_str());
		// printf("key=[%s] value=[%s]\n", k.c_str(), v.c_str());
		printf("key=[%s] value=[%s] value.size()=%zu\n", iter->first.c_str(), iter->second.c_str(), iter->second.size());
		
	}
	printf("-------- hgetall end\n");
	}
	//////////////////// do get all ]

	printf("\n");

	//////////////////// do set one [
	{
	printf("-------- hset start\n");
	acl::string attr = "attr2";
	acl::string value = "xxxxxx";
	g_hash.clear();
	// core logic
	if ((g_hash.hset(key.c_str(), attr.c_str(), value.c_str())) < 0)
	{
		printf("hset error:%s\n", g_hash.result_error());
		return -1;
	}

	attr = "attr0";
	value = "000000";
	g_hash.clear();
	// core logic
	if ((g_hash.hset(key.c_str(), attr.c_str(), value.c_str())) < 0)
	{
		printf("hset error:%s\n", g_hash.result_error());
		return -1;
	}

	g_hash.clear();
	result.clear();
	if ((g_hash.hgetall(key.c_str(), result)) == false)
	{
		printf("hgetall error:%s\n", g_hash.result_error());
		return -1;
	}
	for (auto iter=result.begin(); iter!=result.end(); iter++)
	{
		printf("key=[%s] value=[%s]\n", iter->first.c_str(), iter->second.c_str());
	}
	printf("-------- hset end\n");
	}
	//////////////////// do set one ]

	printf("\n");

	//////////////////// do del one [
	{
	printf("-------- hdel start\n");
	acl::string attr = "attr2";
	const char *ptr = attr.c_str();
	g_hash.clear();
	// core logic
	// int hdel(const char* key, const char* names[], size_t argc);
	if ((g_hash.hdel(key.c_str(), &ptr, 1)) < 0)
	{
		printf("hdel error:%s\n", g_hash.result_error());
		return -1;
	}

	g_hash.clear();
	result.clear();
	if ((g_hash.hgetall(key.c_str(), result)) == false)
	{
		printf("hgetall error:%s\n", g_hash.result_error());
		return -1;
	}
	for (auto iter=result.begin(); iter!=result.end(); iter++)
	{
		printf("key=[%s] value=[%s]\n", iter->first.c_str(), iter->second.c_str());
	}
	printf("-------- hdel end\n");
	}
	//////////////////// do del one ]

	printf("\n");

	//////////////////// do set one with '\0' [
	{
	printf("-------- hset start\n");
	acl::string attr = "attr3";
	acl::string value = acl::string(tmp_buffer, 7);
	printf("value.size()=%zu\n", value.size());
	g_hash.clear();
	// core logic
	if ((g_hash.hset(key.c_str(), attr.c_str(), attr.size(), value.c_str(), value.size())) < 0)
	{
		printf("hset error:%s\n", g_hash.result_error());
		return -1;
	}

	g_hash.clear();
	result.clear();
	if ((g_hash.hgetall(key.c_str(), result)) == false)
	{
		printf("hgetall error:%s\n", g_hash.result_error());
		return -1;
	}
	for (auto iter=result.begin(); iter!=result.end(); iter++)
	{
		printf("key=[%s] value=[%s] value.size()=%zu\n", iter->first.c_str(), iter->second.c_str(), iter->second.size());
	}
	printf("-------- hset end\n");
	}
	//////////////////// do set one ]

	printf("\n");

	//////////////////// do delete key [
	// do del key
	g_key.clear();
	int ret = g_key.del_one(key.c_str());
	if (ret < 0)
	{
		printf("delete key fail:key=[%s] error:%s\n", key.c_str(), g_key.result_error());
		return -1;
	}
	printf("delete key[%s] ok\n", key.c_str());
	//////////////////// do delete key ]

	printf("\n");

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
	printf("hello redis\n");
	printf("REDIS_IP=[%s] REDIS_PORT=[%d] REDIS_PASSWORD=[%s]\n", REDIS_IP, REDIS_PORT, REDIS_PASSWORD);

	// must do this
	acl::acl_cpp_init();
	acl::log::stdout_open(true);

	int conn_timeout = 10;
	int rw_timeout = 10;
	acl::string addr("127.0.0.1:6379");

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
		g_key.set_cluster(&cluster, 100); 
		g_hash.set_cluster(&cluster, 100); 
	}
	else
	{
		g_redis.set_client(&client); 
		g_key.set_client(&client); 
		g_hash.set_client(&client); 
	}

	std::string password(REDIS_PASSWORD);
	if (password != "")
	{
		if (g_redis.auth(REDIS_PASSWORD) == false)
		{
			printf("auth fail:error:%s\n", g_redis.result_error());
			return -1;
		}
	}

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

	printf("Run test[%d]\n", testcase);
	ret = test_list[testcase]();
	if (ret != 0)
	{
		printf("test[%d] ERROR\n", testcase);
		return -1;
	}

	client.close();

	return 0;
}
