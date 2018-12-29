#pragma once
#ifndef XUGD_CLIB_THREAD_H
#define XUGD_CLIB_THREAD_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.thread.h
// @LIB: xugd.clib.lib/xugd.clib_d.lib
// @DESCRIPTION: 定义线程（同步、事件、定时器）相关接口类
// @VERSION: 1.0.15.907
//
// @RECORD:
//	编写：Xugd，2015-09-07，初始版本。
//////////////////////////////////////////////////////////////////////////
#include "xugd.common.h"

// <boost/thread>中封装了线程操作类thread，以及互斥量mutex等
namespace xugd{	namespace clib{	
	//////////////////////////////////////////////////////////////////////////
	class XMutexImp;
	class XCLIBCLASS_DECLARE XMutex
	{
	public:
		XMutex();
		~XMutex();

		void Lock();
		bool TryLock();
		void Unlock();

	private:
		CxForbidCopyFun(XMutex);
		XMutexImp *m_pimp;
	};

	// 局部锁定，变量销毁时自动解锁:
	// XMutex mu;
	// ...
	// {
	//   XMutexScopLock mlock(mu);
	//		... // has locked
	// } // after here, will unlocked auto
	class XCLIBCLASS_DECLARE XMutexScopLock
	{
	public:
		explicit XMutexScopLock(XMutex &tex_);
		~XMutexScopLock();

	private:
		CxForbidCopyFun(XMutexScopLock);
		XMutex *m_pTex;
	};

	//////////////////////////////////////////////////////////////////////////
	// 自动重设事件（开始没有信号）
	class XAutoEventImp;
	class XCLIBCLASS_DECLARE XAutoEvent
	{
	public:
		XAutoEvent();
		~XAutoEvent();

		void Wait();
		// 等待到信号返回true；超时返回false
		bool TimeWait(long nMilliSecond_);
		bool TimeWait(int nHoure_, int nMinutes_, int nSecond_);

		// 通知所有等待者（即Wait成功）
		void Notify();

	private:
		CxForbidCopyFun(XAutoEvent);
		XAutoEventImp *m_pimp;
	};

	//////////////////////////////////////////////////////////////////////////
	class XCLIBCLASS_DECLARE XThread 
	{
	public:
		typedef void (*FunOneParam)(void *pParam_);

		// 需要通过Start来启动
		XThread(FunOneParam pFun_);
		~XThread();
		// 如果已启动，不做任何处理；否则，启动线程
		void Start(void *pParam_);
		void Stop();

		// 可用于判断当前线程是否处于活动状态
		bool IsAlive();

		static void Start(FunOneParam pFun_, void *pParam_);

		static void Sleep(long nMilliSecond_);
		static void Sleep(int nHoure_, int nMinutes_, int nSecond_);

	private:
		CxForbidCopyFun(XThread);
		bool m_bInThread;
		void *m_pParam;
		void *m_pThread;
		FunOneParam m_fun;
		XMutex m_muThread;

		void CallFun();
	};


	//////////////////////////////////////////////////////////////////////////
	// 定时器
	class XTimerImp;
	class XCLIBCLASS_DECLARE XTimer
	{
	public:
		typedef void (*Callback)(const XTimer&, void *);

		// 设定回调函数与定时的毫秒数（1/1000秒）
		explicit XTimer(long nExpireMillSec_, bool bIsCycle_=false);
		virtual ~XTimer();

		// 多少秒后触发；如果是循环，则调用回调函数后再次启动定时器
		void Start(Callback funCall_, void *pParam_=NULL);
		void Stop();

		// 重设只影响下次定时器，不影响当前的
		void ResetExpire(long nExpireMilliSec_);
		void ResetExpire(int nHoure_, int nMinutes_, int nSecond_);

	protected:
		virtual long NextExpireMS() const;

	private:
		CxForbidCopyFun(XTimer);

		friend class XTimerImp;
		void ToRecycle();

		bool m_bIsCycle;
		long m_nMilliSeconds;
		XTimerImp *m_pimp;
	};

	// 整点定时器：整点时自动调用回调函数
	class XCLIBCLASS_DECLARE XClockTimer:public XTimer
	{
	public: 
		XClockTimer();

	protected:
		virtual long NextExpireMS() const;
	};
} //clib
} // xugd
#endif // XUGD_CLIB_THREAD_H