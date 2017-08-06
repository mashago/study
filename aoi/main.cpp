
#include <stdio.h>
#include "aoi.h"

int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);

	uint32_t aoi_x_len = 10;
	uint32_t aoi_y_len = 10;

	{
	AOI *aoi = new AOI(aoi_x_len, aoi_y_len);
	uint64_t entity_id = 1;
	uint64_t out_aoi_id = 0;
	aoi->add(1, 2, true, true, out_aoi_id, entity_id);
	aoi->print_x_objs();
	aoi->print_all_events();
	printf("\n");

	entity_id = 2;
	out_aoi_id = 0;
	aoi->add(2, 3, true, true, out_aoi_id, entity_id);
	aoi->print_x_objs();
	aoi->print_all_events();
	printf("\n");

	aoi->move(out_aoi_id, 3, 4);
	aoi->print_x_objs();
	aoi->print_all_events();
	printf("\n");

	aoi->move(out_aoi_id, 3, 6);
	aoi->print_x_objs();
	aoi->print_all_events();
	printf("\n");

	aoi->move(out_aoi_id, 13, 14);
	aoi->print_x_objs();
	aoi->print_all_events();

	delete aoi;
	}


	getchar();
	return 0;
}

