#pragma once

/*
* auxiliary.h
*
*
*
*
*/
#pragma once
#include <typeinfo>
#include <vector>
#include <stdio.h>
#include <map>
#include <stack>
#include "counter.h"

/*
*
*�ṩ����
*/
template <typename T>
class ClassInfo
{
public:
	ClassInfo()
	{
		name = typeid(T).name();
	}
	const char * name = 0;
};

/*
*
*SmartPointer ��������ָ��
*���ܱ�ָ�븳ֵ
*/
//template <typename T, typename C = Counter>
//class SmartPointer : public C
template <typename T>
class SmartPointer : public Counter
{
public:
	typedef T Type;
	SmartPointer(T* t) :rawPtr(t)
	{}
	SmartPointer() :rawPtr(0)
	{}
	SmartPointer(const SmartPointer * sp)
	{
		*this = sp;
	}
	~SmartPointer()
	{
		if (0 >= *m_pCounter)
		{
			delete rawPtr;
		}
	}
	/*template <typename RS>
	SmartPointer(RS rs)
	{
	rawPtr = static_cast<T*>(const_cast<typename RS::Type * >(rs.rawPtr));
	}*/
	template <typename RS>
	SmartPointer(const RS& rs)
	{
		rawPtr = static_cast<T*>(const_cast<typename RS::Type *>(rs.rawPtr));
	}

	T* operator->() const
	{
		return const_cast<T*>(rawPtr);
	}

	template<typename RS>
	inline SmartPointer<T>& operator=(const RS & rs)
	{
		if (0 >= Counter::operator=(rs))
		{
			delete rawPtr;
		}
		rawPtr = static_cast<T*>(const_cast<typename RS::Type *>(rs.rawPtr));
		return *this;
	}

	template<typename RS>
	inline SmartPointer<T>& operator=(const RS && rs)
	{
		if (0 >= Counter::operator=(rs))
		{
			delete rawPtr;
		}
		rawPtr = static_cast<T*>(const_cast<typename RS::Type*>(rs.rawPtr));
		return *this;
	}

	inline const SmartPointer& operator=(const SmartPointer& sp)
	{
		Counter::operator=(sp);
		rawPtr = sp.rawPtr;
		return *this;
	}

	inline const SmartPointer& operator=(const SmartPointer&& sp)
	{
		Counter::operator=(sp);
		rawPtr = sp.rawPtr;
	}
public:
	///����������Ϣģ��
	static ClassInfo<T> info;
	const T * rawPtr = nullptr;
private:

};

template <typename T>
ClassInfo<T> SmartPointer<T>::info;

/*
*����ָ��
*Variables���浱ǰ��������б���,������Ч�������ͷ����еı���
*VariablePool�����ջ,ջ��Ϊ��ǰ���� top()ָ��
*
*Local local;
*/
class Variables
{
public:
	template<class T>
	inline T * push(T* object)
	{
		m_objects.push_back(object);
		return object;
	}
	~Variables()
	{
		for (auto iter : m_objects)
		{
			delete iter;
		}
	}
private:
	std::vector<void*> m_objects;
};

class VariablePool
{
	friend class Local;
public:
	VariablePool()
	{
		m_vars.push(new Variables());
	}
	~VariablePool()
	{
		delete m_vars.top();
		m_vars.pop();
	}
	inline void pushNew()
	{
		m_vars.push(new Variables());
	}
	inline void pop()
	{
		m_vars.pop();
	}
	inline Variables* top()
	{
		return m_vars.top();
	}
private:
	std::stack<Variables*> m_vars;
};

class PersistentVariablePool
{
public:
	friend class Persistent;
	inline int oneNew(void * p)
	{
		m_persistents.insert(std::make_pair(0, new Variables()));
		++m_count;
		return m_count - 1;
	}
private:
	std::map<int, Variables*> m_persistents;
	int m_count = 0;
};

extern VariablePool gVariableLocalPool;
extern PersistentVariablePool gVariablePersistentPool;

class Local
{
public:
	Local()
	{
		printf("ddd\n");
		gVariableLocalPool.pushNew();
	}
	~Local()
	{
		delete gVariableLocalPool.top();
		gVariableLocalPool.pop();
	}
};

class  Persistent
{
public:
	Persistent()
	{
		m_id = gVariablePersistentPool.m_count++;
		gVariablePersistentPool.m_persistents.insert(std::make_pair(m_id, new Variables()));
	}
	virtual ~Persistent()
	{
		
	}
	
	 inline void destory()
	{
		 if (0 <= m_id)
		 {
			 --gVariablePersistentPool.m_count;
			 delete gVariablePersistentPool.m_persistents[m_id];
			 m_id = -1;
		 }
		
	}
private:
	int m_id=-1;
public:
	inline void __sys_add_persistent(void * p)
	{
		gVariablePersistentPool.m_persistents[m_id]->push(p);
	}
private:

};

template<class T>
class Scope : public T
{
public:

};

/*
	*����vector
	*
	*/
#define FOR_EARCH_VECTOR(vector, first)\
	auto first = vector.data(); \
	auto count = vector.size(); \
for (decltype(count) i = 0; i < count; ++i, ++first)


/*
	*�����ڴ���
	*
	*/
#define FOR_EARCH_POINTER(pointer, first, len)\
	auto first = pointer; \
for (decltype(len) i = 0; i < len; ++i, ++first)

/*
	*��������
	*
	*/
#define FOR_EARCH_ARRAY(array, first)\
	auto first = array; \
	auto len = sizeof(array);\
for (decltype(len) i = 0; i < len; ++i, ++first)

/*
	*�ڿ���̨��ӡһ��������Ϣ
	*/
#define print(formate,  ... )\
do{\
	printf(formate, __VA_ARGS__); \
	printf("\n"); \
} while (false)