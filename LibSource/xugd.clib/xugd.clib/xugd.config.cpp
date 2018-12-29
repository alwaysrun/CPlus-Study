#include "xugd.config.h"
#include <codecvt>
#include <boost/algorithm/string.hpp>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/xml_parser.hpp>
using namespace boost;
using namespace boost::property_tree;

namespace xugd{	namespace clib{
	const wchar_t	XmlFieldStart=L'<';
	const wchar_t	XmlFieldEnd=L'>';
	const wchar_t	XmlSeperator=L'/';
	const wstring	XmlIgnoreSeps=L"!?";

	class XConFileImp
	{
	public:
		void Read(const wstring& strFile_)
		{
			wifstream inFile(strFile_);
			locale utf8Locale(locale(), new codecvt_utf8<wchar_t>);
			inFile.imbue(utf8Locale);

			wptree ptRead;
			read_xml(inFile, ptRead);
			m_topNode = (ptRead.begin())->first;
			m_xmlTree=ptRead.get_child(m_topNode);
		}

		void Write(const wstring& strFile_)
		{
			wptree ptWrite;
			ptWrite.add_child(m_topNode, m_xmlTree);
			wostringstream outStr;
			write_xml(outStr, ptWrite);

			//////////////////////////////////////////////////////////////////////////
			// write file
			wofstream outFile(strFile_);
			locale utf8Locale(locale(), new codecvt_utf8<wchar_t>);
			outFile.imbue(utf8Locale);

			// Format the xml
			int nIndents = 0;
			bool bInBrack=false;
			wstring strWrite(outStr.str());
			for(auto pos=strWrite.begin(); pos!=strWrite.end(); ++pos)
			{
				// check: '<' '</' '<!' '<?' 
				if(IsXmlStart(*pos))
				{
					if( (pos+1)!=strWrite.end())
					{
						auto chTest=*(pos+1);
						if(IsXmlSeperator(chTest))
							--nIndents;
						else if(!IsXmlIgnoreSep(chTest))
							bInBrack=true;
					}

					// check: "><"
					if(pos != strWrite.begin())
					{
						if(IsXmlEnd(*(pos-1)))
						{
							outFile<<L'\n';
							for(int n=0 ; n<nIndents ; ++n)
								outFile<<L'\t';
						}
					}
				}
				else if(IsXmlEnd(*pos))
				{
					if(bInBrack)
					{
						if(pos != strWrite.begin())
						{
							// check: '/>'
							if(!IsXmlSeperator(*(pos-1)))
								++nIndents;
						}

						bInBrack = false;
					}
				}

				outFile<<*pos;
			} // for

			outFile.close();
		}

		void SetXml(const wstring &strXml_){
			wstringstream inStr;
			inStr<<strXml_;

			read_xml(inStr, m_xmlTree);
			m_topNode = (m_xmlTree.begin())->first;
		}

		wstring GetXml(){
			wostringstream outStr;
			write_xml(outStr, m_xmlTree);

			return outStr.str();
		}

		template<typename T>
		T GetText(const wstring &strPath_)
		{
			return m_xmlTree.get<T>(strPath_);
		}

		template<typename T>
		T GetAttr(const wstring &strPath_, const wstring &strName_)
		{
			wstring strAttrPath = BuildAttrPath(strPath_, strName_);
			return m_xmlTree.get<T>(strAttrPath);
		}

		template<typename T>
		T GetText(const wstring &strPath_, const T& tDefault_)
		{
			return m_xmlTree.get<T>(strPath_, tDefault_);
		}
		template<typename T>
		T GetAttr(const wstring &strPath_, const wstring &strName_, const T& tDefault_)
		{
			wstring strAttrPath = BuildAttrPath(strPath_, strName_);
			return m_xmlTree.get<T>(strAttrPath, tDefault_);
		}

		template<typename T>
		void SetText(const wstring &strPath_, const T& tValue_)
		{
			m_xmlTree.put(strPath_, tValue_);
		}
		template<typename T>
		void SetAttr(const wstring &strPath_, const wstring &strName_, const T& tValue_)
		{
			wstring strAttrPath = BuildAttrPath(strPath_, strName_);
			m_xmlTree.put(strAttrPath, tValue_);
		}

		wstring GetNotes(const wstring &strPath_)
		{
			wstring strNotePath = BuildNotePath(strPath_);

			return m_xmlTree.get<wstring>(strNotePath, L"");
		}
		void SetNotes(const wstring &strPath_, const wstring& strNotes_)
		{
			wstring strNotePath = BuildNotePath(strPath_);
			m_xmlTree.put<wstring>(strNotePath, strNotes_);
		}

