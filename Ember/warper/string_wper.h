#pragma once

#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include <wchar.h>
#include "macro/macro.h"

NAMESPACEBEGINE

enum StringType
{
	ST_ACHAR,//单字符
	ST_WCHAR, //宽字符
	
	ST_UNKHOW,
};

class String
{
public:
	String(const char* astr);
	String(const char*data, int length);
	String(const wchar_t* wstr);
	String(const wchar_t*data, int length);
	String(){};
	String(const String && str);
	~String();
	/*
	内存字节大小
	*/
	inline int size() const
	{
		return _length * _elementSize;
	}
	inline int length() const
	{
		return _length;
	}
	inline const wchar_t * c_wstr()
	{
		return (wchar_t*)(_strData);
	}
	inline const char * c_astr() const
	{
		return _strData;
	}
	inline void convertW()
	{
		if (ST_ACHAR == _st)
		{
			destory();
			_init(_convert((wchar_t*)(_strData)));
		}
	}
	inline void convertA()
	{
		if (ST_WCHAR == _st)
		{
			destory();
			_init(_convert((char*)(_strData)));
		}
	}
	inline const String operator +(const char*str) const
	{
		String rs=*this;
		rs += str;
		return rs;
	}
	inline const String operator +(const wchar_t*str) const
	{
		String rs = *this;
		rs += str;
		return rs;
	}
	inline const String operator +(const String &str) const
	{
		String rs = *this;
		rs._expend(str);
		return rs;
	}
	inline const String & operator+=(const String &str)
	{
		_expend(str);
		return *this;
	}
	inline String operator+=(const char*astr)
	{
		_expend(astr);
		return *this;
	}
	inline const String & operator+=(const wchar_t*str)
	{
		_expend(str);
		return *this;
	}
	inline const String & operator=(const char* str)
	{
		destory();
		_init(str);
		return *this;
	}
	inline const String & operator=(const wchar_t* str)
	{
		destory();
		_init(str);
		return *this;
	}
	inline const String & operator=(const String & str)
	{
		destory();
		if (ST_WCHAR == str._st)
		{
			_init((wchar_t*)(str._strData));
		}
		else if (ST_ACHAR == str._st)
		{
			_init((char*)(str._strData));
		}
		return *this;
	}
	inline bool operator == (const char* str) const
	{
		return _compare(str);
	}
	inline bool operator == (const wchar_t* str) const
	{
		return _compare(str);
	}
	inline bool operator == (const String & str) const
	{
		return _compare(str._strData);
	}
private:
	inline static StringType _stringType(const char*)
	{
		return ST_ACHAR;
	}
	inline static StringType _stringType(const wchar_t*)
	{
		return ST_WCHAR;
	}

template<class T>
	inline void _init(const T * str)
	{
		_st = _stringType(str);
		_elementSize = sizeof(*str);
		_length = _strlen(str);
		_strData = (char*)malloc(_length + 1);
		if (_strData)
		{
			memcpy(_strData, str, _length * _elementSize);
			reinterpret_cast<T*>(_strData)[_length] = 0;
		}
	}
template<class T>
	inline void _init(const T * str, int length)
	{
		_st = _stringType(str);
		_elementSize = sizeof(*str);
		_length = length;
		_strData = (char*)malloc(_length + 1);
		if (_strData)
		{
			memcpy(_strData, str, _length * _elementSize);
			reinterpret_cast<T*>(_strData)[_length] = 0;
		}
	}

	inline void destory() const
	{
		if (_strData)
		{
			free(_strData);
		}
		_length = 0;
		_st = ST_UNKHOW;
		_elementSize = 1;
	}

	inline int _strlen(const char * str) const
	{
		return strlen(str);
	}

	inline int _strlen(const wchar_t * str) const
	{
		return wcslen(str);
	}

	inline StringType _charType(const char * str) const
	{
		return ST_ACHAR;
	}

	inline StringType _charType(const wchar_t * str) const
	{
		return ST_WCHAR;
	}

	//手动释放返回的指针
	inline wchar_t * _convert(const char * str)
	{
		int len = strlen(str);
		size_t size = mbsrtowcs(0, &str, len, 0);
		wchar_t * dest = (wchar_t*)malloc(size);
		mbsrtowcs(dest, &str, size, 0);
		free(dest);
		return dest;
	}
	//手动释放返回的指针
	inline char * _convert(const wchar_t * str)
	{
		int len = wcslen(str);
		size_t size = wcsrtombs(0, &str, len, 0);
		char * dest = (char*)malloc(size);
		wcsrtombs(dest, &str, size, 0);
		free(dest);
		return dest;
	}

	inline void _comine(const char * str)
	{
		int len = strlen(str);
		realloc(_strData, _length + len + 1);
		memcpy(_strData + _length, str, len + 1);
		
	}

	inline void _comine(const wchar_t * str)
	{
		int len = wcslen(str);
		realloc(_strData, (_length + len + 1) * _elementSize);
		memcpy(_strData + _length, str, len + 1);
	}

	template<class T>
	inline void _expend(const T* str)
	{
		if (nullptr == str){ return; }
		if (ST_UNKHOW == _st)
		{
			_init(str);
		}
		else if (_st != _charType(str))
		{
			_comine(_convert(str));
		}
		else
		{
			_comine(str);
		}
	}

	inline const String & _expend(const String & str)
	{
		if (ST_WCHAR == str._st)
		{
			_expend(reinterpret_cast<wchar_t*>(str._strData));
		}
		else
		{
			_expend(str._strData);
		}
		return *this;
	}

	template<class T>
	bool _compare(T * str) const
	{
		if (_st != _charType(str)){ return false; }
		if (_length != _strlen(str)){ return false; }
		if (0 != memcmp(_strData, str, _length * _elementSize)){ return false; }
		return true;
	}

private:
	mutable char * _strData = nullptr;//字符串的原始数据
	mutable int _length = 0;//字符的个数
	mutable StringType _st = ST_UNKHOW; //字符串的类型
	mutable int _elementSize = 1;
};
inline const String operator+(const char *  strL, const String&  strR)
{
	String rs = strL;
	return rs += strR;
}
inline const String operator+(const wchar_t *  strL, const String&  strR)
{
	String rs = strL;
	return rs += strR;
}
inline const bool operator==(const char *  strL, const String&  strR)
{
	return strR == strL;
}
inline const bool operator==(const wchar_t *  strL, const String&  strR)
{
	return strR == strL;
}

NAMESPACEEND