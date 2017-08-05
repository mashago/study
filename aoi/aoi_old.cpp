#include <algorithm>
#include <iterator>
#include <iostream>
#include "aoi_old.h"

AOI::AOI()
	:_cur_check_list_id_creator(0)
	, _cur_object_id_creator(0)
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

int AOI::make_check_list(uint32_t x_len, uint32_t y_len)
{
	int check_list_id = _cur_check_list_id_creator;

	_check_list_id_map[check_list_id] = std::make_pair(x_len, y_len);
	_check_obj_list_map[check_list_id] = NULL;

	//����x�ļ���б�����һ��ԭʼ�ڵ�
	uint64_t out_check_obj_id;
	add(check_list_id, -1999999999, -1999999999, false, false, out_check_obj_id);

	++_cur_check_list_id_creator;

	return check_list_id;
}

bool AOI::add(int check_list_id, int x, int y, bool is_marker, bool is_watcher, uint64_t& out_check_obj_id, int i_ud, uint64_t ull_ud1, uint64_t ull_ud2)
{
	CheckListIDMapIt it_check_list_id = _check_list_id_map.find(check_list_id);
	if (it_check_list_id == _check_list_id_map.end()){
		return false;
	}

	//���ݾ���õ�ǰ���ϵ
	CheckObjListMapIt it = _check_obj_list_map.find(check_list_id);
	if (it != _check_obj_list_map.end()){
		uint32_t x_len = it_check_list_id->second.first;
		uint32_t y_len = it_check_list_id->second.second;

		out_check_obj_id = _cur_object_id_creator;

		CheckObj* check_obj = new CheckObj();
		check_obj->_id = _cur_object_id_creator;
		check_obj->_i_ud = i_ud;
		check_obj->_ull_ud1 = ull_ud1;
		check_obj->_ull_ud2 = ull_ud2;
		check_obj->_x_len = x_len;
		check_obj->_y_len = y_len;
		check_obj->_x = x;
		check_obj->_y = y;
		check_obj->_is_watcher = is_watcher;
		check_obj->_is_marker = is_marker;
		check_obj->_check_list_id = check_list_id;
		check_obj->_org = it->second;
		check_obj->_is_org = false;
		check_obj->_x_pre = NULL;
		check_obj->_x_next = NULL;

		//��¼��map��
		_check_obj_map[check_obj->_id] = check_obj;

		//������׽ڵ�
		if (it->second == NULL){
			it->second = check_obj;
			check_obj->_is_org = true;
		}
		else{
			//�б�x/y��С��������
			CheckObj* test_x_check_obj = it->second->_x_next; //�������ԭʼ�ڵ����һ���ڵ�

			//����x�б�
			if (test_x_check_obj){
				while (test_x_check_obj){
					//һֱ�ҵ�һ�����ڻ��ߵ����Լ��ģ�Ȼ�������ǰ��
					if (check_obj->_x <= test_x_check_obj->_x){
						check_obj->_x_pre = test_x_check_obj->_x_pre;
						check_obj->_x_pre->_x_next = check_obj;

						check_obj->_x_next = test_x_check_obj;
						test_x_check_obj->_x_pre = check_obj;

						break;
					}

					if (!test_x_check_obj->_x_next){
						check_obj->_x_pre = test_x_check_obj;
						test_x_check_obj->_x_next = check_obj;
						break;
					}

					test_x_check_obj = test_x_check_obj->_x_next;
				}
			}
			else{
				check_obj->_x_pre = it->second;
				it->second->_x_next = check_obj;
			}
		}

		//�õ������¼�
		CheckObj* x_pre_check_object = check_obj->_x_pre;
		CheckObj* x_next_check_object = check_obj->_x_next;
		int x_min = x - x_len;
		int x_max = x + x_len;
		int y_min = y - y_len;
		int y_max = y + y_len;

		while (x_pre_check_object){
			if (x_pre_check_object->_x < x_min){
				break;
			}

			//�ж�y�Ƿ��ڷ�Χ�ڣ�����ڣ����ʾ�õ���Enter�ķ�Χ��
			if (x_pre_check_object->_y >= y_min && x_pre_check_object->_y <= y_max){
				make_event(check_obj, x_pre_check_object, EventType::ADD);
			}

			x_pre_check_object = x_pre_check_object->_x_pre;
		}

		while (x_next_check_object){
			if (x_next_check_object->_x > x_max){
				break;
			}

			//�ж�y�Ƿ��ڷ�Χ�ڣ�����ڣ����ʾ�õ���Enter�ķ�Χ��
			if (x_next_check_object->_y >= y_min && x_next_check_object->_y <= y_max){
				make_event(check_obj, x_next_check_object, EventType::ADD);
			}
			x_next_check_object = x_next_check_object->_x_next;
		}

		++_cur_object_id_creator;
		return true;
	}

	return false;
}

