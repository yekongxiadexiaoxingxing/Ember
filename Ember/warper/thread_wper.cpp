#include "thread_wper.h"
#include "system_wper.h"

NAMESPACEBEGINE
/*
将回调函数封装成 ThreadProc类型
*/
DWORD WINAPI _sys_thread(void*param)
{
	ThreadStruct * pts = (ThreadStruct*)(param);
	ThreadProc tp = pts->tp;
	void * param1 = pts->param;
	tp(param1);
	return 0;
}

TID createThread(ThreadProc tp, void * param)
{
	ThreadStruct ts = { tp, param };
	TID pid = CreateThread(NULL, 0, _sys_thread, &ts, 0, 0);
	//_sys_thread执行前ts可能被释放
	return pid;
}

void destoryThread(TID phandle)
{
	CloseHandle(phandle);
}

void waitThread(TID thread,int milliseconds = -1)
{
	WaitForSingleObject(thread, milliseconds);
}

Mutex createMutex()
{
	return CreateMutex(NULL, FALSE, NULL);
}
void destoryMutex(Mutex mutex)
{
	CloseHandle(mutex);
}
void waitMutex(Mutex mutex)
{
	WaitForSingleObject(mutex, INFINITE);
}

void releaseMutex(Mutex mutex)
{
	ReleaseMutex(mutex);
}

void Runable::run()
{

}

int _sys_createThread(void* pP)
{
	Runable* pT = static_cast<Runable*>(pP);
	pT->run();
	return 1;
}

Thread::Thread(Runable* p) : m_r(0), m_tid(0)
{
	m_r = p;
}

Thread::Thread() : m_r(0), m_tid(0)
{

}

Thread::~Thread()
{
	if (!m_tid)
	{
		return;
	}
	destoryThread(m_tid);
}

void Thread::setRunable(Runable *p)
{
	m_r = p;
}

void Thread::start()
{
	if (!m_r)
	{
		throw;
	}
	m_tid = createThread(&_sys_createThread, m_r);
}

void Thread::stop()
{
	if (m_r)
	{
		m_r->stop();
	}
}

void Thread::wait()
{
	waitThread(m_tid);
}

void Thread::destory()
{
	m_r->stop();
	delete m_r;
}

AutoLock::AutoLock(Mutex *mutex)
{
	m_lt = MUTEX_LT;
	pLock = mutex;
}

AutoLock::~AutoLock()
{
	if (MUTEX_LT == m_lt)
	{
		releaseMutex(*((Mutex*)(pLock)));
	}
}

NAMESPACEEND