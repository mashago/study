
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mysql.h"

#define MYSQL_HOST	"127.0.0.1"
#define MYSQL_PORT	3306
#define MYSQL_USER	"evil"
#define MYSQL_PASSWORD	"1"
#define MYSQL_DATABASE	"evil"

int strtol_safe(char *str, int default_value)
{
	char *end;
	long value;

	if (str == NULL)
	{
		printf("strtol_safe:str_null\n");
		return default_value;
	}
	value = strtol(str, &end, 10);
	if (end == str)
	{
		printf("strtol_safe:str_not_long %s\n", str);
		return default_value;
	}

	return (int) value;
}
		


MYSQL * my_open()
{
	MYSQL * conn = NULL;
	conn = mysql_init(NULL);
	if (conn == NULL) 
	{
		printf("my_opon:conn_null\n");
		return NULL;
	}

	mysql_options(conn, MYSQL_SET_CHARSET_NAME, "utf8");

	MYSQL * ret_conn;
	ret_conn = mysql_real_connect(conn, MYSQL_HOST, MYSQL_USER
	, MYSQL_PASSWORD, MYSQL_DATABASE, MYSQL_PORT, NULL, 0);
	if (ret_conn == NULL)
	{
		printf("my_opon:ret_conn_null\n");
		mysql_close(conn);
		return NULL;
	}

	MY_CHARSET_INFO charset_info;
	mysql_get_character_set_info(conn, &charset_info);

	return conn;
}

int my_close(MYSQL * conn)
{
	mysql_close(conn);
	return 0;
}

int my_reconn(MYSQL ** pconn)
{
	MYSQL *new_conn = NULL;
	new_conn = my_open();
	if (new_conn == NULL)
	{
		return -5;
	}
	if (*pconn != NULL)
	{
		mysql_close(*pconn);
	}

	*pconn = new_conn;
	return 0;
}

int my_query(MYSQL **pconn, const char *q, int len)
{
	int ret;
	int count = 0;

retry:
	if (*pconn != NULL)
	{
		ret = mysql_real_query(*pconn, q, len);
		// ret = mysql_query(*pconn, q);
		if (ret == 0)
		{
			return 0;
		}
	}

	ret = 8888;

	if (*pconn != NULL)
	{
		ret = mysql_errno(*pconn);
	}

	if (ret == 2013 || ret == 2006 || ret == 8888)
	{
		printf("my_query:ret=%d count=%d\n", ret, count);
		sleep(1);
		my_reconn(pconn);
		count ++;
		if (count >= 3)
		{
			return ret;
		}
		goto retry;
	}

	return ret;
}

#ifdef TTT

#define SQL_SELECT	"SELECT eid, username, password, icon FROM evil_user LIMIT 1"
int test0_select(MYSQL **pconn)
{
	
	int ret;
	int len;
	char q_buffer[1000];

	int eid;
	char username[40];
	char password[40];
	int icon;

	len = sprintf(q_buffer, SQL_SELECT);

	// printf("test0:q_buffer=%s\n", q_buffer);

	ret = my_query(pconn, q_buffer, len);
	if (ret != 0)
	{
		printf("test0:mysql_query_error %d\n", ret);
		return -6;
	}

	MYSQL_RES *result = NULL;
	MYSQL_ROW row;
	int num_row;

	result = mysql_store_result(*pconn);
	if (result == NULL)
	{
		printf("test0:mysql_result_null\n");
		return -16;
	}

	num_row = mysql_num_rows(result);
	if (num_row <= 0)
	{
		ret = -26;
		printf("test0:mysql_result_empty\n");
		goto cleanup;
	}

	len = mysql_field_count(*pconn);
	// eid, username, password, icon 
	if (len != 4)
	{
		ret = -7;
		printf("test0:mysql_field_count_mismatch %d\n", len);
		goto cleanup;
	}

	row = mysql_fetch_row(result);
	if (row == NULL)
	{
		ret = -3;
		printf("test0:mysql_row_null\n");
		goto cleanup;
	}
		
	eid = strtol_safe(row[0], -1);
	sprintf(username, "%s", row[1]);
	sprintf(password, "%s", row[2]);
	icon = strtol_safe(row[3], -2);

	printf("test0:eid=%d username=%s password=%s icon=%d\n"
	, eid, username, password, icon);


	ret = 0;

cleanup:
	mysql_free_result(result);
	return ret;
}


#define SQL_UPDATE	"UPDATE evil_user SET icon=icon+1 WHERE eid=%d"
int test1_update(MYSQL **pconn)
{
	
	int ret;
	int len;
	char q_buffer[1000];
	int eid;

	eid = 501;
	len = sprintf(q_buffer, SQL_UPDATE, eid);
	printf("test1:q_buffer=%s\n", q_buffer);

	ret = my_query(pconn, q_buffer, len);
	if (ret != 0)
	{
		printf("test1:mysql_query_error %d\n", ret);
		return -6;
	}

	ret = mysql_affected_rows(*pconn);
	printf("test1:mysql_affect_rows=%d\n", ret);

	return 0;
}
		

int main(int argc, char * argv[])
{

	int ret;
	MYSQL * conn;
	ret = mysql_thread_init();
	// printf("mysql_thread_init:ret=%d\n", ret);

	conn = my_open();
	if (conn == NULL) 
	{
		printf("main:conn_null\n");
		return -10;
	}

	test0_select(&conn);
	test1_update(&conn);

	mysql_thread_end();

	return 0;
}

#endif
