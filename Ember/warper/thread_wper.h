#pragma once
#include "macro/macro.h"

using namespace std;

NAMESPACEBEGINE
#ifdef _WIN32
#include <Windows.h>

typedef HANDLE TID;
typedef int(*ThreadProc)(void*);

/*
 用于创建线程的内部结构
 tp 线程的回调函数
 param 线程回调函数参数
 */
struct ThreadStruct
{
	ThreadProc tp;
	void * param;
};
/*
启动创建线程
*/
TID createThread(ThreadProc tp, void * param);
/*销毁一个线程*/
void destoryThread(TID tid);
/*阻塞线程*/
void waitThread(TID thread, int milliseconds);

typedef  HANDLE Mutex;
/*创建互斥体*/
Mutex createMutex();
/*创建互斥体*/
void destoryMutex(Mutex mutex);
/*获取互斥体*/
void getMutex(Mutex mutex);
/*释放互斥体*/
void releaseMutex(Mutex mutex);

/*
创建互斥体
*/

/*
线程的类封装，一个实例表示一个线程
*/
class Runable
{
public:
	virtual void run();
	virtual void stop(){};
};

class Thread
{
public:
	Thread(Runable*);
	Thread();
	~Thread();
	void setRunable(Runable *p);
	void start();
	void stop();
	void wait();
	void destory();
	Runable* m_r;
private:
	TID m_tid;
};


enum LockType
{
	MUTEX_LT,
};

class AutoLock
{
public:
	AutoLock(Mutex *mutex);
	~AutoLock();
private:
	LockType m_lt;
	void * pLock = nullptr;
};
#endif
NAMESPACEEND