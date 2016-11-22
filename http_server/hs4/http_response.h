#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_

#include <time.h>
#include <stdio.h>
#include <string>
#include <map>
#include <algorithm>

static const char * WEEK_STR[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
static const char * MONTH_STR[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
struct http_response_t
{
	enum RESPONSE_STATUS_CODE
	{
		STATUS_NULL = 0
	,	STATUS_OK = 200
	,	STATUS_BAD_REQUEST = 400
	,	STATUS_FORBIDDEN = 403
	,	STATUS_NOT_FOUND = 404
	,	STATUS_METHOD_NOT_ALLOWED = 405
	,	STATUS_SERVER_ERROR = 500
	};

	enum RESPONSE_CONTENT_TYPE
	{
		TYPE_NULL = 0
	,	TYPE_TEXT = 1
	};

	int status;
	int content_type;
	std::string content;

	http_response_t()
	{
		status = 0;
		content_type = TYPE_NULL;
		content = "";
	}

	void init()
	{
		status = 0;
		content_type = TYPE_NULL;
		content = "";
	}

	std::string _get_first_line()
	{
		std::string buffer = "HTTP/1.1 ";

		switch (this->status)
		{
		case STATUS_OK :
			{
				buffer += "200 OK";
				break;
			};
		case STATUS_BAD_REQUEST :
			{
				buffer += "400 Bad Request";
				break;
			};
		case STATUS_FORBIDDEN :
			{
				buffer += "403 Forbidden";
				break;
			};
		case STATUS_NOT_FOUND :
			{
				buffer += "404 Not Found";
				break;
			};
		case STATUS_METHOD_NOT_ALLOWED :
			{
				buffer += "405 Method Not Allowed";
				break;
			};
		case STATUS_SERVER_ERROR :
			{
				buffer += "500 Internal Server Error";
				break;
			};
		default :
			{
				buffer += "500 Internal Server Error";
				break;
			};
		}

		buffer += "\r\n";
		return buffer;
	}

	std::string _get_response_date_header()
	{
		// Date: Sat, 11 Jan 2003 02:44:04 GMT\r\n
		std::string buffer = "";
		char tmp_buffer[100];

		time_t now = time(NULL);
		struct tm detail_date;

		localtime_r(&now, &detail_date);

		sprintf(tmp_buffer, "Date: %s, %d %s %d %02d:%02d:%02d GMT\r\n"
		, WEEK_STR[detail_date.tm_wday]
		, detail_date.tm_mday
		, MONTH_STR[detail_date.tm_mon]
		, detail_date.tm_year + 1900
		, detail_date.tm_hour
		, detail_date.tm_min
		, detail_date.tm_sec
		);

		buffer = tmp_buffer;

		return buffer;
	}

	std::string _get_content_type_header()
	{
		std::string buffer = "";

		buffer = "Content-Type: ";
		switch (this->content_type)
		{
		case TYPE_TEXT :
			{
				buffer += "text/html; charset=utf-8\r\n";
				break;
			}
		default :
			{
				buffer += "text/html; charset=utf-8\r\n";
				break;
			}
		}
		return buffer;
	}

	std::string _get_content_length_header()
	{
		std::string buffer = "";

		buffer = "Content-Length: " + std::to_string((long long unsigned int)this->content.size()) + "\r\n";

		return buffer;
	}

	int pack(std::string &buffer)
	{
		// 1.set first line
		// 2.set date line
		// 3.set Content-Length
		// 4.set Content 
		std::string tmp_buffer = "";
		tmp_buffer.append(_get_first_line());
		tmp_buffer.append(_get_response_date_header());
		tmp_buffer.append(_get_content_type_header());
		tmp_buffer.append(_get_content_length_header());
		tmp_buffer.append("\r\n");
		if (this->content.size() > 0)
		{
			tmp_buffer.append(content);
		}

		buffer = std::move(tmp_buffer);
		return 0;
	}

};

#endif
