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

static void print_set(redisContext *c, const std::string key)
{
	redisReply *reply = (redisReply *)redisCommand(c, "SMEMBERS %s", key.c_str());
	if (reply == NULL)
	{
		return;
	}
	printf("SMEMBERS: type=%d key=%s elements=%lu\n", reply->type, key.c_str(), reply->elements);
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
	// SADD
	int counter = 1;
	do
	{
		const std::string key = "myset";
		std::string value = "N" + std::to_string((long long int)counter);
		redisReply *reply = (redisReply *)redisCommand(c, "SADD %s %s", key.c_str(), value.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("SADD: type=%d key=%s value=%s integer=%lld\n", reply->type, key.c_str(), value.c_str(), reply->integer);
		freeReplyObject(reply);
		counter++;
	}
	while (counter <= 5);
	printf("\n");

	// SREM
	{
		const std::string key = "myset";
		std::string value = "N2";
		redisReply *reply = (redisReply *)redisCommand(c, "SREM %s %s", key.c_str(), value.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("SREM: type=%d key=%s value=%s integer=%lld\n", reply->type, key.c_str(), value.c_str(), reply->integer);
		freeReplyObject(reply);
		printf("\n");
	}

	// SMEMBERS
	{
		const std::string key = "myset";
		redisReply *reply = (redisReply *)redisCommand(c, "SMEMBERS %s", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("SMEMBERS: type=%d key=%s elements=%lu\n", reply->type, key.c_str(), reply->elements);
		for (size_t i = 0; i < reply->elements; i++)
		{
			redisReply *r = reply->element[i];
			printf("[%zu] str=%s\n", i, r->str);
		}
		freeReplyObject(reply);
		printf("\n");
	}

	// SISMEMBER
	{
		const std::string key = "myset";
		std::string value = "N3";
		redisReply *reply = (redisReply *)redisCommand(c, "SISMEMBER %s %s", key.c_str(), value.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("SISMEMBER: type=%d key=%s value=%s integer=%lld\n", reply->type, key.c_str(), value.c_str(), reply->integer);
		freeReplyObject(reply);

		value = "N2";
		reply = (redisReply *)redisCommand(c, "SISMEMBER %s %s", key.c_str(), value.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("SISMEMBER: type=%d key=%s value=%s integer=%lld\n", reply->type, key.c_str(), value.c_str(), reply->integer);
		freeReplyObject(reply);
		printf("\n");
	}

	counter = 3;
	do
	{
		const std::string key = "myset2";
		std::string value = "N" + std::to_string((long long int)counter);
		redisReply *reply = (redisReply *)redisCommand(c, "SADD %s %s", key.c_str(), value.c_str());
		if (reply == NULL)
		{
			return;
		}
		// printf("SADD: type=%d key=%s value=%s integer=%lld\n", reply->type, key.c_str(), value.c_str(), reply->integer);
		freeReplyObject(reply);
		counter++;
	}
	while (counter <= 6);
	printf("\n");
	print_set(c, "myset2");

	// SDIFF
	{
		const std::string key = "myset";
		const std::string key2 = "myset2";
		redisReply *reply = (redisReply *)redisCommand(c, "SDIFF %s %s", key.c_str(), key2.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("SDIFF: type=%d key=%s key2=%s elements=%lu\n", reply->type, key.c_str(), key2.c_str(), reply->elements);
		for (size_t i = 0; i < reply->elements; i++)
		{
			redisReply *r = reply->element[i];
			printf("[%zu] str=%s\n", i, r->str);
		}
		freeReplyObject(reply);
		printf("\n");
	}
	{
		const std::string key = "myset2";
		const std::string key2 = "myset";
		redisReply *reply = (redisReply *)redisCommand(c, "SDIFF %s %s", key.c_str(), key2.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("SDIFF: type=%d key=%s key2=%s elements=%lu\n", reply->type, key.c_str(), key2.c_str(), reply->elements);
		for (size_t i = 0; i < reply->elements; i++)
		{
			redisReply *r = reply->element[i];
			printf("[%zu] str=%s\n", i, r->str);
		}
		freeReplyObject(reply);
		printf("\n");
	}

	// SINTER
	{
		const std::string key = "myset";
		const std::string key2 = "myset2";
		redisReply *reply = (redisReply *)redisCommand(c, "SINTER %s %s", key.c_str(), key2.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("SINTER: type=%d key=%s key2=%s elements=%lu\n", reply->type, key.c_str(), key2.c_str(), reply->elements);
		for (size_t i = 0; i < reply->elements; i++)
		{
			redisReply *r = reply->element[i];
			printf("[%zu] str=%s\n", i, r->str);
		}
		freeReplyObject(reply);
		printf("\n");
	}

	// SUNION
	{
		const std::string key = "myset";
		const std::string key2 = "myset2";
		redisReply *reply = (redisReply *)redisCommand(c, "SUNION %s %s", key.c_str(), key2.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("SUNION: type=%d key=%s key2=%s elements=%lu\n", reply->type, key.c_str(), key2.c_str(), reply->elements);
		for (size_t i = 0; i < reply->elements; i++)
		{
			redisReply *r = reply->element[i];
			printf("[%zu] str=%s\n", i, r->str);
		}
		freeReplyObject(reply);
		printf("\n");
	}

	// SCARD
	{
		const std::string key = "myset";
		redisReply *reply = (redisReply *)redisCommand(c, "SCARD %s", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("SCARD: type=%d key=%s integer=%lld\n", reply->type, key.c_str(), reply->integer);
		freeReplyObject(reply);
		printf("\n");
	}

	// SDIFFSTORE, SINTERSTORE and SUNIONSTORE are the same
	{
		const std::string dest = "myset3";
		const std::string key1= "myset";
		const std::string key2 = "myset2";
		redisReply *reply = (redisReply *)redisCommand(c, "SDIFFSTORE %s %s %s", dest.c_str(), key1.c_str(), key2.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("SDIFFSTORE: type=%d dest=%s key1=%s key2=%s integer=%lld\n", reply->type, dest.c_str(), key1.c_str(), key2.c_str(), reply->integer);
		freeReplyObject(reply);
		printf("\n");
		print_set(c, dest);
	}

	// SRANDMEMBER
	{
		const std::string key = "myset";
		redisReply *reply = (redisReply *)redisCommand(c, "SRANDMEMBER %s", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("SRANDMEMBER: type=%d key=%s str=%s\n", reply->type, key.c_str(), reply->str);
		freeReplyObject(reply);

		int count = 2;
		reply = (redisReply *)redisCommand(c, "SRANDMEMBER %s %d", key.c_str(), count);
		if (reply == NULL)
		{
			return;
		}
		printf("SRANDMEMBER: type=%d key=%s elements=%lu\n", reply->type, key.c_str(), reply->elements);
		for (size_t i = 0; i < reply->elements; i++)
		{
			redisReply *r = reply->element[i];
			printf("[%zu] str=%s\n", i, r->str);
		}
		freeReplyObject(reply);

		count = -2; // rand elements may the same
		reply = (redisReply *)redisCommand(c, "SRANDMEMBER %s %d", key.c_str(), count);
		if (reply == NULL)
		{
			return;
		}
		printf("SRANDMEMBER: type=%d key=%s elements=%lu\n", reply->type, key.c_str(), reply->elements);
		for (size_t i = 0; i < reply->elements; i++)
		{
			redisReply *r = reply->element[i];
			printf("[%zu] str=%s\n", i, r->str);
		}
		freeReplyObject(reply);

		printf("\n");
	}

	// SPOP
	{
		const std::string key = "myset";
		print_set(c, key);
		redisReply *reply = (redisReply *)redisCommand(c, "SPOP %s", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("SPOP: type=%d key=%s str=%s\n", reply->type, key.c_str(), reply->str);
		freeReplyObject(reply);
		print_set(c, key);
		printf("\n");
	}

	{
		std::string keys = "myset myset2 myset3";
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
