#include "thread.h"
#include "warper/system_wper.h"

NAMESPACEBEGINE

void ThreadTask::run()
{

}

ThreadRunable::ThreadRunable()
{
	m_taskCache = &m_tasks[0];
	m_taskDeal = &m_tasks[1];
	Monopolize = false;
	CacheSize = 0;
}

void ThreadRunable::addTask(ThreadTask * threadTask)
{
	if (!threadTask)
	{
		return;
	}
	m_taskCache->push_back(threadTask);
	++CacheSize;
}

void ThreadRunable::run()
{
	while (true)
	{
		{
			AutoLock lock(&m_lock);
			list<ThreadTask*> * pT = m_taskCache;
			m_taskCache = m_taskDeal;
			m_taskDeal = pT;
		}

		list<ThreadTask*>::iterator iterB = m_taskDeal->begin();
		list<ThreadTask*>::iterator iterE = m_taskDeal->end();

		for (; iterB != iterE; ++iterB)
		{
			(*iterB)->run();
			delete *iterB;
		}
		m_taskDeal->clear();
		sleep(1);
	}
}

ThreadPool::ThreadPool()
{
	int count = 32;
	m_threadRunables.resize(count);
	Thread** pTreadTask = m_threadRunables.data();
	for (int i = 0; i < 32; ++i)
	{
		pTreadTask[i] = new Thread(new ThreadRunable());
		pTreadTask[i]->start();
	}
}

ThreadPool::~ThreadPool()
{
	Thread** pTreadTask = m_threadRunables.data();
	for (int i = 0; i < 32; ++i)
	{
		if (pTreadTask[i])
		{
			delete pTreadTask[i];
		}
	}
}

void ThreadPool::addTask(ThreadTask * task)
{
	Thread** pTreadTask = m_threadRunables.data();
	unsigned long long size = m_threadRunables.size();
	unsigned int mint = 0;
	unsigned int index = 0;
	unsigned int i = 0;
	for (; i < size; ++i)
	{
		Thread* pTt = pTreadTask[i];
		if ((((ThreadRunable*)(pTt->m_r))->Monopolize))
		{
			continue;
		}
		if (((ThreadRunable*)(pTt->m_r))->CacheSize <= 5 || mint >((ThreadRunable*)(pTt->m_r))->CacheSize)
		{
			mint = ((ThreadRunable*)(pTt->m_r))->CacheSize;
			index = i;
			break;
		}
	}
	((ThreadRunable*)(pTreadTask[index]->m_r))->addTask(task);
}

NAMESPACEEND