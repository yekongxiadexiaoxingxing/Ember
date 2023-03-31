#pragma once


/*
*
*event.h
*�¼���֧�ֶ��̣߳���Ҫ�Լ�ͬ���¼�����, ����Ӧ����������
*��������̴߳���ʱ��ĵ��ȣ������߳�ֻ�����¼��ص��������߼�
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
	/*ʱ����Ϣ����*/
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
	*StaticDelegate ��̬�����¼��ص�����ָ��
	*ObjectProc	��Ա�¼��ص�����ָ��
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
	*��װ�����Ա�����Ļص�
	*��һ����������һ���¼�
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
	*�۲���ģʽ�� EventDispatcher�����¼�ʱ�Ὣ�¼����͸����ü�����(EventListener)
	*�������ĳ1�¼����ͺȴ����¼��Ĺ���
	*�����ܼ����¼���û���
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
	*�۲���ģʽ�� EventDispatcher�����¼�ʱ�Ὣ�¼����͸����ü�����(EventListener)
	*���о����¼����͵��඼��EventDispatcher������
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