	public:
		inline bool IsXmlStart(wchar_t ch_)
		{
			return XmlFieldStart==ch_;
		}
		inline bool IsXmlEnd(wchar_t ch_)
		{
			return XmlFieldEnd==ch_;
		}
		inline bool IsXmlSeperator(wchar_t ch_)
		{
			return XmlSeperator==ch_;
		}
		inline bool IsXmlIgnoreSep(wchar_t ch_)
		{
			return XmlIgnoreSeps.find(ch_)!=wstring::npos;
		}

		void SetTopNode(const wstring &strTop_)
		{
			m_topNode.assign(strTop_);
		}
		wptree GetChild(const wstring &strPath_)
		{
			return m_xmlTree.get_child(strPath_);
		}
		void AddChild(const wstring &strPath_, const wptree& ptXml_)
		{
			m_xmlTree.add_child(strPath_, ptXml_);
		}
		void SetTree(const wptree& ptXml_, const wstring &strPath_)
		{
			m_xmlTree = ptXml_;
			m_topNode.assign(strPath_);
		}
		wptree& GetTree()
		{
			return m_xmlTree;
		}

	private:
		wstring BuildAttrPath(const wstring &strPath_, const wstring &strName_)
		{
			if(strPath_.empty())
				return L"<xmlattr>." + strName_;

			return strPath_ + L".<xmlattr>." + strName_;
		}
		wstring BuildNotePath(wstring strPath_)
		{
			return strPath_ +L".<xmlcomment>";
		}

		wptree m_xmlTree;
		wstring	m_topNode;
	};

	//////////////////////////////////////////////////////////////////////////
	XConFile::XConFile(const wchar_t *pTopNode_)
	{
		if(CxWstrNullOrEmpty(pTopNode_))
			pTopNode_=L"Config";

		m_pimp = new XConFileImp();
		m_pimp->SetTopNode(pTopNode_);
	}
	XConFile::XConFile(const wstring &strFile_)
	{
		m_pimp = new XConFileImp();
		try 
		{
			m_pimp->Read(strFile_);
		}
		catch(std::exception&){}
	}
	XConFile::~XConFile()
	{
		if(CxNotNull(m_pimp))
			delete m_pimp;
	}

	void XConFile::SetXml(const wstring &strXml_){
		m_pimp->SetXml(strXml_);
	}

	wstring XConFile::GetXml(){
		return m_pimp->GetXml();
	}

	void XConFile::Read(const wstring &strFile_)
	{
		m_pimp->Read(strFile_);
	}

	void XConFile::Write(const wstring &strFile_) const
	{
		m_pimp->Write(strFile_);
	}

	XConFileImp *XConFile::GetImp() const
	{
		return this->m_pimp;
	}
	
	// 获取子节点（strPath）
	void XConFile::GetChildConfig(XConFile& conChild_, const wstring &strPath_) const
	{
		conChild_.GetImp()->SetTree(m_pimp->GetChild(strPath_), strPath_);
	}
	// 把Child作为子节点（strPath）
	void XConFile::AddChildConfig(const XConFile& conChild_, const wstring &strPath_)
	{
		m_pimp->AddChild(strPath_, conChild_.GetImp()->GetTree());
	}

	int XConFile::GetInt(const wstring &strPath_) const
	{
		return m_pimp->GetText<int>(strPath_);
	}
	bool XConFile::GetBool(const wstring &strPath_) const
	{
		return m_pimp->GetText<bool>(strPath_);
	}
	double XConFile::GetDouble(const wstring &strPath_) const
	{
		return m_pimp->GetText<double>(strPath_);
	}
	wstring XConFile::GetWStr(const wstring &strPath_) const
	{
		return m_pimp->GetText<wstring>(strPath_);
	}
	XDateTime XConFile::GetTime(const wstring &strPath_) const
	{
		return m_pimp->GetText<XDateTime>(strPath_);
	}

	int XConFile::GetAttrInt(const wstring &strPath_, const wstring &strName_) const
	{
		return m_pimp->GetAttr<int>(strPath_, strName_);
	}
	bool XConFile::GetAttrBool(const wstring &strPath_, const wstring &strName_) const
	{
		return m_pimp->GetAttr<bool>(strPath_, strName_);
	}
	double XConFile::GetAttrDouble(const wstring &strPath_, const wstring &strName_) const
	{
		return m_pimp->GetAttr<double>(strPath_, strName_);
	}
	wstring XConFile::GetAttrWStr(const wstring &strPath_, const wstring &strName_) const
	{
		return m_pimp->GetAttr<wstring>(strPath_, strName_);
	}
	XDateTime XConFile::GetAttrTime(const wstring &strPath_, const wstring &strName_) const
	{
		return m_pimp->GetAttr<XDateTime>(strPath_, strName_);
	}

