#pragma once
#ifndef XUGD_CLIB_CONFIG_H
#define XUGD_CLIB_CONFIG_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.config.h
// @LIB: xugd.clib.lib/xugd.clib_d.lib
// @DESCRIPTION: 定义XML文件操作类与接口
// @VERSION: 1.0.15.907
//
// @RECORD:
//	编写：Xugd，2015-09-07，初始版本。
//////////////////////////////////////////////////////////////////////////

#include "xugd.common.h"
#include "xugd.time.h"
#include <string>
#include <vector>

namespace xugd{	namespace clib{
using namespace std;
//结构体写入到配置文件以及从配置文件中读取，示例：
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
//		fParent.stChild.strFile = L"File啊我饿";
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
		// 打开指定的配置文件，如果打开失败则生成空的xml树
		explicit XConFile(const wstring &strFile_);

		// 直接设定xml字符串
		void SetXml(const wstring &strXml_);
		wstring GetXml();
		// 读取时会去掉最外层的Node
		void Read(const wstring &strFile_);
		// 写入时外面增加一层Node（即当前xml作为TopNode的子节点）
		void Write(const wstring &strFile_) const;

		// 获取子节点（strPath）
		void GetChildConfig(XConFile& conChild_, const wstring &strPath_) const;
		// 把Child作为子节点（strPath）；在结构体嵌套时，按深度优先处理
		void AddChildConfig(const XConFile& conChild_, const wstring &strPath_);

		//////////////////////////////////////////////////////////////////////////
		// 路径是用.进行分割的，如Config.File
		// 没有对应的值抛出异常
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

		// 没有对应值时，返回缺省值
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

		// 设定
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