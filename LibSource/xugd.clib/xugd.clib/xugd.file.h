#pragma once
#ifndef XUGD_CLIB_FILE_H
#define XUGD_CLIB_FILE_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.file.h
// @LIB: xugd.clib.lib/xugd.clib_d.lib
// @DESCRIPTION: 定义文件（路径与文件名）操作接口类
// @VERSION: 1.0.15.907
//
// @RECORD:
//	编写：Xugd，2015-09-07，初始版本。
//////////////////////////////////////////////////////////////////////////
#include "xugd.common.h"
#include <string>
#include <list>

namespace xugd{	namespace clib{
using namespace std;

	//////////////////////////////////////////////////////////////////////////
	// 路径相关操作
	class XPathImp;
	class XCLIBCLASS_DECLARE XPath
	{
	public:
		XPath();
		XPath(const wstring &strPath_);
		XPath(const XPath& xOther_);
		~XPath();

		XPath& operator=(const XPath& xOther_);
		XPath& operator=(const wstring &strPath_);
		// 组合路径："parent"+"child" = "parent\child"
		XPath& operator+(const XPath& xOther_);
		XPath& operator+(const wstring &strPath_);
		
		wstring ToString() const;
		wstring ParentPath() const;
		wstring FileName() const;
		// include dot: ".ext"
		wstring ExtName() const;
		wstring FileWithoutExt() const;
		// 如果是绝对路径返回x:\或/，否则返回空
		wstring RootPath() const;

		// 转换为绝对路径：如果是绝对路径直接返回；否则根据程序运行路径组合后返回
		XPath& ToAbsolut();
		// Ext=空：移除扩展名；否则替换扩展名（原来没有则添加）
		XPath& ReplaceExt(const wstring &strExt_);

		bool IsEmpty() const; // 没有设定过路径
		bool IsAbsolute() const; // 是否是绝对路径
		bool IsExists() const;
		bool IsFile() const;
		bool IsDirectory()const;

		// 判断是否是有效路径（所有字符都合法）
		static bool IsValidPath(const wstring &strPath_);
		static wstring GetRunPath(); // 获取当前程序运行目录

	private:
		XPathImp *m_pimp;
	};

	//////////////////////////////////////////////////////////////////////////
	// 文件相关操作
	class XCLIBCLASS_DECLARE XFile
	{
	public:
		static bool IsExists(const wstring &strPath_);
		static bool IsFile(const wstring &strPath_);
		static bool IsDirectory(const wstring &strPath_);

		static unsigned long GetSize(const wstring &strPath_);
		static time_t GetLastWrite(const wstring &strPath_);
		static void SetLastWrite(const wstring &strPath_, time_t tNewTime_);

		static void RemoveFile(const wstring &strPath_);
		static void Rename(const wstring &strOld_, const wstring &strNew_);
		// OverWrite：如果存在，是否覆盖
		static void Copy(const wstring &strFrom_, const wstring &strTo_, bool bOverWrite_=false);

		// 打开Name文件，并追加内容Log；
		// 如果操作文件失败，不抛出异常
		static void AppendLog(const wstring &strName_, const string &strLog_);
		static void AppendLog(const wstring &strName_, const wstring &strLog_);

		static void AppendText(const wstring &strPath_, const string &strText_);
		static void AppendText(const wstring &strPath_, const wstring &strText_);
		// 以指定方式打开文件，并设为可写宽字符文件
		static void OpenFile(wofstream &outFile_, const wstring &strPath_, ios_base::open_mode nMode_);
		static void OpenFile(wifstream &inFile_, const wstring &strPath_, ios_base::open_mode nMode_);
	};

	//////////////////////////////////////////////////////////////////////////
	// 目录相关操作
	struct XFileInfo
	{
		wstring Name;
		bool IsDir;		// 是否是目录
	};
	class XCLIBCLASS_DECLARE XDir
	{
	public:
		// 递归创建文件夹，如果失败抛出异常；
		// 如果文件夹已存在，返回false；
		static bool CreateDir(const wstring &strPath_);
		// 递归删除文件夹
		static void RemoveDir(const wstring &strPath_);

		static list<XFileInfo> EnumFiles(const wstring &strPath_);
	};
	
} //clib
} // xugd
#endif // XUGD_CLIB_FILE_H