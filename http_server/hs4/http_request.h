#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_

extern "C"
{
#include <stdio.h>
}
#include <string>
#include <map>
#include <algorithm>

#include "http_util.h"

#define MAX_HTTP_REQUEST_URL_SIZE 5120

struct http_request_t
{
	enum HTTP_METHOD
	{
		METHOD_NULL = 0
	,	METHOD_GET = 1
	,	METHOD_POST = 2
	};

	int method;
	std::string url;
	std::map<std::string, std::string> param_map;
	std::map<std::string, std::string> header_map;
	std::string content;

	http_request_t()
	{
		method = METHOD_NULL;
		url = "";
		param_map.clear();
		header_map.clear();
		content = "";
	};

	void init()
	{
		method = METHOD_NULL;
		url = "";
		param_map.clear();
		header_map.clear();
		content = "";
	}

	// set string line, not include /r/n. return real line size, include /r/n.
	// if return == 0, means not include a full line
	// if return < 0, means http line format error
	// if return > 0:
	// 1.line.size() > 0, line include a header
	// 1.line.size() == 0, line is /r/n
	int _get_line(std::string &line, const char *buffer, int size)
	{
		line = "";
		int i = 0;
		const char *ptr = buffer;
		bool has_line = false;
		bool has_r = false;

		// skip space
		while (i < size)
		{
			if (*ptr != ' ')
			{
				break;
			}
			ptr++;
			i++;
		}

		while (i < size)
		{
			if (*ptr == '\r')
			{
				ptr++;
				i++;
				has_r = true;
				continue;
			}
			if (*ptr == '\n')
			{
				if (has_r)
				{
					i++;
					has_line = true;
					break;
				}
				else
				{
					// format error
					i = -1;
					break;
				}
			}
			line.append(ptr, 1);
			ptr++;
			i++;
		}

		if (has_line == false)
		{
			if (i == -1)
			{
				// http header format error
				return -1;
			}
			else
			{
				// not include a http header line
				return 0;
			}
		}
		return i;
	}

	// unpack first line.
	// return -1 for format error
	int _unpack_first_line(std::string &line)
	{
		// 1.get method
		// 2.get url
		char method_str[255];
		uint32_t i = 0;
		uint32_t j = 0;

		// 1.
		while (IS_SPACE(line[i]) && i < line.size())
		{
			i++;
		}
		if (i == line.size())
		{
			printf("_unpack_first_line:error, no method\n");
			return -1;
		}

		while (!IS_SPACE(line[i]) && i < line.size() && j < sizeof(method_str) - 1)
		{
			method_str[j] = line[i];
			i++;
			j++;
		}
		if (i == line.size())
		{
			printf("_unpack_first_line:error, no url\n");
			return -1;
		}
		method_str[j] = '\0';
		printf("_unpack_first_line:method_str=%s\n", method_str);

		if (strcasecmp(method_str, "GET") && strcasecmp(method_str, "POST"))
		{
			printf("_unpack_first_line:error, unimplemented method\n");
			return -1;
		}

		if (strcasecmp(method_str, "GET") == 0)
		{
			this->method = METHOD_GET;
		}

		if (strcasecmp(method_str, "POST") == 0)
		{
			this->method = METHOD_POST;
		}

		// 2.
		while (IS_SPACE(line[i]) && i < line.size())
		{
			i++;
		}
		if (i == line.size())
		{
			// error, no url
			printf("_unpack_first_line:error, no url\n");
			return -1;
		}

		j = 0;
		char tmp_url[MAX_HTTP_REQUEST_URL_SIZE+1];
		if (line.size() - 14 > MAX_HTTP_REQUEST_URL_SIZE) // 14 for "POST  HTTP/1.1"
		{
			// error, url too long
			printf("_unpack_first_line:error, url too long\n");
			return -1;
		}

		while (!IS_SPACE(line[i]) && i < line.size() && j < MAX_HTTP_REQUEST_URL_SIZE)
		{
			tmp_url[j] = line[i];
			i++;
			j++;
		}
		tmp_url[j] = '\0';
		if (i == line.size())
		{
			printf("_unpack_first_line:error, no http version\n");
			return -1;
		}
		printf("_unpack_first_line:tmp_url=%s\n", tmp_url);

		uint32_t url_size = j;
		if (url_size == 0)
		{
			printf("_unpack_first_line:error, no url\n");
			return -1;
		}

		j = 0;
		while (j < url_size && tmp_url[j] != '?')
		{
			this->url.append(tmp_url+j, 1);
			j++;
		}
		// printf("_unpack_first_line:url=%s\n", this->url.c_str());

		if (j == url_size || j == url_size - 1)
		{
			// no param in url, or end with '?', return
			return 0;
		}

		const char *ptr = tmp_url + j + 1;
		std::string param(ptr, url_size - j - 1);
		url_param_to_map(param, this->param_map);

		return 0;
	}


