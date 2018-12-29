#include "xugd.log.h"
#include "xugd.file.h"
#include "xugd.time.h"
#include "xugd.thread.h"
#include "xugd.str.h"
#include "xugd.convert.h"
#include <cstdio>
#include <cstdarg>
#include <queue>
#include <fstream>
#include <process.h>
#include <boost/thread.hpp>
#include <boost/format.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/algorithm/string.hpp>

namespace xugd{	namespace clib{
	using namespace boost;
	//////////////////////////////////////////////////////////////////////////
	// XLogConfig
	XLogConfig::XLogConfig()
	{
		WithTime = true;
		LogLevel = (int)XLogFile::LogDefault;
		m_pFormat = NULL;
	}
	XLogConfig::XLogConfig(const XLogConfig &other_){
		WithTime = other_.WithTime;
		LogLevel = other_.LogLevel;
		wstring str = other_.GetNameFormat();
		SetNameFormat(str);
	}
	XLogConfig& XLogConfig::operator =(const XLogConfig &other_){
		WithTime = other_.WithTime;
		LogLevel = other_.LogLevel;
		wstring str = other_.GetNameFormat();
		SetNameFormat(str);

		return *this;
	}
	XLogConfig::~XLogConfig()
	{
		if(CxNotNull(m_pFormat))
		{
			delete[] m_pFormat;
		}
	}
	void XLogConfig::ToConfig(XConFile &conFile_)
	{
		conFile_.SetAttr(L"", L"WithTime", WithTime);
		conFile_.SetAttr(L"", L"LogLevel", LogLevel);
		conFile_.SetAttr(L"", L"NameFormat", m_pFormat);
	}
	void XLogConfig::FromConfig(const XConFile &conFile_)
	{
		WithTime = conFile_.GetAttr(L"", L"WithTime", true);
		LogLevel = conFile_.GetAttr(L"", L"LogLevel", -1);
		wstring strName = conFile_.GetAttr(L"", L"NameFormat", L"");
		SetNameFormat(strName);
	}
	wstring XLogConfig::GetNameFormat() const
	{
		return m_pFormat;
	}
	void XLogConfig::SetNameFormat(const wstring &strNameFmt_)
	{
		if(CxNotNull(m_pFormat))
		{
			delete[] m_pFormat;
			m_pFormat = NULL;
		}
		m_pFormat = new wchar_t[strNameFmt_.length()+1];
		wcscpy_s(m_pFormat, strNameFmt_.length()+1, strNameFmt_.c_str());
	}

	bool XLogConfig::Read(const wstring &strFile_)
	{
		bool bSucc = false;
		XPath fPath(strFile_);
		wstring strFile = fPath.ToAbsolut().ToString();
		try 
		{
			XConFile conFile;
			conFile.Read(strFile);
			FromConfig(conFile);
			bSucc = true;
		}
		catch(std::exception) {}

		return bSucc;
	}

	void XLogConfig::Write(const wstring &strFile_)
	{
		XPath fPath(strFile_);
		wstring strFile = fPath.ToAbsolut().ToString();

		XConFile conFile;
		ToConfig(conFile);
		conFile.Write(strFile);
	}

	// build: log\{yyyy}\{MM}\{dd}\Name{HH}.log
	void XLogConfig::InitHourLog(const wstring &strName_)
	{
		wstring str = (wformat(L"log\\{yyyy}\\{MM}\\{dd}\\%1%{HH}.log")%strName_).str();
		SetNameFormat(str);
	}
	// build: log\{yyyy}\{MM}\Name{dd}.log
	void XLogConfig::InitDayLog(const wstring &strName_)
	{
		wstring str = (wformat(L"log\\{yyyy}\\{MM}\\%1%{dd}.log")%strName_).str();
		SetNameFormat(str);
	}

	//////////////////////////////////////////////////////////////////////////
	static const wchar_t *LOG_LEVELS[] ={
		L"",
		L"CRIT",
		L"ERR",
		L"WARN",
		L"NOTICE",
		L"INFO",
		L"DEBUG",
		NULL
	};
	class XLogFileImp
	{
	public:
		XLogFileImp(const wstring &strName_, bool bWithTime_)
			: m_writeThread(XLogFileImp::WriteThread)
		{
			m_bStarted = true;
			m_pGetInfo = NULL;
			m_bWithTime = bWithTime_;
			SetNameFormat(strName_);
		}

