// TaskThread tt;
// tt.Start();
// tt.post(&func);
// tt.post([](){});
// tt.post(bind(show, 1, 2));
// tt.post(bind(&Object::show, &obj, 1, 2));
// tt.Stop();
#ifndef TASKTHREAD_H_
#define TASKTHREAD_H_

#include <list>
#include <utility>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>

using namespace std;

/////////////////////////////////////
class TaskOpBase
{
public:
	virtual ~TaskOpBase(){}
	virtual void Execute() = 0;
};

template<typename T>
class TaskOp : public TaskOpBase
{
public:
	typedef typename remove_reference<T>::type TaskType;

	TaskOp(T && _h) :h(forward<T>(_h)){}

	virtual void Execute()
	{
		h();
	}

private:
	TaskType h;
};
/////////////////////////////////////

/////////////////////////////////////
template<typename T>
class SwitchList
{
public:
	typedef list<T> ItemList;

	SwitchList()
	{
	}

	virtual ~SwitchList()
	{
	}

	template<typename TT>
    void Push(TT && task)
	{
        m_pListIn->push_back(forward<TT>(task));
	}

	void Switch()
	{
		swap(m_pListIn, m_pListOut);
	}

	void ClearOut()
	{
		m_pListOut->clear();
	}

	int GetInSize()
	{
		return m_pListIn->size();
	}

	int GetOutSize()
	{
		return m_pListOut->size();
	}

	bool IsInEmpty()
	{
		return m_pListIn->empty();
	}

	bool IsOutEmpty()
	{
		return m_pListOut->empty();
	}

	const ItemList & InList()
	{
		return *m_pListIn;
	}

	const ItemList & OutList()
	{
		return *m_pListOut;
	}

private:
	ItemList	m_list1;
	ItemList	m_list2;

	ItemList	* m_pListIn = &m_list1;
	ItemList	* m_pListOut = &m_list2;
};
/////////////////////////////////////

/////////////////////////////////////
class TaskQueue
{
public:
    TaskQueue(){}
    virtual ~TaskQueue(){};
    TaskQueue(const TaskQueue & o) = delete;
    TaskQueue & operator=(const TaskQueue & o) = delete;

	void Run()
	{
		while (!m_bExit)
		{
			Switch();

			if (tl.IsOutEmpty())
				continue;

			for (auto p : tl.OutList())
			{
				if (p != nullptr)
				{
					p->Execute();
					delete p;
				}
			}
			tl.ClearOut();
		}
	}

	void RunOne()
	{
		TrySwitch();

		if (tl.IsOutEmpty())
			return;

		for (auto p : tl.OutList())
		{
			if (p != nullptr)
			{
				p->Execute();
				delete p;
			}
		}
		tl.ClearOut();
	}

	template<typename T>
	void Post(T && h)
	{
		TaskOp<T> * p = new TaskOp<T>(forward<T>(h));

		unique_lock<mutex> lock(mtx);
		tl.Push(p);
		cv.notify_all();
	}

	//template<>
	void Post(nullptr_t && h)
	{
		unique_lock<mutex> lock(mtx);
		tl.Push(nullptr);
		cv.notify_all();
	}

	void ReqExit()
	{
		if (m_bExit)
			return;
		m_bExit = true;
		Post(nullptr);
	}

	bool IsExit()
	{
		return m_bExit;
	}

private:
	void Switch()
	{
		unique_lock<mutex> lock(mtx);
		cv.wait(lock, [this](){ return !tl.IsInEmpty(); });
		tl.Switch();
	}

	void TrySwitch()
	{
		unique_lock<mutex> lock(mtx);
		if (tl.IsInEmpty())
			return;
		tl.Switch();
	}

private:
	typedef SwitchList<TaskOpBase *> TaskOpList;

	TaskOpList tl;
	mutex mtx;
	condition_variable cv;
	volatile bool m_bExit = false;
};

typedef shared_ptr<TaskQueue> TaskQueuePtr;
/////////////////////////////////////

/////////////////////////////////////
class TaskThread
{
public:
    TaskThread(bool runNow = false)
    {
        if (runNow)
            Start();
    }

    virtual ~TaskThread()
    {
        Stop();
    }

    TaskThread(const TaskThread & o) = delete;
    TaskThread & operator=(const TaskThread & o) = delete;

	bool Start()
	{
		if (m_thread.joinable())
			return false;

		m_thread = thread(bind(&TaskThread::Run, this));
		return true;
	}

	void Stop()
	{
		if (!m_thread.joinable())
			return;
		if (m_thread.get_id() == this_thread::get_id())
			return;

		ReqExit();
		m_thread.join();
	}

	void Run()
	{
		m_taskQueue.Run();
	}

	template<typename T>
	void Post(T && h)
	{
		m_taskQueue.Post(forward<T>(h));
	}

	void ReqExit()
	{
		m_taskQueue.ReqExit();
	}

	bool IsExit()
	{
		return m_taskQueue.IsExit();
	}

private:
	thread m_thread;
	TaskQueue m_taskQueue;
};

typedef shared_ptr<TaskThread> TaskThreadPtr;
/////////////////////////////////////

#endif // TASKTHREAD_H_
