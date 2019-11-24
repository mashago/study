
#include <stdlib.h>
#include "astar.h"

#define STEP 	10
#define OBLIQUE 14

Maze::Maze(int m[MAP_SIZE][MAP_SIZE])
{
	maze = new int[MAP_SIZE][MAP_SIZE];
	for (int i=0; i<MAP_SIZE; i++)
	{
		for (int j=0; j<MAP_SIZE; j++)
		{
			maze[i][j] = m[i][j];
		}
	}
}

Maze::~Maze()
{
	delete []maze;
	for (auto iter = point_list.begin(); iter != point_list.end(); iter++)
	{
		delete *iter;
	}
}

bool min_f(MazePoint *p1, MazePoint *p2)
{
	return p1->F <= p2->F;
}

MazePoint * get_in_list(std::list<MazePoint *> l, MazePoint *p)
{
	for (auto iter = l.begin(); iter != l.end(); ++iter)
	{
		if ((*iter)->X == p->X && (*iter)->Y == p->Y)
		{
			return *iter;
		}
	}
	return nullptr;
}

bool exists_in_list(std::list<MazePoint *> l, int x, int y)
{
	for (auto iter = l.begin(); iter != l.end(); ++iter)
	{
		if ((*iter)->X == x && (*iter)->Y == y)
		{
			return true;
		}
	}
	return false;
}

std::list<MazePath> Maze::FindPath(MazePoint *start, MazePoint *end, bool ignore_conner)
{
	open_list.push_back(start);
	MazePoint *path = nullptr;
	while (open_list.size() != 0)
	{
		open_list.sort(min_f);
		MazePoint *tmp_start = open_list.front();
		open_list.pop_front();
		close_list.push_back(tmp_start);

		std::list<MazePoint *> surroundPoints = SurrroundPoints(tmp_start, ignore_conner);
		for (auto iter = surroundPoints.begin(); iter != surroundPoints.end(); ++iter)
		{
			MazePoint *sp = *iter;
			if (get_in_list(open_list, sp))
			{
				FoundPoint(tmp_start, sp);
			}
			else
			{
				NotFoundPoint(tmp_start, end, sp);
			}
		}
		path = get_in_list(open_list, end);
		if (path)
		{
			break;
		}
	}
	if (!path)
	{
		path = get_in_list(open_list, end);
	}
	std::list<MazePath> ret;
	while (path)
	{
		ret.push_front(MazePath(path->X, path->Y));
		path = path->parent;
	}
	return ret;
}

void Maze::FoundPoint(MazePoint *tempStart, MazePoint *point)
{
	int G = CalcG(tempStart, point);
	if (G < point->G)
	{
		point->parent = tempStart;
		point->G = G;
		point->CalcF();
	}
}

void Maze::NotFoundPoint(MazePoint *tempStart, MazePoint *end, MazePoint *point)
{
	point->parent = tempStart;
	point->G = CalcG(tempStart, point);
	point->H = CalcH(end, point);
	point->CalcF();
	open_list.push_back(point);
}

int Maze::CalcG(MazePoint *start, MazePoint *point)
{
	int G = (abs(point->X - start->X) + abs(point->Y - start->Y)) == 2 ? STEP : OBLIQUE;
	int parentG = point->parent ? point->parent->G : 0;
	return G + parentG;
}

int Maze::CalcH(MazePoint *end, MazePoint *point)
{
	int step = abs(point->X - end->X) + abs(point->Y - end->Y);
	return step * STEP;
}

std::list<MazePoint *> Maze::SurrroundPoints(MazePoint *point, bool ignore_conner)
{
	std::list<MazePoint *> ret;

	for(int x = point->X -1; x <= point->X+1; x++)
	{
		for (int y = point->Y - 1; y <= point->Y + 1; y++)
		{
			if (CanReach(point, x, y, ignore_conner))
			{
				ret.push_back(NewPoint(x, y));
			}
		}
	}
	return ret;
}

bool Maze::CanReach(int x, int y)
{
	return maze[x][y] == 0;
}

bool Maze::CanReach(MazePoint *start, int x, int y, bool ignore_conner)
{
	if (!CanReach(x, y) || exists_in_list(close_list, x, y))
	{
		return false;
	}
	else
	{
		if (abs(x - start->X) + abs(y - start->Y) == 1)
		{
			return true;
		}
		else
		{
			if (CanReach(abs(x - 1), y) && CanReach(x, abs(y - 1)))
			{
				return true;
			}
			else
			{
				return ignore_conner;
			}
		}
	}
}

MazePoint * Maze::NewPoint(int x, int y)
{
	MazePoint *ptr = new MazePoint(x, y);
	point_list.push_back(ptr);
	return ptr;
}


MazePoint::MazePoint(int x, int y) :
F(0), G(0), H(0), X(x), Y(y), parent(nullptr)
{
}

void MazePoint::CalcF()
{
	F = G + H;
}

MazePath::MazePath(int x, int y) :
x(x), y(y)
{
}
