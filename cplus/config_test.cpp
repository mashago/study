#include "Config.h"

int main(int argc, char **argv)
{
	printf("hello config\n");

	Config config;
	int ret = config.LoadConfig("./test.conf");
	printf("ret=%d\n", ret);

	int id = config.GetInt("id", 5);
	const char *path = config.GetString("path", "./");
	int empty = config.GetInt("empty", -1);
	const char *writer = config.GetString("writer", "no writer");
	int date = config.GetInt("date");

	printf("id=%d path=%s empty=%d writer=%s date=%d\n", id, path, empty, writer, date);




	return 0;
}
