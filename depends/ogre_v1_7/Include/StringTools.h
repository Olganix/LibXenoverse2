#ifndef __StringTools__H_
#define __StringTools__H_

#pragma once
/*
#if defined( NONCLIENT_BUILD )
#	define _DllExport __declspec(dllexport)
#else
#	define _DllExport __declspec(dllimport)
#endif*/

//#include <string>
////#include <stdlib.h>
//#include <stdio.h>
//#include <stdarg.h>
#include <OgreString.h>
#include <OgreStringConverter.h>

namespace Ogre 
{
//#ifdef UNICODE
#if OGRE_WCHAR_T_STRINGS
	#define TEXT(quote) L##quote      // r_winnt
	//typedef std::wstring String;
	typedef wchar_t Char;
	//typedef _vscwprintf vScPrintf;
	//typedef vswprintf_s vSPrintf;
	#define vScPrintf _vscwprintf
	#define vSPrintf vswprintf_s
#else

	/*#ifndef TEXT
		//#undef TEXT
		#define TEXT(quote) quote      // r_winnt
	#endif*/

	
	//typedef std::string String;
	typedef char Char;
	//typedef _vscprintf vScPrintf;
	//typedef vsprintf_s vSPrintf;
	#define vScPrintf _vscprintf
	#define vSPrintf vsprintf_s
#endif

	//typedef unsigned char uchar;
	class StringUtilExt : public StringUtil
	{
	public:
		static String format(String format,...)
		{
			va_list args;
			va_start( args, format );
			const Char* f = format.c_str();
			int len = _vscprintf( f, args )+1; // _vscprintf doesn't count terminating '\0'
			Char* buffer = new Char[len];
		//	Char* buffer = (Char*)malloc( len*sizeof(Char) );
			vSPrintf( buffer, len, f, args );
			String s( buffer );
			delete [] buffer;
		//	free( buffer );
			va_end(args);
			return s;
		}

		//using StringUtil::trim;

		/** Overloads the Ogre version with offering the chars to trim.
		*/
		static void trim(String& str, bool left=true, bool right=true, const String delims=" \t\r\n")
		{
			if(right)
				str.erase(str.find_last_not_of(delims)+1); // trim right
			if(left)
				str.erase(0, str.find_first_not_of(delims)); // trim left
		}

		inline static String WSTRtoString(const wchar_t* wstr)
		{
			size_t len = wcslen(wstr);
			char *nstr = new char[len+1];
			wcstombs(nstr,wstr,len+1);
			String newString(nstr);
			delete[] nstr;
			return newString;
		}

		inline static wchar_t* StringtoWSTR(const String& str)
		{
			size_t len = str.length();
			wchar_t *wch = new wchar_t[len+1];
			mbstowcs(wch,str.c_str(),len+1);
			return wch;
		}
	};

	class StringConverterExt : public StringConverter
	{
	public:
		template <class T> static T parse(const String & val, T defaultValue=0)
		{
			if (typeid(T)==typeid(bool))
			{
				return /*dynamic_cast<T>*/(StringUtil::startsWith(val, "true") || StringUtil::startsWith(val, "yes")
						|| StringUtil::startsWith(val, "1"));
				//defaultValue = (StringUtil::startsWith(val, "true") || StringUtil::startsWith(val, "yes")
				//		|| StringUtil::startsWith(val, "1"));
			}
			//if (typeid(T)==typeid(Vector2))
			//{
			//}

			//else
			{
				std::istringstream str(val);
		//			T ret = 0;
				str >> defaultValue;
			}

			return defaultValue;
		}

		/// Mainly there to fool out Compilers
		inline static String parse(const String & val, const String &defaultValue=StringUtil::BLANK)
		{
			return val;
		}
		inline static const Char* parse(const String & val, const Char * defaultValue)
		{
			return val.c_str();
		}

	};

}
#endif //__StringTools__H_