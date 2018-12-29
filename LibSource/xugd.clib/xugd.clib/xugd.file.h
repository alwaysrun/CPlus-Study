#pragma once
#ifndef XUGD_CLIB_FILE_H
#define XUGD_CLIB_FILE_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.file.h
// @LIB: xugd.clib.lib/xugd.clib_d.lib
// @DESCRIPTION: �����ļ���·�����ļ����������ӿ���
// @VERSION: 1.0.15.907
//
// @RECORD:
//	��д��Xugd��2015-09-07����ʼ�汾��
//////////////////////////////////////////////////////////////////////////
#include "xugd.common.h"
#include <string>
#include <list>

namespace xugd{	namespace clib{
using namespace std;

	//////////////////////////////////////////////////////////////////////////
	// ·����ز���
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
		// ���·����"parent"+"child" = "parent\child"
		XPath& operator+(const XPath& xOther_);
		XPath& operator+(const wstring &strPath_);
		
		wstring ToString() const;
		wstring ParentPath() const;
		wstring FileName() const;
		// include dot: ".ext"
		wstring ExtName() const;
		wstring FileWithoutExt() const;
		// ����Ǿ���·������x:\��/�����򷵻ؿ�
		wstring RootPath() const;

		// ת��Ϊ����·��������Ǿ���·��ֱ�ӷ��أ�������ݳ�������·����Ϻ󷵻�
		XPath& ToAbsolut();
		// Ext=�գ��Ƴ���չ���������滻��չ����ԭ��û������ӣ�
		XPath& ReplaceExt(const wstring &strExt_);

		bool IsEmpty() const; // û���趨��·��
		bool IsAbsolute() const; // �Ƿ��Ǿ���·��
		bool IsExists() const;
		bool IsFile() const;
		bool IsDirectory()const;

		// �ж��Ƿ�����Ч·���������ַ����Ϸ���
		static bool IsValidPath(const wstring &strPath_);
		static wstring GetRunPath(); // ��ȡ��ǰ��������Ŀ¼

	private:
		XPathImp *m_pimp;
	};

	//////////////////////////////////////////////////////////////////////////
	// �ļ���ز���
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
		// OverWrite��������ڣ��Ƿ񸲸�
		static void Copy(const wstring &strFrom_, const wstring &strTo_, bool bOverWrite_=false);

		// ��Name�ļ�����׷������Log��
		// ��������ļ�ʧ�ܣ����׳��쳣
		static void AppendLog(const wstring &strName_, const string &strLog_);
		static void AppendLog(const wstring &strName_, const wstring &strLog_);

		static void AppendText(const wstring &strPath_, const string &strText_);
		static void AppendText(const wstring &strPath_, const wstring &strText_);
		// ��ָ����ʽ���ļ�������Ϊ��д���ַ��ļ�
		static void OpenFile(wofstream &outFile_, const wstring &strPath_, ios_base::open_mode nMode_);
		static void OpenFile(wifstream &inFile_, const wstring &strPath_, ios_base::open_mode nMode_);
	};

	//////////////////////////////////////////////////////////////////////////
	// Ŀ¼��ز���
	struct XFileInfo
	{
		wstring Name;
		bool IsDir;		// �Ƿ���Ŀ¼
	};
	class XCLIBCLASS_DECLARE XDir
	{
	public:
		// �ݹ鴴���ļ��У����ʧ���׳��쳣��
		// ����ļ����Ѵ��ڣ�����false��
		static bool CreateDir(const wstring &strPath_);
		// �ݹ�ɾ���ļ���
		static void RemoveDir(const wstring &strPath_);

		static list<XFileInfo> EnumFiles(const wstring &strPath_);
	};
	
} //clib
} // xugd
#endif // XUGD_CLIB_FILE_H