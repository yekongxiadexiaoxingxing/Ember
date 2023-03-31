#pragma once

template <class T> class Singleton
{
public:
	~Singleton()
	{
		delete m_p;
	}
	inline static T& instance()
	{
		if (0 == m_p)
		{
			m_p = new T();
		}
		return *m_p;
	}
protected:
	T& operator=(const T& t);
	Singleton(const T& t);
private:
	static T* m_p;
};
template<class T> T* Singleton<T>::m_p = 0;