	// 没有对应值时，返回缺省值
	int XConFile::GetText(const wstring &strPath_, int nDefault_) const
	{
		return m_pimp->GetText(strPath_, nDefault_);
	}
	bool XConFile::GetText(const wstring &strPath_, bool bDefault_) const
	{
		return m_pimp->GetText(strPath_, bDefault_);
	}
	double XConFile::GetText(const wstring &strPath_, double fDefault_) const
	{
		return m_pimp->GetText(strPath_, fDefault_);
	}
	wstring XConFile::GetText(const wstring &strPath_, const wstring &strDefault_) const
	{
		return m_pimp->GetText<wstring>(strPath_, strDefault_);
	}
	wstring XConFile::GetText(const wstring &strPath_, const wchar_t *strDefault_) const
	{
		wstring strTmp;
		if(CxNotNull(strDefault_))
			strTmp = strDefault_;
		return m_pimp->GetText<wstring>(strPath_, strTmp);
	}
	XDateTime XConFile::GetText(const wstring &strPath_, const XDateTime &tDefault_) const
	{
		return m_pimp->GetText(strPath_, tDefault_);
	}

	int XConFile::GetAttr(const wstring &strPath_, const wstring &strName_, int nDefault_) const
	{
		return m_pimp->GetAttr(strPath_, strName_, nDefault_);
	}
	bool XConFile::GetAttr(const wstring &strPath_, const wstring &strName_, bool bDefault_) const
	{
		return m_pimp->GetAttr(strPath_, strName_, bDefault_);
	}
	double XConFile::GetAttr(const wstring &strPath_, const wstring &strName_, double fDefault_) const
	{
		return m_pimp->GetAttr(strPath_, strName_, fDefault_);
	}
	wstring XConFile::GetAttr(const wstring &strPath_, const wstring &strName_, const wstring &strDefault_) const
	{
		return m_pimp->GetAttr<wstring>(strPath_, strName_, strDefault_);
	}
	wstring XConFile::GetAttr(const wstring &strPath_, const wstring &strName_, const wchar_t *strDefault_) const
	{
		wstring strTmp;
		if(CxNotNull(strDefault_))
			strTmp = strDefault_;
		return m_pimp->GetAttr<wstring>(strPath_, strName_, strTmp);
	}
	XDateTime XConFile::GetAttr(const wstring &strPath_, const wstring &strName_, const XDateTime &tDefault_) const
	{
		return m_pimp->GetAttr(strPath_, strName_, tDefault_);
	}

	// 设定
	void XConFile::SetText(const wstring &strPath_, int nValue_)
	{
		m_pimp->SetText(strPath_, nValue_);
	}
	void XConFile::SetText(const wstring &strPath_, bool bValue_)
	{
		m_pimp->SetText(strPath_, bValue_);
	}
	void XConFile::SetText(const wstring &strPath_, double fValue_)
	{
		m_pimp->SetText(strPath_, fValue_);
	}
	void XConFile::SetText(const wstring &strPath_, const wstring &strValue_)
	{
		m_pimp->SetText<wstring>(strPath_, strValue_);
	}
	void XConFile::SetText(const wstring &strPath_, const wchar_t *pValue_)
	{
		wstring strTmp;
		if(CxNotNull(pValue_))
			strTmp = pValue_;
		m_pimp->SetText<wstring>(strPath_, strTmp);
	}
	void XConFile::SetText(const wstring &strPath_, const XDateTime &tValue_)
	{
		m_pimp->SetText(strPath_, tValue_);
	}

	void XConFile::SetAttr(const wstring &strPath_, const wstring &strName_, int nValue_)
	{
		m_pimp->SetAttr(strPath_, strName_, nValue_);
	}
	void XConFile::SetAttr(const wstring &strPath_, const wstring &strName_, bool bValue_)
	{
		m_pimp->SetAttr(strPath_, strName_, bValue_);
	}
	void XConFile::SetAttr(const wstring &strPath_, const wstring &strName_, double fValue_)
	{
		m_pimp->SetAttr(strPath_, strName_, fValue_);
	}
	void XConFile::SetAttr(const wstring &strPath_, const wstring &strName_, const wstring &strValue_)
	{
		m_pimp->SetAttr<wstring>(strPath_, strName_, strValue_);
	}
	void XConFile::SetAttr(const wstring &strPath_, const wstring &strName_, const wchar_t *pValue_)
	{
		wstring strTmp;
		if(CxNotNull(pValue_))
			strTmp = pValue_;
		m_pimp->SetAttr<wstring>(strPath_, strName_, strTmp);
	}
	void XConFile::SetAttr(const wstring &strPath_, const wstring &strName_, const XDateTime &tValue_)
	{
		m_pimp->SetAttr(strPath_, strName_, tValue_);
	}

