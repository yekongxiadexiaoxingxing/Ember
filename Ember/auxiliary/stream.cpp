#include "Stream.h"

NAMESPACEBEGINE

namespace Auxiliary
{

Stream::Stream():m_pData(0), m_size(0), m_currIndex(0),m_attach(false)
{

}

Stream::Stream(void * pData, long long unsigned int size):m_pData((char*)pData), m_size(size), m_currIndex(0), m_attach(true)
{

}

Stream::Stream(long long unsigned int size):m_pData(0), m_size(0), m_currIndex(0),m_attach(false)
{
	realloc(size);
}

Stream::~Stream()
{
	if(m_attach && m_pData)
	{
		free(m_pData);
	}
}

template<class T>
void Stream::serialize(T & t)
{
	t.Serilization(*this);
}

template<class T>
void Stream::deserialize(T & t)
{
	t.deserialize(*this);
}

void Stream::readFrom(void *pScr, unsigned long int size)
{
	realloc(size);
	memmove((m_pData + m_currIndex), pScr, (size_t)size);
	m_currIndex += size;
}

void Stream::writeTo(void *pDst, unsigned long int size)
{
	if(0 >= m_currIndex - size)
	{
		throw;
	}
	memmove(pDst, m_pData + m_currIndex - size, (size_t)size);
	m_currIndex -= size;
}

}//end Auxiliary
NAMESPACEEND