		void SetNameFormat(const wstring &strName_){
			m_strNameFormat = trim_copy(strName_);
			if(m_strNameFormat.empty())
				return;

			NameTimer(m_nameTimer, this);
			m_nameTimer.Start(XLogFileImp::NameTimer, this);
		}

		void SetCallback(XLogFile::PFunGetLogCallback pfun_){
			m_pGetInfo = pfun_;
		}

		~XLogFileImp()
		{
			try 
			{
				if(m_bStarted)
				{
					Close();
				}
				m_bStarted = false;
			}
			catch(...){}
		}

		static void WriteLog(const wstring &strLog_, XLogFileImp *pThis_)
		{
			if(pThis_->m_bReopen)
			{
				try {
					pThis_->m_outFile.close();
					pThis_->m_bReopen = false;
				}
				catch(...){}
			}
			if(!pThis_->m_outFile.is_open())
			{
				XPath logPath(pThis_->m_strFileName);
				logPath.ToAbsolut();
				XDir::CreateDir(logPath.ParentPath());

				XFile::OpenFile(pThis_->m_outFile, logPath.ToString(), ios_base::app);
				auto pos = pThis_->m_outFile.seekp(0, ios::end).tellp();
				if(pos>0)
					pThis_->m_outFile<<endl;
				pThis_->m_outFile<<"#### Open by Process "<<_getpid()<<" ####"<<endl;
			}

			pThis_->m_outFile<<strLog_<<flush;
		}

		static void WriteThread(void *pThis_){
			XLogFileImp *pImp = (XLogFileImp*)pThis_;
			try 
			{
				while(true)
				{
					pImp->m_quEvent.TimeWait(0, 0, 30);
					while(true)
					{
						wstring strLog;
						{
							mutex::scoped_lock lker(pImp->m_quMutex);
							if(pImp->m_quLog.empty())
								break;
							strLog = pImp->m_quLog.front();
							pImp->m_quLog.pop();
						}

						WriteLog(strLog, pImp);
					}
				}
			}
			catch(std::exception &ex){
				string strInfo = (format("Open %1% failed: %2%")
						%(XConvert::Wstr2A(pImp->m_strFileName))
						%ex.what()).str();
				XFile::AppendLog(L"xugd.clog.err", strInfo);
			}
		}
		static void NameTimer(const XTimer &tm_, void *pThis_){
			XLogFileImp *pImp = (XLogFileImp*)pThis_;
			if(CxIsNull(pImp) || !pImp->m_bStarted) return;
			try 
			{
				XDateNow dtNow;
				wstring strName = dtNow.ToWString(pImp->m_strNameFormat.c_str());
				if(strName != pImp->m_strFileName)
				{
					pImp->m_strFileName = strName;
					pImp->m_bReopen = true;
				}
			}
			catch(...){}
		}

		void WriteLine()
		{
			if(!m_strNameFormat.empty())
			{
				mutex::scoped_lock lker(m_quMutex);
				m_quLog.push(L"\n");
			}
		}

		void AddLogStr(const wstring &strLog_){
			if(!m_bStarted) return;

			if(!m_strNameFormat.empty())
			{
				mutex::scoped_lock lker(m_quMutex);
				m_quLog.push(strLog_);
				m_quEvent.Notify();
				m_writeThread.Start(this);
			}

			if(CxNotNull(m_pGetInfo))
				m_pGetInfo(strLog_);
		}

		void Write(const wstring &strLog_, XLogFile::LogLevel euLevel_, bool bWithTime_, bool bWithLine_, const wchar_t *pPrefix_=L""){
			wostringstream strOut;
			if(bWithTime_)
			{
				XDateNow dtNow;
				strOut<<dtNow.ToWString(true);
			}
			strOut<<"["<<Level2Str(euLevel_)<<"]:"<<"\t";
			strOut<<pPrefix_;
			strOut<<strLog_;
			if(bWithLine_)
				strOut<<"\n";

			AddLogStr(strOut.str());
		}

