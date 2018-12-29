#pragma once
#ifndef XUGD_CLIB_TIME_H
#define XUGD_CLIB_TIME_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.config.h
// @LIB: xugd.clib.lib/xugd.clib_d.lib
// @DESCRIPTION: 定义时间操作接口
// @VERSION: 1.0.15.1021
//
// @RECORD:
//	编写：Xugd，2015-09-07，初始版本。
//	完善：Xugd，2015-10-21，获取时间字符串缓冲区由静态数组改为类内指针（数组）
//////////////////////////////////////////////////////////////////////////
#include "xugd.common.h"
#include "xugd.time.h"
#include <time.h>
#include <string>
#include <iostream>
#include <sys/timeb.h>

// 可通过<boost/date_time/posix_time/posix_time.hpp>获取精确到微秒的时间ptime：
//  microsec_clock::universal_time()格林威治时间；
//  microsec_clock::local_time() 本地时间 
namespace xugd{	namespace clib{
	using namespace std;

	//////////////////////////////////////////////////////////////////////////
	// @CLASS: XDateTime
	// @DESCRIPTION: 定义时间相关的常用操作接口的类
	// @VERSION: 1.0.15.1021
	//
	// @RECORD:
	//	编写：Xugd，2015-09-07，初始版本。
	//	完善：Xugd，2015-10-21，获取时间字符串缓冲区由静态数组改为类内指针（数组）
	//////////////////////////////////////////////////////////////////////////
	class XCLIBCLASS_DECLARE XDateTime
	{
	public:
		//////////////////////////////////////////////////////////////////////////

		XDateTime(); // 默认构造函数，全部为零
		/////////////////////////////////////////////////
		// @DESCRIPTION: 使用timeb构造类，会先把时间转换为本地时间
		// @PARAMS: 
		//		tTime：带时区信息的时间
		/////////////////////////////////////////////////
		XDateTime(const timeb &tTime_);

		/////////////////////////////////////////////////
		// @DESCRIPTION: 使用time_t和毫秒数构造类，会先把时间转换为本地时间
		// @PARAMS: 
		//		tTime：时间（从1970-1-1日零时到现在的秒数）
		//		nMSec：毫秒数
		/////////////////////////////////////////////////
		XDateTime(time_t tTime_, unsigned short nMSec_=0);

		/////////////////////////////////////////////////
		// @DESCRIPTION: 使用tm和毫秒数构造类
		// @PARAMS: 
		//		tTime：tm类型，精确到秒
		//		nMSec：毫秒数
		/////////////////////////////////////////////////
		XDateTime(const tm &tTime_, unsigned short nMSec_=0); 

		// 析构函数
		virtual ~XDateTime();

		//////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////
		// @DESCRIPTION: 使用本地时间重设时间信息
		// @PARAMS: 无
		/////////////////////////////////////////////////
		void Now();	

		/////////////////////////////////////////////////
		// @DESCRIPTION: 从字符串中获取时间信息（不做任何转换）
		// @PARAMS: 
		//		pTime：时间字符串
		/////////////////////////////////////////////////
		void FromWString(const wchar_t *pTime_);	// 宽字符格式
		void FromAString(const char *pTime_);		// 窄字符格式
		
		/////////////////////////////////////////////////
		// @DESCRIPTION: 使用Epoch和毫秒数构造类，会先把时间转换为本地时间
		// @PARAMS: 
		//		nSeconds：从1970-1-1日零时到现在的秒数
		//		nMSec：毫秒数
		/////////////////////////////////////////////////
		void FromEpoch1970(int nSeconds_, int nMSec_=0);

		//////////////////////////////////////////////////////////////////////////

		// 年
		int Year() const 
		{
			return m_tTime.tm_year + 1900;
		}
		// 月
		int Month() const
		{
			return m_tTime.tm_mon + 1;
		}
		// 日
		int Day() const 
		{
			return m_tTime.tm_mday;
		}
		// 小时
		int Hour() const
		{
			return m_tTime.tm_hour;
		}
		// 分钟
		int Minute() const
		{
			return m_tTime.tm_min;
		}
		// 秒
		int Second() const
		{
			return m_tTime.tm_sec;
		}

