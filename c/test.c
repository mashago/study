#ifdef __linux__
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <strings.h>
#include <math.h>

int test0() {
	return 0;
}

int t1(int x, int y) {
	
	printf("test1:x=%d y=%d\n", x, y);

	return 0;
}
int test1()
{
	t1(1, 2);
	return 0;
}

// trim right
int t2(char *str) {
	int len = strlen(str);
	printf("test2:len=%d str=[%s]\n", len, str);

	if (len == 0) {
		return 0;
	}

	for (int i=len-1; i>=0; i--) {
		if (str[i] == ' ') {
			str[i] = 0;
			break;
		}
	}
	
	len = strlen(str);
	printf("test2:len=%d str=[%s]\n", len, str);
	return 0;
}

int test2()
{
	char aa[20];
	sprintf(aa, "aaaa");
	t2(aa);
	return 0;
}

// trim left
int t3(char *str) {
	char *ptr;
	ptr = str;
	int len = strlen(str);
	printf("test3:len=%d str=[%s]\n", len, str);

	if (len == 0) {
		return 0;
	}

	for (int i=0; i<len; i++) {
		if (str[i] != ' ') {
			break;
		}
		ptr++;
	}
	// strcpy(str, ptr);
	sprintf(str, "%s", ptr);

	len = strlen(str);
	printf("test3:len=%d str=[%s]\n", len, str);
	return 0;
}

int test3()
{
	char aa[20];
	sprintf(aa, "aaaa");
	t3(aa);
	return 0;
}

int t4(char *str)
{
	int len = 0;
	len = strlen(str);
	printf("test4:len = %d\n", len);

	return 0;
}

int test4()
{
	char aa[20];
	sprintf(aa, "aaaa");
	t4(aa);
	return 0;
}

int test5()
{
	int a = 0;
	if (1) {
		a = 2;
	}
	printf("test5:a = %d\n", a);

	return 0;
}


int test6()
{
	int a = 0;
	a |= 1;
	printf("test6:a1 = %d\n", a);

	a = 2;
	a |= 1;
	printf("test6:a1 = %d\n", a);

	a = 3;
	a |= 1;
	printf("test6:a1 = %d\n", a);


	a = 0;
	a |= 2;
	printf("test6:a2 = %d\n", a);

	a = 1;
	a |= 2;
	printf("test6:a2 = %d\n", a);

	a = 2;
	a |= 2;
	printf("test6:a2 = %d\n", a);

	a = 3;
	a |= 2;
	printf("test6:a2 = %d\n", a);

	return 0;
}

// test va_start(), vsnprintf(), va_end()
int t7(const char *fmt, ...)
{
	int len = 0;
	const int MAX_BUFFER = 10;
	char buffer[MAX_BUFFER];
	va_list argptr;

	va_start(argptr, fmt);
	len = vsnprintf(buffer, MAX_BUFFER, fmt, argptr);
	va_end(argptr);

	printf("len=%d buffer=%s\n", len, buffer);

	return 0;
}

int test7()
{
	const char * fmt = "%s %s %s";
	t7(fmt, "aaa", "bbb", "ccc");
	return 0;
}

int test8()
{
	const char * in_buffer = "abcdefghi jklmn";
	char name[50];
	bzero(name, 50);
	sscanf(in_buffer, "%s", name);
	printf("name1=%s\n", name);

	bzero(name, 50);
	sscanf(in_buffer, "%5s", name);
	printf("name2=%s\n", name);

	return 0;
}

int test9()
{
	const char * in_buffer = "abcdefghi jklmn";
	char out_buffer[100];
	bzero(out_buffer, 100);

	sprintf(out_buffer, "%s", in_buffer);
	printf("out_buffer=[%s]\n", out_buffer);
	return 0;
}

int t10(int card_list[4][5])
{
	for (int i=0; i<4; i++) {
		for (int j=0; j<5; j++) {
			card_list[i][j] = i-j;
		}
	}

	for (int i=0; i<4; i++) {
		for (int j=0; j<5; j++) {
			printf("test10:card_list[%d][%d]=%d\n", i, j, card_list[i][j]);
		}
	}

	return 0;
}
int test10()
{
	int card_list[4][5];
	for (int i=0; i<4; i++) {
		for (int j=0; j<5; j++) {
			card_list[i][j] = i+j;
		}
	}

	for (int i=0; i<4; i++) {
		for (int j=0; j<5; j++) {
			printf("before:card_list[%d][%d]=%d\n", i, j, card_list[i][j]);
		}
	}
	t10(card_list);

	return 0;
}

typedef struct _design_mission_t {
	int mid;
	int mtype;
} design_mission_t;


