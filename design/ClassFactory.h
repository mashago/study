#ifndef _CLASS_FACTORY_
#define _CLASS_FACTORY_

#include <deque>
#include <algorithm>

class CDeleteObject
{
public:
	template<typename T>
	void operator() (const T *ptr) const
	{
		delete ptr;
	}
};

template<typename T>
class CReadyQueue
{
private:
	typedef std::deque<T*> CQueue;
	CQueue m_Queue;

public:
	CReadyQueue() {}
	~CReadyQueue() 
	{
		std::for_each(m_Queue.begin(), m_Queue.end(), CDeleteObject());
	}

	bool Init(int nQueueSize)
	{
		for (int i=0; i<nQueueSize; ++i)
		{
			T *p = new T;
			if (!p)
			{
				return false;
			}
			p->Init(); // All T should have Init()
			m_Queue.push_back(p);
		}
		return true;
	}

	void Push(T *p)
	{
		m_Queue.push_back(p);
	}

	T * Pop()
	{
		if (m_Queue.empty())
		{
			return NULL;
		}

		T *p = m_Queue.front();
		m_Queue.pop_front();
		return p;
	}

	size_t Size()
	{
		size_t size = m_Queue.size();
		return size;
	}
};

template<typename T>
class CClassFactory
{
private:
	CReadyQueue<T> m_Pool;
	CClassFactory() {};
	~CClassFactory() {};

public:
	static CClassFactory * GetThis()
	{
		static CClassFactory instance;
		return &instance;
	}

	bool Init(int nSize)
	{
		return m_Pool.Init(nSize);
	}

	void Push(T *p)
	{
		if (!p) { return; }
		p->Init();
		m_Pool.Push(p);
	}

	T * Pop()
	{
		T *p = m_Pool.Pop();
		if (p) return p;

		// over size, extra T
		p = new T;
		p->Init();
		return p;
	}

	size_t Size()
	{
		return m_Pool.Size();
	}

};

#endif
