#include <algorithm>
#include <iterator>
#include <iostream>
#include "aoi_x.h"

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

	/*
	aoi_obj->_x_len = m_aoi_x_len;
	aoi_obj->_y_len = m_aoi_y_len;
	aoi_obj->_x = x;
	aoi_obj->_y = y;

	aoi_obj->_x_pre = NULL;
	aoi_obj->_x_next = NULL;
	aoi_obj->_y_pre = NULL;
	aoi_obj->_y_next = NULL;
	*/
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
		// XXX should will return?
	}
	else
	{
		auto add_aoi_obj = [](CheckObj *aoi_obj, AOIAxis axis)
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
				if (aoi_obj->_x <= next_aoi_obj->_x)
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
		add_aoi_obj(aoi_obj, AOIAxis::X_AXIS);
		add_aoi_obj(aoi_obj, AOIAxis::Y_AXIS);
	}

	// create add event
	obj_create_event(aoi_obj, AOIEventType::ADD);

	return true;
}

void AOI::remove(uint64_t aoi_id)
{
	CheckObjMapIt it = _check_obj_map.find(aoi_id);
	if (it == _check_obj_map.end())
	{
		printf("AOI::remove aoi obj not exists %llu\n", aoi_id);
		return;
	}

	if (it->second->_is_org)
	{
		printf("AOI::remove head aoi obj %llu\n", aoi_id);
		return;
	}

	CheckObj *aoi_obj = it->second;

	// create remove event
	obj_create_event(aoi_obj, AOIEventType::REMOVE);

	// fix pre and next
	aoi_obj->_x_pre->_x_next = aoi_obj->_x_next;
	if (aoi_obj->_x_next){
		aoi_obj->_x_next->_x_pre = aoi_obj->_x_pre;
	}

	// cleanup
	delete aoi_obj;
	_check_obj_map.erase(it);
}