design_mission_t t11(design_mission_t * dlist, int mid) {
	// design_mission_t & dmis = *(dlist + mid);
	design_mission_t dmis = dlist[mid];
	printf("dmis.mid=%d mtype=%d\n", dmis.mid, dmis.mtype);
	dmis.mid = 33;
	dmis.mtype = 44;

	return dmis;
}

int test11()
{
	design_mission_t dlist[20];
	bzero(dlist, sizeof(dlist));
	for (int i=0;i<20;i++) {
		dlist[i].mid = i;
		dlist[i].mtype = 20-i;
	}
	design_mission_t mis = t11(dlist, 2);
	printf("mis.mid=%d, mtype=%d\n", mis.mid, mis.mtype);
	return 0;
}

int test12()
{
	int a = 2;
	int b = 0;
	int times = 10;
	b = pow(a, times);

	printf("test12:b=%d times=%d\n", b, times);

	return 0;
}

typedef struct _player_t {
	int eid;
	int round;
} player_t;

int test13()
{
	int size = 8;
	player_t player_list[size];
	bzero(&player_list, size);

	player_list[0].eid = 1;
	player_list[0].round = 1;
	player_list[1].eid = 2;
	player_list[1].round = 1;
	player_list[2].eid = 3;
	player_list[2].round = 1;
	player_list[3].eid = -4;
	player_list[3].round = 1;

	player_list[4].eid = 1;
	player_list[4].round = 2;
	player_list[5].eid = 2;
	player_list[5].round = 2;

	player_list[6].eid = -1;
	player_list[6].round = 3;
	player_list[7].eid = -1;
	player_list[7].round = 3;

	int round = -1;
	int not_ready_round = 0;
	for (int i = size-1;i >= 0; i--) {
		player_t * player = player_list + i;
		if (player->eid <= 0) {
			if (player->round < round && round > 0) {
				printf("test13:player_list_bug %d %d\n", player->round, round);
				return 0;
			}
			not_ready_round = player->round;
			round = -1;
			continue;
		}
		if (player->round == not_ready_round) {
			continue;
		}

		if (player->round > round) {
			round = player->round;
		}
	}

	printf("round = %d\n", round);

	return 0;
}

#define MAX_MATCH	10
typedef struct _match_t {
	long match_id;
} match_t;
match_t g_match_list[MAX_MATCH];

int get_match1(match_t ** pmatch)
{
	bzero(g_match_list, sizeof(g_match_list));
	g_match_list[0].match_id = 112;

	for (int i=0;i<MAX_MATCH;i++) {
		match_t * match=g_match_list + i;
		if (match->match_id != 0) {
			*pmatch = match;
			return 0;
		}
	}

	return 0;
}

int test14()
{
	match_t * pmatch = NULL;
	get_match1(&pmatch);
	if (pmatch != NULL) {
		printf("test14 match_id = %ld\n", pmatch->match_id);
	} else {
		printf("test14 == NULL\n");
	}

	return 0;
}

match_t * get_match2()
{
	bzero(g_match_list, sizeof(g_match_list));
	g_match_list[0].match_id = 112;

	for (int i=0;i<MAX_MATCH;i++) {
		match_t * match=g_match_list + i;
		if (match->match_id != 0) {
			return match;
		}
	}
	return NULL;
}

int test15()
{
	match_t * pmatch = NULL;
	pmatch = get_match2();
	if (pmatch != NULL) {
		printf("test15 match_id=%ld\n", pmatch->match_id);
	} else {
		printf("test15 == NULL\n");
	}
	return 0;
}


#define PRINT_HELLO printf("hello world\n")
#define PRINT_HELLO_NUM(num, ...)  do { printf("hello world num=%d\n", num); } while(0)
#define PRINT_HELLO_RETURN do { printf("hello world return\n"); return 0; } while(0)

int test16()
{

#ifdef __linux__
	printf("this is for linux\n");
#elif defined __APPLE__
	printf("this is for apple\n");
#else
	printf("this is for other\n");
#endif

	PRINT_HELLO;
	PRINT_HELLO_NUM(20);
	PRINT_HELLO_RETURN;

	return 0;
}

int test17()
{

	int running;
	int MAX_NAME = 100;
	int NAME_SIZE = 30; 
	char name[MAX_NAME][NAME_SIZE];
	bzero(name, sizeof(name));
	int count;

	FILE *pfile = fopen("name_boy.txt", "r");
	if (pfile == NULL) {
		printf("test17:pfile_null\n");
		return -5;
	}

	count = 0;
	running = 1;
	while (running) {
		if (NULL == (fgets(name[count], NAME_SIZE, pfile))) {
			running = 0;
			break;
		}

		if ((name[count][strlen(name[count])-1]) == '\n' 
		&& strlen(name[count]) == 1) {
			continue;
		}

		if (name[count][strlen(name[count])-1] == '\n') {
			name[count][strlen(name[count])-1] = '\0';
		}

		printf("test17:name[%d]=%s\n", count, name[count]);

		count ++;
		if (count >= MAX_NAME) {
			running = 0;
			break;
		}
	}
	fclose(pfile);

	return 0;
}

