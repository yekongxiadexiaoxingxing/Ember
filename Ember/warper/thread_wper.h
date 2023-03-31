#pragma once
#include "macro/macro.h"

using namespace std;

NAMESPACEBEGINE
#ifdef _WIN32
#include <Windows.h>

typedef HANDLE TID;
typedef int(*ThreadProc)(void*);

/*
 ���ڴ����̵߳��ڲ��ṹ
 tp �̵߳Ļص�����
 param �̻߳ص���������
 */
struct ThreadStruct
{
	ThreadProc tp;
	void * param;
};
/*
���������߳�
*/
TID createThread(ThreadProc tp, void * param);
/*����һ���߳�*/
void destoryThread(TID tid);
/*�����߳�*/
void waitThread(TID thread, int milliseconds);

typedef  HANDLE Mutex;
/*����������*/
Mutex createMutex();
/*����������*/
void destoryMutex(Mutex mutex);
/*��ȡ������*/
void getMutex(Mutex mutex);
/*�ͷŻ�����*/
void releaseMutex(Mutex mutex);

/*
����������
*/

/*
�̵߳����װ��һ��ʵ����ʾһ���߳�
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