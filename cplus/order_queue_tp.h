
#include <list>
using std::list;

template <class T, class KeyType>
class OrderQueue
{
private:
	list<T> container;
	// const T &name = T const &name, so below both type are ok
	typedef bool (*CompareFun)(T const &a, T const &b);
	// typedef bool (*CompareFun)(const T &a, const T &b);
	CompareFun compare;

	typedef KeyType (*GetKeyFun)(const T &a);
	// typedef KeyType (*GetKeyFun)(T const &a);
	GetKeyFun getKey;
public:
	// below 3 ways are ok
	OrderQueue(const CompareFun &c, const GetKeyFun &g) : compare(c), getKey(g) {};
	// OrderQueue(CompareFun c, GetKeyFun g) : compare(c), getKey(g) {};
	// OrderQueue(const CompareFun c, const GetKeyFun g) : compare(c), getKey(g) {};
	~OrderQueue() {};

	bool insert(T data)
	{
		auto iter = position(data);
		container.insert(iter, data);
		return true;
	}

	bool update(T data)
	{
		erase(getKey(data));
		return insert(data);
	}

	bool erase(KeyType key)
	{
		for (auto iter = container.begin(); iter != container.end(); iter++)
		{
			if (getKey(*iter) == key)
			{
				container.erase(iter);
				return true;
			}
		}
		return true;
	}

	list<T> & getContainer()
	{
		return container;
	}

	// void clear();

private:
	typename list<T>::iterator position(T data)
	{
		for (auto iter = container.begin(); iter != container.end(); iter++)
		{
			if (compare(data, *iter) == true)
			{
				return iter;
			}
		}
		return container.end();
	}
};

