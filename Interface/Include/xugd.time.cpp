#include "xugd.time.h"
#include <chrono>
#include <sstream>
#include <ctime>
#include <iomanip>

namespace xugd{	namespace clib{
//using namespace std;
//////////////////////////////////////////////////////////////////////////
// Ê±¼ä²Ù×÷
//////////////////////////////////////////////////////////////////////////

std::string getTimeString(bool bLocal, bool bIncludeMS) {
	auto tNow = std::chrono::system_clock::now();
	//auto tmNow = std::chrono::system_clock::to_time_t(tNow);
	auto tSeconds = std::chrono::duration_cast<std::chrono::seconds>(tNow.time_since_epoch());
	auto secNow = tSeconds.count();
	tm tmNow;
	if (bLocal) {
		localtime_s(&tmNow, &secNow);
	}
	else {
		gmtime_s(&tmNow, &secNow);
	}

	std::ostringstream oss;
	oss << std::put_time(&tmNow, "%Y-%m-%d %H:%M:%S");
	if (bIncludeMS) {
		auto tMilli = std::chrono::duration_cast<std::chrono::milliseconds>(tNow.time_since_epoch());
		auto ms = tMilli - tSeconds;
		oss << "." << std::setfill('0') << std::setw(3) << ms.count();
	}

	return oss.str();
}

std::string XuTime::getLocalTime(bool bInlcudeMill) {
	return getTimeString(true, bInlcudeMill);
}

std::string XuTime::getSystemTime(bool bInlcudeMill) {
	return getTimeString(false, bInlcudeMill);
}

} //clib
} // xugd