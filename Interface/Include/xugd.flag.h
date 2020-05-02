#pragma once
#ifndef XUGD_CLIB_FLAG_H
#define XUGD_CLIB_FLAG_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.flag.h
// @LIB: xugd.clib.lib/xugd.clib_d.lib
// @DESCRIPTION: �����־λ������
// @VERSION: 1.0.15.907
//
// @RECORD:
//	��д��Xugd��2015-09-07����ʼ�汾��
//////////////////////////////////////////////////////////////////////////
#include "xugd.common.h"

namespace xugd{	namespace clib{
	class XCLIBCLASS_DECLARE XFlag 
	{
	public: 

		// �Ƿ��������λ��
		template<typename T>
		static bool Check(T tFlag_, T toCheck_)
		{
			return (toCheck_!=0) && ((tFlag_&toCheck_)==toCheck_);
		}

		// �Ƿ�����������е���һλ���λ
		template<typename T>
		static bool CheckAny(T tFlag_, T toCheck_)
		{
			return (0!=(tFlag_&toCheck_));
		}

		// �趨λ
		template<typename T>
		static void Set(T &tFlag_, T toSet_)
		{
			tFlag_ = (T)(tFlag_ | toSet_);
		}

		// �Ƴ�λ
		template<typename T>
		static void Clear(T tFlag_, T toClear_)
		{
			tFlag_ = (T)(tFlag_ & (~toClear_));
		}

		// ���
		template<typename T>
		static T Diff(T tFirst_, T tSecond_)
		{
			return (T)(tFirst_ ^ tSecond_);
		}
	};
} //clib
} // xugd
#endif //XUGD_CLIB_FLAG_H