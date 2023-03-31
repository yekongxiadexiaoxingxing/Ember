#pragma once


/*
*
*event.h
*事件不支持多线程，需要自己同步事件数据, 但不应该这样做，
*最好让主线程处理时间的调度，其他线程只处理事件回掉函数的逻辑
*
*/

#include <typeinfo.h>
#include "macro/macro.h"
#include "object/object.h"
#include "prototype/prototype.h"

NAMESPACEBEGINE
using namespace Frame::Object;
namespace Event
{
	/*时间消息类型*/
	enum EventMessage
	{
		EM_NONE,
		EM_MOUSE,
		EM_FILE,
		EM_KEYBOARD,
		EM_NET,

		EM_END,
		EM_USER = EM_END + 4096,
	};
	//EventMessage

	enum EventState
	{
		ES_INIT,
		ES_DEALED,
	};

	class EventMsg : public Prototype<EventMsg, Object>
	{
	public:
		EventMsg()
		{}
		EventMsg(EventMessage msg, EventState state)
		{
			this->msg = msg;
			this->state = state;
		}
		EventMessage msg = EM_NONE;
		EventState state = ES_INIT;
	};


	/*
	*StaticDelegate 静态函数事件回调函数指针
	*ObjectProc	成员事件回调函数指针
	*/
	typedef void(*StaticProc)(Object::Handle sender, EventMsg::Handle  event);
	typedef void(Object::*ObjectProc)(Object::Handle sender, EventMsg::Handle event);
#define DEFEVENTCALLBACK(Class, defname) typedef void(Class::defname)(Object::Handle sender, EventMsg::Handle event);
	union EventProc
	{
		StaticProc * sProc =0;
		ObjectProc * oProc ;
	};
	enum EventProcType
	{
		STATIC_PROC,
		OBJECT_PROC,
		NONE_PROC
	};
	class EventAdaptor : public Prototype<EventAdaptor>
	{
	public:
		EventAdaptor(StaticProc * proc): _eventProcType(STATIC_PROC)
		{
			_eventProc.sProc = proc;
		}
		EventAdaptor(Object::Handle object, ObjectProc * proc) :_owner(object), _eventProcType(OBJECT_PROC)
		{
			_eventProc.oProc = proc;
		}
		inline void run(Object::Handle sender, EventMsg::Handle event)
		{
			if (STATIC_PROC == _eventProcType)
			{
				(*(_eventProc.sProc))(sender, event);
			}
			else if (OBJECT_PROC == _eventProcType)
			{
				
			}
		}
	private:
		Object::Handle  _owner;
		EventProcType _eventProcType = NONE_PROC;
		EventProc  _eventProc;
	};
	/*
	*封装多个成员函数的回调
	*在一个类中生命一个事件
	*eg.  ObjectDelegate MouseEvent
	*/
	class EventDelegate : public Prototype<EventDelegate>
	{
	public:
		EventDelegate(){}
		virtual void trigger(Object::Handle sender, EventMsg::Handle event) const
		{
			FOR_EARCH_VECTOR(_delegates, pointer)
			{
				(*pointer)->run(sender, event);
			}
		}
		inline void operator +=(EventAdaptor::Handle da)
		{
			_delegates.push_back(da);
		}
		inline void operator =(EventAdaptor::Handle da)
		{
			_delegates.clear();
			_delegates.push_back(da);
		}
	private:
		std::vector<EventAdaptor::Handle> _delegates;
	};

	class EvnetInterface : public Prototype<EvnetInterface, Object>
	{
	public:
#define createEventAda(proc) _sys_createEventAdaptor(&proc)
		template<typename F>
		inline EventAdaptor::Handle _sys_createEventAdaptor(F  f)
		{
#pragma message("11111111111111")
			return EventAdaptor::create(Object::Handle(), f);
		}
		/*inline EventAdaptor::Handle _sys_createEventAdaptor(StaticProc  f)
		{
			return EventAdaptor::create(f);
		}*/
	};

	/*
	*观察者模式， EventDispatcher派送事件时会将事件派送个他得监听者(EventListener)
	*具用添加某1事件类型喝触发事件的功能
	*所用能监听事件类得基类
	*/
	class EventListener : public Prototype<EventListener, EvnetInterface>
	{
	public:
		EventListener()
		{
		}
		~EventListener()
		{
		
		}
		inline void addListenEvent(EventMessage msg, EventAdaptor::Handle proc)
		{
			_eventProcMap.insert(std::make_pair(msg, proc));
		}
		inline void trigger(Object::Handle sender, EventMsg::Handle event)
		{
			_eventProcMap[event->msg]->run(sender, event);
		}

	private:
		std::map<EventMessage, EventAdaptor::Handle> _eventProcMap;
	};

	/*
	*观察者模式， EventDispatcher派送事件时会将事件派送个他得监听者(EventListener)
	*所有具有事件发送的类都是EventDispatcher的子类
	*
	*/
	class EventDispatcher : public Prototype<EventDispatcher, EvnetInterface>
	{
	public:
		EventDispatcher(){}
		inline void dispatching(Object::Handle sender, EventMsg::Handle event)
		{
			FOR_EARCH_VECTOR(_listeners, iter)
			{
				(*iter)->trigger(sender, event);
			}
		}
	private:
		std::vector<EventListener::Handle> _listeners;
	};

}//end event
NAMESPACEEND