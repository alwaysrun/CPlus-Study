#include "xugd.common.h"
#include "xugd.time.h"
#include "xugd.str.h"
#include "xugd.convert.h"
#include <string.h>
#include <stdio.h>
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace xugd{	namespace clib{
	//////////////////////////////////////////////////////////////////////////
	// Date
	wchar_t* const XDateTime::FmtYear=L"{yyyy}";
	wchar_t* const XDateTime::FmtYearShort=L"{yy}";
	wchar_t* const XDateTime::FmtMonth=L"{MM}";
	wchar_t* const XDateTime::FmtDay=L"{dd}";
	wchar_t* const XDateTime::FmtHour=L"{HH}";
	wchar_t* const XDateTime::FmtHour12=L"{hh}"; // 12Ð¡Ê±ÖÆ
	wchar_t* const XDateTime::FmtMinute=L"{mm}";
	wchar_t* const XDateTime::FmtSecond=L"{ss}";
	wchar_t* const XDateTime::FmtMilliSecond=L"{fff}";

	//////////////////////////////////////////////////////////////////////////
	XDateTime::XDateTime()
	{
		m_pWideFormat = NULL;
	}

	XDateTime::XDateTime(const timeb &tTime_)
	{
		tm tmNow;
		localtime_s(&tmNow, &tTime_.time);
		Init(tmNow, tTime_.millitm);
	}
	XDateTime::XDateTime(time_t tTime_, unsigned short nMSec_)
	{
		tm tmNow;
		localtime_s(&tmNow, &tTime_);
		Init(tmNow, nMSec_);
	}
	XDateTime::XDateTime(const tm &tTime_, unsigned short nMSec_)
	{
		Init(tTime_, nMSec_);
	}
	XDateTime::~XDateTime(){
		if(CxNotNull(m_pWideFormat)){
			delete[] m_pWideFormat;
			m_pWideFormat = NULL;
		}
	}

	void XDateTime::Now()
	{
		struct timeb tNow;
		tm tmNow;
		ftime(&tNow);
		localtime_s(&tmNow, &tNow.time);
		Init(tmNow, tNow.millitm);
	}

	void XDateTime::FromWString(const wchar_t *pTime_)
	{
		string strTime=XConvert::Wstr2A(pTime_);
		FromAString(strTime.c_str());
	}

	void XDateTime::FromAString(const char *pTime_){
		boost::posix_time::ptime pt = boost::posix_time::time_from_string(pTime_);
		this->m_tTime = boost::posix_time::to_tm(pt);
		this->m_nMillSecond = (unsigned short)(pt.time_of_day().fractional_seconds()/1000);
	}

	void XDateTime::FromEpoch1970(int nSeconds_, int nMSec_){
		boost::gregorian::date dt1970(1970, 1, 1);
		long tmZone = 0;
		_get_timezone(&tmZone);
		auto diffTime = boost::posix_time::time_duration(0, 0, nSeconds_ + tmZone, nMSec_*1000);
		boost::posix_time::ptime pt1970(dt1970, diffTime);

		this->m_tTime = boost::posix_time::to_tm(pt1970);
		this->m_nMillSecond = (nMSec_==0)?0:(unsigned short)(pt1970.time_of_day().fractional_seconds()/1000);
	}

	const wchar_t* XDateTime::ToWString(bool bHasMSec_) const
	{
		m_wideTime[0]=NULL;
		if(bHasMSec_)
			swprintf_s(m_wideTime, 32, L"%04d-%02d-%02d %02d:%02d:%02d.%03d",
				this->Year(), this->Month(), this->Day(),
				this->Hour(), this->Minute(), this->Second(),
				this->MilliSecond());
		else
			swprintf_s(m_wideTime, 32, L"%04d-%02d-%02d %02d:%02d:%02d",
				this->Year(), this->Month(), this->Day(),
				this->Hour(), this->Minute(), this->Second());

		return (m_wideTime);
	}

	const char* XDateTime::ToAString(bool bHasMSec_) const{
		char* szTime = (char*)m_wideTime;
		szTime[0]=NULL;
		if(bHasMSec_)
			sprintf_s(szTime, 32, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
			this->Year(), this->Month(), this->Day(),
			this->Hour(), this->Minute(), this->Second(),
			this->MilliSecond());
		else
			sprintf_s(szTime, 32, "%04d-%02d-%02d %02d:%02d:%02d",
			this->Year(), this->Month(), this->Day(),
			this->Hour(), this->Minute(), this->Second());

		return (szTime);
	}

	wchar_t* XDateTime::TryGetTime(wchar_t **ppFormat_) const
	{
		wchar_t *pIn = *ppFormat_;

		*ppFormat_=pIn+4; // default skip 4 character
		if(XWptrCountEqual(pIn, FmtYear, 6)) // {yyyy}
		{
			swprintf_s(m_wideTime, 10 ,L"%4d", this->Year());
			*ppFormat_=pIn+6;
		}
		else if(XWptrCountEqual(pIn, FmtYearShort, 4)) // {yy}
		{
			swprintf_s(m_wideTime, 10 ,L"%02d", this->Year()%100);
		}
		else if(XWptrCountEqual(pIn, FmtMonth, 4)) // {MM}
		{
			swprintf_s(m_wideTime, 10 ,L"%02d", this->Month());
		}
		else if(XWptrCountEqual(pIn, FmtDay, 4)) // {dd}
		{
			swprintf_s(m_wideTime, 10 ,L"%02d", this->Day());
		}
		else if(XWptrCountEqual(pIn, FmtHour, 4)) // {HH}
		{
			swprintf_s(m_wideTime, 10 ,L"%02d", this->Hour());
		}
		else if(XWptrCountEqual(pIn, FmtHour12, 4)) // {hh}
		{
			swprintf_s(m_wideTime, 10 ,L"%02d", this->Hour()%12);
		}
		else if(XWptrCountEqual(pIn, FmtMinute, 4)) // {mm}
		{
			swprintf_s(m_wideTime, 10 ,L"%02d", this->Minute());
		}
		else if(XWptrCountEqual(pIn, FmtSecond, 4)) // {ss}
		{
			swprintf_s(m_wideTime, 10 ,L"%02d", this->Second());
		}
		else if(XWptrCountEqual(pIn, FmtMilliSecond, 5)) //{fff}
		{
			swprintf_s(m_wideTime, 10 ,L"%03d", this->MilliSecond());
			*ppFormat_=pIn+5;
		}
		else
		{
			m_wideTime[0]=*pIn;
			m_wideTime[1]=NULL;
			*ppFormat_=pIn+1;
		}

		return m_wideTime;
	}

	const wchar_t* XDateTime::ToWString(const wchar_t *pFormat_) const
	{
		if(CxIsNull(m_pWideFormat)){
			m_pWideFormat = new wchar_t[1024];
			CxZeroMemory(m_pWideFormat, 1024*sizeof(wchar_t));
		}

		wchar_t *pOut=m_pWideFormat;
		wchar_t *pIn;
		pIn=(wchar_t*)static_cast<const wchar_t*>(pFormat_);

		*pOut=NULL;
		while(*pIn)
		{
			if(*pIn=='{')
			{
				wchar_t *pTime=TryGetTime(&pIn);
				while(*pTime)
					*pOut++=*pTime++;
			}
			else
			{
				*pOut++=*pIn++;
			}
		}
		*pOut=NULL;

		return m_pWideFormat;
	}

	long XDateTime::Time2MSec(int nHoure_, int nMinutes_, int nSecond_)
	{
		return (long)(boost::posix_time::time_duration(nHoure_, nMinutes_, nSecond_).total_milliseconds());
	}

	long XDateTime::Second2MSec(int nSecond_){
		return (long)(nSecond_ * OneSecond2MSec);
	}

	void XDateTime::Init(const tm &tTime_, unsigned short nMSec_)
	{
		m_pWideFormat = NULL;
		memcpy(&m_tTime, &tTime_, sizeof(tm));
		m_nMillSecond = nMSec_;
	}

} //clib
} // xugd