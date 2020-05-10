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

void XuEvent::wait(bool reset) {
	std::unique_lock<std::mutex> lker(m_mtx);
	if(reset) m_bNotified = false;
	m_cv.wait(lker, [this] {return this->m_bNotified; });
}

bool XuEvent::wait(int nSec, int nMillSec, bool reset) {
	std::unique_lock<std::mutex> lker(m_mtx);
	if (reset) m_bNotified = false;
	std::chrono::seconds secs(nSec);
	std::chrono::milliseconds mills(nMillSec);
	auto ret = m_cv.wait_for(lker, secs + mills, [this] {return this->m_bNotified; });
	//return (ret != std::cv_status::timeout);
	return ret;
}

void XuEvent::notifyOne() {
	m_bNotified = true;
	m_cv.notify_one();
}
void XuEvent::notifyAll() {
	m_bNotified = true;
	m_cv.notify_all();
}

void XuEvent::reset() {
	m_bNotified = false;
}

} //clib
} // xugd