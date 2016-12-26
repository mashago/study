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
	// 1. set a key
	{
		const std::string key = "foo";
		redisReply *reply = (redisReply *)redisCommand(c, "SET %s %s", key.c_str(), "hello redis");
		if (reply == NULL)
		{
			return;
		}
		printf("SET: type=%d key=%s str=%s\n", reply->type, key.c_str(), reply->str);
		freeReplyObject(reply);
	}
	{
		const std::string key = "foo";
		redisReply *reply = (redisReply *)redisCommand(c, "SET %s", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("SET: type=%d key=%s str=%s\n", reply->type, key.c_str(), reply->str);
		freeReplyObject(reply);
		printf("\n");
	}

	// 2. set a key with binary value
	{
		const std::string key = "foo2";
		char buffer[10];
		memset(buffer, 0, sizeof(buffer));
		buffer[0] = 'a';
		buffer[1] = 'b';
		buffer[2] = '\0';
		buffer[3] = 'c';
		buffer[4] = 'd';

		redisReply *reply = (redisReply *)redisCommand(c, "SET %s %b", key.c_str(), buffer, sizeof(buffer));
		if (reply == NULL)
		{
			return;
		}
		printf("SET: key=%s str=%s\n", key.c_str(), reply->str);
		freeReplyObject(reply);
		printf("\n");
	}

	// 3. get
	{
		std::string key = "foo";
		redisReply *reply = (redisReply *)redisCommand(c, "GET %s", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("GET: type=%d key=%s str=%s len=%d\n", reply->type, key.c_str(), reply->str, reply->len);
		freeReplyObject(reply);

		key = "foo2";
		reply = (redisReply *)redisCommand(c, "GET %s", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("GET: key=%s str=%s len=%d\n", key.c_str(), reply->str, reply->len);
		freeReplyObject(reply);
		printf("\n");
	}

	// 4. increase and return
	{
		const std::string key = "counter";
		redisReply *reply = (redisReply *)redisCommand(c, "INCR %s", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("INCR: type=%d key=%s value=%lld\n", reply->type, key.c_str(), reply->integer);
		freeReplyObject(reply);

		reply = (redisReply *)redisCommand(c, "INCR %s", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("INCR: type=%d key=%s value=%lld\n", reply->type, key.c_str(), reply->integer);
		freeReplyObject(reply);
		printf("\n");
	}

	// 5. append, strlen
	{
		std::string key = "foo";
		redisReply *reply = (redisReply *)redisCommand(c, "GET %s", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("GET: type=%d key=%s str=%s len=%d\n", reply->type, key.c_str(), reply->str, reply->len);
		freeReplyObject(reply);

		key = "foo";
		reply = (redisReply *)redisCommand(c, "APPEND %s 123", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("APPEND: key=%s integer=%lld\n", key.c_str(), reply->integer);
		freeReplyObject(reply);

		key = "foo2";
		reply = (redisReply *)redisCommand(c, "STRLEN %s", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("STRLEN: key=%s integer=%lld\n", key.c_str(), reply->integer);
		freeReplyObject(reply);

		printf("\n");
	}

	// 6. mset, mget
	{
		const std::string key1 = "foo";
		const std::string value1 = "v1";
		const std::string key2 = "foo2";
		const std::string value2 = "v2";
		redisReply *reply = (redisReply *)redisCommand(c, "MSET %s %s %s %s", key1.c_str(), value1.c_str(), key2.c_str(), value2.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("MSET: type=%d str=%s\n", reply->type, reply->str);
		freeReplyObject(reply);

		reply = (redisReply *)redisCommand(c, "MGET %s %s", key1.c_str(), key2.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("MGET: type=%d str=%s\n", reply->type, reply->str);
		for (int i = 0; i < reply->elements; i++)
		{
			redisReply *r = reply->element[i];
			printf("[%d] str=%s\n", i, r->str);
		}
		freeReplyObject(reply);

		printf("\n");
	}

	// 7. delete
	{
		std::string key = "foo";
		redisReply *reply = (redisReply *)redisCommand(c, "DEL %s", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("DEL: type=%d key=%s integer=%lld str=%s\n", reply->type, key.c_str(), reply->integer, reply->str);
		freeReplyObject(reply);

		key = "foo_nx";
		reply = (redisReply *)redisCommand(c, "DEL %s", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("DEL: type=%d key=%s integer=%lld str=%s\n", reply->type, key.c_str(), reply->integer, reply->str);
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
