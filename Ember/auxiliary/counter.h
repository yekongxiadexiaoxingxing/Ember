#pragma once

class Counter
{
public:
	Counter()
	{
		m_pCounter = new unsigned int(1);
	}

	virtual ~Counter()
	{
		--(*m_pCounter);
		if((this->m_pCounter) <= 0)
		{
			delete m_pCounter;
		}
	}
	Counter(const Counter & ic)
	{
		++(*ic.m_pCounter);
		this->m_pCounter = ic.m_pCounter;
	}

	int operator = (const Counter &ic)
	{
		int res = --*(this->m_pCounter);
		++*(ic.m_pCounter);
		this->m_pCounter = ic.m_pCounter;
		return res;
	}

	int operator = (const Counter &&ic)
	{
		int res = --*(this->m_pCounter);
		++*(ic.m_pCounter);
		this->m_pCounter = ic.m_pCounter;
		return res;
	}
	unsigned int refCounter()
	{
		return *m_pCounter;
	}
protected:
	mutable unsigned int *m_pCounter;
};