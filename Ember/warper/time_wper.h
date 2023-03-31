#pragma once

#include <Windows.h>
#include "auxiliary/math.h"
#include "macro/macro.h"

/*
支持单线程定时器， 支持多线程定时器
*/
NAMESPACEBEGINE

 LARGE_INTEGER cpuCycle;
 LARGE_INTEGER cycleFrequency;

inline  void initModule()
{
	QueryPerformanceCounter(&cpuCycle);
	QueryPerformanceFrequency(&cycleFrequency);
}

///获取cpu加电以来经过的时间纳秒
inline double getTimeStampCount()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	//return (unsigned long long)(((double)(li.QuadPart - cpuCycle.QuadPart)) / cycleFrequency.QuadPart * 1000000000);//获取cpu加电以来经过的时间纳秒
	return (double)(((double)(li.QuadPart - cpuCycle.QuadPart)) / cycleFrequency.QuadPart * 1000);//获取cpu加电以来经过的时间毫秒秒
}

class ElapseTime
{
public:
	ElapseTime(double interval = 0) :_interval(interval)
	{
		_triggerTime = getTimeStampCount();
	}
	inline void setInterval(double interval)
	{
		_interval = interval;
	}
	///返回值如果到达返回true， 否则false
	inline bool elapse()
	{
		double curTime = getTimeStampCount();
		bool elapsed = Equred(_interval, curTime - _triggerTime);
		if (elapsed)
		{
			_triggerTime = curTime;
		}
		return elapsed;
	}
private:
	double _interval = 0;
	double _triggerTime = 0;
};

NAMESPACEEND