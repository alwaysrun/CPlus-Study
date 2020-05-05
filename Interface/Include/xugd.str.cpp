#include "xugd.str.h"
#include <sstream>

namespace xugd{	namespace clib{
using namespace std;
//////////////////////////////////////////////////////////////////////////
// ×Ö·û´®²Ù×÷
//////////////////////////////////////////////////////////////////////////
std::string XuStr::int2Str(int nNumber) {
	return std::to_string(nNumber);
}
std::string XuStr::uint2Str(unsigned int nNumber) {
	return std::to_string(nNumber);
}
std::string XuStr::double2Str(double fNumber) {
	return std::to_string(fNumber);
}

int XuStr::str2Int(const std::string &strNumber) {
	return std::stoi(strNumber);
}
unsigned int XuStr::str2Uint(const std::string &strNumber) {
	return (unsigned int)std::stoul(strNumber);
}
double XuStr::str2Double(const std::string &strNumber) {
	return std::stod(strNumber);
}

std::vector<std::string> XuStr::split(const std::string &strTotal, bool bDiscardEmpty_) {
	vector<string> vecResult;

	string temp;
	istringstream iss(strTotal);
	while (getline(iss, temp)) {
		if (bDiscardEmpty_ && temp.empty())
			continue;

		vecResult.push_back(temp);
	}

	return vecResult;
}

std::vector<std::string> XuStr::split(const std::string &strTotal, char chDelim, bool bDiscardEmpty_) {
	vector<string> vecResult;

	string temp;
	istringstream iss(strTotal);
	while (getline(iss, temp, chDelim)) {
		if (bDiscardEmpty_ && temp.empty())
			continue;

		vecResult.push_back(temp);
	}

	return std::move(vecResult);
}

} //clib
} // xugd