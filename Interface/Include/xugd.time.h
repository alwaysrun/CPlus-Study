#pragma once
#ifndef XUGD_CLIB_TIME_H
#define XUGD_CLIB_TIME_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.time.h
// @DESCRIPTION: ����ʱ����ز�������
// @VERSION: 1.0.20.502
//
// @RECORD:
//	��д��Xugd��2020-05-02����ʼ�汾��
//////////////////////////////////////////////////////////////////////////
#include <string>

namespace xugd{	namespace clib{
	//////////////////////////////////////////////////////////////////////////
	// ʱ�����
	//////////////////////////////////////////////////////////////////////////
	class XuTime{
	public:
		// ��ȡʱ�䣨���ػ�ϵͳ���ַ���(2020-05-02 14:40:31.015)
		static std::string getLocalTime(bool bInlcudeMill = true);
		static std::string getSystemTime(bool bInlcudeMill = true);

	private:
	};

} //clib
} // xugd

#endif	//XUGD_CLIB_TIME_H