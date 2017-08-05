
#pragma once

#include <stdint.h>
#include <unordered_map>
#include <list>

enum AOIEventType
{
	ENTER = 1, //进入
	LEAVE, //离开
	IN, //一直在里面
	ADD, //增加
	REMOVE,//删除
};

class Event 
{
public:
	AOIEventType _event; //事件 => 触发者 进入/立刻/移动 观察者 范围，要将触发者消息告诉观察者
	uint64_t _check_list_id; //检测列表的id
	uint64_t _marker_aoi_id; //触发者
	uint64_t _watcher_aoi_id; //观察者
	uint64_t _marker_entity_id;
	uint64_t _watcher_entity_id;
};

class CheckObj {
public:
	uint64_t _aoi_id; // aoi unique id //区别标记每个对象
	uint64_t _entity_id;
	uint64_t _check_list_id; //检测列表的id

	uint32_t _x_len; //检测x的长度
	uint32_t _y_len; //检测y的长度
	int _x; //x位置
	int _y; //y位置
	CheckObj* _x_pre; //前一个
	CheckObj* _x_next; //后一个
	CheckObj* _org; //原始节点

	bool _is_org; //是否原点
	bool _is_watcher; //是否观察者
	bool _is_marker; //是否触发者
};

class AOI
{
public:
	typedef std::list<Event*> EventList;
	typedef EventList::iterator EventListIt;
	typedef std::list<uint64_t> ObjIDList;
	typedef ObjIDList::iterator ObjIDListIt;
	
	//构造函数
	AOI();
	~AOI();

	//创建一个检测列表，返回对应的检测列表id(根据x_len和y_len进行分组检测)
	int make_check_list(uint32_t aoi_x_len, uint32_t aoi_y_len);

	//加入AOI中的检测列表中，并返回一个唯一id
	bool add(int check_list_id, int x, int y, bool is_marker, bool is_watcher, uint64_t& out_aoi_id, uint64_t entity_id)

	//移除AOI
	void remove(uint64_t aoi_id);

	//移动(可以在业务层决定x/y如果是少量移动的时候不进行触发)
	void move(uint64_t aoi_id, int x, int y);

	//得到所有的事件
	AOI::EventList& get_all_events();
	void clear_events();

	//得到所有在范围内的id列表
	AOI::ObjIDList& get_all_around_ids(uint64_t aoi_id);

	//得到对应的对象
	CheckObj* get_check_object(uint64_t aoi_id);

	//打印所有列表
	void print_x_objs();
	void print_all_events();

private:
	void make_event(CheckObj* marker, CheckObj* watcher, EventType ev_type);
	void obj_create_event(CheckObj *aoi_obj, AOIEventType event_type);

	//测试生成事件
	void check_left_move_make_event(CheckObj* check_object, CheckObj* x_check_object, int old_min_x, int old_max_x, int new_min_x, int new_max_x, int old_min_y, int old_max_y, int new_min_y, int new_max_y);
	void check_right_move_make_event(CheckObj* check_object,CheckObj* x_check_object, int old_min_x, int old_max_x, int new_min_x, int new_max_x, int old_min_y, int old_max_y, int new_min_y, int new_max_y);
	void check_no_move_make_event(CheckObj* check_object, CheckObj* x_check_object, int old_min_x, int old_max_x, int new_min_x, int new_max_x, int old_min_y, int old_max_y, int new_min_y, int new_max_y);

	//检测列表id和x_len/y_len对应
	typedef std::unordered_map<int, std::pair<uint32_t, uint32_t>> CheckListIDMap;
	// typedef CheckListIDMap::iterator CheckListIDMapIt;
	CheckListIDMap _check_list_id_map;

	int _cur_check_list_id; //当前检测列表索引生成
	uint64_t _cur_aoi_id; //当前对象id生成

	//check_list_id, check_obj_list(检测对象列表，首对象为系统默认对象)
	typedef std::unordered_map<int, CheckObj*> CheckObjListMap;
	// typedef CheckObjListMap::iterator CheckObjListMapIt;
	CheckObjListMap _check_obj_list_map;

	//check_obj_id, check_obj
	typedef std::unordered_map<uint64_t, CheckObj*> CheckObjMap;
	typedef CheckObjMap::iterator CheckObjMapIt;
	CheckObjMap _check_obj_map;

	EventList _event_list; //事件列表

	std::list<uint64_t> _all_around_ids; //范围内id列表
};
