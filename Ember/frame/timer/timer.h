#pragma once

#include <vector>
#include "macro/macro.h"
#include "event/event.h"
#include "auxiliary/auxiliary.h"
#include "auxiliary/observe.h"
#include "warper/time_wper.h"
#include "auxiliary/singleton.h"

NAMESPACEBEGINE
using namespace std;
using namespace Event;

class TimerMgr;

class Timer : public Prototype<Timer, EventDelegate, Object>
{
public:
	friend class TimerMgr;
public:
	Timer(double interval = 1000) :_elapseTime(interval)
	{
		
	}

	inline void setInterval(double interval)
	{
		_elapseTime.setInterval(interval);
	}

	///设置非异步的定时器
	static int setTimer(Timer * timer);
	///设置异步的定时器
	static int setAsynTimer(Timer * timer);
	static void delTimer(int timer);
	static void delAsynTimer(int timer);
	inline void onUpdate(Observe<Timer::Handle> * timerMgr)
	{
		this->trigger(self, EventMsg::Handle());
	}
private:
	int _id = -1;
	ElapseTime _elapseTime;
};

class TimerMgr : public Prototype<TimerMgr, Observe<Timer::Handle>, Object>
{
public:
	void update()
	{
		FOR_EARCH_VECTOR(_timers, timer)
		{
			if ((*timer)->_elapseTime.elapse())
			{
				notify((*timer)->_id);
			}
		}
	}
private:
	vector<Timer::Handle> _timers;
};

#define gTimerMgr Singleton<TimerMgr>::instance()

NAMESPACEEND