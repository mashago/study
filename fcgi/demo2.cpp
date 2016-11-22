
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcgi_stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <map>

#define MAX_CONTENT_LENGTH 4096 // 4k

void print_env(const char *key)
{
		// or use re-defined printf
		FCGI_printf("<br>%s: <i>%s</i><br>\r\n"
		, key, getenv(key));
}

// split content "k1=v1&k2=v2" to map
void param_to_map(std::string &content, std::map<std::string, std::string> &param_map)
{
    size_t pos = content.find("&");
    while (pos != std::string::npos && content.size() != 0)
    {
        std::string pairStr = content.substr(0, pos);
        content = content.substr(pos+1);

        size_t pos2 = pairStr.find("=");
        if (pos2 != std::string::npos)
        {
            std::string key = pairStr.substr(0, pos2);
            std::string value = pairStr.substr(pos2+1);
            param_map.insert(std::make_pair(key, value));
        }

        pos = content.find("&");
    }

    if (content.size() > 0)
    {
        size_t pos2 = content.find("=");
        if (pos2 != std::string::npos)
        {
            std::string key = content.substr(0, pos2);
            std::string value = content.substr(pos2+1);
            param_map.insert(std::make_pair(key, value));
        }
    }
}

int handle_client(std::string &content)
{
	// 1.split to map
	// 2.save into db

	// content : "sid=1&uid=123456&type=10"
	
	int sid = 0;
	uint64_t uid = 0;
	int type = 0;
	std::map<std::string, std::string> param_map;
	std::stringstream ss;

	// 1
	param_to_map(content, param_map);
	auto iter = param_map.find("sid");
	if (iter == param_map.end())
	{
		// without sid
		return -1;
	}
	ss << iter->second;
	ss >> sid;


	iter = param_map.find("uid");
	if (iter == param_map.end())
	{
		// without uid
		return -1;
	}
	ss << iter->second;
	ss >> uid;


	iter = param_map.find("type");
	if (iter == param_map.end())
	{
		// without type
		return -1;
	}
	ss << iter->second;
	ss >> type;

	// 2.
	

	return 0;
}

int main(int argc, char **argv)
{
	int ret = 0;

	char buffer[MAX_CONTENT_LENGTH+1];
	bzero(buffer, sizeof(buffer));
	std::string content = "";

	while (FCGI_Accept() >= 0)
	{
		// NOTE: must has Content-type
		/*
		FCGI_printf("Content-type: text/html\r\n" 
		"\r\n" 
		"<title>FastCGI Hello!</title>" 
		"<h1>FastCGI Hello!</h1>" 
		"Request number %d<br>\r\n"
		, ++count);
		
		print_env("SERVER_NAME");
		print_env("SERVER_ADDR");
		print_env("SERVER_PORT");
		print_env("REMOTE_ADDR");
		print_env("REMOTE_PORT");
		print_env("SCRIPT_FILENAME");
		print_env("QUERY_STRING");
		print_env("REQUEST_METHOD");
		print_env("CONTENT_TYPE");
		print_env("CONTENT_LENGTH");
		print_env("SCRIPT_NAME");
		print_env("REQUEST_URI");
		print_env("DOCUMENT_URI");
		*/

		FCGI_printf("Content-type: text/html\r\n" 
		"\r\n");

		int content_length = atoi(getenv("CONTENT_LENGTH"));

		if (content_length > MAX_CONTENT_LENGTH)
		{
			FCGI_printf("<br>CONTENT_LENGTH over size, content_length=%d max=%d<br>\r\n", content_length, MAX_CONTENT_LENGTH);
			continue;
		}

		if (content_length > 0)
		{
			int ret = FCGI_fread(buffer, content_length, 1, FCGI_stdin);
			if (ret != 1)
			{
				// something go wrong
				FCGI_printf("<br>FCGI_fread error %d<br>\r\n", ret);
				continue;
			}

			content = std::string(buffer, content_length);
		}

		ret = handle_client(content);
		FCGI_printf("%d\r\n", ret);

	}
	return 0;
};
