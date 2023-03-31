#pragma once

/*
* object.h
*
*
*
*
*/
#include "macro/macro.h"
#include "prototype/prototype.h"

NAMESPACEBEGINE

namespace Frame
{
namespace Object
{

	class Object : public Prototype<Object>
	{
	public:
		template<class Object>
		class CallBackAdaptor : public Prototype<CallBackAdaptor>
		{
		public:
			typedef void(*StaticProc)(typename Object::Handle sender, typename Object::Handle  param);
			typedef void(Object::*ObjectProc)(typename Object::Handle sender, typename Object::Handle  param);
			union CallBack
			{
				StaticProc * sProc = 0;
				ObjectProc * oProc;
			};
			enum CallBackType
			{
				STATIC_PROC,
				OBJECT_PROC,
				NONE_PROC
			};
		public:
			CallBackAdaptor(StaticProc * proc) : _eventProcType(STATIC_PROC)
			{
				_eventProc.sProc = proc;
			}
			CallBackAdaptor(Object::Handle object, ObjectProc * proc) :_owner(object), _eventProcType(OBJECT_PROC)
			{
				_eventProc.oProc = proc;
			}
			inline void run(Object::Handle sender, Object::Handle param)
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
			CallBackType _eventProcType = NONE_PROC;
			CallBack  _eventProc;
		};
#define createCbAda(proc) _sys_createEventAdaptor(&proc)
		inline CallBackAdaptor::Handle _sys_createEventAdaptor(CallBackAdaptor<T>::StaticProc f)
		{

		}
		inline CallBackAdaptor<T>::Handle _sys_createEventAdaptor(CallBackAdaptor<T>::ObjectProc  f)
		{

		}
	public:
		Object(){}
		Object(const Object & object){}
		Object(const Object && object){}
	};



}//end Object
}

NAMESPACEEND