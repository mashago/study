#ifndef _MYSQL_OPERATOR_T_H_
#define _MYSQL_OPERATOR_T_H_

#include <string>
#include "mysql.h"

class mysql_operator_t
{
private:
	std::string host;
	int port;
	std::string user;
	std::string password;
	std::string db_name;
	std::string charset;

	MYSQL *conn;
	MYSQL_RES *result;
	int err;
	int reconnect();
	MYSQL * real_connect();
public:
	mysql_operator_t();
	~mysql_operator_t();

	// must do init first
	void init(std::string host, int port, std::string user, std::string password, std::string db_name, std::string charset = "utf8");
	int connect();
	int close();

	// out_buffer size should be 2 * in_buffer size + 1
	int escape_string(char *out_buffer, const char *in_buffer, int len);

	// query will clean last query result
	int query(const char *sql, int len);
	int field_count();
	int num_rows();
	int affected_rows();
	const MYSQL_ROW & fetch_row();
	void clean_result();

	int get_err();
};

#endif
