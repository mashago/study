#include <algorithm>
#include <iterator>
#include <iostream>
#include "aoi_old.h"

AOI::AOI()
	:_cur_check_list_id(0)
	, _cur_aoi_id(0)
{

}

AOI::~AOI()
{
	for (auto &it : _check_obj_map)
	{
		CheckObj* check_obj = it.second;
		delete check_obj;
	}
}

int AOI::make_check_list(uint32_t aoi_x_len, uint32_t aoi_y_len)
{
	int check_list_id = _cur_check_list_id;

	_check_list_id_map[check_list_id] = std::make_pair(aoi_x_len, aoi_y_len);
	_check_obj_list_map[check_list_id] = NULL;

	// add a head aoi obj
	uint64_t out_aoi_id;
	add(check_list_id, -1999999999, -1999999999, false, false, out_aoi_id);

	++_cur_check_list_id;

	return check_list_id;
}

bool AOI::add(int check_list_id, int x, int y, bool is_marker, bool is_watcher, uint64_t& out_aoi_id, uint64_t entity_id)
{
	auto check_list_iter = _check_list_id_map.find(check_list_id);
	if (check_list_iter == _check_list_id_map.end())
	{
		printf("add no such aoi check list\n")
		return false;
	}

	auto iter = _check_obj_list_map.find(check_list_id);
	if (iter == _check_obj_list_map.end())
	{
		printf("add no such aoi obj list\n")
		return false;
	}
	CheckObj *head_aoi_obj = iter.second()

	uint32_t aoi_x_len = check_list_iter->second.first;
	uint32_t aoi_y_len = check_list_iter->second.second;

	out_aoi_id = _cur_aoi_id++;

	CheckObj* aoi_obj = new CheckObj();
	aoi_obj->_aoi_id = out_aoi_id;
	aoi_obj->_entity_id = entity_id;
	aoi_obj->_check_list_id = check_list_id;

	aoi_obj->_x_len = aoi_x_len;
	aoi_obj->_y_len = aoi_y_len;
	aoi_obj->_x = x;
	aoi_obj->_y = y;
	aoi_obj->_is_watcher = is_watcher;
	aoi_obj->_is_marker = is_marker;

	aoi_obj->_org = iter->second;
	aoi_obj->_is_org = false;
	aoi_obj->_x_pre = NULL;
	aoi_obj->_x_next = NULL;


	_check_obj_map[aoi_obj->_aoi_id] = aoi_obj;

	// head aoi obj
	if (head_aoi_obj == NULL)
	{
		// define head aoi obj
		iter->second = aoi_obj;
		aoi_obj->_is_org = true;
		// XXX should will return?
	}
	else
	{
		// assert will never update head aoi obj
		CheckObj *point_aoi_obj = head_aoi_obj;
		CheckObj *next_aoi_obj = point_aoi_obj->_x_next;
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
		while (!point_aoi_obj);
	}

	// create add event
	obj_create_event(aoi_obj, EventType::ADD)

	return true;
}

void AOI::remove(uint64_t aoi_id)
{
	CheckObjMapIt it = _check_obj_map.find(aoi_id);
	if (it = _check_obj_map.end())
	{
		print("AOI::remove aoi obj not exists %lu\n", aoi_id)
		return
	}

	if (it->second->_is_org)
	{
		print("AOI::remove head aoi obj %lu\n", aoi_id)
		return
	}

	CheckObj *aoi_obj = it->second;

	// create remove event
	obj_create_event(aoi_obj, EventType::REMOVE)

	// fix pre and next
	aoi_obj->_x_pre->_x_next = aoi_obj->_x_next;
	if (aoi_obj->_x_next){
		aoi_obj->_x_next->_x_pre = aoi_obj->_x_pre;
	}

	// cleanup
	delete aoi_obj;
	_check_obj_map.erase(it);
}

