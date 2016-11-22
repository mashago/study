#ifndef __Config_H__
#define __Config_H__

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>

class Config
{
private:
	std::map<std::string, std::string> m_oConfigItem;

public:
	int LoadConfig(const char *configFile)
	{
		FILE *fp = fopen(configFile, "r");
		if (!fp)
		{
			return -1;
		}

		char line[1024];
		while (fgets(line, sizeof(line), fp))
		{
			char *pHead = line;
			while (*pHead && isspace(*pHead))
			{
				pHead++;
			}
			if (line[0] == '#' || *pHead =='\0')
			{
				continue;
			}
			char key[50], value[50];
			if (sscanf(line, "%s %s", key, value) != 2)
			{
				continue;
			}
			m_oConfigItem[std::string(key)] = std::string(value);
		}
		fclose(fp);
		return 0;
	}

	const char * GetString(const char *key, const char *defValue=NULL)
	{
		std::map<std::string, std::string>::iterator iter = m_oConfigItem.find(std::string(key));
		if (iter != m_oConfigItem.end())
		{
			return iter->second.c_str();
		}
		return defValue;
	}

	int GetInt(const char *key, int defValue=0)
	{
		std::map<std::string, std::string>::iterator iter = m_oConfigItem.find(std::string(key));
		if (iter != m_oConfigItem.end())
		{
			return atoi(iter->second.c_str());
		}
		return defValue;
	}


};

#endif
