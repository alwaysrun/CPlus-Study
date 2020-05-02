#pragma once
#ifndef XUGD_CLIB_LOG_H
#define XUGD_CLIB_LOG_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.log.h
// @LIB: xugd.clib.lib/xugd.clib_d.lib
// @DESCRIPTION: ������־��¼�ӿ���
// @VERSION: 1.0.17.208
//
// @RECORD:
//	��д��Xugd��2015-09-07����ʼ�汾��
//	���ƣ�Xugd��2017-02-08��������־�ȼ�����жϽӿڡ�
//////////////////////////////////////////////////////////////////////////
#include "xugd.common.h"
#include "xugd.config.h"
#include <string>
#include <sstream>

namespace xugd{	namespace clib{
	using namespace std;
	//////////////////////////////////////////////////////////////////////////
	// �����ļ�������һ��ʹ�ÿ��ַ����������Ϊ���룩
	class XCLIBCLASS_DECLARE XLogConfig
	{
	public:
		XLogConfig();
		XLogConfig(const XLogConfig &other_);
		~XLogConfig();
		XLogConfig& operator=(const XLogConfig &other_);
		void ToConfig(XConFile &conFile_);
		void FromConfig(const XConFile &conFile_);
		bool Read(const wstring &strFile_);
		void Write(const wstring &strFile_);
		// build: log\{yyyy}\{MM}\{dd}\Name{HH}.log
		void InitHourLog(const wstring &strName_);
		// build: log\{yyyy}\{MM}\Name{dd}.log
		void InitDayLog(const wstring &strName_);

		bool WithTime;
		// ��־��¼�ȼ���Խ���¼��־Խ�࣬0����¼�κ���־
		int LogLevel;
		// �ļ�����ʽ��
		/// ��ʹ��{xxxx}����ʾ�ꡢ�¡��յ�ʱ��,x�ľ���ֵ�ο�XDateTime.ToString��˵����
		/// ��"Log\{yyyy}\{MM}\{dd}\MyLog {HH}.log"���ǰ������մ���Ŀ¼����ÿСʱ����һ���µ��ļ�
		wstring GetNameFormat() const;
		void SetNameFormat(const wstring &strNameFmt_);
	private:
		wchar_t *m_pFormat;
	};

	//////////////////////////////////////////////////////////////////////////
	class XLogFileImp;
	// д��־�ļ�����write�������д��ӿڶ����Զ���ӻ��У�
	class XCLIBCLASS_DECLARE XLogFile
	{
	public:
		enum LogLevel
		{
			LogNone = 0,
			LogCrit,
			LogError,
			LogWarn,
			LogNotice,
			// Infomation
			LogInfo,
			LogDefault = LogInfo,

			LogDebug,
			LogAll,
		};

		typedef void (*PFunGetLogCallback)(const wstring &strInfo_);
	public:
		XLogFile();
		// ��ʼ�������Զ�����־
		explicit XLogFile(const XLogConfig &logConfig_); 
		// WithTime��ÿ����־ǰ�涼�����ʱ��
		explicit XLogFile(const wstring &strNameFormat_, bool bWithTime_=true);
		~XLogFile();

		// ����־�ļ�������Ѵ򿪣����ȹر�
		// ����ǿ��ļ����򲻼�¼��־
		void Open(const wstring &strNameFormat_, bool bWithTime_=true);
		void Close();
		// ͨ���ص�������ȡд��־������
		void SetGetLogCallback(PFunGetLogCallback pfun, bool bDenyLogFile_=false);

		// �趨��¼�����趨��Ӱ���µļ�¼��Ϣ��
		// ��Χ��Ч����false������true��
		bool SetLogLevel(LogLevel euLevel_);
		LogLevel GetLogLevel() {return m_euLogLevel;}
		bool IsLogDebug() {return m_euLogLevel>=LogDebug;}
		bool IsLogLevel(LogLevel euLevel_) { return euLevel_<=m_euLogLevel;}

		// ԭ��������ļ���
		void Write(const wstring &strLog_, LogLevel euLevel_, bool bAddLine_=true, bool bWithTime_=true);
		void Write(const string &strLog_, LogLevel euLevel_, bool bAddLine_=true, bool bWithTime_=true);
		// ���Զ���β����ӻ��з�
		void WriteLine(LogLevel euLevel_, const wchar_t *pFmt_, ...);
		void WriteLine(LogLevel euLevel_, const char *pFmt_, ...);
		//ֻ���һ�����з�
		void WriteLine();

		// �������нӿڣ�������β����ӻ��з�
		void Info(const wchar_t *pFmt_, ...);
		void Info(const char *pFmt_, ...);
		void Notice(const wchar_t *pFmt_, ...);
		void Notice(const char *pFmt_, ...);
		void Warn(const wchar_t *pFmt_, ...);
		void Warn(const char *pFmt_, ...);
		void Error(const wchar_t *pFmt_, ...);
		void Error(const char *pFmt_, ...);
		void Debug(const wchar_t *pFmt_, ...);
		void Debug(const char *pFmt_, ...);

		// ��¼ΪLogInfo��������ǰ׺'#'
		void Called(const wchar_t *pFmt_, ...);
		void Called(const char *pFmt_, ...);
		// ��¼ΪLogInfo��������ǰ׺'>'
		void Send(const wchar_t *pFmt_, ...);
		void Send(const char *pFmt_, ...);
		// ��¼ΪLogInfo��������ǰ׺'<'
		void Receive(const wchar_t *pFmt_, ...);
		void Receive(const char *pFmt_, ...);
		// ��¼ΪLogInfo��----Name [Version]----
		void Version(const string &strName_, const string &strVersion_);
		// ��¼ΪLogCrit
		void Except(std::exception &ex_, const string &strInfo_);

		// ��ʽ��д��(WriteLine)����ͨ��wostringstream����ʽ����
		// outStream<<L'string'<<12<<endl;
		// Ȼ���ʽ������Ϣд�뵽��־�������Formater
		void WriteFormater(wostringstream &outStr_, LogLevel euLevel_);
		void WriteFormater(ostringstream &outStr_, LogLevel euLevel_);

	private:
		CxForbidCopyFun(XLogFile);
		bool IsNeedLog(LogLevel euLevel_){
			return (m_bLogToFile || CxNotNull(m_pGetInfo)) && (euLevel_<=m_euLogLevel);
		}
		bool m_bLogToFile;
		LogLevel m_euLogLevel;
		XLogFileImp *m_pimp;
		PFunGetLogCallback m_pGetInfo;
	};
} //clib
} // xugd
#endif // XUGD_CLIB_LOG_H