		void WriteLine(XLogFile::LogLevel euLevel_, const wchar_t *pFmt_, va_list ap_, const wchar_t *pPrefix_=L""){
			int nLen = _vscwprintf(pFmt_, ap_) + 1;
			scoped_array<wchar_t> wBuff(new wchar_t[nLen]);
			vswprintf(wBuff.get(), nLen, pFmt_, ap_);
			
			Write(wBuff.get(), euLevel_, m_bWithTime, true, pPrefix_);
		}

		void Write(const char *strLog_, XLogFile::LogLevel euLevel_, bool bWithTime_, bool bWithLine_, const char *pPrefix_=""){
			wostringstream strOut;
			if(bWithTime_)
			{
				XDateNow dtNow;
				strOut<<dtNow;
			}
			strOut<<"["<<Level2Str(euLevel_)<<"]:"<<"\t";
			strOut<<pPrefix_;
			strOut<<strLog_;
			if(bWithLine_)
				strOut<<"\n";

			AddLogStr(strOut.str());
		}

		void WriteLine(XLogFile::LogLevel euLevel_, const char *pFmt_, va_list ap_, const char *pPrefix_=""){
			int nLen = _vscprintf(pFmt_, ap_) + 1;
			scoped_array<char> wBuff(new char[nLen]);
			vsprintf_s(wBuff.get(), nLen, pFmt_, ap_);

			Write(wBuff.get(), euLevel_, m_bWithTime, true, pPrefix_);
		}

		void Close(){
			m_pGetInfo = NULL;

			m_nameTimer.Stop();

			if(!m_quLog.empty()){
				m_quEvent.Notify();
				XThread::Sleep(200);
			}

			m_writeThread.Stop();
			if(m_outFile.is_open())
				m_outFile.close();

			m_bStarted = false;
		}

		const wchar_t *Level2Str(XLogFile::LogLevel euLevel_){
			if(euLevel_>XLogFile::LogDebug)
				euLevel_ = XLogFile::LogDebug;
			return LOG_LEVELS[euLevel_];
		}
	private:
		bool m_bStarted;
		bool m_bWithTime;
		wstring m_strNameFormat;

		bool m_bReopen;
		wstring m_strFileName;
		wofstream m_outFile;

		mutex m_quMutex;
		XAutoEvent m_quEvent;
		queue<wstring> m_quLog;

		XThread m_writeThread;
		XClockTimer m_nameTimer;
		XLogFile::PFunGetLogCallback m_pGetInfo;
	};

	XLogFile::XLogFile(){
		m_pimp = NULL;
		m_pGetInfo = NULL;
		m_bLogToFile = false;
		m_euLogLevel = LogDefault;
		Open(L"", true);
	}

	XLogFile::XLogFile(const XLogConfig &logConfig_){
		m_pimp = NULL;
		m_pGetInfo = NULL;
		m_bLogToFile = false;
		m_euLogLevel = (LogLevel)logConfig_.LogLevel;
		Open(logConfig_.GetNameFormat(), logConfig_.WithTime);
	} 
	XLogFile::XLogFile(const wstring &strNameFormat_, bool bWithTime_){
		m_pimp = NULL;
		m_pGetInfo = NULL;
		m_bLogToFile = false;
		m_euLogLevel = LogDefault;
		Open(strNameFormat_, bWithTime_);
	}
	XLogFile::~XLogFile(){
		if(CxNotNull(m_pimp))
		{
			try {
				m_pimp->Close();
				delete m_pimp;
			}
			catch(...){}
			m_pimp = NULL;
		}
	}

	void XLogFile::Open(const wstring &strNameFormat_, bool bWithTime_){
		if(m_bLogToFile && CxNotNull(m_pimp)){
			m_pimp->Close();
			delete m_pimp;
			m_pimp = NULL;
		}

		m_bLogToFile = !strNameFormat_.empty();
		if(CxIsNull(m_pimp)){
			m_pimp = new XLogFileImp(strNameFormat_, bWithTime_);
			if(CxNotNull(m_pGetInfo))
				m_pimp->SetCallback(m_pGetInfo);
		}
		else{
			m_pimp->SetNameFormat(strNameFormat_);
		}
	}
	void XLogFile::Close(){
		m_bLogToFile = false;
		m_pGetInfo = NULL;
		if(CxNotNull(m_pimp)){
			m_pimp->SetCallback(NULL);
			m_pimp->Close();
		}
	}

