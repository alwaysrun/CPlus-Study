#pragma once
#ifndef XUGD_CLIB_CONFIG_H
#define XUGD_CLIB_CONFIG_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.config.h
// @LIB: xugd.clib.lib/xugd.clib_d.lib
// @DESCRIPTION: ����XML�ļ���������ӿ�
// @VERSION: 1.0.15.907
//
// @RECORD:
//	��д��Xugd��2015-09-07����ʼ�汾��
//////////////////////////////////////////////////////////////////////////

#include "xugd.common.h"
#include "xugd.time.h"
#include <string>
#include <vector>

namespace xugd{	namespace clib{
using namespace std;
//�ṹ��д�뵽�����ļ��Լ��������ļ��ж�ȡ��ʾ����
//struct FileConfig
//{
//	int nFlag;
//	wstring strFile;
//	XDateTime dtSet;
//
//	void ToConfig(XConFile &conFile_){
//		conFile_.SetAttr(L"", L"Flag", nFlag);
//		conFile_.SetText(L"", L"File", strFile);
//		conFile_.SetAttr(L"", L"SetTime", dtSet);
//	}
//	void FromConfig(const XConFile &conFile_){
//		nFlag=conFile_.GetAttr(L"", L"Flag", 0);
//		strFile=conFile_.GetText(L"", L"File", L"");
//		dtSet=conFile_.GetAttrTime(L"", L"SetTime");
//	}
//};
//struct FileParent
//{
//	wstring strNone;
//	FileConfig stChild;
//
//	void ToConfig(XConFile &conFile_){
//		conFile_.SetText(L"", L"None", strNone);
//
//		XConFile conChild;
//		stChild.ToConfig(conChild);
//		conFile_.AddChildConfig(conChild, L"Child");
//	}
//	void FromConfig(const XConFile &conFile_){
//		strNone = conFile_.GetText(L"", L"None", L"");
//
//		XConFile conChild;
//		conFile_.GetChildConfig(conChild, L"Child");
//		stChild.FromConfig(conChild);
//	}
//};
//void WriteConfig(const wstring &strFile_, FileParent &fCon);
//void TestConfig(const wstring &strFile_){
//	FileParent fParent;
//	try{
//		XConFile conParent;
//		conParent.Read(strFile_);
//		fParent.FromConfig(conParent);
//	}
//	catch(std::exception &ex){
//		fParent.strNone = L"Nothing";
//		fParent.stChild.nFlag = 1234;
//		fParent.stChild.strFile = L"File���Ҷ�";
//		fParent.stChild.dtSet.Now();
//
//		WriteConfig(strFile_, fParent);
//	}
//}
//void WriteConfig(const wstring &strFile_, FileParent &fCon){
//	try {
//		XConFile conFile;
//		fCon.ToConfig(conFile);
//		conFile.Write(strFile_);
//	}
//	catch(std::exception &ex){}
//}
	class XConFileImp;
	class XCLIBCLASS_DECLARE XConFile
	{
	public:
		explicit XConFile(const wchar_t *pTopNode_=L"Config");
		~XConFile();
		// ��ָ���������ļ��������ʧ�������ɿյ�xml��
		explicit XConFile(const wstring &strFile_);

		// ֱ���趨xml�ַ���
		void SetXml(const wstring &strXml_);
		wstring GetXml();
		// ��ȡʱ��ȥ��������Node
		void Read(const wstring &strFile_);
		// д��ʱ��������һ��Node������ǰxml��ΪTopNode���ӽڵ㣩
		void Write(const wstring &strFile_) const;

		// ��ȡ�ӽڵ㣨strPath��
		void GetChildConfig(XConFile& conChild_, const wstring &strPath_) const;
		// ��Child��Ϊ�ӽڵ㣨strPath�����ڽṹ��Ƕ��ʱ����������ȴ���
		void AddChildConfig(const XConFile& conChild_, const wstring &strPath_);

