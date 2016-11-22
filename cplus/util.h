#ifndef __UTIL_H__
#define __UTIL_H__

#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>


double get_time_ms();
bool load_file(const char *file, std::string &content);
std::string url_encode(const std::string &uri);
std::string url_decode(const std::string &uri);

// trim space at front and back
std::string trim_str(const std::string &input);

// split "a=1&b=2" string to map
void param_to_map(std::string content, std::map<std::string, std::string> &param_map);

// split string to set by token, ignore empty string
void split_to_set(const std::string &content, const std::string &token, std::set<std::string> &data);

// split string to vector by token
void split_to_vector(const std::string &content, const std::string &token, std::vector<std::string> &data, bool add_empty = true);


double get_time_ms()
{
	struct timeval tv;    
	gettimeofday(&tv,NULL);
	return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
}

bool load_file(const char *file, std::string &content)
{
	std::ifstream fin;
	fin.open("Illegal.txt");
	if (!fin.is_open())
	{
		return false;
	}

	std::stringstream ss;
	ss << fin.rdbuf();
	content = ss.str();

	fin.close();

	return true;
}

static const char safe[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  
  
std::string url_encode(const std::string &uri)  
{  
    std::string ret;   
    const unsigned char *ptr = (const unsigned char *)uri.c_str();  
    ret.reserve(uri.length());  
  
    for (; *ptr ; ++ptr)  
    {  
        if (!safe[*ptr])   
        {  
            char buf[5];  
            memset(buf, 0, 5);  
#ifdef WIN32  
            _snprintf_s(buf, 5, "%%%X", (*ptr));  
#else  
            snprintf(buf, 5, "%%%X", (*ptr));  
#endif  
            ret.append(buf);      
        }  
        else if (*ptr==' ')  
        {  
            ret+='+';  
        }  
        else{  
            ret += *ptr;  
        }  
    }  
    return ret;  
}

std::string url_decode(const std::string &uri)  
{  
    //Note from RFC1630:  "Sequences which start with a percent sign  
    //but are not followed by two hexadecimal characters (0-9,A-F,a-f) are reserved  
    //for future extension"  
    const unsigned char *ptr = (const unsigned char *)uri.c_str();  
    std::string ret;  
    ret.reserve(uri.length());  
    for (; *ptr; ++ptr)  
    {  
        if (*ptr == '%')  
        {  
            if (*(ptr + 1))  
            {  
                char a = *(ptr + 1);  
                char b = *(ptr + 2);  
                // if (!((a >= 0x30 && a < 0x40) || (a >= 0x41 && a < 0x47))) continue;  
                // if (!((b >= 0x30 && b < 0x40) || (b >= 0x41 && b < 0x47))) continue;  
				// 0-9, A-F, a-f
                if (!((a >= 0x30 && a < 0x40) || (a >= 0x41 && a < 0x47) || (a >= 0x61 && a < 0x67))) continue;  
                if (!((b >= 0x30 && b < 0x40) || (b >= 0x41 && b < 0x47) || (b >= 0x61 && b < 0x67))) continue;  
                char buf[3];  
                buf[0] = a;  
                buf[1] = b;  
                buf[2] = 0;  
                ret += (char)strtoul(buf, NULL, 16);  
                ptr += 2;  
                continue;  
            }  
        }  
        if (*ptr=='+')  
        {  
            ret+=' ';  
            continue;  
        }  
        ret += *ptr;  
    }  
    return ret;  
} 

// trim space at front and back
std::string trim_str(const std::string &input)
{
	if (input.size() == 0)
	{
		return std::string("");
	}

	size_t head = 0;
	size_t tail = input.size() - 1;

	while (head != tail)
	{
		if (input[head] != ' ')
		{
			break;
		}
		head++;
	}

	while (head != tail)
	{
		if (input[tail] != ' ')
		{
			break;
		}
		tail--;
	}

	// only one char
	if (head == tail)
	{
		if (input[head] == ' ')
		{
			return "";
		}
		return std::string(1, input[head]);
	}

	return input.substr(head, tail-head+1);
}

// split "a=1&b=2" string to map
void param_to_map(std::string content, std::map<std::string, std::string> &param_map)
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

// split string to set by token, ignore empty string
void split_to_set(const std::string &content, const std::string &token, std::set<std::string> &data)
{
	if (content.size() == 0)
	{
		return;
	}
	if (token.size() == 0)
	{
		data.insert(content);
		return;
	}
	size_t start_pos = 0;
	size_t pos = content.find(token, start_pos);
	while (pos != std::string::npos && start_pos < content.size())
	{
		std::string d = content.substr(start_pos, pos - start_pos);
		d = trim_str(d);
		if (d != "")
		{
			data.insert(d);
		}

		start_pos = pos + token.size();

		pos = content.find(token, start_pos);
	}

	if (start_pos < content.size())
	{
		// put last buffer into data
		std::string d = content.substr(start_pos);
		if (d != "")
		{
			data.insert(d);
		}
	}
}

// split string to vector by token
void split_to_vector(const std::string &content, const std::string &token, std::vector<std::string> &data, bool add_empty)
{
	if (content.size() == 0)
	{
		return;
	}
	if (token.size() == 0)
	{
		data.push_back(content);
		return;
	}
	size_t start_pos = 0;
	size_t pos = content.find(token, start_pos);
	while (pos != std::string::npos && start_pos < content.size())
	{
		std::string d = content.substr(start_pos, pos - start_pos);
		if (d.size() != 0 || add_empty)
		{
			data.push_back(d);
		}

		start_pos = pos + token.size();

		pos = content.find(token, start_pos);
	}

	if (start_pos == content.size() && add_empty)
	{
		// content end with token, add a empty string into data
		data.push_back("");
	}

	if (start_pos < content.size())
	{
		// put last buffer into data
		std::string d = content.substr(start_pos);
		if (d.size() != 0 || add_empty)
		{
			data.push_back(d);
		}
	}
}

#endif