	wstring XConFile::GetNotes(const wstring &strPath_) const
	{
		return m_pimp->GetNotes(strPath_);
	}
	void XConFile::SetNotes(const wstring &strPath_, const wstring &strNotes_)
	{
		m_pimp->SetNotes(strPath_, strNotes_);
	}

	//////////////////////////////////////////////////////////////////////////
	class XSimpleXmlImp{
	public:
		void SetXml(const string &strXml_){
			stringstream inStr;
			inStr<<strXml_;

			read_xml(inStr, m_xmlTree);
		}

		string GetXml(){
			ostringstream outStr;
			write_xml(outStr, m_xmlTree);

			return outStr.str();
		}

		template<typename T>
		T GetText(const string &strPath_, const T& tDefault_)
		{
			return m_xmlTree.get<T>(strPath_, tDefault_);
		}

		void GetMulti(const string &strPath_, vector<string> &vecText_, const string &strName_){
			string strLeafName = "";
			if(!strName_.empty())
				strLeafName = BuildAttrPath("", strName_);
			GetMulti(m_xmlTree, strPath_, vecText_, strLeafName);
		}
		void GetMulti(const ptree &ptXml_, const string &strPath_, vector<string> &vecText_, const string &strName_){
			if(strPath_.empty()) return;

			string::size_type nSep = strPath_.find('.');
			if(string::npos == nSep){	// its the leaf
				auto rngLeaf = ptXml_.equal_range(strPath_);
				for(auto rngIt=rngLeaf.first ; rngIt!=rngLeaf.second ; ++rngIt){
					vecText_.push_back((rngIt->second).get<string>(strName_));
				}
				return;
			}

			// Split the path
			string strNode = strPath_.substr(0, nSep);
			string strNewPath = strPath_.substr(nSep+1);
			auto rngNodes = ptXml_.equal_range(strNode);
			for(auto rngIt=rngNodes.first ; rngIt!=rngNodes.second ; ++rngIt){
				GetMulti(rngIt->second, strNewPath, vecText_, strName_);
			}
		}

		template<typename T>
		T GetAttr(const string &strPath_, const string &strAttr_, const T& tDefault_)
		{
			string strAttrPath = BuildAttrPath(strPath_, strAttr_);
			return m_xmlTree.get<T>(strAttrPath, tDefault_);
		}
		
	private:
		string BuildAttrPath(const string &strPath_, const string &strAttr_)
		{
			if(strPath_.empty())
				return "<xmlattr>." + strAttr_;

			return strPath_ + ".<xmlattr>." + strAttr_;
		}

		ptree m_xmlTree;
	};

	XSimpleXmlA::XSimpleXmlA(const string &strXml_){
		m_pimp = new XSimpleXmlImp();
		if(!strXml_.empty())
			m_pimp->SetXml(strXml_);
	}
	XSimpleXmlA::~XSimpleXmlA(){
		if(CxNotNull(m_pimp)){
			delete m_pimp;
			m_pimp = NULL;
		}
	}

	void XSimpleXmlA::SetXml(const string &strXml_){
		if(!strXml_.empty())
			m_pimp->SetXml(strXml_);
	}

	string XSimpleXmlA::GetXml(){
		return m_pimp->GetXml();
	}

	int XSimpleXmlA::GetText(const string &strPath_, int nDefault_){
		return m_pimp->GetText(strPath_, nDefault_);
	}
	string XSimpleXmlA::GetText(const string &strPath_, const string &strDefault_){
		return m_pimp->GetText(strPath_, strDefault_);
	}
	vector<string> XSimpleXmlA::GetMultiText(const string &strPath_){
		vector<string> vecText;
		m_pimp->GetMulti(strPath_, vecText, "");
		return vecText;
	}

	int XSimpleXmlA::GetAttr(const string &strPath_, const string &strName_, int nDefault_){
		return m_pimp->GetAttr(strPath_, strName_, nDefault_);
	}
	string XSimpleXmlA::GetAttr(const string &strPath_, const string &strName_, const string &strDefault_){
		return m_pimp->GetAttr(strPath_, strName_, strDefault_);
	}
	vector<string> XSimpleXmlA::GetMultiAttr(const string &strPath_, const string &strName_){
		vector<string> vecText;
		m_pimp->GetMulti(strPath_, vecText, strName_);
		return vecText;
	}

} //clib
} // xugd