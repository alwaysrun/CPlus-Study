#pragma once
#ifndef XUGD_APR_STR_H
#define XUGD_APR_STR_H
#include "xugd.apr.h"
#include "apr_strings.h"

namespace xugd{ namespace apr{
	// �Ƽ�ʹ��<boost/algorithm/string.hpp>�е�string_algo��������
#ifdef __cplusplus
	extern "C" {
#endif

		XAPRAPI_DECLARE(int) XStrNatureCmp(char const *first_, char const *second_, bool bIgnoreCase_=false);

		

#ifdef __cplusplus
	}
#endif
} // apr
} // xugd
#endif // XUGD_APR_STR_H