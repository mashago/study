extern "C"
{
#include <stdio.h>
#include <unistd.h>
#include <string.h>
}
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
			sleep(count);
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

int test0(MYSQL **pconn)
{
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

int test1(MYSQL **pconn)
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

int main(int argc, char **argv)
{
	printf("hello mysql\n");

	MYSQL *conn;
	conn = my_open();

	test0(&conn);

	if (conn != NULL)
	{
		mysql_close(conn);
	}
	return 0;
}

