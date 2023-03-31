#pragma once


/*
*
*event.h
*
*/

#include "macro/macro.h"
#include "event/event.h"

NAMESPACEBEGINE

namespace Event
{
	/*
	*
	*mouse.h
	*
	*/
	enum MouseEventType
	{
		MET_DOWN,
		MET_UP,
		MET_MOVE,
		MET_CLICK,
		MET_DOUBLECLICK,
	};
	enum MouseButton
	{
		MB_LEFT,
		MB_RIGHT,
		MB_MIDDLE,
	};

	class MouseEvent : public Prototype<MouseEvent, EventMsg>
	{
	public:
		MouseEvent(int x, int y) :Prototype(MouseEvent::Self, EventMsg(EM_MOUSE, ES_INIT))
		{
			this->x = x;
			this->y = y;
		}
		MouseEventType type;
		MouseButton button;
		int x = 0;
		int y = 0;
	};

	class MouseListener : public Prototype<MouseListener, EventListener, EventDispatcher>
	{
	public:
		MouseListener(){}
	};

}//end event
NAMESPACEEND