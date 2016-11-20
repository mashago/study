#include <stdio.h>
#include "alpha_list.h"


int main(int argc, char *argv[])
{
	int ret;
	// printf("hello world\n");
	// printf("sizeof(Alpha)=%lu\n", sizeof(Alpha));

	Alpha alist;
	init_alpha_list(&alist);

	Item item1;
	sprintf(item1.name, "ka");

	ret = add_alpha_item(item1, &alist);
	printf("add_alpha_item:item1.name=[%s] ret=%d\n", item1.name, ret);

	Item *result = NULL;
	result = get_alpha_item(item1, &alist);
	if (result == NULL) {
		printf("get_alpha_item:ka_not_found\n");
	} else {
		printf("get_alpha_item:ka_found_result.name=[%s]\n", result->name);
	}
		

	Item item2;
	sprintf(item2.name, "ca");
	result = get_alpha_item(item2, &alist);
	if (result == NULL) {
		printf("get_alpha_item:ca_not_found\n");
	} else {
		printf("get_alpha_item:ca_found_result.name=[%s]\n", result->name);
	}

	Item item3;
	sprintf(item3.name, "c_a");

	ret = add_alpha_item(item3, &alist);
	printf("add_alpha_item:item3.name=[%s] ret=%d\n", item3.name, ret);

	result = get_alpha_item(item3, &alist);
	if (result == NULL) {
		printf("get_alpha_item:ca_not_found\n");
	} else {
		printf("get_alpha_item:ca_found_result.name=[%s]\n", result->name);
	}


	return 0;
}
