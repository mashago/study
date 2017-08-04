
#pragma once

#include <stdint.h>

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