	void XLogFile::SetGetLogCallback(PFunGetLogCallback pfun, bool bDenyLogFile_){
		m_pGetInfo = pfun;
		m_pimp->SetCallback(pfun);

		if(CxNotNull(pfun) && bDenyLogFile_)
		{
			Open(L"", true);
		}
	}

	bool XLogFile::SetLogLevel(LogLevel euLevel_){
		if(euLevel_<0 || euLevel_>LogAll)
			return false;

		if(CxNotNull(m_pimp))
			WriteLine(LogNotice, L"SetLogLevel(%s)", m_pimp->Level2Str(euLevel_));
		m_euLogLevel = euLevel_;
		return true;
	}

	void XLogFile::Write(const wstring &strLog_, LogLevel euLevel_, bool bAddLine_, bool bWithTime_){
		if(IsNeedLog(euLevel_))
			m_pimp->Write(strLog_, euLevel_, bWithTime_, bAddLine_);
	}
	void XLogFile::Write(const string &strLog_, LogLevel euLevel_, bool bAddLine_, bool bWithTime_){
		if(IsNeedLog(euLevel_))
			m_pimp->Write(strLog_.c_str(), euLevel_, bWithTime_, bAddLine_);
	}

	void XLogFile::WriteLine(LogLevel euLevel_, const wchar_t *pFmt_, ...){
		if(IsNeedLog(euLevel_)){
			va_list ap;
			va_start(ap, pFmt_);
			m_pimp->WriteLine(euLevel_, pFmt_, ap);
			va_end(ap);
		}
	}
	void XLogFile::WriteLine(LogLevel euLevel_, const char *pFmt_, ...){
		if(IsNeedLog(euLevel_)){
			va_list ap;
			va_start(ap, pFmt_);
			m_pimp->WriteLine(euLevel_, pFmt_, ap);
			va_end(ap);
		}
	}
	void XLogFile::WriteLine(){
		if(CxNotNull(m_pimp))
			m_pimp->WriteLine();
	}

	void XLogFile::Info(const wchar_t *pFmt_, ...){
		if(!IsNeedLog(LogInfo)) return;

		va_list ap;
		va_start(ap, pFmt_);
		m_pimp->WriteLine(LogInfo, pFmt_, ap);
		va_end(ap);
	}
	void XLogFile::Info(const char *pFmt_, ...){
		if(!IsNeedLog(LogInfo)) return;

		va_list ap;
		va_start(ap, pFmt_);
		m_pimp->WriteLine(LogInfo, pFmt_, ap);
		va_end(ap);
	}

	void XLogFile::Notice(const wchar_t *pFmt_, ...){
		if(!IsNeedLog(LogNotice)) return;

		va_list ap;
		va_start(ap, pFmt_);
		m_pimp->WriteLine(LogNotice, pFmt_, ap);
		va_end(ap);
	}
	void XLogFile::Notice(const char *pFmt_, ...){
		if(!IsNeedLog(LogNotice)) return;

		va_list ap;
		va_start(ap, pFmt_);
		m_pimp->WriteLine(LogNotice, pFmt_, ap);
		va_end(ap);
	}

	void XLogFile::Warn(const wchar_t *pFmt_, ...){
		if(!IsNeedLog(LogWarn)) return;

		va_list ap;
		va_start(ap, pFmt_);
		m_pimp->WriteLine(LogWarn, pFmt_, ap);
		va_end(ap);
	}
	void XLogFile::Warn(const char *pFmt_, ...){
		if(!IsNeedLog(LogWarn)) return;

		va_list ap;
		va_start(ap, pFmt_);
		m_pimp->WriteLine(LogWarn, pFmt_, ap);
		va_end(ap);
	}

