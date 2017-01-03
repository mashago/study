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
	// SET & GET
	{
		std::string key = "foo";
		std::string value = "bar";
		std::string script = "return redis.call('SET', KEYS[1], ARGV[1])";
		redisReply *reply = (redisReply *)redisCommand(c, "EVAL %s 1 %s %s"
				, script.c_str(), key.c_str(), value.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("SET: type=%d str=%s\n", reply->type, reply->str);
		freeReplyObject(reply);

		script = "return redis.call('GET', KEYS[1])";
		reply = (redisReply *)redisCommand(c, "EVAL %s 1 %s"
				, script.c_str(), key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("GET: type=%d str=%s\n", reply->type, reply->str);
		freeReplyObject(reply);

		printf("\n");
	}

	// HMSET & HGETALL
	{
		std::string key = "luahash";
		std::string script = "\
			redis.call('HMSET', KEYS[1], 'masha', 100, 'peter', 80, 'jobs', 60) \
			local values = redis.call('HGETALL', KEYS[1]) \
			return values \
			";
		redisReply *reply = (redisReply *)redisCommand(c, "EVAL %s 1 %s"
				, script.c_str(), key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("Script: type=%d key=%s elements=%lu\n", reply->type, key.c_str(), reply->elements);
		for (size_t i = 0; i < reply->elements; i++)
		{
			redisReply *r = reply->element[i];
			printf("[%zu] str=%s\n", i, r->str);
		}
		freeReplyObject(reply);

		printf("\n");
	}

	// ZSET
	int counter = 1;
	do
	{
		std::string key = "luazset";
		std::string member = "masha" + std::to_string(counter);
		std::string script = "\
			local rank = redis.call('ZSCORE', KEYS[1], ARGV[1]) \
			if rank == false then \
				rank = redis.call('ZCARD', KEYS[1]) + 1 \
				redis.call('ZADD', KEYS[1], rank, ARGV[1]) \
			end \
			return rank \
			";
		redisReply *reply = (redisReply *)redisCommand(c, "EVAL %s 1 %s %s"
				, script.c_str(), key.c_str(), member.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("Script: type=%d key=%s integer=%lld str=%s\n", reply->type, key.c_str(), reply->integer, reply->str);
		freeReplyObject(reply);
		counter++;
	}
	while (counter <= 5);
	printf("\n");
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
