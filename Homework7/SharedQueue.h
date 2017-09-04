//
//  SharedQueue.h
//  Lab12
//

#ifndef __Lab12__SharedQueue__
#define __Lab12__SharedQueue__

#include <mutex>

// Adapt LList code below to implement SharedQueue
// (then remove this comment)

template <typename T>
class SharedQueue {
public:
	SharedQueue();
	~SharedQueue();
	bool IsEmpty() const;
	void Add(T value);
	bool Remove(T &item);
	void Print();
private:
	struct QueueItem {
		T item;
		QueueItem *next;
	};
	std::mutex lock;
	QueueItem *head, *tail;
};

template <typename T>
SharedQueue<T>::SharedQueue()
{
	head = tail = 0;
}

template <typename T>
SharedQueue<T>::~SharedQueue()
{
	T tmp;
	while (Remove(tmp))
	{
	}
}


template <typename T>
bool SharedQueue<T>::IsEmpty() const
{
	return (head == 0);
}


template <typename T>
void SharedQueue<T>::Add(T value)
{
	lock.lock();
	if (head == 0)
	{
		head = tail = new QueueItem;
		head->item = value;
		head->next = 0;
	}
	else {
		tail->next = new QueueItem;
		tail = tail->next;
		tail->item = value;
		tail->next = 0;
	}
	lock.unlock();
}


template <typename T>
bool SharedQueue<T>::Remove(T &item)
{
	lock.lock();
	if (head == 0)
	{
		lock.unlock();
		return false;
	}
	else if (head == tail)
	{
		item = head->item;
		delete head;
		head = tail = 0;
	}
	else {
		item = head->item;
		QueueItem *tmp = head;
		head = head->next;
		tmp->next = 0;
		delete tmp;
	}
	lock.unlock();
	return true;
}


template <typename T>
void SharedQueue<T>::Print()
{
	lock.lock();
	QueueItem *tmp = head;
	while (tmp != 0)
	{
		std::cout << tmp->value << " ";
		tmp = tmp->next;
	}
	lock.unlock();
}








#endif /* defined(__Lab12__SharedQueue__) */

