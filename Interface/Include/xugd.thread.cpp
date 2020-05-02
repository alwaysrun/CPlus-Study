#include "xugd.thread.h"
#include <thread>
#include <chrono>

namespace xugd{	namespace clib{
//using namespace std;
//////////////////////////////////////////////////////////////////////////
// Ïß³Ì²Ù×÷
//////////////////////////////////////////////////////////////////////////
void XuThread::sleep(int nSeconds, int nMills) {
	std::chrono::seconds secs(nSeconds);
	std::chrono::milliseconds mills(nMills);
	std::this_thread::sleep_for(secs + mills);
}

} //clib
} // xugd