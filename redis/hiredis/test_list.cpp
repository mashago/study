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

static void print_list(redisContext *c, const std::string key)
{
	int start = 0, stop = -1;
	redisReply *reply = (redisReply *)redisCommand(c, "LRANGE %s %d %d", key.c_str(), start, stop);
	if (reply == NULL)
	{
		return;
	}
	printf("LRANGE: type=%d key=%s start=%d stop=%d elements=%lu\n", reply->type, key.c_str(), start, stop, reply->elements);
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
	// 1. push into list
	int counter = 1;
	do
	{
		const std::string key = "mylist";
		std::string value = "L" + std::to_string(counter);
		redisReply *reply = (redisReply *)redisCommand(c, "LPUSH %s %s", key.c_str(), value.c_str());
		if (reply == NULL)
		{
			return;
		}
		// return length of list
		printf("LPUSH: type=%d key=%s integer=%lld\n", reply->type, key.c_str(), reply->integer);
		freeReplyObject(reply);

		value = "R" + std::to_string(counter);
		reply = (redisReply *)redisCommand(c, "RPUSH %s %s", key.c_str(), value.c_str());
		if (reply == NULL)
		{
			return;
		}
		// return length of list
		printf("RPUSH: type=%d key=%s integer=%lld\n", reply->type, key.c_str(), reply->integer);
		freeReplyObject(reply);
		counter++;
		printf("\n");
	}
	while (counter <= 5);

	// 2. get length
	{
		const std::string key = "mylist";
		redisReply *reply = (redisReply *)redisCommand(c, "LLEN %s", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("LLEN: type=%d key=%s integer=%lld\n", reply->type, key.c_str(), reply->integer);
		freeReplyObject(reply);
		printf("\n");
	}

	// 3. pop left or right
	{
		const std::string key = "mylist";
		redisReply *reply = (redisReply *)redisCommand(c, "LPOP %s", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("LPOP: type=%d key=%s str=%s integer=%lld\n", reply->type, key.c_str(), reply->str, reply->integer);
		freeReplyObject(reply);

		reply = (redisReply *)redisCommand(c, "RPOP %s", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("RPOP: type=%d key=%s str=%s integer=%lld\n", reply->type, key.c_str(), reply->str, reply->integer);
		freeReplyObject(reply);

		printf("\n");
	}

	// 4. get range
	{
		const std::string key = "mylist";
		int start = 0, stop = -1;
		redisReply *reply = (redisReply *)redisCommand(c, "LRANGE %s %d %d", key.c_str(), start, stop);
		if (reply == NULL)
		{
			return;
		}
		printf("LRANGE: type=%d key=%s start=%d stop=%d elements=%lu\n", reply->type, key.c_str(), start, stop, reply->elements);
		for (size_t i = 0; i < reply->elements; i++)
		{
			redisReply *r = reply->element[i];
			printf("[%zu] str=%s\n", i, r->str);
		}
		freeReplyObject(reply);

		start = 0; stop = 1;
		reply = (redisReply *)redisCommand(c, "LRANGE %s %d %d", key.c_str(), start, stop);
		if (reply == NULL)
		{
			return;
		}
		printf("LRANGE: type=%d key=%s start=%d stop=%d elements=%lu\n", reply->type, key.c_str(), start, stop, reply->elements);
		for (size_t i = 0; i < reply->elements; i++)
		{
			redisReply *r = reply->element[i];
			printf("[%zu] str=%s\n", i, r->str);
		}
		freeReplyObject(reply);

		printf("\n");
	}
	
	// rm
	{
		const std::string key = "mylist";
		int count = 1; std::string value = "L1"; // remove left first match
		redisReply *reply = (redisReply *)redisCommand(c, "LREM %s %d %s", key.c_str(), count, value.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("LREM: type=%d key=%s count=%d value=%s integer=%lld\n", reply->type, key.c_str(), count, value.c_str(), reply->integer);
		freeReplyObject(reply);

		count = 0; value = "R1"; // remove all
		reply = (redisReply *)redisCommand(c, "LREM %s %d %s", key.c_str(), count, value.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("LREM: type=%d key=%s count=%d value=%s integer=%lld\n", reply->type, key.c_str(), count, value.c_str(), reply->integer);
		freeReplyObject(reply);
		printf("\n");
	}

	// index
	{
		const std::string key = "mylist";
		int index = 0;
		redisReply *reply = (redisReply *)redisCommand(c, "LINDEX %s %d", key.c_str(), index);
		if (reply == NULL)
		{
			return;
		}
		printf("LINDEX: type=%d key=%s index=%d str=%s\n", reply->type, key.c_str(), index, reply->str);
		freeReplyObject(reply);
		printf("\n");
	}

	// set
	{
		const std::string key = "mylist";
		int index = 0; std::string value = "N1";
		redisReply *reply = (redisReply *)redisCommand(c, "LSET %s %d %s", key.c_str(), index, value.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("LSET: type=%d key=%s index=%d str=%s\n", reply->type, key.c_str(), index, reply->str);
		freeReplyObject(reply);
		printf("\n");
		print_list(c, key);
	}

	// trim
	{
		const std::string key = "mylist";
		int start = 1, end = 3;
		redisReply *reply = (redisReply *)redisCommand(c, "LTRIM %s %d %d", key.c_str(), start, end);
		if (reply == NULL)
		{
			return;
		}
		printf("LTRIM: type=%d key=%s start=%d end=%d str=%s\n", reply->type, key.c_str(), start, end, reply->str);
		freeReplyObject(reply);
		print_list(c, key);
		printf("\n");
	}

	// insert
	{
		const std::string key = "mylist";
		std::string flag = "BEFORE";
		std::string pivot = "L2";
		std::string value = "N2";
		redisReply *reply = (redisReply *)redisCommand(c, "LINSERT %s %s %s %s", key.c_str(), flag.c_str(), pivot.c_str(), value.c_str());
		if (reply == NULL)
		{
			return;
		}
		// return length of list
		printf("LINSERT: type=%d key=%s integer=%lld\n", reply->type, key.c_str(), reply->integer);
		freeReplyObject(reply);
		print_list(c, key);
		printf("\n");
	}

	// RPOPLPUSH
	{
		const std::string key = "mylist";
		redisReply *reply = (redisReply *)redisCommand(c, "RPOPLPUSH %s %s", key.c_str(), key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("RPOPLPUSH: type=%d key=%s str=%s\n", reply->type, key.c_str(), reply->str);
		freeReplyObject(reply);
		print_list(c, key);
		printf("\n");
	}

	{
		std::string key = "mylist";
		redisReply *reply = (redisReply *)redisCommand(c, "DEL %s", key.c_str());
		if (reply == NULL)
		{
			return;
		}
		printf("DEL: type=%d key=%s integer=%lld\n", reply->type, key.c_str(), reply->integer);
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