void AOI::move(uint64_t check_obj_id, int x, int y)
{
	CheckObjMapIt it = _check_obj_map.find(check_obj_id);
	if (it != _check_obj_map.end() && !it->second->_is_org){
		CheckObj* check_obj = it->second;

		//进行移动判断
		uint64_t marker_id = check_obj->_id;
		uint32_t check_x_len = check_obj->_x_len;
		uint32_t check_y_len = check_obj->_y_len;

		//区分向左移还是向右移动，得到三个集合：离开观察范围，一直在范围内，进入范围三个集合
		int old_x = check_obj->_x;
		int new_x = x;
		int old_min_x = old_x - check_x_len;
		int old_max_x = old_x + check_x_len;
		int new_min_x = new_x - check_x_len;
		int new_max_x = new_x + check_x_len;

		int old_y = check_obj->_y;
		int new_y = y;
		int old_min_y = old_y - check_y_len;
		int old_max_y = old_y + check_y_len;
		int new_min_y = new_y - check_y_len;
		int new_max_y = new_y + check_y_len;

		CheckObj* x_pre_check_object = check_obj->_x_pre;
		CheckObj* x_next_check_object = check_obj->_x_next;

		if (old_x != new_x){
			CheckObj* x_insert_pos_object = NULL; //找到插入的位置

			//左移动(new_min_x<old_min_x)
			if (x < old_x){
				//最前面的一个
				CheckObj* x_head_pos_object = NULL;

				//移动并将当前位置左边的范围节点进行处理
				while (x_pre_check_object){
					//查找插入的位置(插入到这个节点的后面next)
					if (!x_insert_pos_object && x_pre_check_object->_x <= x){
						x_insert_pos_object = x_pre_check_object;
					}

					//已经小于new_min则退出
					if (x_pre_check_object->_x < new_min_x){
						break;
					}

					//判断所有左边，得到离开观察范围，一直在范围内，进入范围三个集合
					if (check_obj->_is_marker && x_pre_check_object->_is_watcher){
						check_left_move_make_event(check_obj, x_pre_check_object, old_min_x, old_max_x, new_min_x, new_max_x, old_min_y, old_max_y, new_min_y, new_max_y);
					}

					//下一个
					x_head_pos_object = x_pre_check_object;
					x_pre_check_object = x_pre_check_object->_x_pre;
				}

				//遍历右边
				while (x_next_check_object){
					if (!x_head_pos_object){
						x_head_pos_object = x_next_check_object;
					}

					//已经小于new_min则退出
					if (x_next_check_object->_x > old_max_x){
						break;
					}

					//判断所有右边，得到离开观察范围，一直在范围内，进入范围三个集合
					if (check_obj->_is_marker && x_next_check_object->_is_watcher){
						check_left_move_make_event(check_obj, x_next_check_object, old_min_x, old_max_x, new_min_x, new_max_x, old_min_y, old_max_y, new_min_y, new_max_y);
					}

					x_next_check_object = x_next_check_object->_x_next;
				}

				//在insert_pos右边插入
				if (x_insert_pos_object){
					//先将check_obj前后安置好
					check_obj->_x_pre->_x_next = check_obj->_x_next;
					if (check_obj->_x_next){
						check_obj->_x_next->_x_pre = check_obj->_x_pre;
					}

					//再插入新位置
					check_obj->_x_next = x_insert_pos_object->_x_next;
					if (check_obj->_x_next){
						check_obj->_x_next->_x_pre = check_obj;
					}

					x_insert_pos_object->_x_next = check_obj;
					check_obj->_x_pre = x_insert_pos_object;
				}
				//在head_pos左边插入
				else if (x_head_pos_object){
					//先将check_obj前后安置好
					check_obj->_x_pre->_x_next = check_obj->_x_next;
					if (check_obj->_x_next){
						check_obj->_x_next->_x_pre = check_obj->_x_pre;
					}

					//再插入新位置
					if (x_head_pos_object->_x_pre){
						x_head_pos_object->_x_pre->_x_next = check_obj;
					}
					check_obj->_x_pre = x_head_pos_object->_x_pre;

					x_head_pos_object->_x_pre = check_obj;
					check_obj->_x_next = x_head_pos_object;
				}
			}
			//右移动(new_max_x>old_max_x)
			else if (x > old_x){
				CheckObj* x_tail_pos_object = NULL; //z最尾的一个节点

				//先处理右边部分
				while (x_next_check_object){
					//查找插入的位置(插入到这个节点的前面pre)
					if (!x_insert_pos_object && x_next_check_object->_x >= x){
						x_insert_pos_object = x_next_check_object;
					}

					if (x_next_check_object->_x > new_max_x){
						break;
					}

					//判断所有右边，得到离开观察范围，一直在范围内，进入范围三个集合
					if (check_obj->_is_marker && x_next_check_object->_is_watcher){
						check_right_move_make_event(check_obj, x_next_check_object, old_min_x, old_max_x, new_min_x, new_max_x, old_min_y, old_max_y, new_min_y, new_max_y);
					}

					x_tail_pos_object = x_next_check_object;
					x_next_check_object = x_next_check_object->_x_next;
				}

				//处理左边部分
				while (x_pre_check_object){
					if (!x_tail_pos_object){
						x_tail_pos_object = x_pre_check_object;
					}

					if (x_pre_check_object->_x < old_min_x){
						break;
					}

					//判断所有右边，得到离开观察范围，一直在范围内，进入范围三个集合
					if (check_obj->_is_marker && x_pre_check_object->_is_watcher){
						check_right_move_make_event(check_obj, x_pre_check_object, old_min_x, old_max_x, new_min_x, new_max_x, old_min_y, old_max_y, new_min_y, new_max_y);
					}
					x_pre_check_object = x_pre_check_object->_x_pre;
				}

				//在insert_pos左边插入
				if (x_insert_pos_object){
					//先将check_obj前后安置好
					check_obj->_x_pre->_x_next = check_obj->_x_next;
					if (check_obj->_x_next){
						check_obj->_x_next->_x_pre = check_obj->_x_pre;
					}

					if (x_insert_pos_object->_x_pre){
						x_insert_pos_object->_x_pre->_x_next = check_obj;
					}
					check_obj->_x_pre = x_insert_pos_object->_x_pre;

					x_insert_pos_object->_x_pre = check_obj;
					check_obj->_x_next = x_insert_pos_object;
				}
				//在tail_pos右边插入
				else if (x_tail_pos_object){
					//先将check_obj前后安置好
					check_obj->_x_pre->_x_next = check_obj->_x_next;
					if (check_obj->_x_next){
						check_obj->_x_next->_x_pre = check_obj->_x_pre;
					}

					check_obj->_x_next = x_tail_pos_object->_x_next;
					if (x_tail_pos_object->_x_next){
						x_tail_pos_object->_x_next->_x_pre = check_obj;
					}

					x_tail_pos_object->_x_next = check_obj;
					check_obj->_x_pre = x_tail_pos_object;
				}
			}
		}
		//x没有移动，判断x_in_set
		else{
			while (x_pre_check_object){
				if (x_pre_check_object->_x < old_min_x){
					break;
				}

				if (check_obj->_is_marker && x_pre_check_object->_is_watcher){
					check_no_move_make_event(check_obj, x_pre_check_object, old_min_x, old_max_x, new_min_x, new_max_x, old_min_y, old_max_y, new_min_y, new_max_y);
				}

				x_pre_check_object = x_pre_check_object->_x_pre;
			}

			while (x_next_check_object){
				if (x_next_check_object->_x > old_max_x){
					break;
				}

				if (check_obj->_is_marker && x_next_check_object->_is_watcher){
					check_no_move_make_event(check_obj, x_next_check_object, old_min_x, old_max_x, new_min_x, new_max_x, old_min_y, old_max_y, new_min_y, new_max_y);
				}

				x_next_check_object = x_next_check_object->_x_next;
			}
		}

		//修改x的位置
		check_obj->_x = x;
		check_obj->_y = y;
	}
}

