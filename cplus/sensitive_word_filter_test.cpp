
#include <stdio.h>
#include <string>
#include <set>
#include <algorithm>
#include "sensitive_word_filter.h"
#include "util.h"

std::string news = "新浪体育讯　　奥运会开幕前的几天，从里约传来的都是各种不靠谱的声音，这让外界对于奥运会开幕式多了更多隐忧。对于一届奥运会而言，开幕式是皇冠上的明珠。里约还能带给我们一届令人难忘的开幕式吗？【点击观看开幕式GIF图】 　　尤其是开幕式的预算，由最初的1.139亿美元，削减到5590万美元，减少了近一半。根据媒体的报道，这个数字是北京奥运会的1/20，是伦敦奥运会的1/12。 　　北京奥运会，我们在鸟巢见证了美轮美奂的开幕式；伦敦奥运会的开幕式也盛况空前。充足的预算，让之前的两届奥运会大量使用了高科技技术，人力物力也极大丰富， 都配得上“恢弘”二字。 　　开幕式并不是一场炫富表演，金钱虽可以为其镀上一层光环，却并不能丰富其内涵。一个浑身珠光宝气的丽人，骨子里可能还是那个没见过世面的村姑。 　　里约奥运会开幕式，用一种素面朝天的姿态，打造了一场简约而不简单的盛宴。它充分挖掘了巴西的文明，从生命的起源展示巴西的历史，热情似火的舞蹈和悦耳的音乐，成了开幕式最好的点缀。 　　在创意上，开幕式总导演绞尽脑汁，绚丽的色彩，精巧的设计，充分利用了光影的特点，让人拍案叫绝。绿色、和平和环保的主题贯彻始终，体现了巴西人的情怀。点燃火炬的方式并不复杂，不似当年李宁追月般那么动人，但酷炫的艺术感还是让人拍案叫绝。 　　它没有那么壮观，不像北京奥运会和伦敦奥运会几乎邀请了全世界最重要国家的元首出席。一共只有45个国家的领导人出席。 　　并不自卑的巴西人不需要通过这样一届开幕式向世界证明他们的存在。他们就是把开幕式办成一届大Party，就像狂欢节或者沙滩上没日没夜的派对一样。姑娘、小伙子们载歌载舞，像在海滩上一样随意，肆意宣泄激情，让世界看到了巴西人们快乐的一面。 　　里约开幕式，用自己的行动证明，金钱与伟大的开幕式并不能划等号。里约用了更少的钱，却办了一届让更多人感动的奥运会开幕式。 　　正如奥地利著名作家茨威格在《巴西：未来之国》中描述得那样，巴西承载了人类文明的希望。这种未来并不是指经济的发展，也并不单单是科技、艺术的发展，而是自由、平等、博爱之精神。 　　里约奥运会开幕式证明了茨威格并没有看走眼，巴西就是人类文明的未来之国。这届奥运会的开幕式也为未来树立了一座新的标杆，毕竟奥运会不会只在最发达的国家举办，它也不应该成为一种盲目攀比的载体，传递文明、展现自我才是其精神内核，一个民族的文明和它的价值观与金钱没有太多关系，不应该被其玷污。在千疮百孔中，里约办了一场伟大的奥运会开幕式。它可能没那么光彩夺目，但它用丰富的内涵和气质，更让我们动容。当吉赛尔·邦辰穿过场地中央进行走秀时，你不仅仅被她的容颜打动，更是被她浑身上下散发的优雅气质而感染。她就是这届奥运会开幕式的象征。 　　我相信，在未来的漫长岁月中，里约奥运会开幕式会以其独特的特质永载奥运史册。(工匠精神)"; 

// std::string news = "在千疮百孔中，里约办了一场伟大的奥运会开幕式。它可能没那么光彩夺目，但它用丰富的内涵和气质，更让我们动容。当吉赛尔·邦辰穿过场地中央进行走秀时，你不仅仅被她的容颜打动，更是被她浑身上下散发的优雅气质而感染。她就是这届奥运会开幕式的象征。 　　我相信，在未来的漫长岁月中，里约奥运会开幕式会以其独特的特质永载奥运史册。(工匠精神)"; 



