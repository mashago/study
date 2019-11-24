#include <list>
#define MAP_SIZE 12

class MazePoint;

class MazePoint
{
public:
	MazePoint(int x, int y);
	void CalcF();
	int F;
	int G;
	int H;
	int X;
	int Y;
	MazePoint *parent;
};

class MazePath
{
public:
	int x;
	int y;
	MazePath(int x, int y);
};

class Maze
{
public:
	Maze(int maze[MAP_SIZE][MAP_SIZE]);
	~Maze();
	std::list<MazePath> FindPath(MazePoint *start, MazePoint *end, bool ignore_conner);
	
private:
	int (*maze)[MAP_SIZE];
	std::list<MazePoint *> close_list;
	std::list<MazePoint *> open_list;
	std::list<MazePoint *> point_list;

	void FoundPoint(MazePoint *tempStart, MazePoint *point);
	void NotFoundPoint(MazePoint *tempStart, MazePoint *end, MazePoint *point);
	int CalcG(MazePoint *start, MazePoint *point);
	int CalcH(MazePoint *end, MazePoint *point);
	std::list<MazePoint *> SurrroundPoints(MazePoint *point, bool ignore_conner);
	bool CanReach(int x, int y);
	bool CanReach(MazePoint *start, int x, int y, bool ignore_conner);
	MazePoint * NewPoint(int x, int y);
};
