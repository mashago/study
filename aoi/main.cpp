
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
#include "aoi_x.h"

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
	return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
}

struct Entity
{
	uint64_t entity_id;
	uint64_t aoi_id;
	int x;
	int y;
	int move_x;
	int move_y;
};

int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);

	srand(time(NULL));

	uint32_t aoi_x_len = 50;
	uint32_t aoi_y_len = 50;
	uint32_t x_range = 1000;
	uint32_t y_range = 1000;
	int ENTITY_NUM = 1000;

	uint64_t entity_id = 1;
	std::list<Entity> entity_list;
	for (int i = 0; i < ENTITY_NUM; ++i)
	{
		Entity e;
		e.entity_id = entity_id++;
		e.x = rand() % x_range;
		e.y = rand() % y_range;
		e.move_x = rand() % x_range;
		e.move_y = rand() % y_range;
		entity_list.push_back(e);
	}

	{
	using namespace AOI_X_SPACE;
	double start_time = get_time_ms();
	AOI *aoi = new AOI(aoi_x_len, aoi_y_len);

	double add_start_time = get_time_ms();
	for (Entity &e : entity_list)
	{
		aoi->add(e.x, e.y, true, true, e.aoi_id, e.entity_id);
	}
	printf("insert time = %lfms\n", get_time_ms() - add_start_time);
	// aoi->print_x_objs();
	// aoi->print_y_objs();
	// aoi->print_all_events();
	// printf("***************************\n");

	double move_start_time = get_time_ms();
	for (Entity &e : entity_list)
	{
		aoi->move(e.aoi_id, e.move_x, e.move_y);
	}
	printf("move time = %lfms\n", get_time_ms() - move_start_time);
	// aoi->print_x_objs();
	// aoi->print_y_objs();
	// aoi->print_all_events();
	// printf("***************************\n");
	printf("event num=%zu\n", aoi->get_all_events().size());

	double remove_start_time = get_time_ms();
	for (Entity &e : entity_list)
	{
		aoi->remove(e.aoi_id);
	}
	printf("remove time = %lfms\n", get_time_ms() - remove_start_time);
	// aoi->print_x_objs();
	// aoi->print_y_objs();
	// aoi->print_all_events();
	aoi->clear_events();

	delete aoi;
	double end_time = get_time_ms();
	printf("time offset = %lfms\n", end_time - start_time);
	}
	printf("\n");
	{
	using namespace AOI_ORI_SPACE;
	double start_time = get_time_ms();
	AOI *aoi = new AOI(aoi_x_len, aoi_y_len);

	double add_start_time = get_time_ms();
	for (Entity &e : entity_list)
	{
		aoi->add(e.x, e.y, true, true, e.aoi_id, e.entity_id);
	}
	printf("insert time = %lfms\n", get_time_ms() - add_start_time);
	// aoi->print_x_objs();
	// aoi->print_y_objs();
	// aoi->print_all_events();
	// printf("***************************\n");

	double move_start_time = get_time_ms();
	for (Entity &e : entity_list)
	{
		aoi->move(e.aoi_id, e.move_x, e.move_y);
	}
	printf("move time = %lfms\n", get_time_ms() - move_start_time);
	// aoi->print_x_objs();
	// aoi->print_y_objs();
	// aoi->print_all_events();
	// printf("***************************\n");
	printf("event num=%zu\n", aoi->get_all_events().size());

	double remove_start_time = get_time_ms();
	for (Entity &e : entity_list)
	{
		aoi->remove(e.aoi_id);
	}
	printf("remove time = %lfms\n", get_time_ms() - remove_start_time);
	// aoi->print_x_objs();
	// aoi->print_y_objs();
	// aoi->print_all_events();
	aoi->clear_events();

	delete aoi;
	double end_time = get_time_ms();
	printf("time offset = %lfms\n", end_time - start_time);
	}





	// getchar();
	return 0;
}

