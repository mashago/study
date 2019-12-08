
#include <stdlib.h>
#include "astar.h"

#define STEP 	10
#define OBLIQUE 14

MazePath::MazePath(int x, int y) :
x(x), y(y)
{
}

class MazePoint
{
public:
	MazePoint(int x, int y);
	void calc_f();
	int F;
	int G;
	int H;
	int x;
	int y;
	MazePoint *parent;
};

MazePoint::MazePoint(int x, int y) :
F(0), G(0), H(0), x(x), y(y), parent(nullptr)
{
}

void MazePoint::calc_f()
{
	F = G + H;
}

const int *maze;
int max_x = 0;
int max_y = 0;
bool ignore_conner = false;
std::list<MazePoint *> close_list;
std::list<MazePoint *> open_list;
std::list<MazePoint *> point_list;

bool min_f(MazePoint *p1, MazePoint *p2)
{
	return p1->F <= p2->F;
}

MazePoint * new_point(int x, int y)
{
	MazePoint *ptr = new MazePoint(x, y);
	point_list.push_back(ptr);
	return ptr;
}

MazePoint * get_in_list(std::list<MazePoint *> l, MazePoint *p)
{
	for (auto iter = l.begin(); iter != l.end(); ++iter)
	{
		if ((*iter)->x == p->x && (*iter)->y == p->y)
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
		if ((*iter)->x == x && (*iter)->y == y)
		{
			return true;
		}
	}
	return false;
}

bool CanReach(int x, int y)
{
	if (x < 0 || x >= max_x || y < 0 || y >= max_y) {
		return false;
	}
	return *(maze + x * max_y + y) == 0;
}

bool CanReach(MazePoint *start, int x, int y)
{
	if (!CanReach(x, y) || exists_in_list(close_list, x, y))
	{
		return false;
	}
	if (abs(x - start->x) + abs(y - start->y) == 1)
	{
		return true;
	}

	int conner[2][2];
	if (start->x < x) {
		if (start->y < y) {
			conner[0][0] = x - 1;
			conner[0][1] = y;
			conner[1][0] = x;
			conner[1][1] = y - 1;
		}
		else
		{
			conner[0][0] = x - 1;
			conner[0][1] = y;
			conner[1][0] = x;
			conner[1][1] = y + 1;
		}
	}
	else
	{
		if (start->y < y) {
			conner[0][0] = x + 1;
			conner[0][1] = y;
			conner[1][0] = x;
			conner[1][1] = y - 1;
		}
		else
		{
			conner[0][0] = x + 1;
			conner[0][1] = y;
			conner[1][0] = x;
			conner[1][1] = y + 1;
		}
	}

	bool c1 = CanReach(conner[0][0], conner[0][1]);
	bool c2 = CanReach(conner[1][0], conner[1][1]);
	if (c1 && c2)
	{
		return true;
	}
	else
	{
		return (c1 | c2) && ignore_conner;
	}
}

std::list<MazePoint *> get_surround_points(MazePoint *point)
{
	std::list<MazePoint *> ret;
	for(int x = point->x -1; x <= point->x+1; x++)
	{
		for (int y = point->y - 1; y <= point->y + 1; y++)
		{
			if (x == point->x && y == point->y)
			{
				continue;
			}
			if (CanReach(point, x, y))
			{
				ret.push_back(new_point(x, y));
			}
		}
	}
	return ret;
}

int calc_g(MazePoint *start, MazePoint *point)
{
	int G = (abs(point->x - start->x) + abs(point->y - start->y)) == 2 ? STEP : OBLIQUE;
	int parentG = point->parent ? point->parent->G : 0;
	return G + parentG;
}

int calc_h(MazePoint *end, MazePoint *point)
{
	int step = abs(point->x - end->x) + abs(point->y - end->y);
	return step * STEP;
}

void refresh_open_point(MazePoint *step_point, MazePoint *surround_point)
{
	int G = calc_g(step_point, surround_point);
	if (G < surround_point->G)
	{
		surround_point->parent = step_point;
		surround_point->G = G;
		surround_point->calc_f();
	}
}

void add_open_point(MazePoint *step_point, MazePoint *end, MazePoint *surround_point)
{
	surround_point->parent = step_point;
	surround_point->G = calc_g(step_point, surround_point);
	surround_point->H = calc_h(end, surround_point);
	surround_point->calc_f();
	open_list.push_back(surround_point);
}

std::list<MazePath> MazeFindPath(int *m, int mx, int my, MazePath sp, MazePath ep, bool ic)
{
	maze = m;
	max_x = mx;
	max_y = my;
	ignore_conner = ic;
	MazePoint *start = new_point(sp.x, sp.y);
	MazePoint *end = new_point(ep.x, ep.y);

	open_list.push_back(start);
	MazePoint *path = nullptr;
	while (open_list.size() != 0)
	{
		open_list.sort(min_f);
		MazePoint *step_point = open_list.front();
		open_list.pop_front();
		close_list.push_back(step_point);

		std::list<MazePoint *> surround_points = get_surround_points(step_point);
		for (auto iter = surround_points.begin(); iter != surround_points.end(); ++iter)
		{
			MazePoint *surround_point = *iter;
			if (get_in_list(open_list, surround_point))
			{
				refresh_open_point(step_point, surround_point);
			}
			else
			{
				add_open_point(step_point, end, surround_point);
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
		ret.push_front(MazePath(path->x, path->y));
		path = path->parent;
	}

	for (auto iter = point_list.begin(); iter != point_list.end(); iter++)
	{
		delete *iter;
	}

	return ret;
}


