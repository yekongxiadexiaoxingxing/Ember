#pragma once


#include <list>
#include <vector>

#include "auxiliary/Singleton.h"
#include "macro/macro.h"
#include "warper/thread_wper.h"

NAMESPACEBEGINE


class ThreadTask
{
public:
	virtual void run();
	virtual ~ThreadTask(){};
};

class ThreadRunable : public Runable
{
	friend class ThreadPool;
public:
	ThreadRunable();
	~ThreadRunable();
	void run();
	void addTask(ThreadTask * threadTask);
private:
	list<ThreadTask*> *m_taskCache, *m_taskDeal;
	list<ThreadTask*> m_tasks[2];
	Mutex m_lock;

	unsigned int CacheSize;
	bool Monopolize; //是否独占一条线程
};

class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();
	void addTask(ThreadTask * task);
	void delegetleThread();//托管线程
private:
	vector<Thread*> m_threadRunables;
};

#define gThreadPool Singleton<ThreadPool>::instance()

NAMESPACEEND