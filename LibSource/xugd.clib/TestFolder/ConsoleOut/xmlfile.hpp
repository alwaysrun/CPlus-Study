#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/xml_parser.hpp>
using namespace boost;
using namespace boost::property_tree;

#include <map>
#include <vector>
#include <codecvt>
#include <locale>
#include <iostream>
using namespace std;

typedef std::codecvt<wchar_t, char, std::mbstate_t> code_type;

const wstring XMLATTR = L"<xmlattr>";
const wstring XMLCOMMENT = L"<xmlcomment>";
const wstring XMLATTR_DOT = L"<xmlattr>.";
const wstring XMLCOMMENT_DOT = L"<xmlcomment>.";

class ConfigParser
{
public:

	ConfigParser() : m_conv(new code_type("CHS"))
	{
	}

	~ConfigParser()
	{
	}

	static void Read(const wstring& fileName, wptree& ptree)
	{
		std::wifstream f(fileName);
		std::locale utf8Locale(std::locale(), new std::codecvt_utf8<wchar_t>);
		f.imbue(utf8Locale); //先转换一下
		// wcout.imbue(std::locale("chs")); //初始化cout为中文输出格式

		//用wptree去解析
		property_tree::read_xml(f, ptree);
	}

	static void Write(const wstring& fileName, wptree& ptree)
	{
		std::wostringstream f;
		//std::locale utf8Locale(std::locale(), new std::codecvt_utf8<wchar_t>);
		//f.imbue(utf8Locale);

		property_tree::write_xml(f, ptree);

		std::wofstream file(fileName);
		std::locale utf8Locale(std::locale(), new std::codecvt_utf8<wchar_t>);
		file.imbue(utf8Locale);
		wstring strWrite=f.str();
		int nIndentCount = 0;
		bool bInBrack = false;
		auto pos=strWrite.begin();
		//for(; pos!=strWrite.end(); ++pos)
		//{
		//	file<<*pos;
		//	if(*pos == L'>')
		//		break;
		//}
		//++pos;

		for(; pos!=strWrite.end(); ++pos)
		{
			if(*pos==L'<')
			{
				if(pos!= (strWrite.end()-1))
				{
					if(*(pos+1)==L'/')
					{
						if(nIndentCount>0) --nIndentCount;
					}
					else if(*(pos+1)!=L'?' && *(pos+1)!=L'!')
						bInBrack = true;
				}


				if(pos != strWrite.begin())
				{
					if(*(pos-1) == L'>')
					{
						file<<L'\n';
						for(int n=0 ; n<nIndentCount ; ++n)
							file<<L'\t';
					}
				}
			}
			else if(*pos==L'>')
			{
				if(bInBrack)
				{
					if(pos != strWrite.begin())
					{
						if(*(pos-1) != L'/')
							++nIndentCount;
					}
				}
				bInBrack = false;
			}
			file<<*pos;
		}
		file.close();
	}

	// convert UTF-8 string to wstring
	std::wstring to_wstr(const std::string& str)
	{
		return m_conv.from_bytes(str);
	}

	// convert wstring to UTF-8 string
	std::string to_str(const std::wstring& str)
	{
		return m_conv.to_bytes(str);
	}

	//获取子节点列表
	auto Descendants(const wptree& root, const wstring& key)->decltype(root.get_child_optional(key))
	{
		return root.get_child_optional(key);
	}

	//根据子节点属性获取子节点列表
	template<typename T>
	vector<wptree> GetChildsByAttr(const wptree& parant, const wstring& tagName, const wstring& attrName, const T& attrVal)
	{
		vector<wptree> v;

		for (auto& child : parant)
		{
			if (child.first != tagName)
				continue;

			auto attr = Attribute<T>(child, attrName);

			if (attr&&*attr == attrVal)
				v.push_back(child.second);
		}

		return v;
	}

	//获取节点的某个属性值
	template<typename R>
	optional<R> Attribute(const wptree& node, const wstring& attrName)
	{
		return node.get_optional<R>(XMLATTR_DOT + attrName);
	}

	//获取节点的某个属性值，默认为string
	optional<wstring> Attribute(const wptree& node, const wstring& attrName)
	{
		return Attribute<wstring>(node, attrName);
	}

	//获取value_type的某个属性值
	template<typename R>
	optional<R> Attribute(const wptree::value_type& pair, const wstring& attrName)
	{
		if (pair.first == XMLATTR)
			return pair.second.get_optional<R>(attrName);
		else if (pair.first == XMLCOMMENT)
			return optional<R>();
		else
			return pair.second.get_optional<R>(XMLATTR_DOT + attrName);
	}

	//获取value_type的某个属性值，默认为string
	optional<wstring> Attribute(const wptree::value_type& pair, const wstring& attrName)
	{
		return Attribute<wstring>(pair, attrName);
	}

	//根据某个属性生成一个<string, ptree>的multimap
	//template<class F = std::function<bool(wstring&)> >
	//multimap<wstring, wptree> MakeMapByAttr(const wptree& root, const wstring& key, const wstring& attrName, F predict = [](wstring& str){return true; })
	//{
	//	multimap<wstring, wptree> resultMap;
	//	auto list = Descendants(root, key);
	//	if (!list)
	//		return resultMap;

	//	for (auto& item : *list)
	//	{
	//		auto attr = Attribute(item, attrName);
	//		if (attr&&predict(*attr))
	//			resultMap.insert(std::make_pair(*attr, item.second));
	//	}

	//	return resultMap;
	//}

private:
	std::wstring_convert<code_type> m_conv;
};