	void XLogFile::Error(const wchar_t *pFmt_, ...){
		if(!IsNeedLog(LogError)) return;

		va_list ap;
		va_start(ap, pFmt_);
		m_pimp->WriteLine(LogError, pFmt_, ap);
		va_end(ap);
	}
	void XLogFile::Error(const char *pFmt_, ...){
		if(!IsNeedLog(LogError)) return;

		va_list ap;
		va_start(ap, pFmt_);
		m_pimp->WriteLine(LogError, pFmt_, ap);
		va_end(ap);
	}

	void XLogFile::Debug(const wchar_t *pFmt_, ...){
		if(!IsNeedLog(LogDebug)) return;

		va_list ap;
		va_start(ap, pFmt_);
		m_pimp->WriteLine(LogDebug, pFmt_, ap);
		va_end(ap);
	}
	void XLogFile::Debug(const char *pFmt_, ...){
		if(!IsNeedLog(LogDebug)) return;

		va_list ap;
		va_start(ap, pFmt_);
		m_pimp->WriteLine(LogDebug, pFmt_, ap);
		va_end(ap);
	}

	void XLogFile::Called(const wchar_t *pFmt_, ...){
		if(!IsNeedLog(LogInfo)) return;

		va_list ap;
		va_start(ap, pFmt_);
		//m_pimp->WriteLine();
		m_pimp->WriteLine(LogInfo, pFmt_, ap, L"#");
		va_end(ap);
	}
	void XLogFile::Called(const char *pFmt_, ...){
		if(!IsNeedLog(LogInfo)) return;

		va_list ap;
		va_start(ap, pFmt_);
		//m_pimp->WriteLine();
		m_pimp->WriteLine(LogInfo, pFmt_, ap, "#");
		va_end(ap);
	}

	void XLogFile::Send(const wchar_t *pFmt_, ...){
		if(!IsNeedLog(LogInfo)) return;

		va_list ap;
		va_start(ap, pFmt_);
		m_pimp->WriteLine(LogInfo, pFmt_, ap, L">");
		va_end(ap);
	}
	void XLogFile::Send(const char *pFmt_, ...){
		if(!IsNeedLog(LogInfo)) return;

		va_list ap;
		va_start(ap, pFmt_);
		m_pimp->WriteLine(LogInfo, pFmt_, ap, ">");
		va_end(ap);
	}

	void XLogFile::Receive(const wchar_t *pFmt_, ...){
		if(!IsNeedLog(LogInfo)) return;

		va_list ap;
		va_start(ap, pFmt_);
		m_pimp->WriteLine(LogInfo, pFmt_, ap, L"<");
		va_end(ap);
	}
	void XLogFile::Receive(const char *pFmt_, ...){
		if(!IsNeedLog(LogInfo)) return;

		va_list ap;
		va_start(ap, pFmt_);
		m_pimp->WriteLine(LogInfo, pFmt_, ap, "<");
		va_end(ap);
	}

	void XLogFile::Version(const string &strName_, const string &strVersion_){
		if(!IsNeedLog(LogInfo)) return;

		string str = (format("----%1% [V%2%]----")%strName_ %strVersion_).str();
		m_pimp->Write(str.c_str(), LogInfo, true, true);
	}
	void XLogFile::Except(std::exception &ex_, const string &strInfo_){
		if(!IsNeedLog(LogCrit)) return;

		string strLog = (format("%1% fail: %2%")%strInfo_ %ex_.what()).str();
		m_pimp->Write(strLog.c_str(), LogCrit, true, "!");
	}


	// 格式化：可通过wostringstream来格式化，
	// outStream<<'string'<<12<<endl;
	// 然后格式化的信息写入到日志，并清空Formater
	void XLogFile::WriteFormater(wostringstream &outStr_, LogLevel euLevel_){
		if(IsNeedLog(euLevel_)){
			m_pimp->Write(outStr_.str(), euLevel_, true, true);
		}

		outStr_.str(L"");
	}
	void XLogFile::WriteFormater(ostringstream &outStr_, LogLevel euLevel_){
		if(IsNeedLog(euLevel_)){
			m_pimp->Write(outStr_.str().c_str(), euLevel_, true, true);
		}

		outStr_.str("");
	}
} //clib
} // xugd