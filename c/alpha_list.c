#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <strings.h>
#include "alpha_list.h"


int init_alpha_list(Alpha *list)
{
	bzero(list, sizeof(Alpha));
	return 0;	
}

static int get_alpha_index(const char ch)
{
	const int a_num = 97;
	const int z_num = 122;
	if (ch < a_num || ch > z_num) {
		return -1;
	}
	return ch-'a';
}

int add_alpha_item(Item item, Alpha *list)
{

	if (list == NULL) {
		return -1;
	}

	int len = strlen(item.name);
	if (len == 0) {
		return -1;
	}

	for (int i=0; i<len; i++) {
		if (get_alpha_index(item.name[i]) < 0) {
			return -1;
		}
	}

	Alpha *head;
	head = list;
	for (int i=0; i<len; i++) {
		int index = get_alpha_index(item.name[i]);
		// printf("index=%d\n", index);

		if (head->alpha_table[index] == NULL) {
			// printf("alpha null\n");
			head->alpha_table[index] = (Alpha *) malloc(sizeof(Alpha));
			if (head->alpha_table[index] == NULL) {
				return -1;
			}
			bzero(head->alpha_table[index], sizeof(Alpha));
		}
		head = head->alpha_table[index];

		if (i == len - 1) {
			head->item = item;
			// printf("put item, item.name=[%s]\n", head->item.name);
		}
	}

	return 0;	
}

Item * get_alpha_item(Item item, Alpha *list)
{
	if (list == NULL) {
		return NULL;
	}

	int len = strlen(item.name);
	if (len == 0) {
		return NULL;
	}

	const char *name = item.name;
	Alpha *last_node = list;
	while (*name != '\0') {
		int index = get_alpha_index(*name);
		if (index < 0) {
			return NULL;
		}
		last_node = last_node->alpha_table[index];	
		if (last_node == NULL) {
			return NULL;
		}
		name++;
	}

	return &last_node->item;
}
