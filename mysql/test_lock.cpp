extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#include <sys/time.h>
#endif
}

#include <thread>
#include <vector>
#include "mysql_operator.h"

#define MYSQL_HOST "127.0.0.1"
#define MYSQL_PORT 3306
#define MYSQL_USER "masha"
#define MYSQL_PASSWORD "masha"
#define MYSQL_DB "test"
#define MYSQL_CHARSET "utf8"

double get_time_ms()
{
#ifdef WIN32
	return 0;
#else
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
#endif
}

void do_init()
{
	int ret = 0;
	mysql_operator_t op;
	op.init(MYSQL_HOST, MYSQL_PORT, MYSQL_USER, MYSQL_PASSWORD, MYSQL_DB, MYSQL_CHARSET);
	ret = op.connect();
	if (ret != 0)
	{
		printf("do_init:connect fail ret=%d get_err=%d\n", ret, op.get_err());
		return;
	}

	char sql[100];
	sprintf(sql, "UPDATE test_lock SET num=0");
	printf("sql=%s\n", sql);

	ret = op.query(sql, strlen(sql));
	if (ret != 0)
	{	
		printf("do_init:query error, get_err=%d\n", op.get_err());
		return;
	}
	op.clean_result();
	printf("do_init done\n");
}

void do_update(int times, int id, int offset)
{
	int ret = 0;
	mysql_operator_t op;
	op.init(MYSQL_HOST, MYSQL_PORT, MYSQL_USER, MYSQL_PASSWORD, MYSQL_DB, MYSQL_CHARSET);
	ret = op.connect();
	if (ret != 0)
	{
		printf("do_update:connect fail ret=%d get_err=%d\n", ret, op.get_err());
		return;
	}

	char sql[100];
	sprintf(sql, "CALL update_lock_num(%d, %d)", id, offset);
	// printf("sql=%s\n", sql);

	for (int i = 0; i < times; ++i)
	{
		// printf("i=%d\n", i);
		ret = op.query(sql, strlen(sql));
		if (ret != 0)
		{	
			printf("do_update:query error, get_err=%d\n", op.get_err());
			return;
		}
		/*
		int field_count = op.field_count();
		int num_rows = op.num_rows();
		printf("field_count=%d num_rows=%d\n", field_count, num_rows);
		while (const MYSQL_ROW &row = op.fetch_row())
		{
			printf("row[0]=%s\n", row[0]);
		}
		*/
		op.clean_result();
	}
	// printf("do_update done\n");
}

void do_check_same(int id, int expect_result)
{
	int ret = 0;
	mysql_operator_t op;
	op.init(MYSQL_HOST, MYSQL_PORT, MYSQL_USER, MYSQL_PASSWORD, MYSQL_DB, MYSQL_CHARSET);
	ret = op.connect();
	if (ret != 0)
	{
		printf("do_check_same:connect fail ret=%d get_err=%d\n", ret, op.get_err());
		return;
	}

	char sql[100];
	sprintf(sql, "SELECT num FROM test_lock WHERE id=%d", id);
	printf("sql=%s\n", sql);

	ret = op.query(sql, strlen(sql));
	if (ret != 0)
	{	
		printf("do_check_same:query error, get_err=%d\n", op.get_err());
		return;
	}
	const MYSQL_ROW &row = op.fetch_row();
	int result = atoi(row[0]);
	op.clean_result();
	if (result == expect_result)
	{
		printf("do_check_same true %d\n", result);
	}
	else
	{
		printf("do_check_same false %d:%d\n", result, expect_result);
	}
}

void do_check_diff(int expect_result)
{
	int ret = 0;
	mysql_operator_t op;
	op.init(MYSQL_HOST, MYSQL_PORT, MYSQL_USER, MYSQL_PASSWORD, MYSQL_DB, MYSQL_CHARSET);
	ret = op.connect();
	if (ret != 0)
	{
		printf("do_check_diff:connect fail ret=%d get_err=%d\n", ret, op.get_err());
		return;
	}

	char sql[100];
	sprintf(sql, "SELECT SUM(num) FROM test_lock");
	printf("sql=%s\n", sql);

	ret = op.query(sql, strlen(sql));
	if (ret != 0)
	{	
		printf("do_check_diff:query error, get_err=%d\n", op.get_err());
		return;
	}
	const MYSQL_ROW &row = op.fetch_row();
	int result = atoi(row[0]);
	op.clean_result();
	if (result == expect_result)
	{
		printf("do_check_diff true %d\n", result);
	}
	else
	{
		printf("do_check_diff false %d:%d\n", result, expect_result);
	}
}

void do_update_same(int times)
{
	const int THREAD_NUM = 50;
	const int ID = 1;
	const int OFFSET = 1;
	const int EXPECT_RESULT = THREAD_NUM * times * OFFSET;

	do_init();

	double start_time = get_time_ms();
	std::vector<std::thread> thread_list;
	for (int i = 1; i <= THREAD_NUM; ++i)
	{
		thread_list.push_back(std::thread(do_update, times, ID, OFFSET));
	}

	for (auto &th : thread_list)
	{
		th.join();
	}
	double end_time = get_time_ms();
	printf("******* use time=%lf\n", end_time-start_time);	

	do_check_same(ID, EXPECT_RESULT);
	printf("\n");
}

void do_update_diff(int times)
{
	const int ID_MAX = 50;
	const int THREAD_NUM = ID_MAX;
	const int OFFSET = 1;
	const int EXPECT_RESULT = THREAD_NUM * times * OFFSET;

	do_init();

	double start_time = get_time_ms();
	std::vector<std::thread> thread_list;
	for (int i = 1; i <= THREAD_NUM; ++i)
	{
		thread_list.push_back(std::thread(do_update, times, i, OFFSET));
	}

	for (auto &th : thread_list)
	{
		th.join();
	}
	double end_time = get_time_ms();
	printf("******* use time=%lf\n", end_time-start_time);	

	do_check_diff(EXPECT_RESULT);
	printf("\n");
}

void do_update_one(int times)
{
	const int THREAD_NUM = 1;
	const int ID = 1;
	const int OFFSET = 1;
	const int EXPECT_RESULT = THREAD_NUM * times * OFFSET;

	do_init();

	double start_time = get_time_ms();
	std::vector<std::thread> thread_list;
	for (int i = 1; i <= THREAD_NUM; ++i)
	{
		thread_list.push_back(std::thread(do_update, times, ID, OFFSET));
	}

	for (auto &th : thread_list)
	{
		th.join();
	}
	double end_time = get_time_ms();
	printf("******* use time=%lf\n", end_time-start_time);	

	do_check_same(ID, EXPECT_RESULT);
	printf("\n");
}

int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);

	const int TIMES = 1000;

	do_update_same(TIMES);
	do_update_diff(TIMES);
	// do_update_one(TIMES*20); // too slow

	return 0;
}
