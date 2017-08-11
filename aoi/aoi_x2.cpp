#include <algorithm>
#include <iterator>
#include <iostream>
#include <algorithm>
#include "aoi_x2.h"

namespace AOI_X2_SPACE
{
AOI::AOI(uint32_t aoi_x_len, uint32_t aoi_y_len) : m_cur_aoi_id(0), m_aoi_x_len(aoi_x_len), m_aoi_y_len(aoi_y_len), m_head_aoi_obj(nullptr)
{
	// add a head aoi obj
	uint64_t out_aoi_id;
	add(-1999999999, -1999999999, false, false, out_aoi_id, 0);
}

AOI::~AOI()
{
	for (auto &it : m_aoi_obj_map)
	{
		AOIObj* aoi_obj = it.second;
		delete aoi_obj;
	}
}

bool AOI::add(int x, int y, bool is_marker, bool is_watcher, uint64_t& out_aoi_id, uint64_t entity_id)
{

	out_aoi_id = m_cur_aoi_id++;

	AOIObj* aoi_obj = new AOIObj();
	aoi_obj->_aoi_id = out_aoi_id;
	aoi_obj->_entity_id = entity_id;

	aoi_obj->_x_aoi_len = m_aoi_x_len;
	aoi_obj->_y_aoi_len = m_aoi_y_len;
	aoi_obj->_x = x;
	aoi_obj->_y = y;

	aoi_obj->_x_pre = nullptr;
	aoi_obj->_x_next = nullptr;

	aoi_obj->_is_org = false;
	aoi_obj->_is_watcher = is_watcher;
	aoi_obj->_is_marker = is_marker;

	m_aoi_obj_map[aoi_obj->_aoi_id] = aoi_obj;

	// head aoi obj
	if (m_head_aoi_obj == NULL)
	{
		// define head aoi obj
		m_head_aoi_obj = aoi_obj;
		aoi_obj->_is_org = true;
		return true;
	}

	// assert will never update head aoi obj
	AOIObj *point_aoi_obj = m_head_aoi_obj;
	AOIObj *next_aoi_obj = point_aoi_obj->_x_next;
	do
	{
		// no more next, set as next
		if (!next_aoi_obj)
		{
			aoi_obj->_x_pre = point_aoi_obj;
			point_aoi_obj->_x_next = aoi_obj;
			break;
		}

		// find until obj x > new obj x, and set as obj pre
		if (aoi_obj->_x <= next_aoi_obj->_x)
		{
			aoi_obj->_x_pre = next_aoi_obj->_x_pre;
			aoi_obj->_x_pre->_x_next = aoi_obj;

			aoi_obj->_x_next = next_aoi_obj;
			next_aoi_obj->_x_pre = aoi_obj;
			break;
		}
		
		point_aoi_obj = next_aoi_obj;
		next_aoi_obj = point_aoi_obj->_x_next;
	} 
	while (point_aoi_obj);

	// create add event
	obj_create_event(aoi_obj, AOIEventType::ADD);

	return true;
}

void AOI::remove(uint64_t aoi_id)
{
	AOIObj *aoi_obj = get_aoi_obj(aoi_id);
	if (!aoi_obj || aoi_obj->_is_org)
	{
		return;
	}

	// create remove event
	obj_create_event(aoi_obj, AOIEventType::REMOVE);

	// fix pre and next
	aoi_obj->_x_pre->_x_next = aoi_obj->_x_next;
	if (aoi_obj->_x_next)
	{
		aoi_obj->_x_next->_x_pre = aoi_obj->_x_pre;
	}

	// cleanup
	delete aoi_obj;
	m_aoi_obj_map.erase(aoi_id);
}

void AOI::move(uint64_t aoi_id, int x, int y)
{
	AOIObj *aoi_obj = get_aoi_obj(aoi_id);
	if (!aoi_obj || aoi_obj->_is_org)
	{
		return;
	}

	if (aoi_obj->_x == x && aoi_obj->_y == y)
	{
		return;
	}

	// 1. get old pos around vec
	// 2. move aoi obj
	// 3. get new pos around vec
	// 4. intersection for move, diff for remove and add

	// 1. get old pos around vec
	std::vector<AOIObj *> old_around_obj_vec = get_around_obj(aoi_obj, true);

	// 2. move aoi obj
	int old_x = aoi_obj->_x;
	aoi_obj->_x = x;
	aoi_obj->_y = y;

	if (x == old_x)
	{
		// only y change, will not change link list	
	}
	if (x < old_x)
	{
		// check pre obj
		AOIObj *point_obj = aoi_obj->_x_pre;
		while (point_obj)
		{
			if (point_obj->_x <= x)
			{
				break;
			}

			AOIObj *next_obj = aoi_obj->_x_next;
			point_obj->_x_pre->_x_next = aoi_obj;
			point_obj->_x_next = next_obj;
			if (next_obj)
			{
				next_obj->_x_pre = point_obj;
			}

			aoi_obj->_x_next = point_obj;
			aoi_obj->_x_pre = point_obj->_x_pre;
			point_obj->_x_pre = aoi_obj;

			point_obj = aoi_obj->_x_pre;
		}
	}
	else
	{
		// check next obj
		AOIObj *point_obj = aoi_obj->_x_next;
		while (point_obj)
		{
			if (point_obj->_x >= x)
			{
				break;
			}

			AOIObj *next_obj = point_obj->_x_next;
			aoi_obj->_x_pre->_x_next = point_obj;
			aoi_obj->_x_next = next_obj;
			if (next_obj)
			{
				next_obj->_x_pre = aoi_obj;
			}

			point_obj->_x_next = aoi_obj;
			point_obj->_x_pre = aoi_obj->_x_pre;
			aoi_obj->_x_pre = point_obj;

			point_obj = aoi_obj->_x_next;
		}
	}

	// 3. get new pos around vec
	std::vector<AOIObj *> new_around_obj_vec = get_around_obj(aoi_obj, true);
	
	// 4. intersection for move, diff for remove and add
	uint32_t old_around_obj_num = old_around_obj_vec.size();
	uint32_t new_around_obj_num = new_around_obj_vec.size();
	uint32_t diff_num = old_around_obj_num >= new_around_obj_num ? old_around_obj_num : new_around_obj_num;

	std::vector<AOIObj *> notify_move_vec(old_around_obj_num + new_around_obj_num);
	{
	auto iter = std::set_intersection(old_around_obj_vec.begin(), old_around_obj_vec.end(), new_around_obj_vec.begin(), new_around_obj_vec.end(), notify_move_vec.begin());
	notify_move_vec.resize(iter - notify_move_vec.begin());
	}

	
	std::vector<AOIObj *> notify_leave_vec(diff_num);
	{
	auto iter = std::set_difference(old_around_obj_vec.begin(), old_around_obj_vec.end(), new_around_obj_vec.begin(), new_around_obj_vec.end(), notify_leave_vec.begin());
	notify_leave_vec.resize(iter - notify_leave_vec.begin());
	}

	std::vector<AOIObj *> notify_enter_vec(diff_num);
	{
	auto iter = std::set_difference(new_around_obj_vec.begin(), new_around_obj_vec.end(), old_around_obj_vec.begin(), old_around_obj_vec.end(), notify_enter_vec.begin());
	notify_enter_vec.resize(iter - notify_enter_vec.begin());
	}

	// 5. make event
	auto make_event_list = [this](AOIObj *aoi_obj, std::vector<AOIObj *> &notify_obj_vec, AOIEventType event_type)
	{
		for (AOIObj *notify_obj : notify_obj_vec)
		{
			make_event(aoi_obj, notify_obj, event_type);
		}
	};
	make_event_list(aoi_obj, notify_move_vec, AOIEventType::IN);
	make_event_list(aoi_obj, notify_leave_vec, AOIEventType::LEAVE);
	make_event_list(aoi_obj, notify_enter_vec, AOIEventType::ENTER);

}

AOI::EventList& AOI::get_all_events()
{
	return m_event_list;
}

void AOI::clear_events()
{
	for (auto ptr : m_event_list)
	{
		delete ptr;
	}
	m_event_list.clear();
}

AOI::AOIIdList AOI::get_all_around_ids(uint64_t aoi_id)
{
	AOIObj *aoi_obj = get_aoi_obj(aoi_id);
	if (!aoi_obj || aoi_obj->_is_org)
	{
		AOIIdList all_around_ids;
		return all_around_ids;
	}

	return get_around_obj_id(aoi_obj);
}

AOIObj* AOI::get_aoi_obj(uint64_t aoi_id)
{
	auto it = m_aoi_obj_map.find(aoi_id);
	if (it == m_aoi_obj_map.end())
	{
		return NULL;
	}

	return it->second;
}

void AOI::print_x_objs()
{
	std::cout << "X objs : ";
	AOIObj* aoi_obj = m_head_aoi_obj;
	while (aoi_obj)
	{
		std::cout << " [ ";
		std::cout << " id:" << aoi_obj->_aoi_id;
		std::cout << " x:" << aoi_obj->_x;
		std::cout << " y:" << aoi_obj->_y;
		std::cout << " entity_id:" << aoi_obj->_entity_id;
		std::cout << " ] ";

		aoi_obj = aoi_obj->_x_next;
	}

	std::cout << std::endl;
}

void AOI::print_all_events()
{
	std::cout << "Event : ";
	for (auto it = m_event_list.begin(); it != m_event_list.end(); ++it){
		Event *ev = *it;
		std::cout << " [ ";
		std::cout << " maker_id:" << ev->_marker_aoi_id;
		std::cout << " watcher_id:" << ev->_watcher_aoi_id;
		if (ev->_event == 1){
			std::cout << " ENTER ";
		}
		else if (ev->_event == 2){
			std::cout << " LEAVE ";
		}
		else if (ev->_event == 3){
			std::cout << " IN ";
		}
		else if (ev->_event == 4){
			std::cout << " ADD ";
		}
		else if (ev->_event == 5){
			std::cout << " REMOVE ";
		}

		std::cout << " ] ";
		delete ev;
	}

	std::cout << std::endl;

	m_event_list.clear();
}


//////////////////////////////////////////////////////


std::vector<uint64_t> AOI::get_around_obj_id(AOIObj *aoi_obj, bool do_sort)
{

	// 1. get range
	// 2. check pre aoi obj
	// 3. check next aoi obj

	std::vector<uint64_t> around_vec; 
	
	// 1. get range
	int x_min = aoi_obj->_x - aoi_obj->_x_aoi_len;
	int x_max = aoi_obj->_x + aoi_obj->_x_aoi_len;
	int y_min = aoi_obj->_y - aoi_obj->_y_aoi_len;
	int y_max = aoi_obj->_y + aoi_obj->_y_aoi_len;

	AOIObj* point_aoi_obj = nullptr;
	
	// 2. check pre aoi obj
	point_aoi_obj = aoi_obj->_x_pre;
	while (point_aoi_obj)
	{
		if (point_aoi_obj->_x < x_min)
		{
			// min over
			break;
		}

		// check y
		if (point_aoi_obj->_y >= y_min && point_aoi_obj->_y <= y_max)
		{
			around_vec.push_back(point_aoi_obj->_aoi_id);
		}

		point_aoi_obj = point_aoi_obj->_x_pre;
	}

	// 3. check next aoi obj
	point_aoi_obj = aoi_obj->_x_next;
	while (point_aoi_obj)
	{
		if (point_aoi_obj->_x > x_max)
		{
			// max over
			break;
		}

		// check y
		if (point_aoi_obj->_y >= y_min && point_aoi_obj->_y <= y_max)
		{
			around_vec.push_back(point_aoi_obj->_aoi_id);
		}

		point_aoi_obj = point_aoi_obj->_x_next;
	}

	if (do_sort)
	{
		std::sort(around_vec.begin(), around_vec.end());
	}

	return around_vec;
}

std::vector<AOIObj *> AOI::get_around_obj(AOIObj *aoi_obj, bool do_sort)
{

	// 1. get range
	// 2. check pre aoi obj
	// 3. check next aoi obj
	
	std::vector<AOIObj *> around_vec; 

	// 1. get range
	int x_min = aoi_obj->_x - aoi_obj->_x_aoi_len;
	int x_max = aoi_obj->_x + aoi_obj->_x_aoi_len;
	int y_min = aoi_obj->_y - aoi_obj->_y_aoi_len;
	int y_max = aoi_obj->_y + aoi_obj->_y_aoi_len;

	AOIObj* point_aoi_obj = nullptr;
	
	// 2. check pre aoi obj
	point_aoi_obj = aoi_obj->_x_pre;
	while (point_aoi_obj)
	{
		if (point_aoi_obj->_x < x_min)
		{
			// min over
			break;
		}

		// check y
		if (point_aoi_obj->_y >= y_min && point_aoi_obj->_y <= y_max)
		{
			around_vec.push_back(point_aoi_obj);
		}

		point_aoi_obj = point_aoi_obj->_x_pre;
	}

	// 3. check next aoi obj
	point_aoi_obj = aoi_obj->_x_next;
	while (point_aoi_obj)
	{
		if (point_aoi_obj->_x > x_max)
		{
			// max over
			break;
		}

		// check y
		if (point_aoi_obj->_y >= y_min && point_aoi_obj->_y <= y_max)
		{
			around_vec.push_back(point_aoi_obj);
		}

		point_aoi_obj = point_aoi_obj->_x_next;
	}

	if (do_sort)
	{
		std::sort(around_vec.begin(), around_vec.end());
	}

	return around_vec;
}

void AOI::make_event(AOIObj* marker, AOIObj* watcher, AOIEventType ev_type)
{
	Event *ev = new Event;
	ev->_event = ev_type;
	ev->_marker_aoi_id = marker->_aoi_id;
	ev->_watcher_aoi_id = watcher->_aoi_id;
	ev->_marker_entity_id = marker->_entity_id;
	ev->_watcher_entity_id = watcher->_entity_id;

	m_event_list.push_back(ev);
}

void AOI::obj_create_event(AOIObj *aoi_obj, AOIEventType event_type)
{
	std::vector<AOIObj *> around_vec = get_around_obj(aoi_obj);
	for (AOIObj *around_obj : around_vec)
	{
		make_event(aoi_obj, around_obj, event_type);
	}
}

};
