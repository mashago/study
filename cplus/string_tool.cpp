#include <iostream>
#include <string>
#include <vector>

inline static void RegexReplaceString(const std::string &src, std::string &dest, std::vector<std::string> params)
{
	unsigned index = 0;
	dest = "";
	char regexBuff[8];
	unsigned maxParam = 10;
	maxParam = (params.size() > maxParam ? maxParam : params.size());
	for (unsigned i = 0; i < maxParam; i++)
	{
		snprintf(regexBuff, 9, "{{%u}}", i);

		std::string::size_type nfind = 0;
		if ((nfind = src.find(regexBuff, index)) == std::string::npos)
		{
			break;
		}

		dest += src.substr(index, nfind - index);
		dest += params[i];
		index = nfind + 5;
	}

	if (index < (src.size() - 1))
	{
		dest += src.substr(index);
	}
}

int main(int argc, char **argv)
{
	std::cout << "hello string tool" << std::endl;

	std::string dest1;
	std::string src1 = "请问{{0}},您支不支持{{1}}连任呢?";
	std::vector<std::string> params1;
	params1.push_back("主席");
	params1.push_back("董特首");
	RegexReplaceString(src1, dest1, params1);
	std::cout << "dest1=" << dest1 << std::endl;

	std::string dest2;
	std::string src2 = "支持啊";
	std::vector<std::string> params2;
	params2.push_back("废话");
	RegexReplaceString(src2, dest2, params2);
	std::cout << "dest2=" << dest2 << std::endl;

	std::string dest3;
	std::string src3 = "中央这么早就{{0}},会不会有种{{1}}的意思呢?";
	std::vector<std::string> params3;
	params3.push_back("支持");
	params3.push_back("钦定");
	RegexReplaceString(src3, dest3, params3);
	std::cout << "dest3=" << dest3 << std::endl;

	std::string dest4;
	std::string src4 = "你们啊,不要总想弄个{{0}},然后把我{{1}}一番.我{{2}},{{3}}.西方的{{4}},我和他{{5}}.你们{{6}}有个好,{{7}},{{8}}得比谁都{{9}}!";
	std::vector<std::string> params4;
	params4.push_back("大新闻");
	params4.push_back("批判");
	params4.push_back("身经百战");
	params4.push_back("见得多了");
	params4.push_back("华莱士");
	params4.push_back("谈笑风生");
	params4.push_back("香港记者");
	params4.push_back("出了什么事");
	params4.push_back("跑");
	params4.push_back("快");
	params4.push_back("生气");
	RegexReplaceString(src4, dest4, params4);
	std::cout << "dest4=" << dest4 << std::endl;


	return 0;
}
