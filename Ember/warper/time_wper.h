#pragma once

#include <Windows.h>
#include "auxiliary/math.h"
#include "macro/macro.h"

/*
֧�ֵ��̶߳�ʱ���� ֧�ֶ��̶߳�ʱ��
*/
NAMESPACEBEGINE

 LARGE_INTEGER cpuCycle;
 LARGE_INTEGER cycleFrequency;

inline  void initModule()
{
	QueryPerformanceCounter(&cpuCycle);
	QueryPerformanceFrequency(&cycleFrequency);
}

///��ȡcpu�ӵ�����������ʱ������
inline double getTimeStampCount()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	//return (unsigned long long)(((double)(li.QuadPart - cpuCycle.QuadPart)) / cycleFrequency.QuadPart * 1000000000);//��ȡcpu�ӵ�����������ʱ������
	return (double)(((double)(li.QuadPart - cpuCycle.QuadPart)) / cycleFrequency.QuadPart * 1000);//��ȡcpu�ӵ�����������ʱ�������
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
	///����ֵ������ﷵ��true�� ����false
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