
#include <stdio.h>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "mysql_operator.h"

mysql_operator_t::mysql_operator_t() 
: host(""), port(0), user(""), password(""), db_name(""), charset(""), conn(NULL), result(NULL), err(0)
{
}

mysql_operator_t::~mysql_operator_t()
{
}

void mysql_operator_t::init(std::string host, int port, std::string user, std::string password, std::string db_name, std::string charset)
{
	this->host = host;
	this->port = port;
	this->user = user;
	this->password = password;
	this->db_name = db_name;
	this->charset = charset;
}

MYSQL * mysql_operator_t::real_connect()
{
	// 1.do mysql_init()
	// 2.set mysql options
	// 3.do mysql_real_connect()
	
	MYSQL *new_conn = NULL;
	new_conn = mysql_init(NULL);  // NULL ?
	if (new_conn == NULL)
	{
		return NULL;
	}

	mysql_options(new_conn, MYSQL_SET_CHARSET_NAME, charset.c_str());

	MYSQL *ret_conn;
	ret_conn = mysql_real_connect(new_conn, host.c_str(), user.c_str(), password.c_str(), db_name.c_str(), port, NULL, 0);
	if (ret_conn == NULL)
	{
		this->err = mysql_errno(new_conn);
		mysql_close(new_conn);
		return NULL;
	}

	return new_conn;
}

int mysql_operator_t::connect()
{
	if (this->conn != NULL)
	{
		return -1;
	}
	this->conn = real_connect();
	if (this->conn == NULL)
	{
		return -2;
	}
	return 0;
}

int mysql_operator_t::close()
{
	if (this->conn != NULL)
	{
		clean_result();
		mysql_close(this->conn);
		this->conn = NULL;
	}
	return 0;
}

int mysql_operator_t::reconnect()
{
	MYSQL *new_conn = NULL;
	new_conn = real_connect();
	if (new_conn == NULL)
	{
		return -1;
	}
	close();
	this->conn = new_conn;
	return 0;
}

int mysql_operator_t::escape_string(char *out_buffer, const char *in_buffer, int len)
{
	return mysql_real_escape_string(this->conn, out_buffer, in_buffer, len);
}

int mysql_operator_t::query(const char *sql, int len)
{
	// 1.do mysql_real_query
	// 2.clean mysql result
	// 3.do mysql_store_result, event sql is not a select
	// 4.if error is disconnect, reconnect and try again
	
	int ret = 0;
	int reconn_count = 0;
	do
	{
		if (this->conn != NULL)
		{
			ret = mysql_real_query(this->conn, sql, len);
			if (ret == 0)
			{
				clean_result();
				this->result = mysql_store_result(this->conn); // when exec is NOT select, this call is ok
				return ret;
			}
		}

		ret = -1;

		this->err = 8888; // 8888 for connect null

		if (this->conn != NULL)
		{
			this->err = mysql_errno(this->conn);
		}

		if (this->err == 2013 || this->err == 2006 || this->err == 8888)
		{
			printf("query:disconnect errno=%d\n", this->err);
			#ifdef WIN32
			Sleep(1000);
			#else
			sleep(1);
			#endif
			
			reconnect();
			reconn_count++;
		}
		else
		{
			// execute error
			return ret;
		}
	}
	while (reconn_count < 3);

	return ret;
}

int mysql_operator_t::field_count()
{
	if (this->conn == NULL)
	{
		return -1;
	}

	return mysql_field_count(this->conn);
}

int mysql_operator_t::num_rows()
{
	if (this->conn == NULL)
	{
		return -1;
	}

	if (this->result == NULL)
	{
		return -1;
	}

	return mysql_num_rows(this->result);
}

int mysql_operator_t::affected_rows()
{
	if (this->conn == NULL)
	{
		return -1;
	}

	return mysql_affected_rows(this->conn);
}

const MYSQL_ROW & mysql_operator_t::fetch_row()
{
	static MYSQL_ROW row = NULL;
	if (this->conn == NULL)
	{
		row = NULL;
		return row;
	}

	if (this->result == NULL)
	{
		row = NULL;
		return row;
	}

	row = mysql_fetch_row(this->result);

	return row;
}

/*
void mysql_operator_t::clean_result()
{
	this->err = 0;
	if (this->result == NULL)
	{
		return;
	}

	mysql_free_result(this->result);
	this->result = NULL;
}
*/

void mysql_operator_t::clean_result()
{
	this->err = 0;

	if (this->result != NULL)
	{
		mysql_free_result(this->result);
		this->result = NULL;
	}

	while(!mysql_next_result(this->conn))
	{
		this->result = mysql_store_result(this->conn);
		mysql_free_result(this->result);
	} 

	this->result = NULL;
}

int mysql_operator_t::get_err()
{
	return this->err;
}

