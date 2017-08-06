
#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <set>
#include <utility>
#include "aoi.h"

#ifdef WIN32
inline void gettimeofday(struct timeval *tp, void *ptr)
{
	uint64_t  intervals;
	FILETIME  ft;

	GetSystemTimeAsFileTime(&ft);

	intervals = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
	intervals -= 116444736000000000;

	tp->tv_sec = (long)(intervals / 10000000);
	tp->tv_usec = (long)((intervals % 10000000) / 10);
}
#endif

double get_time_ms()
{
	struct timeval tv;    
	gettimeofday(&tv, NULL);
	double time_ms = tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
	return time_ms;
}


int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);

	srand(time(NULL));

	uint32_t aoi_x_len = 10;
	uint32_t aoi_y_len = 10;
	int ENTITY_NUM = 100;

	{
	double start_time = get_time_ms();
	AOI *aoi = new AOI(aoi_x_len, aoi_y_len);
	uint64_t entity_id = 1;
	uint64_t out_aoi_id = 0;
	std::set<uint64_t> aoi_id_set;
	for (int i = 0; i < ENTITY_NUM; ++i)
	{
		int x = rand() % 100;
		int y = rand() % 100;
		aoi->add(x, y, true, true, out_aoi_id, entity_id++);
		aoi_id_set.insert(out_aoi_id);
	}

	for (uint64_t aoi_id : aoi_id_set)
	{
		int x = rand() % 100;
		int y = rand() % 100;
		aoi->move(aoi_id, x, y);
	}

	for (uint64_t aoi_id : aoi_id_set)
	{
		aoi->remove(aoi_id);
	}
	// aoi->print_x_objs();
	// aoi->print_all_events();
	aoi->clear_events();

	delete aoi;
	double end_time = get_time_ms();
	printf("time offset = %lfms\n", end_time - start_time);
	}


	getchar();
	return 0;
}

