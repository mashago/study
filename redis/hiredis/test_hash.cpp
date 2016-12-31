#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stddef.h>
#include <string.h>
#include <hiredis/hiredis.h>

#include <string>

static const char *REDIS_HOST = "127.0.0.1";
static const int REDIS_PORT = 6379;

static void test_ping(redisContext *c)
{
	// 1. run a command
	redisReply *reply = (redisReply *)redisCommand(c, "PING");
	printf("PING: %s\n", reply->str);

	// 2. free reply
	freeReplyObject(reply);
}

static void test(redisContext *c)
{
	// 1. set a hash key
	{
		const std::string key = "car:1";
		std::string field = "name";
		std::string value = "BMW";
		redisReply *reply = (redisReply *)redisCommand(c, "HSET %s %s %s", key.c_str(), field.c_str(), value.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("HSET: type=%d key=%s field=%s integer=%lld\n", reply->type, key.c_str(), field.c_str(), reply->integer);
		freeReplyObject(reply);

		field = "price";
		int price = 500;
		reply = (redisReply *)redisCommand(c, "HSET %s %s %d", key.c_str(), field.c_str(), price);
		if (reply == NULL)
		{
			return;
		}
		printf("HSET: type=%d key=%s field=%s integer=%lld\n", reply->type, key.c_str(), field.c_str(), reply->integer);
		freeReplyObject(reply);
		printf("\n");
	}

	// 2. hget
	{
		const std::string key = "car:1";
		std::string field = "name";
		redisReply *reply = (redisReply *)redisCommand(c, "HGET %s %s", key.c_str(), field.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("HGET: type=%d key=%s str=%s\n", reply->type, key.c_str(), reply->str);
		freeReplyObject(reply);

		field = "price";
		reply = (redisReply *)redisCommand(c, "HGET %s %s", key.c_str(), field.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("HGET: type=%d key=%s str=%s\n", reply->type, key.c_str(), reply->str);
		freeReplyObject(reply);
		printf("\n");
	}

	// 3. hmset
	{
		const std::string key = "car:2";
		std::string field1 = "name";
		std::string value1 = "Benz";
		std::string field2 = "price";
		std::string value2 = "1000";
		redisReply *reply = (redisReply *)redisCommand(c, "HMSET %s %s %s %s %s", key.c_str(), field1.c_str(), value1.c_str(), field2.c_str(), value2.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("HMSET: type=%d key=%s str=%s\n", reply->type, key.c_str(), reply->str);
		freeReplyObject(reply);
		printf("\n");
	}

	// 4. hmget
	{
		const std::string key = "car:1";
		std::string field1 = "name";
		std::string field2 = "price";
		redisReply *reply = (redisReply *)redisCommand(c, "HMGET %s %s %s", key.c_str(), field1.c_str(), field2.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("HMGET: type=%d key=%s elements=%lu\n", reply->type, key.c_str(), reply->elements);
		for (size_t i = 0; i < reply->elements; i++)
		{
			redisReply *r = reply->element[i];
			printf("[%zu] str=%s\n", i, r->str);
		}
		freeReplyObject(reply);
		printf("\n");
	}

	// 5. hgetall
	{
		const std::string key = "car:2";
		redisReply *reply = (redisReply *)redisCommand(c, "HGETALL %s", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("HGETALL: type=%d key=%s elements=%lu\n", reply->type, key.c_str(), reply->elements);
		for (size_t i = 0; i < reply->elements; i++)
		{
			redisReply *r = reply->element[i];
			printf("[%zu] str=%s\n", i, r->str);
		}
		freeReplyObject(reply);
		printf("\n");
	}
}

int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);

	const struct timeval timeout = {1, 500000};

	// create redis connection
	redisContext *c = redisConnectWithTimeout(REDIS_HOST, REDIS_PORT, timeout);
	if (c == NULL || c->err)
	{
		if (c)
		{
			printf("Connection error: %s\n", c->errstr);
			redisFree(c);
		}
		else
		{
			printf("Connection error: can't allocate redis context\n");
		}
		exit(1);
	}
	printf("Connection success\n");

	test_ping(c);
	test(c);

	// MUST free redis connection in the end
	redisFree(c);

	return 0;
}