AOI::EventList& AOI::get_all_events()
{
	return _event_list;
}

void AOI::clear_events()
{
	_event_list.clear();
}

AOI::ObjIDList& AOI::get_all_around_ids(uint64_t check_obj_id)
{
	_all_around_ids.clear();

	//得到符合要求数据
	CheckObjMapIt it_check_obj = _check_obj_map.find(check_obj_id);
	if (it_check_obj != _check_obj_map.end() && !it_check_obj->second->_is_org){
		CheckObj* check_obj = it_check_obj->second;
		int x_min = check_obj->_x - check_obj->_x_len;
		int x_max = check_obj->_x + check_obj->_x_len;
		int y_min = check_obj->_y - check_obj->_y_len;
		int y_max = check_obj->_y + check_obj->_y_len;

		//左边
		CheckObj* x_pre_check_object = check_obj->_x_pre;
		while (x_pre_check_object){
			if (x_pre_check_object->_x < x_min){
				break;
			}

			if (x_pre_check_object->_y >= y_min && x_pre_check_object->_y <= y_max){
				_all_around_ids.push_back(x_pre_check_object->_id);
			}

			x_pre_check_object = x_pre_check_object->_x_pre;
		}

		CheckObj* x_next_check_object = check_obj->_x_next;
		while (x_next_check_object){
			if (x_next_check_object->_x > x_max){
				break;
			}

			if (x_next_check_object->_y >= y_min && x_next_check_object->_y <= y_max){
				_all_around_ids.push_back(x_next_check_object->_id);
			}

			x_next_check_object = x_next_check_object->_x_next;
		}
	}

	return _all_around_ids;
}

