#pragma once
#include <stdlib.h>
#include <string.h>
#include "../macro/macro.h"
#define STREAMUNITSIZE 512

NAMESPACEBEGINE

namespace Auxiliary
{


class Stream
{
public:
	Stream();
	Stream(void * pData, long long unsigned int size);
	Stream(long long unsigned int size);
	~Stream();
public:
template<class T>
inline T& operator <<(const T& t)
{
	const char *p = (const char*)(&t);
	Realloc(sizeof(T));
	memmove(m_pData + m_size, p, sizeof(T));
	m_size += sizeof(T);
	m_currIndex += sizeof(T);
}

template<class T>
inline T& operator >>(T& t)
{
	//试试数组吧
	const char *p = (const char*)(&t);
	char *pT = m_currIndex - sizeof(T);
	if(0 >= pT)
	{
		return t;
	}
	memmove(p, pT, sizeof(T));
	m_currIndex -= sizeof(T);
	m_size -= sizeof(T);
}

template<class T>
void serialize(T & t);

template<class T>
void deserialize(T& t);

///读数据到Stream
void readFrom(void *pScr, unsigned long int size);
///将Stream的数据写出
void writeTo(void *pDst, unsigned long  int size);

private:
	inline void realloc(unsigned long long size)
	{
		if(m_currIndex + size >= m_size)
		{
			m_pData = (char*)::realloc(m_pData, (size_t)(m_size + STREAMUNITSIZE));
			m_size += STREAMUNITSIZE;
		}
	}
	
protected:
	char *m_pData;
	long long unsigned int m_size;
	long long unsigned int m_currIndex;
	bool m_attach;
};


}//end Auxiliary
NAMESPACEEND