void fill_words(std::set<std::string> &words_set)
{
	words_set.clear();

	const char *file = "Illegal.txt";
	std::string content;
	if (load_file(file, content) == false)
	{
		printf("load file fail\n");
		return;
	}

	std::string token;
	token = ",";

	split_to_set(content, token, words_set);
}

int test0(std::set<std::string> &words_set, std::vector<std::string> &words_vec)
{
	printf("\n---- test0 ----\n");

	double time1, time2;
	bool is_find = false;
	std::string input = "";

	time1 = get_time_ms();
	for (size_t i = 0; i < words_vec.size(); i++)
	{
		input = words_vec[i];
		for (auto iter = words_set.begin(); iter != words_set.end(); iter++)
		{
			size_t pos = input.find(*iter);
			if (pos != std::string::npos)
			{
				is_find = true;
				break;
			}
		}
		if (is_find == false)
		{
			printf("test0:find error, input=[%s]\n", input.c_str());
			break;
		}
	}
	time2 = get_time_ms();
	printf("test0:find all time=%lf\n", time2-time1);

	int head_size = 0;

	// add f at front 
	head_size = 100;
	time1 = get_time_ms();
	for (size_t i = 0; i < words_vec.size(); i++)
	{
		input = std::string(head_size, 'f') + words_vec[i];
		for (auto iter = words_set.begin(); iter != words_set.end(); iter++)
		{
			size_t pos = input.find(*iter);
			if (pos != std::string::npos)
			{
				is_find = true;
				break;
			}
		}
		if (is_find == false)
		{
			printf("test0:find error, input=[%s]\n", input.c_str());
			break;
		}
	}
	time2 = get_time_ms();
	printf("test1:find f front%d time=%lf\n", head_size, time2-time1);

	// add f at back 
	head_size = 100;
	time1 = get_time_ms();
	for (size_t i = 0; i < words_vec.size(); i++)
	{
		input = words_vec[i] + std::string(head_size, 'f');
		for (auto iter = words_set.begin(); iter != words_set.end(); iter++)
		{
			size_t pos = input.find(*iter);
			if (pos != std::string::npos)
			{
				is_find = true;
				break;
			}
		}
		if (is_find == false)
		{
			printf("test0:find error, input=[%s]\n", input.c_str());
			break;
		}
	}
	time2 = get_time_ms();
	printf("test1:find f back%d time=%lf\n", head_size, time2-time1);

	// news
	is_find = false;
	time1 = get_time_ms();
	for (auto iter = words_set.begin(); iter != words_set.end(); iter++)
	{
		size_t pos = news.find(*iter);
		if (pos != std::string::npos)
		{
			is_find = true;
			break;
		}
	}
	printf("test0:find news is_find=%d size=%lu\n", is_find, news.size());
	time2 = get_time_ms();
	printf("test0:find news time=%lf\n", time2-time1);

	return 0;
}

int test1(sensitive_filter_t &filter, std::vector<std::string> &words_vec)
{
	printf("\n---- test1 ----\n");

	double time1, time2;
	bool is_find = false;
	std::string input = "";

	time1 = get_time_ms();
	for (size_t i = 0; i < words_vec.size(); i++)
	{
		input = words_vec[i];
		is_find = filter.find(input);
		if (is_find == false)
		{
			printf("error, input=[%s]\n", input.c_str());
			break;
		}
	}
	time2 = get_time_ms();
	printf("test1:find all time=%lf\n", time2-time1);

	int head_size = 0;

	// add f at front 
	head_size = 100;
	time1 = get_time_ms();
	for (size_t i = 0; i < words_vec.size(); i++)
	{
		input = std::string(head_size, 'f') + words_vec[i];
		is_find = filter.find(input);
		if (is_find == false)
		{
			printf("test1:find f front%d error, input=[%s]\n", head_size, input.c_str());
			break;
		}
	}
	time2 = get_time_ms();
	printf("test1:find f front%d time=%lf\n", head_size, time2-time1);

	// add f at back 
	head_size = 100;
	time1 = get_time_ms();
	for (size_t i = 0; i < words_vec.size(); i++)
	{
		input = words_vec[i] + std::string(head_size, 'f');
		is_find = filter.find(input);
		if (is_find == false)
		{
			printf("test1:find f back%d error, input=[%s]\n", head_size, input.c_str());
			break;
		}
	}
	time2 = get_time_ms();
	printf("test1:find f back%d time=%lf\n", head_size, time2-time1);

	
	time1 = get_time_ms();
	is_find = filter.find(news);
	printf("test1:find news is_find=%d size=%lu\n", is_find, news.size());
	time2 = get_time_ms();
	printf("test1:find news time=%lf\n", time2-time1);

	return 0;
}