CheckObj* AOI::get_check_object(uint64_t check_obj_id)
{
	CheckObjMapIt it = _check_obj_map.find(check_obj_id);
	if (it != _check_obj_map.end()){
		return it->second;
	}

	return NULL;
}

void AOI::print_x_objs()
{
	for (auto it = _check_obj_list_map.begin(); it != _check_obj_list_map.end(); ++it){
		std::cout << "List ID " << it->first << " = ";

		CheckObj* check_obj = it->second;
		while (check_obj){
			std::cout << " [ ";
			std::cout << " id:" << check_obj->_id;
			std::cout << " x:" << check_obj->_x;
			std::cout << " i_ud:" << check_obj->_i_ud;
			std::cout << " ull_ud1:" << check_obj->_ull_ud1;
			std::cout << " ull_ud2:" << check_obj->_ull_ud2;
			std::cout << " ] ";

			check_obj = check_obj->_x_next;
		}

		std::cout << std::endl;
	}
}


void AOI::print_all_events()
{
	std::cout << "Event : ";
	for (EventListIt it = _event_list.begin(); it != _event_list.end(); ++it){
		Event *ev = *it;
		std::cout << " [ ";
		std::cout << " maker_id:" << ev->_marker_id;
		std::cout << " watcher_id:" << ev->_watcher_id;
		if (ev->_event == 0){
			std::cout << " ENTER ";
		}
		else if (ev->_event == 1){
			std::cout << " LEAVE ";
		}
		else if (ev->_event == 2){
			std::cout << " IN ";
		}
		else if (ev->_event == 3){
			std::cout << " ADD ";
		}
		else if (ev->_event == 4){
			std::cout << " REMOVE ";
		}

		std::cout << " ] ";
		delete ev;
	}

	std::cout << std::endl;

	_event_list.clear();
}


void AOI::make_event(CheckObj* marker, CheckObj* watcher, EventType ev_type)
{
	Event *ev = new Event;
	ev->_check_list_id = marker->_check_list_id;
	ev->_event = ev_type;
	ev->_marker_id = marker->_id;
	ev->_watcher_id = watcher->_id;
	ev->_marker_i_ud = marker->_i_ud;
	ev->_marker_ull_ud1 = marker->_ull_ud1;
	ev->_marker_ull_ud2 = marker->_ull_ud2;
	ev->_watcher_i_ud = watcher->_i_ud;
	ev->_watcher_ull_ud1 = watcher->_ull_ud1;
	ev->_watcher_ull_ud2 = watcher->_ull_ud2;

	_event_list.push_back(ev);
}

void AOI::obj_create_event(CheckObj *aoi_obj, AOIEventType event_type)
{
	// 1. get range
	// 2. check pre aoi obj
	// 3. check next aoi obj

	// 1. get range
	int x_min = aoi_obj->_x - aoi_obj->_x_len;
	int x_max = aoi_obj->_x + aoi_obj->_x_len;
	int y_min = aoi_obj->_y - aoi_obj->_y_len;
	int y_max = aoi_obj->_y + aoi_obj->_y_len;

	CheckObj* point_aoi_obj = nullptr;
	
	// 2. check pre aoi obj
	point_aoi_obj = aoi_obj->_x_pre;
	while (point_aoi_obj)
	{
		if (point_aoi_obj->_x < x_min)
		{
			// x over
			break;
		}

		// check if y in range, make add event
		if (point_aoi_obj->_y >= y_min && point_aoi_obj->_y <= y_max)
		{
			make_event(aoi_obj, point_aoi_obj, event_type);
		}

		point_aoi_obj = point_aoi_obj->_x_pre;
	}

	// 3. check next aoi obj
	point_aoi_obj = aoi_obj->_x_next;
	while (point_aoi_obj)
	{
		if (point_aoi_obj->_x > x_max)
		{
			// x over
			break;
		}

		// check if y in range, make add event
		if (point_aoi_obj->_y >= y_min && point_aoi_obj->_y <= y_max)
		{
			make_event(aoi_obj, point_aoi_obj, event_type);
		}
		point_aoi_obj = point_aoi_obj->_x_next;
	}
}

