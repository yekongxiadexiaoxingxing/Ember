#pragma once
#include <string>
#include <stdlib.h>
namespace Ext
{
	template <class T, class P> class Base_String;

	typedef Base_String<std::wstring, std::string> WString;
	typedef Base_String<std::string, std::wstring> MString;

	inline const WString & StringAssin(const WString & wstr, const std::string &str);

	inline const MString & StringAssin(const MString &mstr, const std::wstring &wstr);

	template <class T, class P>
	class Base_String : public T
	{
	public:
		typedef T Type;
		Base_String()
		{
		}
		Base_String(typename const P::value_type* pStr)
		{
			typename const Base_String::value_type *pT = codeConver(pStr);
			*this = pT;
			free((void*)pT);
		}
		Base_String(typename const T::value_type* pStr)
		{
			//static_cast<typename T>(*this) = pStr;
			T::operator=(pStr);
		}
		//需要添加右值处理
		Base_String(const P&& str)
		{
			StringAssin(*this, static_cast<const P&>(str));
		}

		Base_String(const P& str)
		{
			StringAssin(*this, str);
		}

		Base_String(const T&& str)
		{
			//StringAssin(*this, reinterpret_cast<const P&>(str));
			T::operator=(str);
		}

		Base_String(const T& str)
		{
			//StringAssin(*this, str);
			T::operator=(str);
		}

		/*const Base_String<T, P>& operator=(const P& p)
		{
			return StringAssin(*this, p);
		}*/

		///返回所占内存的字节数
		inline size_t size()
		{
			return T::size() * sizeof(Type);
		}

		inline size_t size() const
		{
			return T::size() * sizeof(Type);
		}

		const Base_String<T, P>& operator=(typename const P::value_type *pStr)
		{
			typename const T::value_type *pT	=	codeConver(pStr);
 			//static_cast<T>(*this)	=	pT;
			//(T)(*this)	=	pT;
			T::operator=(pT);
			free((void*)(pT));
			return *this;
		}

		const Base_String<T, P>& operator=(typename const T::value_type *pStr)
		{
			//static_cast<T>(*this)	=	pStr;
			T::operator=(pStr);
			return *this;
		} 

		const Base_String<T, P>& operator=(const Base_String<T, P> & rhs)
		{
			//(T)(*this) = (rhs);
			T::operator=(rhs);
			return *this;
		}

		/*operator P () const
		{
			typename const P::value_type *pT = codeConver(this->c_str());
			P p = pT;
			free((void*)(pT));
			return p;
		}*/

		/*operator std::string () const
		{
			typename const std::string::value_type *pT = codeConver(this->c_str());
			std::string p = pT;
			free((void*)(pT));
			return p;
		}


		operator T () const
		{
			typename const T::value_type *pT = codeConver(this->c_str());
			T p = pT;
			free((void*)(pT));
			return p;
		}*/

		/*const T operator +(typename T::value_type *pStr)
		{
			return T::operator+(pStr);
		}*/

		const T operator +(typename P::value_type *pStr)
		{
			const T::value_type* p = codeConver(pStr);
			T cz = T::operator+(p);
			free(void*(p));
			return cz;
		}

		/*const T operator +(typename const T &pStr)
		{
			return T::operator+(pStr);
		}

		const T operator +(typename const P &pStr)
		{
			T t = p;
			return T::operator+(t);
		}*/

		const T& operator +=(typename const P::value_type *pStr)
		{
			const T::value_type* p = codeConver(pStr);
			*this += p;
			free((void*)(p));
			return *this;
		}

		/*const T& operator +=(typename const P::value_type *pStr) const
		{
			const T::value_type* p = CodeConver(pStr);
			*this += p;
			free((void*) (p));
			return *this;
		}*/

		const T& operator +=(typename const T::value_type *pStr)
		{
			T::operator+=(pStr);
			return *this;
		}

		const T& operator +=(const T &str)
		{
			T::operator+=(str);
			return *this;
		}

		const wchar_t * c_wstr() const
		{
			if(1 < sizeof(Type))
			{
				return (const wchar_t *)c_str();
			}
			else
			{
				return codeConver((const char*)c_str());
			}
		}
		const char * c_sstr() const
		{
			if(1 < sizeof(Type))
			{
				return codeConver((const wchar_t*)c_str());
			}
			else
			{
				return (const char *)c_str();
			}
		}

