#pragma once
#ifndef XUGD_CLIB_THREAD_H
#define XUGD_CLIB_THREAD_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.thread.h
// @DESCRIPTION: 定义线程相关操作与类
// @VERSION: 1.0.20.502
//
// @RECORD:
//	编写：Xugd，2020-05-02，初始版本。
//////////////////////////////////////////////////////////////////////////
//#include <thread>
#include <mutex>
#include <condition_variable>


namespace xugd{	namespace clib{
	//////////////////////////////////////////////////////////////////////////
	// 线程操作
	//////////////////////////////////////////////////////////////////////////
	// 互斥量mutex可以通过lock_guard或unique_lock（提供lock与unlock操作接口，可以保护已经lock的mutex）
	// 方便地进行管理（在离开作用域时，自动unlock）

	class XuThread{
	public:
		static void sleep(int nSeconds, int nMills = 0);
	};

	class XuEvent{  // No signal when init
	public:
		// reset：是否重设为无信号；
		//		默认不重设: 此时需要通过reset设为无信号状态，否则会一直为有信号状态
		//		为true时，每次wait前会自动重设为无信号状态
		void wait(bool reset=false);
		bool wait(int nSec, int nMillSec=0, bool reset = false);

		void notifyOne();
		void notifyAll();

		void reset();

	private:
		bool m_bNotified = false;
		std::mutex m_mtx;
		std::condition_variable m_cv;
	};

} //clib
} // xugd

#endif	//XUGD_CLIB_THREAD_H