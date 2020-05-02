#include "xugd.apr.dir.h"
#include "xugd.private.h"

#include "apr_file_io.h"

namespace xugd{ namespace apr{

	XAPRAPI_DECLARE(void) XDirMake(const char *pDir_)
	{
		apr_status_t rv;
		if(AprFail(rv=apr_dir_make_recursive(pDir_, 0, g_xugd_aprPool)))
			throw AprFileException("apr_dir_make_recursive", rv, pDir_);
	}

	XAPRAPI_DECLARE(void) XDirRemove(const char *pDir_)
	{
		apr_status_t rv;
		if(AprFail(rv=apr_dir_remove(pDir_, g_xugd_aprPool)))
			throw AprFileException("apr_dir_remove", rv, pDir_);
	}
//////////////////////////////////////////////////////////////////////////
} // apr
} // xugd