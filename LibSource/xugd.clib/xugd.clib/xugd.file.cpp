#include "xugd.file.h"
#include "xugd.str.h"
#include "xugd.convert.h"
#include <codecvt>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

namespace xugd{	namespace clib{
	using namespace boost;
	using namespace boost::filesystem;
	class XPathImp{
	public:
		void SetPath(wstring strPath_)
		{
			wpath pTmp(strPath_);
			if(pTmp.has_relative_path())
			{
				trim_left_if(strPath_, is_any_of("/\\"));
				m_filePath = strPath_;
			}
			else{ // x:/ or x:
				m_filePath = pTmp.root_name();
				m_filePath += "/";
			}
		}
		wpath& Get()
		{
			return m_filePath;
		}
	private:
		wpath m_filePath;
	};

	//////////////////////////////////////////////////////////////////////////
	XPath::XPath()
	{
		m_pimp = new XPathImp();
	}
	XPath::XPath(const wstring &strPath_)
	{
		m_pimp = new XPathImp();
		m_pimp->SetPath(strPath_);
	}
	XPath::XPath(const XPath& xOther_)
	{
		if(CxIsNull(m_pimp))
			m_pimp = new XPathImp();

		m_pimp->Get() = xOther_.ToString();
	}
	XPath::~XPath()
	{
		if(CxNotNull(m_pimp))
			delete m_pimp;
	}

	XPath& XPath::operator=(const XPath& xOther_)
	{
		m_pimp->Get() = xOther_.ToString();
		return *this;
	}
	XPath& XPath::operator=(const wstring &strPath_)
	{
		m_pimp->SetPath(strPath_);
		return *this;
	}
	// 组合路径："parent"+"child" = "parent\child"
	XPath& XPath::operator+(const XPath& xOther_)
	{
		m_pimp->Get() /= xOther_.ToString();
		return *this;
	}
	XPath& XPath::operator+(const wstring &strPath_)
	{
		if(!strPath_.empty())
		{
			wstring str(strPath_);
			XWstrTrimEnd(str, L"/\\");
			m_pimp->Get() /= str;
		}
		return *this;
	}
	
	wstring XPath::ToString() const
	{
		return (m_pimp->Get()).c_str();
	}
	wstring XPath::ParentPath() const
	{
		return (m_pimp->Get()).parent_path().c_str();
	}
	wstring XPath::FileName() const
	{
		return (m_pimp->Get()).filename().c_str();
	}
	wstring XPath::ExtName() const
	{
		return (m_pimp->Get()).extension().c_str();
	}
	wstring XPath::FileWithoutExt() const
	{
		return (m_pimp->Get()).stem().c_str();
	}
	wstring XPath::RootPath() const
	{
		return (m_pimp->Get()).root_path().c_str();
	}

	XPath& XPath::ToAbsolut()
	{
		if(!IsAbsolute())
		{
			wpath pRun = initial_path();
			pRun /= m_pimp->Get();
			m_pimp->Get() = pRun;
		}

		return *this;
	}
	XPath& XPath::ReplaceExt(const wstring &strExt_)
	{
		m_pimp->Get().replace_extension(strExt_);

		return *this;
	}

	bool XPath::IsEmpty() const
	{
		return (m_pimp->Get()).empty();
	}
	bool XPath::IsAbsolute() const
	{
		return (m_pimp->Get()).is_absolute();
	}
	bool XPath::IsExists() const
	{
		return exists(m_pimp->Get());
	}
	bool XPath::IsFile() const
	{
		return is_regular_file(m_pimp->Get());
	}
	bool XPath::IsDirectory()const
	{
		return is_directory(m_pimp->Get());
	}

	// 判断是否是有效路径（所有字符都合法）
	bool XPath::IsValidPath(const wstring &strPath_)
	{
		wstring wstr(strPath_);
		string astr = XConvert::Wstr2A(wstr);
		return native(astr);
	}
	wstring XPath::GetRunPath()
	{
		return initial_path().c_str();
	}

