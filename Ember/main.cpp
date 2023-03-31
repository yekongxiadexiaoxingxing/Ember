#include <stdio.h>
#include "event/event.h"
#include <Windows.h>
#include "warper/string_wper.h"
#include "auxiliary/observe.h"
#include "timer/timer.h"
USINGNAMESPACE


class CD : public Prototype<CD, Event::EvnetInterface >
{
public:
	CD()
	{
		gTimerMgr.addObserver(EventTimer);
		EventTimer->setInterval(1000);
		//EventTimer += createEventAda(onEventTimer_);
		EventTimer += createEventAda(CD::onEventTimer);
	}
	void onUpdate(void* p)
	{
		print("cd run!\n");
	}
	Timer::Handle EventTimer;
private:
	void onEventTimer(CD::Handle sender, EventMsg::Handle handle)
	{

	}
	static void onEventTimer_(CD::Handle sender, EventMsg::Handle handle)
	{

	}
};

class MuliMedia : public Prototype<MuliMedia, Event::EventDispatcher, Event::EventListener, Observe<CD::Handle>>
{
public:
	void open()
	{
		notify();
	}
};


class A
{


};

//typedef SmartPointer<A> Handle;

int main(int argc, int argv)
{
	MuliMedia::Handle media = MuliMedia::create();
	CD::Handle cd = CD::create();
	media->addObserver(cd);
	media->open();

	gTimerMgr.update();
	//Handle handle = new A;
//	Handle a;
//	handle = handle;
	getchar();
	return 1;
}