int test2(sensitive_filter_t &filter, std::vector<std::string> &words_vec)
{
	printf("\n---- test2 ----\n");

	double time1, time2;
	bool is_find = false;
	std::string input = "";

	time1 = get_time_ms();
	for (size_t i = 0; i < words_vec.size(); i++)
	{
		input = words_vec[i];
		is_find = filter.find2(input);
		if (is_find == false)
		{
			printf("test2:find2 all error, input=[%s]\n", input.c_str());
			break;
		}
	}
	time2 = get_time_ms();
	printf("test2:find2 all time=%lf\n", time2-time1);

	int head_size = 0;

	// add f at front 
	head_size = 100;
	time1 = get_time_ms();
	for (size_t i = 0; i < words_vec.size(); i++)
	{
		input = std::string(head_size, 'f') + words_vec[i];
		is_find = filter.find2(input);
		if (is_find == false)
		{
			printf("test2:find2 f front%d error, input=[%s]\n", head_size, input.c_str());
			break;
		}
	}
	time2 = get_time_ms();
	printf("test2:find2 f front%d time=%lf\n", head_size, time2-time1);

	// add f at back 
	head_size = 100;
	time1 = get_time_ms();
	for (size_t i = 0; i < words_vec.size(); i++)
	{
		input = words_vec[i] + std::string(head_size, 'f');
		is_find = filter.find2(input);
		if (is_find == false)
		{
			printf("test2:find2 f back%d error, input=[%s]\n", head_size, input.c_str());
			break;
		}
	}
	time2 = get_time_ms();
	printf("test2:find2 f back%d time=%lf\n", head_size, time2-time1);

	
	time1 = get_time_ms();
	is_find = filter.find2(news);
	printf("test2:find2 news is_find=%d size=%lu\n", is_find, news.size());
	time2 = get_time_ms();
	printf("test2:find2 news time=%lf\n", time2-time1);

	return 0;
}

int test3(sensitive_filter_t &filter, std::vector<std::string> &words_vec)
{
	printf("\n---- test3 ----\n");

	double time1, time2;
	bool is_find = false;
	std::string input = "";

	time1 = get_time_ms();
	for (size_t i = 0; i < words_vec.size(); i++)
	{
		input = words_vec[i];
		is_find = filter.find3(input);
		if (is_find == false)
		{
			printf("test3:find3 all error, input=[%s]\n", input.c_str());
			break;
		}
	}
	time2 = get_time_ms();
	printf("test3:find3 all time=%lf\n", time2-time1);

	int head_size = 0;

	// add f at front 
	head_size = 100;
	time1 = get_time_ms();
	for (size_t i = 0; i < words_vec.size(); i++)
	{
		input = std::string(head_size, 'f') + words_vec[i];
		is_find = filter.find3(input);
		if (is_find == false)
		{
			printf("test3:find3 f front%d error, input=[%s]\n", head_size, input.c_str());
			break;
		}
	}
	time2 = get_time_ms();
	printf("test3:find3 f front%d time=%lf\n", head_size, time2-time1);

	// add f at back 
	head_size = 100;
	time1 = get_time_ms();
	for (size_t i = 0; i < words_vec.size(); i++)
	{
		input = words_vec[i] + std::string(head_size, 'f');
		is_find = filter.find3(input);
		if (is_find == false)
		{
			printf("test3:find3 f back%d error, input=[%s]\n", head_size, input.c_str());
			break;
		}
	}
	time2 = get_time_ms();
	printf("test3:find3 f back%d time=%lf\n", head_size, time2-time1);

	
	time1 = get_time_ms();
	is_find = filter.find3(news);
	printf("test3:find3 news is_find=%d size=%lu\n", is_find, news.size());
	time2 = get_time_ms();
	printf("test3:find3 news time=%lf\n", time2-time1);

	return 0;
}

