#pragma once

/*
* object.h
*
*
*
*
*/
#include "macro/macro.h"
#include "auxiliary/auxiliary.h"

NAMESPACEBEGINE

typedef class {} Abstract;
typedef void(Abstract::*FunctionAbstract)();
class PrototypeFuncContainer
{
public:
	PrototypeFuncContainer(void * pOwner, FunctionAbstract  pProc) :m_pOwner(pOwner), m_pProc(pProc)
	{
	
	}
public:
	void * m_pOwner = nullptr;
	FunctionAbstract  m_pProc;
};
template <typename T, typename ... Args>
class Prototype :  public Args...
{
public:
	static Abstract Self;
	Prototype(){}
	///用于初始化列表
	Prototype(Abstract a, Args&&... args) :Args(args)...{}
	virtual ~Prototype()
	{
		m_persistent.destory();
	}

	///创建智能指针模块
	typedef SmartPointer<T> Handle;
	template <typename ... Params>
	friend inline static  Handle create(Params&& ... params);
	//typedef T* Handle;
	template <typename ... Params>
	inline static Handle  create(Params&& ... params)
	{
		//T * t = new T(params...);
		Handle t = new T(params...);
		t->self = t;
		//gVariableLocalPool.top()->push(t);
		return t ;
		//return  Handle(new T(params...));
	}

	///创建一个当前类的成员, 当该类删除时，该类的所有通过newMember创建的成员都回自动删除
	template<class C, typename ... Params>
	inline C*  newMember(Params&& ... params)
	{
		C * t = new C(params...);
		m_persistent.__sys_add_persistent(t);
		return t;
		//return  Handle(new T(params...));
	}

	template <typename ... Params>
	inline static Handle local(Params&& ... params)
	{
		T * t = 0;
		t = new T(params...);
		gVariableLocalPool.top()->push(t);
		return t;
		//return  Handle(new T(params...));
	}
	///创建成员事件回调函数模块
#define eventproc(proc) _sys_createProc(&proc)
	template<typename P>
	inline PrototypeFuncContainer _sys_createProc(P p)//必须引用
	{
		P *pp = &p;
		void * pointer = (void*)(pp);
		FunctionAbstract   pFa = *((FunctionAbstract *)pp);
		int a = sizeof(FunctionAbstract);
		return PrototypeFuncContainer((void*)this, pFa);
	}
	Handle self;
private:
	
	///类相关的持久变量域
	Persistent m_persistent;
};


template <typename T, typename ... Args>
Abstract Prototype<T, Args...>::Self;

NAMESPACEEND