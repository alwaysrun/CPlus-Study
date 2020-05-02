#pragma once
#ifndef XUGD_CLIB_TIME_H
#define XUGD_CLIB_TIME_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.config.h
// @LIB: xugd.clib.lib/xugd.clib_d.lib
// @DESCRIPTION: ����ʱ������ӿ�
// @VERSION: 1.0.15.1021
//
// @RECORD:
//	��д��Xugd��2015-09-07����ʼ�汾��
//	���ƣ�Xugd��2015-10-21����ȡʱ���ַ����������ɾ�̬�����Ϊ����ָ�루���飩
//////////////////////////////////////////////////////////////////////////
#include "xugd.common.h"
#include "xugd.time.h"
#include <time.h>
#include <string>
#include <iostream>
#include <sys/timeb.h>

// ��ͨ��<boost/date_time/posix_time/posix_time.hpp>��ȡ��ȷ��΢���ʱ��ptime��
//  microsec_clock::universal_time()��������ʱ�䣻
//  microsec_clock::local_time() ����ʱ�� 
namespace xugd{	namespace clib{
	using namespace std;

	//////////////////////////////////////////////////////////////////////////
	// @CLASS: XDateTime
	// @DESCRIPTION: ����ʱ����صĳ��ò����ӿڵ���
	// @VERSION: 1.0.15.1021
	//
	// @RECORD:
	//	��д��Xugd��2015-09-07����ʼ�汾��
	//	���ƣ�Xugd��2015-10-21����ȡʱ���ַ����������ɾ�̬�����Ϊ����ָ�루���飩
	//////////////////////////////////////////////////////////////////////////
	class XCLIBCLASS_DECLARE XDateTime
	{
	public:
		//////////////////////////////////////////////////////////////////////////

		XDateTime(); // Ĭ�Ϲ��캯����ȫ��Ϊ��
		/////////////////////////////////////////////////
		// @DESCRIPTION: ʹ��timeb�����࣬���Ȱ�ʱ��ת��Ϊ����ʱ��
		// @PARAMS: 
		//		tTime����ʱ����Ϣ��ʱ��
		/////////////////////////////////////////////////
		XDateTime(const timeb &tTime_);

		/////////////////////////////////////////////////
		// @DESCRIPTION: ʹ��time_t�ͺ����������࣬���Ȱ�ʱ��ת��Ϊ����ʱ��
		// @PARAMS: 
		//		tTime��ʱ�䣨��1970-1-1����ʱ�����ڵ�������
		//		nMSec��������
		/////////////////////////////////////////////////
		XDateTime(time_t tTime_, unsigned short nMSec_=0);

		/////////////////////////////////////////////////
		// @DESCRIPTION: ʹ��tm�ͺ�����������
		// @PARAMS: 
		//		tTime��tm���ͣ���ȷ����
		//		nMSec��������
		/////////////////////////////////////////////////
		XDateTime(const tm &tTime_, unsigned short nMSec_=0); 

		// ��������
		virtual ~XDateTime();

		//////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////
		// @DESCRIPTION: ʹ�ñ���ʱ������ʱ����Ϣ
		// @PARAMS: ��
		/////////////////////////////////////////////////
		void Now();	

		/////////////////////////////////////////////////
		// @DESCRIPTION: ���ַ����л�ȡʱ����Ϣ�������κ�ת����
		// @PARAMS: 
		//		pTime��ʱ���ַ���
		/////////////////////////////////////////////////
		void FromWString(const wchar_t *pTime_);	// ���ַ���ʽ
		void FromAString(const char *pTime_);		// խ�ַ���ʽ
		
		/////////////////////////////////////////////////
		// @DESCRIPTION: ʹ��Epoch�ͺ����������࣬���Ȱ�ʱ��ת��Ϊ����ʱ��
		// @PARAMS: 
		//		nSeconds����1970-1-1����ʱ�����ڵ�����
		//		nMSec��������
		/////////////////////////////////////////////////
		void FromEpoch1970(int nSeconds_, int nMSec_=0);

		//////////////////////////////////////////////////////////////////////////

		// ��
		int Year() const 
		{
			return m_tTime.tm_year + 1900;
		}
		// ��
		int Month() const
		{
			return m_tTime.tm_mon + 1;
		}
		// ��
		int Day() const 
		{
			return m_tTime.tm_mday;
		}
		// Сʱ
		int Hour() const
		{
			return m_tTime.tm_hour;
		}
		// ����
		int Minute() const
		{
			return m_tTime.tm_min;
		}
		// ��
		int Second() const
		{
			return m_tTime.tm_sec;
		}

		// 0~999(1/1000 second)
		int MilliSecond() const
		{
			return m_nMillSecond;
		}

		// ���ڼ���0~6, 0 is Sunday
		int DayOfWeek() const
		{ 
			return m_tTime.tm_wday;
		}

		//////////////////////////////////////////////////////////////////////////
		// ���ʽ��������λ2010
		static wchar_t* const FmtYear;
		// ���ʽ��������λ10
		static wchar_t* const FmtYearShort; 
		// �¸�ʽ����
		static wchar_t* const FmtMonth;
		// �ո�ʽ����
		static wchar_t* const FmtDay;
		// Сʱ��ʽ������24Сʱ�ƣ�
		static wchar_t* const FmtHour;
		// Сʱ��ʽ������12Сʱ�ƣ�
		static wchar_t* const FmtHour12;
		// ���Ӹ�ʽ����
		static wchar_t* const FmtMinute;
		// ���ʽ����
		static wchar_t* const FmtSecond;
		// �����ʽ����
		static wchar_t* const FmtMilliSecond;

		//////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////
		// @DESCRIPTION: ��ʽ��ʱ�䣻ʹ���ڲ����飨���̰߳�ȫ����
		//			����yyyy-MM-dd HH:mm:ss[.fff]��ʽ���ַ���
		// @PARAMS: 
		//		bHasMSec������Ƿ��������
		/////////////////////////////////////////////////
		const wchar_t* ToWString(bool bHasMSec_=false) const;	// ���ַ���ʽ
		const char* ToAString(bool bHasMSec_=false) const;	// խ�ַ���ʽ

		/////////////////////////////////////////////////
		// @DESCRIPTION: ���ݸ�ʽ���ַ�����ʽ��ʱ�䣻ʹ���ڲ����飨���̰߳�ȫ�����1024�ַ���
		//		year:{yyyy}; month:{MM}; day:{dd}
		//		hour:{HH}; minute:{mm}; second:{ss}
		//		millisecond:{fff}
		// @PARAMS: 
		//		pFormat����ʽ���ַ���
		/////////////////////////////////////////////////
		const wchar_t* ToWString(const wchar_t *pFormat_) const;

		/////////////////////////////////////////////////
		// @DESCRIPTION: �����ʽ��ʱ�䵽���У�ʹ���ڲ����飨���̰߳�ȫ����
		//			Ϊyyyy-MM-dd HH:mm:ss.fff��ʽ���ַ���
		// @PARAMS: 
		//		out�������
		//		xTm��ʱ��
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
		// ʱ��ת��Ϊ����
		static long Time2MSec(int nHoure_, int nMinutes_, int nSecond_);
		static long Second2MSec(int nSecond_);

		// ʱ�����Ӧ�ĺ�����
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
	// @DESCRIPTION: ��ȡ��ǰʱ������ӿڵ���
	// @VERSION: 1.0.15.907
	//
	// @RECORD:
	//	��д��Xugd��2015-09-07����ʼ�汾��
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