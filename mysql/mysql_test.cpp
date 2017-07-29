extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <string.h>
}
#include <string>
#include <sstream>
#include <map>
#include "mysql.h"

#define MYSQL_HOST "127.0.0.1"
#define MYSQL_PORT 3306
#define MYSQL_USER "masha"
#define MYSQL_PASSWORD "masha"
#define MYSQL_DB "test"

MYSQL * my_open()
{
	MYSQL * conn = NULL;
	printf("my_open: mysql version: %s\n", mysql_get_client_info());

	conn = mysql_init(NULL);  // NULL ?
	if (conn == NULL)
	{
		printf("my_open:mysql_init null\n");
	}

	// ref: setup utf8 connection
	// http://stackoverflow.com/questions/8112153/process-utf-8-data-from-mysql-in-c-and-give-result-back
	mysql_options(conn, MYSQL_SET_CHARSET_NAME, "utf8");   // was "utf8"
	// peter: SET NAMES is optional:
	//    mysql_options(conn, MYSQL_INIT_COMMAND, "SET NAMES utf8");  // was "utf8"
	MYSQL * ret_conn;
	ret_conn = mysql_real_connect(conn, MYSQL_HOST, MYSQL_USER,
		MYSQL_PASSWORD, MYSQL_DB, MYSQL_PORT, NULL, 0);
	if (ret_conn == NULL)
	{
		printf("my_open:mysql_real_connect null %d %s\n", mysql_errno(conn), MYSQL_HOST);
		mysql_close(conn);
		return NULL;
	}

	MY_CHARSET_INFO charset_info;
	mysql_get_character_set_info(conn, &charset_info);
	// pls check:  charset=utf8_general_ci  collate=utf8  num=33
	printf("mysql charset=%s collate=%s num=%d\n"
	, charset_info.name, charset_info.csname, charset_info.number);
	return conn;
}

int my_close(MYSQL *conn)
{
	mysql_close(conn);
	return 0;
}


int my_reconn(MYSQL **pconn)
{
    MYSQL *new_conn = NULL;
    new_conn = my_open();
    if (new_conn == NULL)
	{
        return -55;
    }
    // only close and re-new the *pconn when new_conn is ready!
    if (*pconn != NULL) {
        my_close(*pconn);
    }
    *pconn = new_conn;
    return 0;
}

int my_query(MYSQL **pconn, const char *q, int len)
{
    int ret;
    int count = 0;
	do
	{
		if (*pconn != NULL)
		{
			ret = mysql_real_query(*pconn, q, len);
			if (ret == 0)
			{
				return 0;
			}
		}
		// implicit: ret != 0 : means error
		ret = 8888;  // means *pconn is null

		if (*pconn != NULL)
		{
			ret = mysql_errno(*pconn);
		}
		if (ret==2013 || ret==2006 || ret==8888)
		{
			// sleep depends on count
			printf("my_query:retry %d", count);
			#ifdef WIN32
			Sleep(count * 1000);
			#else
			sleep(count);
			#endif
			my_reconn(pconn);
			count++;
		}
		else
		{
			return ret;
		}
	}
	while (count < 3);
    return ret;
}

/*
CREATE TABLE `test_table` (
	`mid` int(10) unsigned NOT NULL,
	`username` char(100) NOT NULL,
	`password` char(100) NOT NULL,
	`num` int(10) unsigned NOT NULL DEFAULT '0',
	PRIMARY KEY (`mid`),
	UNIQUE KEY `username` (`username`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8
*/
int test0(MYSQL **pconn)
{
	int ret = 0;
	const char *sql = "SELECT * FROM test_table";
	ret = my_query(pconn, sql, strlen(sql));
	if (ret != 0)
	{
		printf("ret = %d\n", ret);
		return -1;
	}

	MYSQL_RES *result;
	MYSQL_ROW row;
	int field_count = 0;
	int num_row = 0;

	result = mysql_store_result(*pconn);
	if (result == NULL)
	{
		printf("result null\n");
		return -1;
	}

	field_count = mysql_field_count(*pconn);
	printf("field_count=%d\n", field_count);

	num_row = mysql_num_rows(result);
	printf("num_row=%d\n", num_row);
	
	while ((row = mysql_fetch_row(result)) != NULL)
	{
		for (int i = 0; i < field_count; i++)
		{
			printf("row[%d]=%s ", i, row[i]);
		}
		printf("\n");
	}

	mysql_free_result(result);

	return 0;
}

int test1(MYSQL **pconn)
{
	int ret = 0;
	const char *sql = "DESC test_table";
	ret = my_query(pconn, sql, strlen(sql));
	if (ret != 0)
	{
		printf("ret = %d\n", ret);
		return -1;
	}

	MYSQL_RES *result;
	MYSQL_ROW row;
	int field_count = 0;
	int num_row = 0;

	result = mysql_store_result(*pconn);
	if (result == NULL)
	{
		printf("result null\n");
		return -1;
	}

	field_count = mysql_field_count(*pconn);
	printf("field_count=%d\n", field_count);

	num_row = mysql_num_rows(result);
	printf("num_row=%d\n", num_row);
	
	while ((row = mysql_fetch_row(result)) != NULL)
	{
		for (int i = 0; i < field_count; i++)
		{
			printf("row[%d]=%s ", i, row[i]);
		}
		printf("\n");
	}

	mysql_free_result(result);

	return 0;
}

