#include "xugd.apr.str.h"
#include "../xugd.clib/xugd.common.h"

namespace xugd{ namespace apr{
	
	//////////////////////////////////////////////////////////////////////////
	// natural order comparison	
	XAPRAPI_DECLARE(int) XStrNatureCmp(char const *first_, char const *second_, bool bIgnoreCase_)
	{
		if(bIgnoreCase_)
			return apr_strnatcasecmp(first_, second_);

		return apr_strnatcmp(first_, second_);
	}

} // apr
} // xugd