void AOI::remove(uint64_t check_obj_id)
{
	CheckObjMapIt it = _check_obj_map.find(check_obj_id);
	if (it != _check_obj_map.end() && !it->second->_is_org){
		//�Է��������Ľ���֪ͨ
		CheckObj* check_obj = it->second;

		CheckObj* x_pre_check_object = check_obj->_x_pre;
		CheckObj* x_next_check_object = check_obj->_x_next;
		int x_min = check_obj->_x - check_obj->_x_len;
		int x_max = check_obj->_x + check_obj->_x_len;
		int y_min = check_obj->_y - check_obj->_y_len;
		int y_max = check_obj->_y + check_obj->_y_len;

		while (x_pre_check_object){
			if (x_pre_check_object->_x < x_min){
				break;
			}

			//�ж�y�Ƿ��ڷ�Χ�ڣ�����ڣ����ʾ�õ���Enter�ķ�Χ��
			if (x_pre_check_object->_y >= y_min && x_pre_check_object->_y <= y_max){
				make_event(check_obj, x_pre_check_object, EventType::REMOVE);
			}

			x_pre_check_object = x_pre_check_object->_x_pre;
		}

		while (x_next_check_object){
			if (x_next_check_object->_x > x_max){
				break;
			}

			//�ж�y�Ƿ��ڷ�Χ�ڣ�����ڣ����ʾ�õ���Enter�ķ�Χ��
			if (x_next_check_object->_y >= y_min && x_next_check_object->_y <= y_max){
				make_event(check_obj, x_next_check_object, EventType::REMOVE);
			}
			x_next_check_object = x_next_check_object->_x_next;
		}


		//����x/y��ǰ���ϵ
		check_obj->_x_pre->_x_next = check_obj->_x_next;
		if (check_obj->_x_next){
			check_obj->_x_next->_x_pre = check_obj->_x_pre;
		}
		delete check_obj;
		_check_obj_map.erase(it);
	}
}

void AOI::move(uint64_t check_obj_id, int x, int y)
{
	CheckObjMapIt it = _check_obj_map.find(check_obj_id);
	if (it != _check_obj_map.end() && !it->second->_is_org){
		CheckObj* check_obj = it->second;

		//�����ƶ��ж�
		uint64_t marker_id = check_obj->_id;
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

		if (old_x != new_x){
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
	for (CheckObjListMapIt it = _check_obj_list_map.begin(); it != _check_obj_list_map.end(); ++it){
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

void AOI::check_left_move_make_event(CheckObj* check_object, CheckObj* x_check_object, int old_min_x, int old_max_x, int new_min_x, int new_max_x, int old_min_y, int old_max_y, int new_min_y, int new_max_y)
{
	//�뿪��Χ
	if (x_check_object->_x > new_max_x && x_check_object->_x >= old_min_x && x_check_object->_x <= old_max_x){
		//�ж�y���꣬�����ǰ��y�ķ�Χ�ڣ����ʾ���뿪
		if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y){
			make_event(check_object, x_check_object, EventType::LEAVE);
		}
	}
	//һֱ�ڷ�Χ��
	else if (x_check_object->_x >= new_min_x && x_check_object->_x <= new_max_x && x_check_object->_x >= old_min_x && x_check_object->_x <= old_max_x){
		//ͬʱ���¾�������
		if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y && x_check_object->_y >= new_min_y &&x_check_object->_y <= new_max_y){
			make_event(check_object, x_check_object, EventType::IN);
		}
		//��ǰ�ھ���������ʾ���뿪
		else if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y){
			make_event(check_object, x_check_object, EventType::LEAVE);
		}
		//����������������ʾ�ǽ���
		else if (x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y){
			make_event(check_object, x_check_object, EventType::ENTER);
		}
	}
	//���뷶Χ
	else if (x_check_object->_x >= new_min_x && x_check_object->_x <= new_max_x &&  !(x_check_object->_x >= old_min_x && x_check_object->_x <= old_max_x)){
		//���·�Χ��
		if (x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y){
			make_event(check_object, x_check_object, EventType::ENTER);
		}
	}
}

void AOI::check_right_move_make_event(CheckObj* check_object, CheckObj* x_check_object, int old_min_x, int old_max_x, int new_min_x, int new_max_x, int old_min_y, int old_max_y, int new_min_y, int new_max_y)
{
	//�뿪��Χ
	if (x_check_object->_x < new_min_x && x_check_object->_x >= old_min_x && x_check_object->_x <= old_max_x){
		//�ж�y���꣬�����ǰ��y�ķ�Χ�ڣ����ʾ���뿪
		if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y){
			make_event(check_object, x_check_object, EventType::LEAVE);
		}
	}
	//һֱ�ڷ�Χ��
	else if (x_check_object->_x >= new_min_x && x_check_object->_x <= new_max_x && x_check_object->_x >= old_min_x && x_check_object->_x <= old_max_x){
		//ͬʱ���¾�������
		if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y && x_check_object->_y >= new_min_y &&x_check_object->_y <= new_max_y){
			make_event(check_object, x_check_object, EventType::IN);
		}
		//��ǰ�ھ���������ʾ���뿪
		else if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y){
			make_event(check_object, x_check_object, EventType::LEAVE);
		}
		//����������������ʾ�ǽ���
		else	if (x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y){
			make_event(check_object, x_check_object, EventType::ENTER);
		}
	}
	//���뷶Χ
	else if (x_check_object->_x >= new_min_x && x_check_object->_x <= new_max_x && !(x_check_object->_x >= old_min_x && x_check_object->_x <= old_max_x)){
		//���·�Χ��
		if (x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y){
			make_event(check_object, x_check_object, EventType::ENTER);
		}
	}
}

void AOI::check_no_move_make_event(CheckObj* check_object, CheckObj* x_check_object, int old_min_x, int old_max_x, int new_min_x, int new_max_x, int old_min_y, int old_max_y, int new_min_y, int new_max_y)
{
	//���ԭ��y�ڷ�Χ�ڣ����ڲ����µ�y��Χ�ڣ����ʾ�뿪
	if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y && !(x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y)){
		make_event(check_object, x_check_object, EventType::LEAVE);
	}
	//ԭ���ڣ�����Ҳ�ڣ����ʾһֱ����
	else if (x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y && x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y){
		make_event(check_object, x_check_object, EventType::IN);
	}
	//ԭ�����ڣ������ڣ����ʾ����
	else if (!(x_check_object->_y >= old_min_y && x_check_object->_y <= old_max_y) && x_check_object->_y >= new_min_y && x_check_object->_y <= new_max_y){
		make_event(check_object, x_check_object, EventType::ENTER);
	}
}