		operator const char * ()
		{
			return c_sstr();
		}

		operator const wchar_t * ()
		{
			return c_wstr();
		}

	};

	inline const WString & stringAssin(WString & wstr, const std::string &str)
	{
		std::string::value_type *pT = const_cast<std::string::value_type*>(str.c_str());
		size_t sizeO = str.size() + 1;
		size_t maxSize = (mbstowcs(NULL, pT, sizeO) + 1);
		WString::value_type *pW = static_cast<wchar_t*>(malloc(maxSize * sizeof(WString::value_type)));
		maxSize = mbstowcs(pW, pT, sizeO);
		pW[maxSize] = 0;
		wstr = pW;
		free(pW);
		return wstr;
	}

	inline const MString & stringAssin(MString &mstr, const std::wstring &wstr)
	{
		wchar_t *pW = const_cast<wchar_t*>(wstr.c_str());
		size_t sizeO = wstr.size() + 1;
		size_t maxSize = wcstombs(NULL, pW, sizeO) + 1;
		char *pT = static_cast<char*>(malloc(maxSize * sizeof(char)));
		maxSize = wcstombs(pT, pW, sizeO);
		*(MString::Type*)(&mstr) = pT;
		free(pT);
		return mstr;
	}

	
	/*inline const WString::value_type * CodeConver(const MString::value_type *pStr)
		{
			MString::value_type *pW = const_cast<MString::value_type *>(pStr);
			size_t sizeO = strlen(pStr);
			size_t maxSize = sizeof(WString::value_type) * (1 + mbstowcs(NULL, pStr, sizeO));
			WString::value_type *pT = (WString::value_type *)(malloc(maxSize * sizeof(*pStr)));
			maxSize = mbstowcs(pT, pStr, sizeO);
			pT[maxSize] = 0;
			return pT;
		}
	
		inline const MString::value_type * CodeConver(const WString::value_type *pStr)
		{
			WString::value_type *pW = const_cast<WString::value_type *>(pStr);
			size_t sizeO = wcslen(pStr);//(wcslen(pStr) + 1) * sizeof(*pStr);
			size_t maxSize = sizeof(MString::value_type) * (wcstombs(NULL, pStr, sizeO) + 1);
			MString::value_type *pT = (MString::value_type *)(malloc(maxSize * sizeof(*pStr)));
			maxSize = wcstombs(pT, pStr, sizeO);
			pT[maxSize] = 0;
			return pT;
		}*/
	inline const wchar_t * codeConver(const char *pStr)
	{
		char *pW = const_cast<char *>(pStr);
		size_t sizeO = strlen(pStr);
		size_t maxSize = sizeof(wchar_t) * (1 + mbstowcs(NULL, pStr, sizeO));
		wchar_t *pT = (wchar_t *)(malloc(maxSize * sizeof(*pStr)));
		maxSize = mbstowcs(pT, pStr, sizeO);
		pT[maxSize] = 0;
		return pT;
	}

	inline const char * codeConver(const wchar_t *pStr)
	{
		WString::value_type *pW = const_cast<wchar_t *>(pStr);
		size_t sizeO = wcslen(pStr);
		size_t maxSize = sizeof(char) * (wcstombs(NULL, pStr, sizeO) + 1);
		char *pT = (char *)(malloc(maxSize * sizeof(*pStr)));
		maxSize = wcstombs(pT, pStr, sizeO);
		pT[maxSize] = 0;
		return pT;
	}

#ifdef _UNICODE
	typedef Base_String<std::wstring, std::string> String;
	//typedef WString String;
#else
	typedef Base_String<std::string, std::wstring> String;
	//typedef MString String;
#endif

	inline String  operator +(const char *pStr, const String & rhs)
	{
		String lhs = pStr;
		lhs += rhs;
		return lhs;
	}

	inline String  operator +(const wchar_t *pStr, const String & rhs)
	{
		String lhs = pStr;
		lhs += rhs;
		return lhs;
	}

	class Utf8String
	{

	};

}
