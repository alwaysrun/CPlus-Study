#include "stdafx.h"
#include "xugd.thread.h"
#include "xugd.time.h"
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace xugd{	namespace clib{
	using namespace boost;
	using namespace boost::asio;

	//////////////////////////////////////////////////////////////////////////
	class XMutexImp
	{
	public:
		mutex& Get()
		{
			return m_locker;
		}

	private:
		boost::mutex m_locker;
	};

	XMutex::XMutex()
	{
		m_pimp = new XMutexImp();
	}
	XMutex::~XMutex()
	{
		if(CxNotNull(m_pimp))
		{
			try
			{
				m_pimp->Get().unlock();
				delete m_pimp;
			}
			catch(...){}
		}
	}
	void XMutex::Lock()
	{
		m_pimp->Get().lock();
	}
	bool XMutex::TryLock()
	{
		return m_pimp->Get().try_lock();
	}
	void XMutex::Unlock()
	{
		m_pimp->Get().unlock();
	}

	XMutexScopLock::XMutexScopLock(XMutex &tex_):m_pTex(&tex_)
	{
		m_pTex->Lock();
	}
	XMutexScopLock::~XMutexScopLock()
	{
		m_pTex->Unlock();
	}

	//////////////////////////////////////////////////////////////////////////
	class XAutoEventImp
	{
	public:
		void Wait(){
			mutex::scoped_lock lk(m_mLocker);
			m_conVar.wait(m_mLocker);
		}

		bool Wait(posix_time::time_duration tdWait_){
			mutex::scoped_lock lk(m_mLocker);
			return m_conVar.timed_wait(m_mLocker, tdWait_);
		}

		void Notify()
		{
			m_conVar.notify_all();
		}
	private:
		mutex m_mLocker;
		condition_variable_any m_conVar;
	};

	XAutoEvent::XAutoEvent(){
		m_pimp = new XAutoEventImp();
	}
	XAutoEvent::~XAutoEvent(){
		if(CxNotNull(m_pimp))
		{
			try
			{
				m_pimp->Notify();
				this_thread::sleep(posix_time::milliseconds(100));
				delete m_pimp;
				m_pimp = NULL;
			}
			catch(...){}
		}
	}

	void XAutoEvent::Wait(){
		m_pimp->Wait();
	}
	bool XAutoEvent::TimeWait(long nMilliSecond_){
		return m_pimp->Wait(posix_time::milliseconds(nMilliSecond_));
	}
	bool XAutoEvent::TimeWait(int nHoure_, int nMinutes_, int nSecond_){
		return m_pimp->Wait(posix_time::time_duration(nHoure_, nMinutes_, nSecond_));
	}

	// 通知所有等待者（即Wait成功）
	void XAutoEvent::Notify(){
		m_pimp->Notify();
	}

	//////////////////////////////////////////////////////////////////////////
	XThread::XThread(FunOneParam pFun_)
	{
		m_bInThread = false;
		m_pThread = NULL;

		m_fun = pFun_;
		m_pParam = NULL;
		// thread(CallFun);
	}
	XThread::~XThread()
	{
		Stop();
	}
	void XThread::Start(void *pParam_)
	{
		if(m_bInThread) return;

		{
			XMutexScopLock lker(m_muThread);
			if(!m_bInThread)
			{
				if(CxNotNull(m_pThread)){
					delete m_pThread;
					m_pThread = NULL;
				}

				m_pParam = pParam_;
				m_bInThread = true;
				m_pThread = new thread(boost::bind(&XThread::CallFun, this));
			}
		}
	}
	void XThread::Stop()
	{
		if(!m_bInThread) return;

		{
			XMutexScopLock lker(m_muThread);
			if(CxNotNull(m_pThread))
			{
				try 
				{
					thread *pThr = (thread *)m_pThread;
					m_pThread = NULL;

					pThr->interrupt();
					pThr->timed_join(posix_time::seconds(5));
					delete pThr;

					m_bInThread = false;
				}
				catch(...){}
			}
			else{
				m_bInThread = false;
			}
		}
	}

	bool XThread::IsAlive(){
		return m_bInThread;
	}

	void XThread::CallFun()
	{
		try 
		{
			m_fun(m_pParam);
		}
		catch(...){}
		m_bInThread = false;
	}

	void XThread::Start(FunOneParam pFun_, void *pParam_)
	{
		thread(pFun_, pParam_);
	}

	void XThread::Sleep(long nMillSecs_)
	{
		this_thread::sleep(posix_time::milliseconds(nMillSecs_));
	}
	void XThread::Sleep(int nHoure_, int nMinutes_, int nSecond_)
	{
		this_thread::sleep(posix_time::time_duration(nHoure_, nMinutes_, nSecond_));
	}

	//////////////////////////////////////////////////////////////////////////
	class XTimerImp
	{
	public:
		XTimerImp(XTimer *pParent_, XTimer::Callback funCall_)
			:m_timer(m_ios) 
		{
			m_pParam = NULL;
			m_bToStop = false;

			m_pParent = pParent_;
			m_pFun = funCall_;
		}
		~XTimerImp()
		{
			if(!m_bToStop)
				Stop();
		}

		void Restart(long nExpireMillSec_)
		{
			if(m_bToStop) return;

			m_timer.expires_from_now(posix_time::milliseconds(nExpireMillSec_));
			m_timer.async_wait(boost::bind(&XTimerImp::MyCallback, this, boost::asio::placeholders::error));
		}

		void Start(long nExpireMillSec_, void *pParam_)
		{
			m_bToStop = false;
			m_pParam = pParam_;

			Restart(nExpireMillSec_);

			system::error_code er;
			m_ios.run(er);
			//std::string str = er.message();
			//std::cout<<"After run: "<<str<<std::endl;
		}

		void Stop()
		{
			m_bToStop = true;
			system::error_code er;
			m_timer.cancel(er);
			std::string str = er.message();
		}

	private:
		bool m_bToStop;
		io_service m_ios;
		deadline_timer m_timer;
		// io_service::work m_worker;

		void *m_pParam;
		XTimer *m_pParent;
		XTimer::Callback m_pFun;

		void MyCallback(const boost::system::error_code&)
		{
			if(m_bToStop) return;

			m_pFun(*m_pParent, m_pParam);
			m_pParent->ToRecycle();
		}
	};

	XTimer::XTimer(long nExpireMillSec_, bool bIsCycle_)
	{
		m_nMilliSeconds = nExpireMillSec_;
		m_bIsCycle = bIsCycle_;
		m_pimp = NULL;
	}

	XTimer::~XTimer()
	{
		Stop();
	}

	void XTimer::Start(Callback funCall_, void *pParam_)
	{
		if(CxIsNull(funCall_)) return;

		Stop();
		m_pimp = new XTimerImp(this, funCall_);
		thread(boost::bind(&XTimerImp::Start, m_pimp, NextExpireMS(), pParam_));
		// m_pimp->Start(NextExpireMS(), pParam_);
	}
	void XTimer::Stop()
	{
		if(CxNotNull(m_pimp))
		{
			try
			{
				m_pimp->Stop();
				delete m_pimp;
				m_pimp = NULL;
			}
			catch(...){}
		}
	}
	void XTimer::ToRecycle()
	{
		if(CxNotNull(m_pimp) && m_bIsCycle)
		{
			m_pimp->Restart(NextExpireMS());
		}
	}

	void XTimer::ResetExpire(long nExpireMillSec_)
	{
		m_nMilliSeconds = nExpireMillSec_;
	}
	void XTimer::ResetExpire(int nHoure_, int nMinutes_, int nSecond_)
	{
		m_nMilliSeconds = (long)posix_time::time_duration(nHoure_, nMinutes_, nSecond_).total_milliseconds();
	}

	long XTimer::NextExpireMS() const
	{
		return m_nMilliSeconds;
	}

	//////////////////////////////////////////////////////////////////////////
	XClockTimer::XClockTimer()
		:XTimer(XDateTime::OneHour2Msec, true)
	{
	}

	long XClockTimer::NextExpireMS() const
	{
		posix_time::ptime ptNow = posix_time::microsec_clock::universal_time();
		// posix_time::ptime ptNow = posix_time::second_clock::universal_time();
		auto nextClock = posix_time::time_duration(ptNow.time_of_day().hours()+1, 0, 0);
		// for test:
		// auto nextClock = posix_time::time_duration(ptNow.time_of_day().hours(), ptNow.time_of_day().minutes()+1, 0);
		posix_time::ptime ptExpire = posix_time::ptime(ptNow.date(), nextClock);
		
		long nMilli = (long)(ptExpire-ptNow).total_milliseconds()+1;
		if(nMilli<1000) // at least 1second
			nMilli = 1000;
		
		// for test:
		//nMilli = 10*1000; // 10 second
		return nMilli;
	}
} //clib
} // xugd