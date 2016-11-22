#ifndef _HTTP_UTIL_H_
#define _HTTP_UTIL_H_

#include <string>
#include <map>

#define IS_SPACE(x) isspace((int)(x))

std::string get_simple_page_content()
{
	char tmp_buffer[100];

	time_t now = time(NULL);
	struct tm detail_date;

	localtime_r(&now, &detail_date);

	sprintf(tmp_buffer, "Date: %d-%d-%d %02d:%02d:%02d GMT\r\n"
	, detail_date.tm_year + 1900
	, detail_date.tm_mon + 1
	, detail_date.tm_mday
	, detail_date.tm_hour
	, detail_date.tm_min
	, detail_date.tm_sec
	);

	std::string buffer;
	buffer.append("<HTML><TITLE>Simple Page</TITLE>\r\n");
	buffer.append("<BODY><P>This is a simple page.\r\n");
	buffer.append("<br>Hello World.\r\n");
	buffer.append("<br>");
	buffer.append(tmp_buffer);
	buffer.append("<br>老婆么么哒\r\n");
	buffer.append("</BODY></HTML>\r\n");
	return buffer;
}

void trim_string(std::string &buffer)
{
	size_t start_pos = 0;
	size_t end_pos = buffer.size() - 1;
	if (buffer.size() == 0)
	{
		return;
	}

	while (IS_SPACE(buffer[start_pos]) && start_pos < buffer.size())
	{
		start_pos++;
	}

	if (start_pos == buffer.size())
	{
		// buffer is full with " "
		buffer = "";
		return;
	}

	while (IS_SPACE(buffer[end_pos]) && end_pos > start_pos)
	{
		end_pos--;
	}

	buffer = buffer.substr(start_pos, end_pos - start_pos + 1);
}

// split "aa=1&bb=2&cc=3" as pair and save into a map
void url_param_to_map(std::string &content, std::map<std::string, std::string> &param_map)
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

// split header "xxx: yyy" as pair and save into header_map
void header_to_map(std::string &header, std::map<std::string, std::string> &header_map)
{
	// 1.find ":", split string
	// 2.trim string
	// 3.add into map
	if (header.size() == 0)
	{
		return;
	}

	size_t pos = header.find(":");
	if (pos == std::string::npos)
	{
		return;
	}
	if (pos == 0)
	{
		// no key
		return;
	}
	if (pos == header.size() - 1)
	{
		// no value
		return;
	}

	std::string key = header.substr(0, pos);
	std::string value = header.substr(pos+1);

	trim_string(key);
	if (key == "")
	{
		// no key
		return;
	}
	trim_string(value);
	if (value == "")
	{
		// no value
		return;
	}

	header_map.insert(std::make_pair(key, value));
	// printf("key=%s value=%s\n", key.c_str(), value.c_str());
}

#endif