		//////////////////////////////////////////////////////////////////////////
		// ·������.���зָ�ģ���Config.File
		// û�ж�Ӧ��ֵ�׳��쳣
		int GetInt(const wstring &strPath_) const;
		bool GetBool(const wstring &strPath_) const;
		double GetDouble(const wstring &strPath_) const;
		wstring GetWStr(const wstring &strPath_) const;
		XDateTime GetTime(const wstring &strPath_) const;

		int GetAttrInt(const wstring &strPath_, const wstring &strName_) const;
		bool GetAttrBool(const wstring &strPath_, const wstring &strName_) const;
		double GetAttrDouble(const wstring &strPath_, const wstring &strName_) const;
		wstring GetAttrWStr(const wstring &strPath_, const wstring &strName_) const;
		XDateTime GetAttrTime(const wstring &strPath_, const wstring &strName_) const;

		// û�ж�Ӧֵʱ������ȱʡֵ
		int GetText(const wstring &strPath_, int nDefault_) const;
		bool GetText(const wstring &strPath_, bool bDefault_) const;
		double GetText(const wstring &strPath_, double fDefault_) const;
		wstring GetText(const wstring &strPath_, const wstring &strDefault_) const;
		wstring GetText(const wstring &strPath_, const wchar_t *strDefault_) const;
		XDateTime GetText(const wstring &strPath_, const XDateTime &tDefault_) const;

		// <row><row_id value="4"/></row>
		// Path: row.rowid; Name value; Return 4
		int GetAttr(const wstring &strPath_, const wstring &strName_, int nDefault_) const;
		bool GetAttr(const wstring &strPath_, const wstring &strName_, bool bDefault_) const;
		double GetAttr(const wstring &strPath_, const wstring &strName_, double fDefault_) const;
		wstring GetAttr(const wstring &strPath_, const wstring &strName_, const wstring &strDefault_) const;
		wstring GetAttr(const wstring &strPath_, const wstring &strName_, const wchar_t *strDefault_) const;
		XDateTime GetAttr(const wstring &strPath_, const wstring &strName_, const XDateTime &tDefault_) const;

		// �趨
		void SetText(const wstring &strPath_, int nValue_);
		void SetText(const wstring &strPath_, bool bValue_);
		void SetText(const wstring &strPath_, double fValue_);
		void SetText(const wstring &strPath_, const wstring &strValue_);
		void SetText(const wstring &strPath_, const wchar_t *pValue_);
		void SetText(const wstring &strPath_, const XDateTime &tValue_);

		void SetAttr(const wstring &strPath_, const wstring &strName_, int nValue_);
		void SetAttr(const wstring &strPath_, const wstring &strName_, bool bValue_);
		void SetAttr(const wstring &strPath_, const wstring &strName_, double fValue_);
		void SetAttr(const wstring &strPath_, const wstring &strName_, const wstring &strValue_);
		void SetAttr(const wstring &strPath_, const wstring &strName_, const wchar_t *pValue_);
		void SetAttr(const wstring &strPath_, const wstring &strName_, const XDateTime &tValue_);

		wstring GetNotes(const wstring &strPath_) const;
		void SetNotes(const wstring &strPath_, const wstring &strNotes_);

	private:
		CxForbidCopyFun(XConFile);

		XConFileImp *GetImp() const;
		XConFileImp *m_pimp;
	};

	class XSimpleXmlImp;
	class XCLIBCLASS_DECLARE XSimpleXmlA{
	public:
		explicit XSimpleXmlA(const string &strXml_);
		~XSimpleXmlA();

		void SetXml(const string &strXml_);
		string GetXml();

		int GetText(const string &strPath_, int nDefault_=0);
		string GetText(const string &strPath_, const string &strDefault_);
		vector<string> GetMultiText(const string &strPath_);

		// <row><row_id value="4"/></row>
		// Path: row.rowid; Name value; Return 4
		int GetAttr(const string &strPath_, const string &strName_, int nDefault_=0);
		string GetAttr(const string &strPath_, const string &strName_, const string &strDefault_);
		vector<string> GetMultiAttr(const string &strPath_, const string &strName_);

	private:
		CxForbidCopyFun(XSimpleXmlA);

		XSimpleXmlImp *m_pimp;
	};
} //clib
} // xugd

#endif // XUGD_CLIB_CONFIG_H