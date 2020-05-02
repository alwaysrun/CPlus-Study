#pragma once
#ifndef XUGD_CLIB_THREAD_H
#define XUGD_CLIB_THREAD_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.thread.h
// @LIB: xugd.clib.lib/xugd.clib_d.lib
// @DESCRIPTION: �����̣߳�ͬ�����¼�����ʱ������ؽӿ���
// @VERSION: 1.0.15.907
//
// @RECORD:
//	��д��Xugd��2015-09-07����ʼ�汾��
//////////////////////////////////////////////////////////////////////////
#include "xugd.common.h"

// <boost/thread>�з�װ���̲߳�����thread���Լ�������mutex��
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

	// �ֲ���������������ʱ�Զ�����:
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
	// �Զ������¼�����ʼû���źţ�
	class XAutoEventImp;
	class XCLIBCLASS_DECLARE XAutoEvent
	{
	public:
		XAutoEvent();
		~XAutoEvent();

		void Wait();
		// �ȴ����źŷ���true����ʱ����false
		bool TimeWait(long nMilliSecond_);
		bool TimeWait(int nHoure_, int nMinutes_, int nSecond_);

		// ֪ͨ���еȴ��ߣ���Wait�ɹ���
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

		// ��Ҫͨ��Start������
		XThread(FunOneParam pFun_);
		~XThread();
		// ����������������κδ������������߳�
		void Start(void *pParam_);
		void Stop();

		// �������жϵ�ǰ�߳��Ƿ��ڻ״̬
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
	// ��ʱ��
	class XTimerImp;
	class XCLIBCLASS_DECLARE XTimer
	{
	public:
		typedef void (*Callback)(const XTimer&, void *);

		// �趨�ص������붨ʱ�ĺ�������1/1000�룩
		explicit XTimer(long nExpireMillSec_, bool bIsCycle_=false);
		virtual ~XTimer();

		// ������󴥷��������ѭ��������ûص��������ٴ�������ʱ��
		void Start(Callback funCall_, void *pParam_=NULL);
		void Stop();

		// ����ֻӰ���´ζ�ʱ������Ӱ�쵱ǰ��
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

	// ���㶨ʱ��������ʱ�Զ����ûص�����
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