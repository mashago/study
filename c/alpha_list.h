
#define ALPHA_TABLE_SIZE	26
#define ALPHA_NAME_SIZE		40

struct word {
	char name[ALPHA_NAME_SIZE];
};

typedef struct word Item;

typedef struct alpha{
	Item item;
	struct alpha * alpha_table[ALPHA_TABLE_SIZE];
} Alpha;

int init_alpha_list(Alpha * list);
int add_alpha_item(Item item, Alpha *list);
Item * get_alpha_item(Item item, Alpha *list);