void AOI::check_left_move_make_event(CheckObj* check_object, CheckObj* x_check_object, int old_min_x, int old_max_x, int new_min_x, int new_max_x, int old_min_y, int old_max_y, int new_min_y, int new_max_y)
{
	//离开范围
	if (x_check_object->_x > new_max_x && x_check_object->_x >= old_min_x && x_check_object->_x <= old_max_x){
		//判断y坐标，如果以前在y的范围内，则表示是离开
		if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y){
			make_event(check_object, x_check_object, EventType::LEAVE);
		}
	}
	//一直在范围内
	else if (x_check_object->_x >= new_min_x && x_check_object->_x <= new_max_x && x_check_object->_x >= old_min_x && x_check_object->_x <= old_max_x){
		//同时在新旧区域内
		if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y && x_check_object->_y >= new_min_y &&x_check_object->_y <= new_max_y){
			make_event(check_object, x_check_object, EventType::IN);
		}
		//以前在旧区域里，则表示是离开
		else if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y){
			make_event(check_object, x_check_object, EventType::LEAVE);
		}
		//现在在新区域里，则表示是进入
		else if (x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y){
			make_event(check_object, x_check_object, EventType::ENTER);
		}
	}
	//进入范围
	else if (x_check_object->_x >= new_min_x && x_check_object->_x <= new_max_x &&  !(x_check_object->_x >= old_min_x && x_check_object->_x <= old_max_x)){
		//在新范围内
		if (x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y){
			make_event(check_object, x_check_object, EventType::ENTER);
		}
	}
}

void AOI::check_right_move_make_event(CheckObj* check_object, CheckObj* x_check_object, int old_min_x, int old_max_x, int new_min_x, int new_max_x, int old_min_y, int old_max_y, int new_min_y, int new_max_y)
{
	//离开范围
	if (x_check_object->_x < new_min_x && x_check_object->_x >= old_min_x && x_check_object->_x <= old_max_x){
		//判断y坐标，如果以前在y的范围内，则表示是离开
		if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y){
			make_event(check_object, x_check_object, EventType::LEAVE);
		}
	}
	//一直在范围内
	else if (x_check_object->_x >= new_min_x && x_check_object->_x <= new_max_x && x_check_object->_x >= old_min_x && x_check_object->_x <= old_max_x){
		//同时在新旧区域内
		if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y && x_check_object->_y >= new_min_y &&x_check_object->_y <= new_max_y){
			make_event(check_object, x_check_object, EventType::IN);
		}
		//以前在旧区域里，则表示是离开
		else if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y){
			make_event(check_object, x_check_object, EventType::LEAVE);
		}
		//现在在新区域里，则表示是进入
		else	if (x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y){
			make_event(check_object, x_check_object, EventType::ENTER);
		}
	}
	//进入范围
	else if (x_check_object->_x >= new_min_x && x_check_object->_x <= new_max_x && !(x_check_object->_x >= old_min_x && x_check_object->_x <= old_max_x)){
		//在新范围内
		if (x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y){
			make_event(check_object, x_check_object, EventType::ENTER);
		}
	}
}

void AOI::check_no_move_make_event(CheckObj* check_object, CheckObj* x_check_object, int old_min_x, int old_max_x, int new_min_x, int new_max_x, int old_min_y, int old_max_y, int new_min_y, int new_max_y)
{
	//如果原来y在范围内，现在不在新的y范围内，则表示离开
	if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y && !(x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y)){
		make_event(check_object, x_check_object, EventType::LEAVE);
	}
	//原来在，现在也在，则表示一直都在
	else if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y && x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y){
		make_event(check_object, x_check_object, EventType::IN);
	}
	//原来不在，现在在，则表示进入
	else if (!(x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y) && x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y){
		make_event(check_object, x_check_object, EventType::ENTER);
	}
}

