#pragma  once

//ʹ��ʮ������ʵ��2άArea Of Interest 
//��ע��ʵ�ֺ��֣�ֻҪ��xά���ϱȽ�y��ֵ�����ж��Ƿ��н����������޸�Ϊ������x˫��������ʵ��

#include <stdint.h>
#include <unordered_map>
#include <list>

//�¼�����
enum EventType{
	ENTER = 0, //����
	LEAVE, //�뿪
	IN, //һֱ������
	ADD, //����
	REMOVE,//ɾ��
};

//�����¼�
class Event {
public:
	uint64_t _check_list_id; //����б��id
	uint64_t _marker_id; //������
	uint64_t _watcher_id; //�۲���
	EventType _event; //�¼� => ������ ����/����/�ƶ� �۲��� ��Χ��Ҫ����������Ϣ���߹۲���
	int _marker_i_ud;
	int _watcher_i_ud;
	uint64_t _marker_ull_ud1;
	uint64_t _watcher_ull_ud1;
	uint64_t _marker_ull_ud2;
	uint64_t _watcher_ull_ud2;
};

//ÿ��һ������б��оʹ���һ���ö���
class CheckObj {
public:
	uint64_t _id; //������ÿ������
	int _i_ud; //�û�����
	uint64_t _ull_ud1; //�û�����
	uint64_t _ull_ud2; //�û�����
	uint32_t _x_len; //���x�ĳ���
	uint32_t _y_len; //���y�ĳ���
	int _x; //xλ��
	int _y; //yλ��
	CheckObj* _x_pre; //ǰһ��
	CheckObj* _x_next; //��һ��
	CheckObj* _org; //ԭʼ�ڵ�
	bool _is_org; //�Ƿ�ԭ��
	bool _is_watcher; //�Ƿ�۲���
	bool _is_marker; //�Ƿ񴥷���
	uint64_t _check_list_id; //����б��id
};

class AOI
{
public:
	typedef std::list<Event*> EventList;
	typedef EventList::iterator EventListIt;
	typedef std::list<uint64_t> ObjIDList;
	typedef ObjIDList::iterator ObjIDListIt;
	
	//���캯��
	AOI();
	~AOI();

	//����һ������б����ض�Ӧ�ļ���б�id(����x_len��y_len���з�����)
	int make_check_list(uint32_t x_len, uint32_t y_len);

	//����AOI�еļ���б��У�������һ��Ψһid
	bool add(int check_list_id, int x, int y, bool is_marker, bool is_watcher, uint64_t& out_check_obj_id, int i_ud = 0, uint64_t ull_ud1 = 0, uint64_t ull_ud2 = 0);

	//�Ƴ�AOI
	void remove(uint64_t check_obj_id);

	//�ƶ�(������ҵ������x/y����������ƶ���ʱ�򲻽��д���)
	void move(uint64_t check_obj_id, int x, int y);

	//�õ����е��¼�
	AOI::EventList& get_all_events();
	void clear_events();

	//�õ������ڷ�Χ�ڵ�id�б�
	AOI::ObjIDList& get_all_around_ids(uint64_t check_obj_id);

	//�õ���Ӧ�Ķ���
	CheckObj* get_check_object(uint64_t check_obj_id);

	//��ӡ�����б�
	void print_x_objs();
	void print_all_events();

private:
	void make_event(CheckObj* marker, CheckObj* watcher, EventType ev_type);

	//���������¼�
	void check_left_move_make_event(CheckObj* check_object, CheckObj* x_check_object, int old_min_x, int old_max_x, int new_min_x, int new_max_x, int old_min_y, int old_max_y, int new_min_y, int new_max_y);
	void check_right_move_make_event(CheckObj* check_object,CheckObj* x_check_object, int old_min_x, int old_max_x, int new_min_x, int new_max_x, int old_min_y, int old_max_y, int new_min_y, int new_max_y);
	void check_no_move_make_event(CheckObj* check_object, CheckObj* x_check_object, int old_min_x, int old_max_x, int new_min_x, int new_max_x, int old_min_y, int old_max_y, int new_min_y, int new_max_y);

	//����б�id��x_len/y_len��Ӧ
	typedef std::unordered_map<int, std::pair<uint32_t, uint32_t>> CheckListIDMap;
	typedef CheckListIDMap::iterator CheckListIDMapIt;
	CheckListIDMap _check_list_id_map;

	int _cur_check_list_id_creator; //��ǰ����б���������
	uint64_t _cur_object_id_creator; //��ǰ����id����

	//check_list_id, check_obj_list(�������б��׶���ΪϵͳĬ�϶���)
	typedef std::unordered_map<int, CheckObj*> CheckObjListMap;
	typedef CheckObjListMap::iterator CheckObjListMapIt;
	CheckObjListMap _check_obj_list_map;

	//check_obj_id, check_obj
	typedef std::unordered_map<uint64_t, CheckObj*> CheckObjMap;
	typedef CheckObjMap::iterator CheckObjMapIt;
	CheckObjMap _check_obj_map;

	EventList _event_list; //�¼��б�

	std::list<uint64_t> _all_around_ids; //��Χ��id�б�
};

