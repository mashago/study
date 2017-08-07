
#pragma once

#include <stdint.h>
#include <unordered_map>
#include <list>
#include <vector>

namespace AOI_X_SPACE
{

enum AOIEventType
{
	ENTER = 1,
	LEAVE,
	IN,
	ADD,
	REMOVE,
};

enum AOIAxis
{
	X_AXIS = 0,
	Y_AXIS = 1,
	Z_AXIS = 2,
};

class Event 
{
public:
	AOIEventType _event; //事件 => 触发者 进入/立刻/移动 观察者 范围，要将触发者消息告诉观察者
	uint64_t _marker_aoi_id; //触发者
	uint64_t _watcher_aoi_id; //观察者
	uint64_t _marker_entity_id;
	uint64_t _watcher_entity_id;
};

class CheckObj
{
public:
	uint64_t _aoi_id; // aoi unique id //区别标记每个对象
	uint64_t _entity_id;

	/*
	uint32_t _x_len; //检测x的长度
	uint32_t _y_len; //检测y的长度
	int _x; //x位置
	int _y; //y位置

	CheckObj* _x_pre; //前一个
	CheckObj* _x_next; //后一个
	CheckObj* _y_pre; //前一个
	CheckObj* _y_next; //后一个
	*/
	
	// 0 for x, 1 for y
	uint32_t _aoi_len_array[2];
	int _pos_array[2];
	CheckObj* _pre_array[2];
	CheckObj* _next_array[2];

	bool _is_org; //是否原点
	bool _is_watcher; //是否观察者
	bool _is_marker; //是否触发者
};

class AOI
{
public:
	typedef std::list<Event*> EventList;
	typedef EventList::iterator EventListIt;
	typedef std::vector<uint64_t> ObjIDList;
	typedef ObjIDList::iterator ObjIDListIt;
	
	AOI(uint32_t aoi_x_len, uint32_t aoi_y_len);
	~AOI();

	//加入AOI中的检测列表中，并返回一个唯一id
	bool add(int x, int y, bool is_marker, bool is_watcher, uint64_t& out_aoi_id, uint64_t entity_id);

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
	CheckObj* get_aoi_obj(uint64_t aoi_id);

	//打印所有列表
	void print_x_objs();
	void print_y_objs();
	void print_all_events();

private:
	std::vector<uint64_t> get_around_obj_vec(CheckObj *aoi_obj);

	void make_event(CheckObj* marker, CheckObj* watcher, AOIEventType ev_type);
	void obj_create_event(CheckObj *aoi_obj, AOIEventType event_type);

	uint64_t m_cur_aoi_id;
	uint32_t m_aoi_x_len;
	uint32_t m_aoi_y_len;

	CheckObj *m_head_aoi_obj;

	//check_obj_id, check_obj
	typedef std::unordered_map<uint64_t, CheckObj*> CheckObjMap;
	typedef CheckObjMap::iterator CheckObjMapIt;
	CheckObjMap _check_obj_map;

	EventList _event_list; //事件列表

	std::vector<uint64_t> _all_around_ids; //范围内id列表
};

};