	// input buffer and buffer size
	// if return 0, buffer not include a full http request
	// if return -1, http request format error
	// else return http request data buffer size
	int unpack(const char *buffer, int buffer_size)
	{
		// 1.get first line
		// 2.get other header
		// 3.get content

		printf("\n");
		printf("unpack:-------- start buffer_size=%d\n", buffer_size);

		int ret = 0;
		const char *ptr = buffer;
		int line_size = 0;
		std::string line_str;
		int total_size = buffer_size;

		// 1.
		line_size = _get_line(line_str, ptr, buffer_size);
		printf("unpack:line_size=%d\n", line_size);
		if (line_size <= 0)
		{
			// not include a line or data error
			return line_size;
		}

		if (line_str.size() == 0)
		{
			// something go wrong, first line should not be /r/n
			return -1;
		}
		// printf("unpack:line_str=%s\n", line_str.c_str());

		ret = _unpack_first_line(line_str);
		if (ret != 0)
		{
			// unpack first line fail
			printf("unpack:unpack first line fail\n");
			return -1;
		}
		ptr += line_size;
		buffer_size -= line_size;

		// debug print
		printf("unpack:method=%d\n", this->method);
		printf("unpack:url=%s\n", this->url.c_str());
		printf("unpack:param_map.size()=%lu\n", this->param_map.size());
		for (auto iter = this->param_map.begin(); iter != this->param_map.end(); iter++)
		{
			printf("[%s]:[%s]\n", iter->first.c_str(), iter->second.c_str());
		}
		printf("unpack:---- after first line buffer_size=%d\n", buffer_size);

		// 2.
		while ((line_size = _get_line(line_str, ptr, buffer_size)) > 0 && line_str.size() > 0)
		{
			// printf("line_str=[%s]\n", line_str.c_str());
			header_to_map(line_str, this->header_map);
			
			ptr += line_size;
			buffer_size -= line_size;
		}

		if (line_size < 0)
		{
			printf("unpack:http request format error\n");
			return -1;
		}
		if (line_size == 0)
		{
			printf("unpack:not inclue a full http request\n");
			return 0;
		}
		if (line_str.size() == 0)
		{
			// get /r/n line, skip it
			ptr += line_size;
			buffer_size -= line_size;
		}

		// debug print
		printf("unpack:header_map.size()=%lu\n", this->header_map.size());
		for (auto iter = this->header_map.begin(); iter != this->header_map.end(); iter++)
		{
			printf("[%s]:[%s]\n", iter->first.c_str(), iter->second.c_str());
		}
		printf("unpack:---- after header buffer_size=%d\n", buffer_size);

		// 3.
		int content_length = 0;
		auto iter = this->header_map.find("Content-Length");
		if (iter != this->header_map.end())
		{
			char *end;
			content_length = (int)strtol(iter->second.c_str(), &end, 10);
			if (end == iter->second.c_str())
			{
				printf("unpack:Content-Length not a int\n");
				return -1;
			}
		}
		printf("unpack:content_length=%d\n", content_length);

		if (content_length > 0)
		{
			if (buffer_size < content_length)
			{
				// not include full content
				return 0;
			}
			this->content = std::move(std::string(ptr, content_length));
			ptr += content_length;
			buffer_size -= content_length;
			printf("unpack:content=[%s]\n", content.c_str());

		}

		printf("unpack:-------- get a full http request\n\n");
		return total_size - buffer_size;
	}
};

#endif
