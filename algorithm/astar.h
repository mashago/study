#include <list>

class MazePath
{
public:
	int x;
	int y;
	MazePath(int x, int y);
};

std::list<MazePath> MazeFindPath(int *maze, int max_x, int max_y, MazePath start, MazePath end, bool ignore_conner);
