#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
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

static void print_zset(redisContext *c, const std::string key)
{
	int start = 0;
	int stop = -1;
	redisReply *reply = (redisReply *)redisCommand(c, "ZRANGE %s %d %d WITHSCORES", key.c_str(), start, stop);
	if (reply == NULL)
	{
		return;
	}
	printf("ZRANGE: type=%d key=%s elements=%lu\n", reply->type, key.c_str(), reply->elements);
	for (size_t i = 0; i < reply->elements; i++)
	{
		redisReply *r = reply->element[i];
		printf("[%zu] str=%s\n", i, r->str);
	}
	freeReplyObject(reply);
	printf("\n");
}

static void test(redisContext *c)
{
	// ZADD
	{
		const std::string key = "myzset";
		int score = 10;
		std::string member = "masha";
		redisReply *reply = (redisReply *)redisCommand(c, "ZADD %s %d %s", key.c_str(), score, member.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("ZADD: type=%d key=%s integer=%lld\n", reply->type, key.c_str(), reply->integer);
		freeReplyObject(reply);

		reply = (redisReply *)redisCommand(c, "ZADD %s 20 peter 15 kelton", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("ZADD: type=%d key=%s integer=%lld\n", reply->type, key.c_str(), reply->integer);
		freeReplyObject(reply);

		score = 30;
		member = "masha";
		reply = (redisReply *)redisCommand(c, "ZADD %s %d %s", key.c_str(), score, member.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("ZADD: type=%d key=%s integer=%lld\n", reply->type, key.c_str(), reply->integer);
		freeReplyObject(reply);
		printf("\n");
	}

	// ZSCORE
	{
		std::string key = "myzset";
		std::string member = "masha";
		redisReply *reply = (redisReply *)redisCommand(c, "ZSCORE %s %s", key.c_str(), member.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("ZSCORE: type=%d key=%s str=%s\n", reply->type, key.c_str(), reply->str);
		freeReplyObject(reply);

		member = "mashanx"; // not exists member
		reply = (redisReply *)redisCommand(c, "ZSCORE %s %s", key.c_str(), member.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("ZSCORE: type=%d key=%s str=%s\n", reply->type, key.c_str(), reply->str);
		freeReplyObject(reply);
		printf("\n");
	}

	// ZRANGE, ZREVRANGE
	{
		std::string key = "myzset";
		int start = 0;
		int stop = -1;
		redisReply *reply = (redisReply *)redisCommand(c, "ZRANGE %s %d %d WITHSCORES", key.c_str(), start, stop);
		if (reply == NULL)
		{
			return;
		}
		printf("ZRANGE: type=%d key=%s elements=%lu\n", reply->type, key.c_str(), reply->elements);
		for (size_t i = 0; i < reply->elements; i++)
		{
			redisReply *r = reply->element[i];
			printf("[%zu] str=%s\n", i, r->str);
		}
		freeReplyObject(reply);

		reply = (redisReply *)redisCommand(c, "ZREVRANGE %s %d %d WITHSCORES", key.c_str(), start, stop);
		if (reply == NULL)
		{
			return;
		}
		printf("ZREVRANGE: type=%d key=%s elements=%lu\n", reply->type, key.c_str(), reply->elements);
		for (size_t i = 0; i < reply->elements; i++)
		{
			redisReply *r = reply->element[i];
			printf("[%zu] str=%s\n", i, r->str);
		}
		freeReplyObject(reply);
		printf("\n");
	}

	// ZRANGEBYSCORE key min max [WITHSCORES] [LIMIT offset count]
	{
		std::string key = "myzset";
		int min = 20;
		int max = 30;
		redisReply *reply = (redisReply *)redisCommand(c, "ZRANGEBYSCORE %s %d %d WITHSCORES", key.c_str(), min, max);
		if (reply == NULL)
		{
			return;
		}
		printf("ZRANGEBYSCORE: type=%d key=%s min=%d max=%d elements=%lu\n", reply->type, key.c_str(), min, max, reply->elements);
		for (size_t i = 0; i < reply->elements; i++)
		{
			redisReply *r = reply->element[i];
			printf("[%zu] str=%s\n", i, r->str);
		}
		freeReplyObject(reply);

		reply = (redisReply *)redisCommand(c, "ZRANGEBYSCORE %s (%d %d WITHSCORES", key.c_str(), min, max);
		if (reply == NULL)
		{
			return;
		}
		printf("ZRANGEBYSCORE: type=%d key=%s min=(%d max=%d elements=%lu\n", reply->type, key.c_str(), min, max, reply->elements);
		for (size_t i = 0; i < reply->elements; i++)
		{
			redisReply *r = reply->element[i];
			printf("[%zu] str=%s\n", i, r->str);
		}
		freeReplyObject(reply);

		printf("\n");
	}

	// ZINCRBY key increment member
	{
		std::string key = "myzset";
		std::string member = "masha";
		int incr = 30;
		redisReply *reply = (redisReply *)redisCommand(c, "ZINCRBY %s %d %s", key.c_str(), incr, member.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("ZINCRBY: type=%d key=%s member=%s str=%s\n", reply->type, key.c_str(), member.c_str(), reply->str);
		freeReplyObject(reply);

		print_zset(c, key);

		printf("\n");
	}

	// ZCARD
	{
		std::string key = "myzset";
		redisReply *reply = (redisReply *)redisCommand(c, "ZCARD %s", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("ZCARD: type=%d key=%s integer=%lld\n", reply->type, key.c_str(), reply->integer);
		freeReplyObject(reply);
		printf("\n");
	}

	// ZCOUNT
	{
		std::string key = "myzset";
		int min = 10;
		int max = 30;
		redisReply *reply = (redisReply *)redisCommand(c, "ZCOUNT %s %d %d", key.c_str(), min, max);
		if (reply == NULL)
		{
			return;
		}
		printf("ZCOUNT: type=%d key=%s min=%d max=%d integer=%lld\n", reply->type, key.c_str(), min, max, reply->integer);
		freeReplyObject(reply);
		printf("\n");
	}

	// ZRANK, ZREVRANK
	{
		std::string key = "myzset";
		std::string member = "masha";
		redisReply *reply = (redisReply *)redisCommand(c, "ZRANK %s %s", key.c_str(), member.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("ZRANK: type=%d key=%s member=%s integer=%lld\n", reply->type, key.c_str(), member.c_str(), reply->integer);
		freeReplyObject(reply);

		reply = (redisReply *)redisCommand(c, "ZREVRANK %s %s", key.c_str(), member.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("ZREVRANK: type=%d key=%s member=%s integer=%lld\n", reply->type, key.c_str(), member.c_str(), reply->integer);
		freeReplyObject(reply);

		member = "mashanx"; // not exists member, type is nil
		reply = (redisReply *)redisCommand(c, "ZREVRANK %s %s", key.c_str(), member.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("ZREVRANK: type=%d key=%s member=%s\n", reply->type, key.c_str(), member.c_str());
		freeReplyObject(reply);
		printf("\n");
	}

	{
		std::string keys = "myzset";
		redisReply *reply = (redisReply *)redisCommand(c, "DEL %s", keys.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("DEL: type=%d keys=%s integer=%lld\n", reply->type, keys.c_str(), reply->integer);
		freeReplyObject(reply);
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