		// 0~999(1/1000 second)
		int MilliSecond() const
		{
			return m_nMillSecond;
		}

		// 星期几：0~6, 0 is Sunday
		int DayOfWeek() const
		{ 
			return m_tTime.tm_wday;
		}

		//////////////////////////////////////////////////////////////////////////
		// 年格式化符，四位2010
		static wchar_t* const FmtYear;
		// 年格式化符，两位10
		static wchar_t* const FmtYearShort; 
		// 月格式化符
		static wchar_t* const FmtMonth;
		// 日格式化符
		static wchar_t* const FmtDay;
		// 小时格式化符（24小时制）
		static wchar_t* const FmtHour;
		// 小时格式化符（12小时制）
		static wchar_t* const FmtHour12;
		// 分钟格式化符
		static wchar_t* const FmtMinute;
		// 秒格式化符
		static wchar_t* const FmtSecond;
		// 毫秒格式化符
		static wchar_t* const FmtMilliSecond;

		//////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////
		// @DESCRIPTION: 格式化时间；使用内部数组（非线程安全）；
		//			返回yyyy-MM-dd HH:mm:ss[.fff]格式的字符串
		// @PARAMS: 
		//		bHasMSec：输出是否包含毫秒
		/////////////////////////////////////////////////
		const wchar_t* ToWString(bool bHasMSec_=false) const;	// 宽字符格式
		const char* ToAString(bool bHasMSec_=false) const;	// 窄字符格式

		/////////////////////////////////////////////////
		// @DESCRIPTION: 根据格式化字符串格式化时间；使用内部数组（非线程安全），最长1024字符；
		//		year:{yyyy}; month:{MM}; day:{dd}
		//		hour:{HH}; minute:{mm}; second:{ss}
		//		millisecond:{fff}
		// @PARAMS: 
		//		pFormat：格式化字符串
		/////////////////////////////////////////////////
		const wchar_t* ToWString(const wchar_t *pFormat_) const;

		/////////////////////////////////////////////////
		// @DESCRIPTION: 输出格式化时间到流中；使用内部数组（非线程安全）；
		//			为yyyy-MM-dd HH:mm:ss.fff格式的字符串
		// @PARAMS: 
		//		out：输出流
		//		xTm：时间
		/////////////////////////////////////////////////
		friend wostream& operator<<(wostream& out_, const XDateTime& xTm_)	
		{
			out_<<xTm_.ToWString(true);
			return out_;
		}
		friend wistream& operator>>(wistream &in_, XDateTime& xTm_)
		{
			wstring strDate, strTime;
			in_>>strDate >> strTime;
			wstring strOut = strDate + L" " + strTime;
			xTm_.FromWString(strOut.c_str());
			return in_;
		}

		//////////////////////////////////////////////////////////////////////////
		// 时间转换为毫秒
		static long Time2MSec(int nHoure_, int nMinutes_, int nSecond_);
		static long Second2MSec(int nSecond_);

		// 时分秒对应的毫秒数
		static const int OneSecond2MSec = 1000;
		static const int OneMinite2MSec = 60 * OneSecond2MSec;
		static const int OneHour2Msec = 60 * OneMinite2MSec;

	private:
		//CxForbidCopyFun(XDateTime)
		void Init(const tm &tTime_, unsigned short nMSec_);
		wchar_t* TryGetTime(wchar_t **ppFormat_) const;
		unsigned short m_nMillSecond;
		tm m_tTime;
		mutable wchar_t m_wideTime[32];
		mutable wchar_t *m_pWideFormat;
	};


	//////////////////////////////////////////////////////////////////////////
	// @CLASS: XDateTime
	// @DESCRIPTION: 获取当前时间操作接口的类
	// @VERSION: 1.0.15.907
	//
	// @RECORD:
	//	编写：Xugd，2015-09-07，初始版本。
	//////////////////////////////////////////////////////////////////////////
	class XCLIBCLASS_DECLARE XDateNow:public XDateTime
	{
	public:
		XDateNow()
		{
			Now();
		}
	};
} //clib
} // xugd
#endif // XUGD_CLIB_TIME_H