int test4(sensitive_filter_t &filter, std::vector<std::string> &words_vec)
{
	printf("\n---- test4 ----\n");

	double time1, time2;
	bool is_replace = false;
	std::string input = "";
	std::string output = "";

	time1 = get_time_ms();
	for (size_t i = 0; i < words_vec.size(); i++)
	{
		input = words_vec[i];
		is_replace = filter.replace(input);
		if (is_replace == false)
		{
			printf("replace error, input=[%s]\n", input.c_str());
			break;
		}
		// printf("input=[%s]\n", input.c_str());

		output = std::string(input.size(), '*');
		if (input != output)
		{
			printf("output error, input=[%s] output=[%s]\n", input.c_str(), output.c_str());
			break;
		}
	}
	time2 = get_time_ms();
	printf("test4:replace all time=%lf\n", time2-time1);

	int head_size = 0;

	// add f at front
	head_size = 100;
	time1 = get_time_ms();
	for (size_t i = 0; i < words_vec.size(); i++)
	{
		input = std::string(head_size, 'f') + words_vec[i];
		is_replace = filter.replace(input);
		if (is_replace == false)
		{
			printf("replace error, input=[%s]\n", input.c_str());
			break;
		}
		// printf("input=[%s]\n", input.c_str());

		output = std::string(head_size, 'f') + std::string(input.size() - head_size, '*');
		if (input != output)
		{
			printf("output error, input=[%s] output=[%s]\n", input.c_str(), output.c_str());
			break;
		}
	}
	time2 = get_time_ms();
	printf("test4:replace f front%d time=%lf\n", head_size, time2-time1);

	// add f at back
	head_size = 100;
	time1 = get_time_ms();
	for (size_t i = 0; i < words_vec.size(); i++)
	{
		input = words_vec[i] + std::string(head_size, 'f');
		is_replace = filter.replace(input);
		if (is_replace == false)
		{
			printf("replace error, input=[%s]\n", input.c_str());
			break;
		}
		// printf("input=[%s]\n", input.c_str());

		output = std::string(input.size() - 1, '*');
		output = output + "f";
		output = std::string(input.size() - head_size, '*') + std::string(head_size, 'f');
		if (input != output)
		{
			printf("output error, input=[%s] output=[%s]\n", input.c_str(), output.c_str());
			break;
		}
	}
	time2 = get_time_ms();
	printf("test4:replace f back%d time=%lf\n", head_size, time2-time1);

	// replace news
	input = news;
	time1 = get_time_ms();
	is_replace = filter.replace(input);
	printf("test4:replace news is_replace=%d size=%lu\n", is_replace, input.size());
	time2 = get_time_ms();
	// printf("test4:news=[%s]\n", input.c_str());
	printf("test4:replace news time=%lf\n", time2-time1);
	return 0;
}

int main(int argc, char **argv)
{
	std::set<std::string> words_set;
	fill_words(words_set);
	std::vector<std::string> words_vec(words_set.size());
	std::copy(words_set.begin(), words_set.end(), words_vec.begin());
	
	printf("words_set.size()=%lu\n", words_set.size());
	printf("words_vec.size()=%lu\n", words_vec.size());
	/*
	for (auto iter = words_set.begin(); iter != words_set.end(); iter++)
	{
		printf("[%s]\n", (*iter).c_str());
	}
	printf("\n");
	*/

	printf("\n---- load ----\n");
	double time1, time2;

	time1 = get_time_ms();
	sensitive_filter_t filter;
	filter.load(words_set);
	time2 = get_time_ms();
	printf("load time=%lf\n", time2-time1);

	time1 = get_time_ms();
	sensitive_filter_t filter2;
	filter2.load2(words_set);
	time2 = get_time_ms();
	printf("load2 time=%lf\n", time2-time1);

	time1 = get_time_ms();
	sensitive_filter_t filter3;
	filter3.load3(words_set);
	time2 = get_time_ms();
	printf("load3 time=%lf\n", time2-time1);

	test0(words_set, words_vec);
	test1(filter2, words_vec); // find, substr find
	test2(filter2, words_vec); // find2, pointer find
	test3(filter3, words_vec); // find3, refrence find

	test4(filter2, words_vec); // replace


	return 0;
}
