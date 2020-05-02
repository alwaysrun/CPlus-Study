#pragma once
#ifndef XUGD_CLIB_LOG_H
#define XUGD_CLIB_LOG_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.log.h
// @LIB: xugd.clib.lib/xugd.clib_d.lib
// @DESCRIPTION: 定义日志记录接口类
// @VERSION: 1.0.17.208
//
// @RECORD:
//	编写：Xugd，2015-09-07，初始版本。
//	完善：Xugd，2017-02-08，增加日志等级相关判断接口。
//////////////////////////////////////////////////////////////////////////
#include "xugd.common.h"
#include "xugd.config.h"
#include <string>
#include <sstream>

namespace xugd{	namespace clib{
	using namespace std;
	//////////////////////////////////////////////////////////////////////////
	// 配置文件（中文一定使用宽字符，否则输出为乱码）
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
		// 日志记录等级：越大记录日志越多，0不记录任何日志
		int LogLevel;
		// 文件名格式：
		/// 可使用{xxxx}来表示年、月、日等时间,x的具体值参考XDateTime.ToString的说明；
		/// 如"Log\{yyyy}\{MM}\{dd}\MyLog {HH}.log"就是按年月日创建目录，并每小时创建一个新的文件
		wstring GetNameFormat() const;
		void SetNameFormat(const wstring &strNameFmt_);
	private:
		wchar_t *m_pFormat;
	};

	//////////////////////////////////////////////////////////////////////////
	class XLogFileImp;
	// 写日志文件：除write外的所有写入接口都会自动添加换行；
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
		// 初始化，并自动打开日志
		explicit XLogFile(const XLogConfig &logConfig_); 
		// WithTime：每条日志前面都有输出时间
		explicit XLogFile(const wstring &strNameFormat_, bool bWithTime_=true);
		~XLogFile();

		// 打开日志文件，如果已打开，则先关闭
		// 如果是空文件，则不记录日志
		void Open(const wstring &strNameFormat_, bool bWithTime_=true);
		void Close();
		// 通过回调函数获取写日志的内容
		void SetGetLogCallback(PFunGetLogCallback pfun, bool bDenyLogFile_=false);

		// 设定记录级别：设定后影响新的记录信息；
		// 范围无效返回false，否则true。
		bool SetLogLevel(LogLevel euLevel_);
		LogLevel GetLogLevel() {return m_euLogLevel;}
		bool IsLogDebug() {return m_euLogLevel>=LogDebug;}
		bool IsLogLevel(LogLevel euLevel_) { return euLevel_<=m_euLogLevel;}

		// 原样输出到文件中
		void Write(const wstring &strLog_, LogLevel euLevel_, bool bAddLine_=true, bool bWithTime_=true);
		void Write(const string &strLog_, LogLevel euLevel_, bool bAddLine_=true, bool bWithTime_=true);
		// 会自动在尾部添加换行符
		void WriteLine(LogLevel euLevel_, const wchar_t *pFmt_, ...);
		void WriteLine(LogLevel euLevel_, const char *pFmt_, ...);
		//只输出一个换行符
		void WriteLine();

		// 后面所有接口，都会在尾部添加换行符
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

		// 记录为LogInfo，且增加前缀'#'
		void Called(const wchar_t *pFmt_, ...);
		void Called(const char *pFmt_, ...);
		// 记录为LogInfo，且增加前缀'>'
		void Send(const wchar_t *pFmt_, ...);
		void Send(const char *pFmt_, ...);
		// 记录为LogInfo，且增加前缀'<'
		void Receive(const wchar_t *pFmt_, ...);
		void Receive(const char *pFmt_, ...);
		// 记录为LogInfo：----Name [Version]----
		void Version(const string &strName_, const string &strVersion_);
		// 记录为LogCrit
		void Except(std::exception &ex_, const string &strInfo_);

		// 格式化写入(WriteLine)：可通过wostringstream来格式化，
		// outStream<<L'string'<<12<<endl;
		// 然后格式化的信息写入到日志，并清空Formater
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