void AOI::move(uint64_t aoi_id, int x, int y)
{
	CheckObjMapIt it = _check_obj_map.find(aoi_id);
	if (it == _check_obj_map.end())
	{
		printf("AOI::move aoi obj not exists %llu\n", aoi_id);
		return;
	}

	if (it->second->_is_org)
	{
		printf("AOI::move head aoi obj %llu\n", aoi_id);
		return;
	}

	CheckObj* check_obj = it->second;

	//�����ƶ��ж�
	// uint64_t marker_id = check_obj->_aoi_id;
	uint32_t check_x_len = check_obj->_x_len;
	uint32_t check_y_len = check_obj->_y_len;

	//���������ƻ��������ƶ����õ��������ϣ��뿪�۲췶Χ��һֱ�ڷ�Χ�ڣ����뷶Χ��������
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

	if (old_x != new_x)
	{
		CheckObj* x_insert_pos_object = NULL; //�ҵ������λ��

		//���ƶ�(new_min_x<old_min_x)
		if (x < old_x){
			//��ǰ���һ��
			CheckObj* x_head_pos_object = NULL;

			//�ƶ�������ǰλ����ߵķ�Χ�ڵ���д���
			while (x_pre_check_object){
				//���Ҳ����λ��(���뵽����ڵ�ĺ���next)
				if (!x_insert_pos_object && x_pre_check_object->_x <= x){
					x_insert_pos_object = x_pre_check_object;
				}

				//�Ѿ�С��new_min���˳�
				if (x_pre_check_object->_x < new_min_x){
					break;
				}

				//�ж�������ߣ��õ��뿪�۲췶Χ��һֱ�ڷ�Χ�ڣ����뷶Χ��������
				if (check_obj->_is_marker && x_pre_check_object->_is_watcher){
					check_left_move_make_event(check_obj, x_pre_check_object, old_min_x, old_max_x, new_min_x, new_max_x, old_min_y, old_max_y, new_min_y, new_max_y);
				}

				//��һ��
				x_head_pos_object = x_pre_check_object;
				x_pre_check_object = x_pre_check_object->_x_pre;
			}

			//�����ұ�
			while (x_next_check_object){
				if (!x_head_pos_object){
					x_head_pos_object = x_next_check_object;
				}

				//�Ѿ�С��new_min���˳�
				if (x_next_check_object->_x > old_max_x){
					break;
				}

				//�ж������ұߣ��õ��뿪�۲췶Χ��һֱ�ڷ�Χ�ڣ����뷶Χ��������
				if (check_obj->_is_marker && x_next_check_object->_is_watcher){
					check_left_move_make_event(check_obj, x_next_check_object, old_min_x, old_max_x, new_min_x, new_max_x, old_min_y, old_max_y, new_min_y, new_max_y);
				}

				x_next_check_object = x_next_check_object->_x_next;
			}

			//��insert_pos�ұ߲���
			if (x_insert_pos_object){
				//�Ƚ�check_objǰ���ú�
				check_obj->_x_pre->_x_next = check_obj->_x_next;
				if (check_obj->_x_next){
					check_obj->_x_next->_x_pre = check_obj->_x_pre;
				}

				//�ٲ�����λ��
				check_obj->_x_next = x_insert_pos_object->_x_next;
				if (check_obj->_x_next){
					check_obj->_x_next->_x_pre = check_obj;
				}

				x_insert_pos_object->_x_next = check_obj;
				check_obj->_x_pre = x_insert_pos_object;
			}
			//��head_pos��߲���
			else if (x_head_pos_object){
				//�Ƚ�check_objǰ���ú�
				check_obj->_x_pre->_x_next = check_obj->_x_next;
				if (check_obj->_x_next){
					check_obj->_x_next->_x_pre = check_obj->_x_pre;
				}

				//�ٲ�����λ��
				if (x_head_pos_object->_x_pre){
					x_head_pos_object->_x_pre->_x_next = check_obj;
				}
				check_obj->_x_pre = x_head_pos_object->_x_pre;

				x_head_pos_object->_x_pre = check_obj;
				check_obj->_x_next = x_head_pos_object;
			}
		}
		//���ƶ�(new_max_x>old_max_x)
		else if (x > old_x){
			CheckObj* x_tail_pos_object = NULL; //z��β��һ���ڵ�

			//�ȴ����ұ߲���
			while (x_next_check_object){
				//���Ҳ����λ��(���뵽����ڵ��ǰ��pre)
				if (!x_insert_pos_object && x_next_check_object->_x >= x){
					x_insert_pos_object = x_next_check_object;
				}

				if (x_next_check_object->_x > new_max_x){
					break;
				}

				//�ж������ұߣ��õ��뿪�۲췶Χ��һֱ�ڷ�Χ�ڣ����뷶Χ��������
				if (check_obj->_is_marker && x_next_check_object->_is_watcher){
					check_right_move_make_event(check_obj, x_next_check_object, old_min_x, old_max_x, new_min_x, new_max_x, old_min_y, old_max_y, new_min_y, new_max_y);
				}

				x_tail_pos_object = x_next_check_object;
				x_next_check_object = x_next_check_object->_x_next;
			}

			//������߲���
			while (x_pre_check_object){
				if (!x_tail_pos_object){
					x_tail_pos_object = x_pre_check_object;
				}

				if (x_pre_check_object->_x < old_min_x){
					break;
				}

				//�ж������ұߣ��õ��뿪�۲췶Χ��һֱ�ڷ�Χ�ڣ����뷶Χ��������
				if (check_obj->_is_marker && x_pre_check_object->_is_watcher){
					check_right_move_make_event(check_obj, x_pre_check_object, old_min_x, old_max_x, new_min_x, new_max_x, old_min_y, old_max_y, new_min_y, new_max_y);
				}
				x_pre_check_object = x_pre_check_object->_x_pre;
			}

			//��insert_pos��߲���
			if (x_insert_pos_object){
				//�Ƚ�check_objǰ���ú�
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
			//��tail_pos�ұ߲���
			else if (x_tail_pos_object){
				//�Ƚ�check_objǰ���ú�
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
	//xû���ƶ����ж�x_in_set
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

	//�޸�x��λ��
	check_obj->_x = x;
	check_obj->_y = y;

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

	//�õ�����Ҫ������
	CheckObjMapIt it_check_obj = _check_obj_map.find(check_obj_id);
	if (it_check_obj != _check_obj_map.end() && !it_check_obj->second->_is_org){
		CheckObj* check_obj = it_check_obj->second;
		int x_min = check_obj->_x - check_obj->_x_len;
		int x_max = check_obj->_x + check_obj->_x_len;
		int y_min = check_obj->_y - check_obj->_y_len;
		int y_max = check_obj->_y + check_obj->_y_len;

		//���
		CheckObj* x_pre_check_object = check_obj->_x_pre;
		while (x_pre_check_object){
			if (x_pre_check_object->_x < x_min){
				break;
			}

			if (x_pre_check_object->_y >= y_min && x_pre_check_object->_y <= y_max){
				_all_around_ids.push_back(x_pre_check_object->_aoi_id);
			}

			x_pre_check_object = x_pre_check_object->_x_pre;
		}

		CheckObj* x_next_check_object = check_obj->_x_next;
		while (x_next_check_object){
			if (x_next_check_object->_x > x_max){
				break;
			}

			if (x_next_check_object->_y >= y_min && x_next_check_object->_y <= y_max){
				_all_around_ids.push_back(x_next_check_object->_aoi_id);
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
	CheckObj* check_obj = m_head_aoi_obj;
	while (check_obj){
		std::cout << " [ ";
		std::cout << " id:" << check_obj->_aoi_id;
		std::cout << " x:" << check_obj->_x;
		std::cout << " entity_id:" << check_obj->_entity_id;
		std::cout << " ] ";

		check_obj = check_obj->_x_next;
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
	//�뿪��Χ
	if (x_check_object->_x > new_max_x && x_check_object->_x >= old_min_x && x_check_object->_x <= old_max_x){
		//�ж�y���꣬�����ǰ��y�ķ�Χ�ڣ����ʾ���뿪
		if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y){
			make_event(check_object, x_check_object, AOIEventType::LEAVE);
		}
	}
	//һֱ�ڷ�Χ��
	else if (x_check_object->_x >= new_min_x && x_check_object->_x <= new_max_x && x_check_object->_x >= old_min_x && x_check_object->_x <= old_max_x){
		//ͬʱ���¾�������
		if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y && x_check_object->_y >= new_min_y &&x_check_object->_y <= new_max_y){
			make_event(check_object, x_check_object, AOIEventType::IN);
		}
		//��ǰ�ھ���������ʾ���뿪
		else if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y){
			make_event(check_object, x_check_object, AOIEventType::LEAVE);
		}
		//����������������ʾ�ǽ���
		else if (x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y){
			make_event(check_object, x_check_object, AOIEventType::ENTER);
		}
	}
	//���뷶Χ
	else if (x_check_object->_x >= new_min_x && x_check_object->_x <= new_max_x &&  !(x_check_object->_x >= old_min_x && x_check_object->_x <= old_max_x)){
		//���·�Χ��
		if (x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y){
			make_event(check_object, x_check_object, AOIEventType::ENTER);
		}
	}
}

void AOI::check_right_move_make_event(CheckObj* check_object, CheckObj* x_check_object, int old_min_x, int old_max_x, int new_min_x, int new_max_x, int old_min_y, int old_max_y, int new_min_y, int new_max_y)
{
	//�뿪��Χ
	if (x_check_object->_x < new_min_x && x_check_object->_x >= old_min_x && x_check_object->_x <= old_max_x){
		//�ж�y���꣬�����ǰ��y�ķ�Χ�ڣ����ʾ���뿪
		if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y){
			make_event(check_object, x_check_object, AOIEventType::LEAVE);
		}
	}
	//һֱ�ڷ�Χ��
	else if (x_check_object->_x >= new_min_x && x_check_object->_x <= new_max_x && x_check_object->_x >= old_min_x && x_check_object->_x <= old_max_x){
		//ͬʱ���¾�������
		if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y && x_check_object->_y >= new_min_y &&x_check_object->_y <= new_max_y){
			make_event(check_object, x_check_object, AOIEventType::IN);
		}
		//��ǰ�ھ���������ʾ���뿪
		else if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y){
			make_event(check_object, x_check_object, AOIEventType::LEAVE);
		}
		//����������������ʾ�ǽ���
		else	if (x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y){
			make_event(check_object, x_check_object, AOIEventType::ENTER);
		}
	}
	//���뷶Χ
	else if (x_check_object->_x >= new_min_x && x_check_object->_x <= new_max_x && !(x_check_object->_x >= old_min_x && x_check_object->_x <= old_max_x)){
		//���·�Χ��
		if (x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y){
			make_event(check_object, x_check_object, AOIEventType::ENTER);
		}
	}
}

void AOI::check_no_move_make_event(CheckObj* check_object, CheckObj* x_check_object, int old_min_x, int old_max_x, int new_min_x, int new_max_x, int old_min_y, int old_max_y, int new_min_y, int new_max_y)
{
	//���ԭ��y�ڷ�Χ�ڣ����ڲ����µ�y��Χ�ڣ����ʾ�뿪
	if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y && !(x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y)){
		make_event(check_object, x_check_object, AOIEventType::LEAVE);
	}
	//ԭ���ڣ�����Ҳ�ڣ����ʾһֱ����
	else if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y && x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y){
		make_event(check_object, x_check_object, AOIEventType::IN);
	}
	//ԭ�����ڣ������ڣ����ʾ����
	else if (!(x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y) && x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y){
		make_event(check_object, x_check_object, AOIEventType::ENTER);
	}
}

