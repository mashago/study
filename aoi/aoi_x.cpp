#include <algorithm>
#include <iterator>
#include <iostream>
#include <algorithm>
#include "aoi_x.h"

namespace AOI_X_SPACE
{
AOI::AOI(uint32_t aoi_x_len, uint32_t aoi_y_len) : m_cur_aoi_id(0), m_aoi_x_len(aoi_x_len), m_aoi_y_len(aoi_y_len), m_head_aoi_obj(nullptr)
{
	// add a head aoi obj
	uint64_t out_aoi_id;
	add(-1999999999, -1999999999, false, false, out_aoi_id, 0);
}

AOI::~AOI()
{
	for (auto &it : _check_obj_map)
	{
		CheckObj* check_obj = it.second;
		delete check_obj;
	}
}

bool AOI::add(int x, int y, bool is_marker, bool is_watcher, uint64_t& out_aoi_id, uint64_t entity_id)
{

	out_aoi_id = m_cur_aoi_id++;

	CheckObj* aoi_obj = new CheckObj();
	aoi_obj->_aoi_id = out_aoi_id;
	aoi_obj->_entity_id = entity_id;

	aoi_obj->_aoi_len_array[0] = m_aoi_x_len;
	aoi_obj->_aoi_len_array[1] = m_aoi_y_len;
	aoi_obj->_pos_array[0] = x;
	aoi_obj->_pos_array[1] = y;

	aoi_obj->_pre_array[0] = nullptr;
	aoi_obj->_pre_array[1] = nullptr;
	aoi_obj->_next_array[0] = nullptr;
	aoi_obj->_next_array[1] = nullptr;

	aoi_obj->_is_org = false;
	aoi_obj->_is_watcher = is_watcher;
	aoi_obj->_is_marker = is_marker;


	_check_obj_map[aoi_obj->_aoi_id] = aoi_obj;

	// head aoi obj
	if (m_head_aoi_obj == NULL)
	{
		// define head aoi obj
		m_head_aoi_obj = aoi_obj;
		aoi_obj->_is_org = true;
		return true;
	}

	auto func_add_aoi_obj = [this](CheckObj *aoi_obj, AOIAxis axis)
	{
		// assert will never update head aoi obj
		CheckObj *point_aoi_obj = m_head_aoi_obj;
		CheckObj *next_aoi_obj = point_aoi_obj->_next_array[axis];
		do
		{
			// no more next, set as next
			if (!next_aoi_obj)
			{
				aoi_obj->_pre_array[axis] = point_aoi_obj;
				point_aoi_obj->_next_array[axis] = aoi_obj;
				break;
			}

			// find until obj x > new obj x, and set as obj pre
			if (aoi_obj->_pos_array[axis] <= next_aoi_obj->_pos_array[axis])
			{
				aoi_obj->_pre_array[axis] = next_aoi_obj->_pre_array[axis];
				aoi_obj->_pre_array[axis]->_next_array[axis] = aoi_obj;

				aoi_obj->_next_array[axis] = next_aoi_obj;
				next_aoi_obj->_pre_array[axis] = aoi_obj;
				break;
			}
			
			point_aoi_obj = next_aoi_obj;
			next_aoi_obj = point_aoi_obj->_next_array[axis];
		} 
		while (point_aoi_obj);
	};
	func_add_aoi_obj(aoi_obj, AOIAxis::X_AXIS);
	func_add_aoi_obj(aoi_obj, AOIAxis::Y_AXIS);

	// create add event
	obj_create_event(aoi_obj, AOIEventType::ADD);

	return true;
}

void AOI::remove(uint64_t aoi_id)
{
	CheckObj *aoi_obj = get_aoi_obj(aoi_id);
	if (!aoi_obj || aoi_obj->_is_org)
	{
		return;
	}

	// create remove event
	obj_create_event(aoi_obj, AOIEventType::REMOVE);

	// fix pre and next
	auto remove_aoi_obj = [this](CheckObj *aoi_obj, AOIAxis axis)
	{
		aoi_obj->_pre_array[axis]->_next_array[axis] = aoi_obj->_next_array[axis];
		if (aoi_obj->_next_array[axis])
		{
			aoi_obj->_next_array[axis]->_pre_array[axis] = aoi_obj->_pre_array[axis];
		}
	};
	remove_aoi_obj(aoi_obj, AOIAxis::X_AXIS);
	remove_aoi_obj(aoi_obj, AOIAxis::Y_AXIS);

	// cleanup
	delete aoi_obj;
	_check_obj_map.erase(aoi_id);
}

void AOI::move(uint64_t aoi_id, int x, int y)
{
	CheckObj *aoi_obj = get_aoi_obj(aoi_id);
	if (!aoi_obj || aoi_obj->_is_org)
	{
		return;
	}

	// 1. get old pos effect vec
	// 2. move aoi obj
	// 3. get new pos effect vec
	// 4. intersection for move, diff for remove and add

	// 1. get old pos effect vec
	std::vector<uint64_t> old_around_obj_vec = get_around_obj_vec(aoi_obj);

	// 2. move aoi obj
	auto aoi_obj_move = [](CheckObj *aoi_obj, AOIAxis axis, int new_pos)
	{
		int old_pos = aoi_obj->_pos_array[axis];
		aoi_obj->_pos_array[axis] = new_pos;

		if (new_pos == old_pos)
		{
			return;
		}
		if (new_pos < old_pos)
		{
			// check pre obj
			CheckObj *point_obj = aoi_obj->_pre_array[axis];
			while (point_obj)
			{
				if (point_obj->_pos_array[axis] <= new_pos)
				{
					break;
				}

				CheckObj *next_obj = aoi_obj->_next_array[axis];
				point_obj->_pre_array[axis]->_next_array[axis] = aoi_obj;
				point_obj->_next_array[axis] = next_obj;
				if (next_obj)
				{
					next_obj->_pre_array[axis] = point_obj;
				}

				aoi_obj->_next_array[axis] = point_obj;
				aoi_obj->_pre_array[axis] = point_obj->_pre_array[axis];
				point_obj->_pre_array[axis] = aoi_obj;

				point_obj = aoi_obj->_pre_array[axis];
			}
		}
		else
		{
			// check next obj
			CheckObj *point_obj = aoi_obj->_next_array[axis];
			while (point_obj)
			{
				if (point_obj->_pos_array[axis] >= new_pos)
				{
					break;
				}

				CheckObj *next_obj = point_obj->_next_array[axis];
				aoi_obj->_pre_array[axis]->_next_array[axis] = point_obj;
				aoi_obj->_next_array[axis] = next_obj;
				if (next_obj)
				{
					next_obj->_pre_array[axis] = aoi_obj;
				}

				point_obj->_next_array[axis] = aoi_obj;
				point_obj->_pre_array[axis] = aoi_obj->_pre_array[axis];
				aoi_obj->_pre_array[axis] = point_obj;

				point_obj = aoi_obj->_next_array[axis];
			}
		}
	};
	aoi_obj_move(aoi_obj, AOIAxis::X_AXIS, x);
	aoi_obj_move(aoi_obj, AOIAxis::Y_AXIS, y);

	// 3. get new pos effect vec
	std::vector<uint64_t> new_pos_effect_vec = get_around_obj_vec(aoi_obj);
	
	// 4. intersection for move, diff for remove and add
	std::vector<uint64_t> effect_move_vec(old_around_obj_vec.size() + new_pos_effect_vec.size());
	{
	auto iter = std::set_intersection(old_around_obj_vec.begin(), old_around_obj_vec.end(), new_pos_effect_vec.begin(), new_pos_effect_vec.end(), effect_move_vec.begin());
	effect_move_vec.resize(iter - effect_move_vec.begin());
	}

	std::vector<uint64_t> effect_leave_vec(old_around_obj_vec.size() + new_pos_effect_vec.size());
	{
	auto iter = std::set_difference(old_around_obj_vec.begin(), old_around_obj_vec.end(), new_pos_effect_vec.begin(), new_pos_effect_vec.end(), effect_leave_vec.begin());
	effect_leave_vec.resize(iter - effect_leave_vec.begin());
	}

	std::vector<uint64_t> effect_enter_vec(old_around_obj_vec.size() + new_pos_effect_vec.size());
	{
	auto iter = std::set_difference(new_pos_effect_vec.begin(), new_pos_effect_vec.end(), old_around_obj_vec.begin(), old_around_obj_vec.end(), effect_enter_vec.begin());
	effect_enter_vec.resize(iter - effect_enter_vec.begin());
	}

	// 5. make event
	auto make_event_list = [this](CheckObj *aoi_obj, std::vector<uint64_t> &aoi_id_vec, AOIEventType event_type)
	{
		for (uint64_t aoi_id : aoi_id_vec)
		{
			auto iter = _check_obj_map.find(aoi_id);
			if (iter == _check_obj_map.end())
			{
				// something go wrong
				continue;
			}
			make_event(aoi_obj, iter->second, event_type);
		}
	};
	make_event_list(aoi_obj, effect_move_vec, AOIEventType::IN);
	make_event_list(aoi_obj, effect_leave_vec, AOIEventType::LEAVE);
	make_event_list(aoi_obj, effect_enter_vec, AOIEventType::ENTER);

}

AOI::EventList& AOI::get_all_events()
{
	return _event_list;
}

void AOI::clear_events()
{
	for (auto ptr : _event_list)
	{
		delete ptr;
	}
	_event_list.clear();
}

AOI::ObjIDList& AOI::get_all_around_ids(uint64_t check_obj_id)
{
	_all_around_ids.clear();

	CheckObj *aoi_obj = get_aoi_obj(check_obj_id);
	if (!aoi_obj || aoi_obj->_is_org)
	{
		return _all_around_ids;
	}


	_all_around_ids = get_around_obj_vec(aoi_obj);

	return _all_around_ids;
}

CheckObj* AOI::get_aoi_obj(uint64_t check_obj_id)
{
	CheckObjMapIt it = _check_obj_map.find(check_obj_id);
	if (it != _check_obj_map.end()){
		return it->second;
	}

	return NULL;
}

void AOI::print_x_objs()
{
	std::cout << "X objs : ";
	CheckObj* check_obj = m_head_aoi_obj;
	while (check_obj)
	{
		std::cout << " [ ";
		std::cout << " id:" << check_obj->_aoi_id;
		std::cout << " x:" << check_obj->_pos_array[AOIAxis::X_AXIS];
		std::cout << " y:" << check_obj->_pos_array[AOIAxis::Y_AXIS];
		std::cout << " entity_id:" << check_obj->_entity_id;
		std::cout << " ] ";

		check_obj = check_obj->_next_array[AOIAxis::X_AXIS];
	}

	std::cout << std::endl;
}

void AOI::print_y_objs()
{
	std::cout << "Y objs : ";
	CheckObj* check_obj = m_head_aoi_obj;
	while (check_obj)
	{
		std::cout << " [ ";
		std::cout << " id:" << check_obj->_aoi_id;
		std::cout << " x:" << check_obj->_pos_array[AOIAxis::X_AXIS];
		std::cout << " y:" << check_obj->_pos_array[AOIAxis::Y_AXIS];
		std::cout << " entity_id:" << check_obj->_entity_id;
		std::cout << " ] ";

		check_obj = check_obj->_next_array[AOIAxis::Y_AXIS];
	}

	std::cout << std::endl;
}


void AOI::print_all_events()
{
	std::cout << "Event : ";
	for (EventListIt it = _event_list.begin(); it != _event_list.end(); ++it){
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

	_event_list.clear();
}


//////////////////////////////////////////////////////


std::vector<uint64_t> AOI::get_around_obj_vec(CheckObj *aoi_obj)
{

	// 1. get range
	// 2. check pre aoi obj
	// 3. check next aoi obj
	// 4. get intersection
	
	auto get_effect_axis = [](CheckObj *aoi_obj, AOIAxis axis)
	{
		std::vector<uint64_t> ret;
		// 1. get range
		int pos_min = aoi_obj->_pos_array[axis] - aoi_obj->_aoi_len_array[axis];
		int pos_max = aoi_obj->_pos_array[axis] + aoi_obj->_aoi_len_array[axis];

		CheckObj* point_aoi_obj = nullptr;
		
		// 2. check pre aoi obj
		point_aoi_obj = aoi_obj->_pre_array[axis];
		while (point_aoi_obj)
		{
			if (point_aoi_obj->_pos_array[axis] < pos_min)
			{
				// min over
				break;
			}

			ret.push_back(point_aoi_obj->_aoi_id);
			point_aoi_obj = point_aoi_obj->_pre_array[axis];
		}

		// 3. check next aoi obj
		point_aoi_obj = aoi_obj->_next_array[axis];
		while (point_aoi_obj)
		{
			if (point_aoi_obj->_pos_array[axis] > pos_max)
			{
				// max over
				break;
			}

			ret.push_back(point_aoi_obj->_aoi_id);
			point_aoi_obj = point_aoi_obj->_next_array[axis];
		}
		std::sort(ret.begin(), ret.end());
		return ret;
	};
	std::vector<uint64_t> x_effect_vec = get_effect_axis(aoi_obj, AOIAxis::X_AXIS);
	std::vector<uint64_t> y_effect_vec = get_effect_axis(aoi_obj, AOIAxis::Y_AXIS);

	// 4. get intersection
	std::vector<uint64_t> effect_vec(x_effect_vec.size() + y_effect_vec.size());
	auto iter = std::set_intersection(x_effect_vec.begin(), x_effect_vec.end()
	, y_effect_vec.begin(), y_effect_vec.end(), effect_vec.begin());
	effect_vec.resize(iter - effect_vec.begin());

	return effect_vec;
}

void AOI::make_event(CheckObj* marker, CheckObj* watcher, AOIEventType ev_type)
{
	Event *ev = new Event;
	ev->_event = ev_type;
	ev->_marker_aoi_id = marker->_aoi_id;
	ev->_watcher_aoi_id = watcher->_aoi_id;
	ev->_marker_entity_id = marker->_entity_id;
	ev->_watcher_entity_id = watcher->_entity_id;

	_event_list.push_back(ev);
}

void AOI::obj_create_event(CheckObj *aoi_obj, AOIEventType event_type)
{
	std::vector<uint64_t> effect_vec = get_around_obj_vec(aoi_obj);
	for (uint64_t aoi_id : effect_vec)
	{
		auto iter = _check_obj_map.find(aoi_id);
		if (iter == _check_obj_map.end())
		{
			// something go wrong
			continue;
		}
		make_event(aoi_obj, iter->second, event_type);
	}
}

};
