extern "C"
{
#include <stdio.h>
#include <string.h>
}

#include "mysql_operator.h"

#define MYSQL_HOST "127.0.0.1"
#define MYSQL_PORT 3306
#define MYSQL_USER "masha"
#define MYSQL_PASSWORD "masha"
#define MYSQL_DB "test"
#define MYSQL_CHARSET "utf8"

mysql_operator_t op;

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

int test0()
{
	int ret = 0;
	const char *sql = "SELECT * FROM test_table";

	for (int i = 0; i < 2; ++i)
	{
	ret = op.query(sql, strlen(sql));
	if (ret != 0)
	{	
		printf("test0:query error, get_err=%d\n", op.get_err());
		return -1;
	}

	int field_count = 0;
	int num_rows = 0;

	field_count = op.field_count();
	num_rows = op.num_rows();
	printf("test0:field_count=%d num_rows=%d\n", field_count, num_rows);

	while (const MYSQL_ROW &row = op.fetch_row())
	{
		printf("row[0]=%s row[1]=%s row[2]=%s row[3]=%s\n"
		, row[0], row[1], row[2], row[3]);
	}

	op.clean_result();
	}

	return 0;
}

int test1()
{
	int ret = 0;

	const char *username = "kelton";
	int len = strlen(username);
	// escape out_buffer size should be 2*n+1
	char *esc_username = new char[len * 2 + 1];
	len = op.escape_string(esc_username, username, len);
	printf("test1:escape_string len=%d esc_username=%s\n", len, esc_username);

	std::string sql = "INSERT INTO test_table (mid, username, password, num) VALUES (3, ";
	sql += "'";
	sql.append(esc_username, len);
	sql += "'";
	sql += ", '987654', 30)";
	int affected_rows = 0;

	printf("test1:sql=%s\n", sql.c_str());
	// ret = op.query(sql, strlen(sql));
	ret = op.query(sql.c_str(), sql.size());
	if (ret != 0)
	{	
		printf("test1:query error, get_err=%d\n", op.get_err());
		ret = -1;
		goto cleanup;
	}

	affected_rows = op.affected_rows();
	printf("test1:add affected_rows=%d\n", affected_rows);

	ret = 0;

cleanup:
	delete []esc_username;
	return ret;
}

int test2()
{
	int ret = 0;
	const char *sql = "UPDATE test_table SET password='xxxxxx' WHERE username='kelton'";
	ret = op.query(sql, strlen(sql));
	if (ret != 0)
	{	
		printf("test2:query error, get_err=%d\n", op.get_err());
		return -1;
	}

	int affected_rows = 0;

	affected_rows = op.affected_rows();
	printf("test2:update affected_rows=%d\n", affected_rows);

	return 0;
}

int test3()
{
	int ret = 0;
	const char *sql = "DELETE FROM test_table WHERE username='kelton'";
	ret = op.query(sql, strlen(sql));
	if (ret != 0)
	{	
		printf("test3:query error, get_err=%d\n", op.get_err());
		return -1;
	}

	int affected_rows = 0;

	affected_rows = op.affected_rows();
	printf("test3:delete affected_rows=%d\n", affected_rows);

	return 0;
}

int main(int argc, char **argv)
{
	printf("hello mysql test2\n");

	int ret = 0;

	op.init(MYSQL_HOST, MYSQL_PORT, MYSQL_USER, MYSQL_PASSWORD, MYSQL_DB, MYSQL_CHARSET);
	ret = op.connect();
	if (ret != 0)
	{
		printf("mysql connect fail get_err=%d\n", op.get_err());
		return 1;
	}

	test0();
	printf("\n");

	/*
	test1();
	printf("\n");
	test0();
	printf("\n");

	test2();
	printf("\n");
	test0();
	printf("\n");

	test3();
	printf("\n");
	test0();
	printf("\n");

	test2();
	printf("\n");
	test0();
	printf("\n");
	*/

	op.close();

	return 0;
};