int test18()
{

	int running;
	int MAX_NAME = 2000;
	int NAME_SIZE = 14; 
	char name_xing[MAX_NAME][NAME_SIZE];
	char name_boy[MAX_NAME][NAME_SIZE];
	char name_girl[MAX_NAME][NAME_SIZE];
	bzero(name_xing, sizeof(name_xing));
	bzero(name_boy, sizeof(name_boy));
	bzero(name_girl, sizeof(name_girl));
	char fmt[10];
	sprintf(fmt, "%%%ds", NAME_SIZE);
	int count;
	FILE *pfile;

	// name_xing
	pfile = fopen("name_xing.txt", "r");
	if (pfile == NULL) {
		printf("test18:pfile_null\n");
		return -5;
	}

	count = 0;
	running = 1;
	while (!feof(pfile)) {

		fscanf(pfile, fmt, name_xing[count]);

		if ((name_xing[count][strlen(name_xing[count])-1]) == '\n' 
		&& strlen(name_xing[count]) == 1) {
			continue;
		}

		printf("test18:name_xing[%d]=%s\n", count, name_xing[count]);

		if (count + 1 >= MAX_NAME) {
			running = 0;
			break;
		}

		count ++;
	}
	fclose(pfile);


	// name_boy
	pfile = fopen("name_boy.txt", "r");
	if (pfile == NULL) {
		printf("test18:pfile_null\n");
		return -5;
	}

	count = 0;
	running = 1;
	while (!feof(pfile)) {

		fscanf(pfile, fmt, name_boy[count]);

		if ((name_boy[count][strlen(name_boy[count])-1]) == '\n' 
		&& strlen(name_boy[count]) == 1) {
			continue;
		}

		printf("test18:name_boy[%d]=%s\n", count, name_boy[count]);

		if (count + 1 >= MAX_NAME) {
			running = 0;
			break;
		}
		count ++;
	}
	fclose(pfile);


	// name_girl
	pfile = fopen("name_girl.txt", "r");
	if (pfile == NULL) {
		printf("test18:pfile_null\n");
		return -5;
	}

	count = 0;
	running = 1;
	while (!feof(pfile)) {

		fscanf(pfile, fmt, name_girl[count]);

		if ((name_girl[count][strlen(name_girl[count])-1]) == '\n' 
		&& strlen(name_girl[count]) == 1) {
			continue;
		}

		printf("test18:name_girl[%d]=%s\n", count, name_girl[count]);

		if (count + 1 >= MAX_NAME) {
			running = 0;
			break;
		}
		count ++;
	}
	fclose(pfile);


	// get alias = name_xing + name_boy
	srandom(time(NULL));

	char alias[30];
	int r1, r2;
	count = 0;
	while (1) {
		r1 = random() % MAX_NAME;
		printf("test18:r1=%d\n", r1);
		if (name_xing[r1][strlen(name_xing[r1])-1] != '\0') {
			break;
		}
		if (count > MAX_NAME) {
			printf("test18:BUGBUG_no_xing\n");
		}
		count ++;
	}

	count = 0;
	while (1) {
		r2 = random() % MAX_NAME;
		printf("test18:r2=%d\n", r2);
		if (name_boy[r2][strlen(name_boy[r2])-1] != '\0') {
			break;
		}
		if (count > MAX_NAME) {
			printf("test18:BUGBUG_no_boy\n");
		}
		count ++;
	}
	sprintf(alias, "%s%s", name_xing[r1], name_boy[r2]);
	printf("test18:alias=%s\n", alias);

	return 0;
}

int test19()
{

	const char * key = "471028";
	size_t keylen = strlen(key);

	// char value[] = "hello";
	for (int i=0; i<5; i++) {
		
		
	}

	for (int i = 0; i < keylen; i++) {
		printf("key[%d]=%d\n", i, key[i]);
	}
	return 0;
}

typedef int (*testcase_t) ();
testcase_t test_list[] = {
	test0
,	test1
,	test2
,	test3
,	test4
,	test5
,	test6
,	test7
,	test8
,	test9
,	test10
,	test11
,	test12
,	test13
,	test14
,	test15
,	test16
,	test17
,	test18
,	test19
};


int main(int argc, char* argv[]) {

	int ret;
	int testcase;
	int maxcase;
	maxcase = sizeof(test_list) / sizeof(testcase_t);
	testcase = maxcase - 1;
	if (argc > 1) {
		testcase = atoi(argv[1]);
		if (testcase < 0 || testcase >= maxcase) {
			testcase = maxcase - 1;
		}
	}
	ret = test_list[testcase]();

	printf("RET %d\n", ret);
		
	return 0;

}
