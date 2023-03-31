#pragma once

#include <vector>
#include "auxiliary.h"

using namespace std;

class Observer
{
public:
	void onUpdate(void*){}

};

template <class Observer>
class Observe
{
public:
	void addObserver(const Observer observer)
	{
		_oservers.push_back(observer);
	}
	void destory()
	{
		FOR_EARCH_VECTOR(_oservers, iter)
		{
			delete *iter;
			iter = nullptr;
		}
	}
protected:
	void notify(int index = -1)
	{
		if (-1 == index)
		{
			FOR_EARCH_VECTOR(_oservers, iter)
			{
				(*iter)->onUpdate(this);
			}
		}
		else if (0 <= index)
		{
			Observer & observer = _oservers.at(index);
			observer->onUpdate(this);
		}
	}
private:
	vector<Observer> _oservers;
};