	//////////////////////////////////////////////////////////////////////////
	bool XFile::IsExists(const wstring &strPath_)
	{
		return exists(strPath_);
	}
	bool XFile::IsFile(const wstring &strPath_)
	{
		return is_regular_file(strPath_);
	}
	bool XFile::IsDirectory(const wstring &strPath_)
	{
		return is_directory(strPath_);
	}

	unsigned long XFile::GetSize(const wstring &strPath_)
	{
		return (long)file_size(strPath_);
	}
	time_t XFile::GetLastWrite(const wstring &strPath_)
	{
		return last_write_time(strPath_);
	}
	void XFile::SetLastWrite(const wstring &strPath_, time_t tNewTime_)
	{
		last_write_time(strPath_, tNewTime_);
	}

	void XFile::RemoveFile(const wstring &strPath_)
	{
		remove(strPath_);
	}
	void XFile::Rename(const wstring &strOld_, const wstring &strNew_)
	{
		rename(strOld_, strNew_);
	}
	void XFile::Copy(const wstring &strFrom_, const wstring &strTo_, bool bOverWrite_)
	{
		copy_option::enum_type euType = boost::filesystem::copy_option::fail_if_exists;
		if(bOverWrite_)
			euType = boost::filesystem::copy_option::overwrite_if_exists;
		copy_file(strFrom_, strTo_, euType);
	}
	void XFile::AppendLog(const wstring &strName_, const string &strLog_){
		AppendLog(strName_, XConvert::Astr2W(strLog_));
	}
	void XFile::AppendLog(const wstring &strName_, const wstring &strLog_){
		try {
			XPath logPath(strName_);
			wofstream outFile(logPath.ToAbsolut().ToString(), ios_base::app);

			outFile<<strLog_<<endl;
			outFile.close();
		}
		catch(...){}
	}

	void XFile::AppendText(const wstring &strPath_, const string &strText_){
		AppendText(strPath_, XConvert::Astr2W(strText_));
	}
	void XFile::AppendText(const wstring &strPath_, const wstring &strText_)
	{
		XPath txtPath(strPath_);
		wofstream outFile(txtPath.ToAbsolut().ToString(), ios_base::app);
		locale utf8Locale(locale(), new codecvt_utf8<wchar_t>);
		outFile.imbue(utf8Locale);

		outFile<<strText_<<endl;
		outFile.close();
	}

	void XFile::OpenFile(wofstream &outFile_, const wstring &strPath_, ios_base::open_mode nMode_)
	{
		outFile_.open(strPath_, nMode_);
		locale utf8Locale(locale(), new codecvt_utf8<wchar_t>);
		outFile_.imbue(utf8Locale);
	}

	void XFile::OpenFile(wifstream &inFile_, const wstring &strPath_, ios_base::open_mode nMode_)
	{
		inFile_.open(strPath_, nMode_);
		locale utf8Locale(locale(), new codecvt_utf8<wchar_t>);
		inFile_.imbue(utf8Locale);
	}

	//////////////////////////////////////////////////////////////////////////
	bool XDir::CreateDir(const wstring &strPath_)
	{
		return create_directories(strPath_);
	}
	void XDir::RemoveDir(const wstring &strPath_)
	{
		remove_all(strPath_);
	}

	list<XFileInfo> XDir::EnumFiles(const wstring &strPath_)
	{
		list<XFileInfo> lstInfos;
		XFileInfo fInfo;
		wpath ptParent(strPath_);
		directory_iterator itEnd;
		directory_iterator pos(ptParent) ;
		for(; pos!=itEnd; ++pos)
		{
			fInfo.Name = pos->path().filename().c_str();
			fInfo.IsDir = is_directory(*pos);
			lstInfos.push_back(fInfo);
		}

		return lstInfos;
	}
} //clib
} // xugd