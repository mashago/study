
#ifndef __TASKTHREAD_H_
#define __TASKTHREAD_H_

// TaskThread tt;
// tt.Start();
// tt.post(&func);
// tt.post([](){});
// tt.post(bind(show, 1, 2));
// tt.post(bind(&Object::show, &obj, 1, 2));
// tt.Stop();

#include <list>
#include <utility>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>

namespace masha
{

// base class for add into list
class ThreadTaskBase
{
public:
	virtual ~ThreadTaskBase() {}
	virtual void Execute() = 0;
};

// task template
template <typename T>
class ThreadTask : public ThreadTaskBase
{
public:
	typedef typename std::remove_reference<T>::type TaskType;
	ThreadTask(T &&_t) : t(std::forward<T>(_t)) {}
	virtual void Execute()
	{
		t();
	}
private:
	TaskType t;
};

// double buffer switch list, store task
template <typename T>
class SwitchList
{
public:
	typedef std::list<T> TaskList;
	
	SwitchList() {}
	~SwitchList() {}

	template <typename TT>
	void Push(TT &&task)
	{
		m_pInList->push_back(std::forward<TT>(task));
	}

	void Switch()
	{
		std::swap(m_pInList, m_pOutList);
	}

	void CleanOut()
	{
		m_pOutList->clear();
	}

	int GetInSize()
	{
		return m_pInList->size();
	}

	int GetOutSize()
	{
		return m_pOutList->size();
	}

	bool IsInEmpty()
	{
		return m_pInList->empty();
	}

	bool IsOutEmpty()
	{
		return m_pOutList->empty();
	}

	const TaskList & InList()
	{
		return *m_pInList;
	}

	const TaskList & OutList()
	{
		return *m_pOutList;
	}

private:
	TaskList m_list1;
	TaskList m_list2;
	TaskList *m_pInList = &m_list1;
	TaskList *m_pOutList = &m_list2;
};

// handle input task
class TaskQueue
{
public:
	TaskQueue() {}
	~TaskQueue() {}
	TaskQueue(const TaskQueue &t) = delete;
	TaskQueue & operator=(const TaskQueue &t) = delete;

	void Run()
	{
		while (!m_bExit)
		{
			Switch();

			if (m_taskList.IsOutEmpty())
			{
				continue;
			}

			for (auto task : m_taskList.OutList())
			{
				if (task != nullptr)
				{
					task->Execute();
				}
			}
			m_taskList.CleanOut();
		}
	}

	template <typename T>
	void Post(T &&t)
	{
		ThreadTask<T> *pTask = new ThreadTask<T>(std::forward<T>(t));

		std::unique_lock<std::mutex> lock(m_mtx);
		m_taskList.Push(pTask);
		m_cv.notify_all();
	}

	void Post(std::nullptr_t &&t)
	{
		std::unique_lock<std::mutex> lock(m_mtx);
		m_taskList.Push(nullptr);
		m_cv.notify_all();
	}

	void ReqExit()
	{
		if (m_bExit)
		{
			return;
		}
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
		std::unique_lock<std::mutex> lock(m_mtx);
		m_cv.wait(lock, [this](){ return !m_taskList.IsInEmpty(); });
		m_taskList.Switch();
	}

private:
	SwitchList<ThreadTaskBase *> m_taskList;
	std::mutex m_mtx;
	std::condition_variable m_cv;
	bool m_bExit = false;
};

// task thread interface
class TaskThread
{
public:
	TaskThread(bool runNow = false)
	{
		if (runNow)
		{
			Start();
		}
	}

	~TaskThread()
	{
		Stop();
	};

	TaskThread(const TaskThread &t) = delete;
	TaskThread & operator=(const TaskThread &t) =delete;

	bool Start()
	{
		if (m_thread.joinable())
		{
			return false;
		}
		// m_thread = std::thread(std::bind(&TaskThread::Run, this));
		// m_thread = std::thread([this](){ this->Run(); });
		m_thread = std::thread([this](){ Run(); });
		return true;
	}

	void Stop()
	{
		if (!m_thread.joinable())
		{
			return;
		}
		if (m_thread.get_id() == std::this_thread::get_id())
		{
			return;
		}

		m_taskQueue.ReqExit();
		m_thread.join();
	}

	void Run()
	{
		m_taskQueue.Run();
	}

	template<typename T>
	void Post(T &&h)
	{
		m_taskQueue.Post(std::forward<T>(h));
	}

	bool IsExit()
	{
		return m_taskQueue.IsExit();
	}

private:
	std::thread m_thread;
	TaskQueue m_taskQueue;
};

}

#endif // __TASKTHREAD_H_
