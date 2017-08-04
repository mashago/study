#pragma  once

//使用十字链表实现2维Area Of Interest 
//备注：实现后发现，只要在x维度上比较y的值就能判断是否有交集，所以修改为单个的x双向链表即可实现

#include <stdint.h>
#include <unordered_map>
#include <list>

//事件类型
enum EventType{
	ENTER = 0, //进入
	LEAVE, //离开
	IN, //一直在里面
	ADD, //增加
	REMOVE,//删除
};

//触发事件
class Event {
public:
	uint64_t _check_list_id; //检测列表的id
	uint64_t _marker_id; //触发者
	uint64_t _watcher_id; //观察者
	EventType _event; //事件 => 触发者 进入/立刻/移动 观察者 范围，要将触发者消息告诉观察者
	int _marker_i_ud;
	int _watcher_i_ud;
	uint64_t _marker_ull_ud1;
	uint64_t _watcher_ull_ud1;
	uint64_t _marker_ull_ud2;
	uint64_t _watcher_ull_ud2;
};

//每在一个检测列表中就创建一个该对象
class CheckObj {
public:
	uint64_t _id; //区别标记每个对象
	int _i_ud; //用户数据
	uint64_t _ull_ud1; //用户数据
	uint64_t _ull_ud2; //用户数据
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
	uint64_t _check_list_id; //检测列表的id
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
	int make_check_list(uint32_t x_len, uint32_t y_len);

	//加入AOI中的检测列表中，并返回一个唯一id
	bool add(int check_list_id, int x, int y, bool is_marker, bool is_watcher, uint64_t& out_check_obj_id, int i_ud = 0, uint64_t ull_ud1 = 0, uint64_t ull_ud2 = 0);

	//移除AOI
	void remove(uint64_t check_obj_id);

	//移动(可以在业务层决定x/y如果是少量移动的时候不进行触发)
	void move(uint64_t check_obj_id, int x, int y);

	//得到所有的事件
	AOI::EventList& get_all_events();
	void clear_events();

	//得到所有在范围内的id列表
	AOI::ObjIDList& get_all_around_ids(uint64_t check_obj_id);

	//得到对应的对象
	CheckObj* get_check_object(uint64_t check_obj_id);

	//打印所有列表
	void print_x_objs();
	void print_all_events();

private:
	void make_event(CheckObj* marker, CheckObj* watcher, EventType ev_type);

	//测试生成事件
	void check_left_move_make_event(CheckObj* check_object, CheckObj* x_check_object, int old_min_x, int old_max_x, int new_min_x, int new_max_x, int old_min_y, int old_max_y, int new_min_y, int new_max_y);
	void check_right_move_make_event(CheckObj* check_object,CheckObj* x_check_object, int old_min_x, int old_max_x, int new_min_x, int new_max_x, int old_min_y, int old_max_y, int new_min_y, int new_max_y);
	void check_no_move_make_event(CheckObj* check_object, CheckObj* x_check_object, int old_min_x, int old_max_x, int new_min_x, int new_max_x, int old_min_y, int old_max_y, int new_min_y, int new_max_y);

	//检测列表id和x_len/y_len对应
	typedef std::unordered_map<int, std::pair<uint32_t, uint32_t>> CheckListIDMap;
	typedef CheckListIDMap::iterator CheckListIDMapIt;
	CheckListIDMap _check_list_id_map;

	int _cur_check_list_id_creator; //当前检测列表索引生成
	uint64_t _cur_object_id_creator; //当前对象id生成

	//check_list_id, check_obj_list(检测对象列表，首对象为系统默认对象)
	typedef std::unordered_map<int, CheckObj*> CheckObjListMap;
	typedef CheckObjListMap::iterator CheckObjListMapIt;
	CheckObjListMap _check_obj_list_map;

	//check_obj_id, check_obj
	typedef std::unordered_map<uint64_t, CheckObj*> CheckObjMap;
	typedef CheckObjMap::iterator CheckObjMapIt;
	CheckObjMap _check_obj_map;

	EventList _event_list; //事件列表

	std::list<uint64_t> _all_around_ids; //范围内id列表
};

