
#pragma once

#include <stdint.h>
#include <unordered_map>
#include <list>
#include <vector>

namespace AOI_X2_SPACE
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
	AOIEventType _event; 
	uint64_t _marker_aoi_id;
	uint64_t _watcher_aoi_id;
	uint64_t _marker_entity_id;
	uint64_t _watcher_entity_id;
};

class AOIObj
{
public:
	uint64_t _aoi_id; // aoi unique id 
	uint64_t _entity_id;
	
	uint32_t _x_aoi_len;
	uint32_t _y_aoi_len;
	int _x;
	int _y;
	AOIObj* _x_pre;
	AOIObj* _x_next;

	bool _is_org;
	bool _is_watcher;
	bool _is_marker;
};

class AOI
{
public:
	typedef std::list<Event*> EventList;
	typedef std::vector<uint64_t> AOIIdList;
	
	AOI(uint32_t aoi_x_len, uint32_t aoi_y_len);
	~AOI();

	// return a aoi_id
	bool add(int x, int y, bool is_marker, bool is_watcher, uint64_t& out_aoi_id, uint64_t entity_id);
	void remove(uint64_t aoi_id);
	void move(uint64_t aoi_id, int x, int y);

	AOI::EventList& get_all_events();
	void clear_events();

	AOI::AOIIdList get_all_around_ids(uint64_t aoi_id);
	AOIObj* get_aoi_obj(uint64_t aoi_id);

	void print_x_objs();
	void print_all_events();

private:
	AOI::AOIIdList get_around_obj_id(AOIObj *aoi_obj, bool do_sort = false);
	std::vector<AOIObj *> get_around_obj(AOIObj *aoi_obj, bool do_sort = false);

	void make_event(AOIObj* marker, AOIObj* watcher, AOIEventType ev_type);
	void obj_create_event(AOIObj *aoi_obj, AOIEventType event_type);

	uint64_t m_cur_aoi_id;
	uint32_t m_aoi_x_len;
	uint32_t m_aoi_y_len;

	AOIObj *m_head_aoi_obj;

	typedef std::unordered_map<uint64_t, AOIObj*> AOIObjMap;
	AOIObjMap m_aoi_obj_map;

	EventList m_event_list;
};

};
