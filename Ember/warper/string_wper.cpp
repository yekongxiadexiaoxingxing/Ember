#include "string_wper.h"
#include <string.h>

NAMESPACEBEGINE

String::String( const char* astr)
{
	_init(astr);
}
String::String( const wchar_t* wstr)
{
	_init(wstr);
}
String::String(const char*astr, int length)
{
	_init(astr, length);
}
String::String(const wchar_t*wstr, int length)
{
	_init(wstr, length);
}

String::String(const String && str)
{
	destory();
	_st = str._st;
	_strData = str._strData;
	_length = str._length;
	_elementSize = str._elementSize;
	str.destory();
}

String::~String()
{
	if (_strData)
	{
		free(_strData);
		_strData = nullptr; 
	}
}
NAMESPACEEND