int test2(MYSQL **pconn)
{
	int ret = 0;
	const char *sql = "INSERT INTO test_table (mid, username, password, num) VALUES (1, 'masha', '123456', 20)";
	ret = my_query(pconn, sql, strlen(sql));
	if (ret != 0)
	{
		printf("ret = %d\n", ret);
		return -1;
	}

	MYSQL_RES *result;
	MYSQL_ROW row;
	int field_count = 0;
	int num_row = 0;

	result = mysql_store_result(*pconn);
	if (result == NULL)
	{
		printf("result null\n");
		return -1;
	}

	field_count =  mysql_field_count(*pconn);
	printf("field_count=%d\n", field_count);

	num_row = mysql_num_rows(result);
	printf("num_row=%d\n", num_row);

	while ((row = mysql_fetch_row(result)) != NULL)
	{
		printf("row[0]=%s row[1]=%s row[2]=%s row[3]=%s\n"
		, row[0], row[1], row[2], row[3]);
	}

	mysql_free_result(result);

	return 0;
}

enum
{
	TYPE_INT = 1,
	TYPE_INT64 = 2,
	TYPE_FLOAT = 3,
	TYPE_STRING = 4,
	TYPE_STORE = 5,
};

std::map<int, std::string> init_type_map()
{
	std::map<int, std::string> type_map;
	type_map[TYPE_INT] = "int(11)";
	type_map[TYPE_INT64] = "bigint(20)";
	type_map[TYPE_FLOAT] = "float";
	type_map[TYPE_STRING] = "varchar(255)";
	type_map[TYPE_STORE] = "blob";
	return type_map;
}

const std::map<int, std::string> & get_type_map()
{
	static std::map<int, std::string> type_map = init_type_map();
	return type_map;
}

int test3(MYSQL **pconn)
{
	std::string table_name = "test_table2";
	std::map<std::string, int> content_map;
	content_map["uuid"] = TYPE_INT64;
	content_map["item_id"] = TYPE_INT;
	content_map["item_num"] = TYPE_INT;
	content_map["item_price"] = TYPE_FLOAT;
	content_map["info"] = TYPE_STRING;
	content_map["detail"] = TYPE_STORE;

	std::map<int, std::string> type_map = get_type_map();

	std::map<std::string, std::string> field_map;
	for (auto iter = content_map.begin(); iter != content_map.end(); iter++)
	{
		field_map[iter->first] = type_map[iter->second];
	}

	std::ostringstream oss;
	oss << "CREATE TABLE `" << table_name << "` (" ;
	for (auto iter = field_map.begin(); iter != field_map.end(); iter++)
	{
		oss << "`" << iter->first << "` " << iter->second << ",";
	}
	oss << ") ENGINE=MyISAM DEFAULT CHARSET=utf8";
	printf("%s\n", oss.str().c_str());

	return 0;


	int ret = 0;
	const char *sql = "INSERT INTO test_table (mid, username, password, num) VALUES (1, 'masha', '123456', 20)";
	ret = my_query(pconn, sql, strlen(sql));
	if (ret != 0)
	{
		printf("ret = %d\n", ret);
		return -1;
	}

	MYSQL_RES *result;
	MYSQL_ROW row;
	int field_count = 0;
	int num_row = 0;

	result = mysql_store_result(*pconn);
	if (result == NULL)
	{
		printf("result null\n");
		return -1;
	}

	field_count =  mysql_field_count(*pconn);
	printf("field_count=%d\n", field_count);

	num_row = mysql_num_rows(result);
	printf("num_row=%d\n", num_row);

	while ((row = mysql_fetch_row(result)) != NULL)
	{
		printf("row[0]=%s row[1]=%s row[2]=%s row[3]=%s\n"
		, row[0], row[1], row[2], row[3]);
	}

	mysql_free_result(result);

	return 0;
}

typedef int (*testcase_t) (MYSQL **pconn);
testcase_t test_list[] =
{
	test0
,	test1
,	test2
,	test3
};

int main(int argc, char **argv)
{
	printf("hello mysql\n");

	MYSQL *conn;
	conn = my_open();

	int ret;
	int maxcase;
	int testcase;

	maxcase = sizeof(test_list) / sizeof(testcase_t);
	testcase = maxcase - 1;

	if (argc > 1) {
		if (!strcmp(argv[1], "all"))
		{
			printf("run all case\n");
			for (int i=0; i<maxcase; i++)
			{
				printf("run case[%d]\n", i);
				ret = test_list[i](&conn);
				if (ret != 0)
				{
					printf("case[%d] ret=%d\n", i, ret);
					goto cleanup;
				}
			}
			goto cleanup;
		}
		testcase = atoi(argv[1]);
		if (testcase < 0 || testcase >= maxcase) 
		{
			testcase = maxcase - 1;
		}
	}

	printf("run case[%d]\n", testcase);
	ret = test_list[testcase](&conn);
	if (ret != 0) 
	{
		printf("case[%d] ret=%d\n", testcase, ret);
	}

cleanup:
	if (conn != NULL)
	{
		mysql_close(conn);
	}

	return 0;
}

