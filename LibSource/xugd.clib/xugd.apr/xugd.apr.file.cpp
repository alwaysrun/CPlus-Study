#include "xugd.apr.h"
#include "xugd.private.h"
#include "xugd.apr.file.h"
#include "xugd.apr.str.h"
#include "../xugd.clib/xugd.common.h"
#include "../xugd.clib/xugd.str.h"

#include "apr_lib.h"

namespace xugd{ namespace apr{
#define FILE_LINE_MAXLEN 1024*8

	XFileInfo::XFileInfo()
	{
		m_fHandle = NULL;
		m_filePool = NULL;
		m_szName[0]=NULL;

		apr_status_t rv;
		AprInitCheck();
		if(AprFail(rv=apr_pool_create(&m_filePool, g_xugd_aprPool)))
			throw AprException("apr_pool_create", rv);
	}

	XFileInfo::~XFileInfo()
	{
		if(CxNotNull(m_fHandle))
		{
			Close();
		}
		if(CxNotNull(m_filePool))
		{
			apr_pool_destroy(m_filePool);
		}

		apr_terminate();
	}

	void XFileInfo::Open(const char* szFile_, FOpenMode euType_)
	{
		if(CxNotNull(m_fHandle))
			throw AprFileException("Another file has opened", APR_FROM_OS_ERROR(ERROR_TOO_MANY_OPEN_FILES), m_szName);

		apr_status_t rv;
		if(AprFail(rv=apr_file_open(&m_fHandle, szFile_, (apr_int32_t)euType_, 0, m_filePool)))
		{
			m_fHandle = NULL;
			throw AprFileException("apr_file_open", rv, szFile_);
		}

		strcpy_s(m_szName, MAX_PATH, szFile_);
	}

	void XFileInfo::Close()
	{
		if(CxIsNull(m_fHandle))
			return;

		apr_file_close(m_fHandle);
		apr_pool_clear(m_filePool);
	}

	apr_size_t XFileInfo::Write(const void *pBuffer_, apr_size_t nBytes_)
	{
		apr_status_t rv;
		if(AprFail(rv=apr_file_write(m_fHandle, pBuffer_, &nBytes_)))
			throw AprFileException("apr_file_write", rv, m_szName);

		return nBytes_;
	}

	apr_size_t XFileInfo::Read(void *pBuffer_, apr_size_t nBytes_)
	{
		apr_status_t rv;
		if(AprFail(rv=apr_file_read(m_fHandle, pBuffer_, &nBytes_)))
		{
			if(CxNotEqual(rv, APR_EOF))
				throw AprFileException("apr_file_read", rv, m_szName);
		}

		return nBytes_;
	}

	char* XFileInfo::Gets()
	{
		apr_status_t rv;
		char *pLine = (char*)apr_palloc(m_filePool, FILE_LINE_MAXLEN);
		if(AprFail(rv=apr_file_gets(pLine, FILE_LINE_MAXLEN, m_fHandle)))
		{
			if(CxNotEqual(rv, APR_EOF))
				throw AprFileException("apr_file_gets", rv, m_szName);
		}

		size_t nLen=strlen(pLine);
		if(CxIsEqual(pLine[nLen-1], '\n'))
			pLine[nLen-1]=NULL;

		return pLine;
	}

	void XFileInfo::Puts(const char *pLine_)
	{
		apr_status_t rv;
		if(AprFail(rv=apr_file_puts(pLine_, m_fHandle)))
			throw AprFileException("apr_file_puts", rv, m_szName);

		if(!XStrEndWith(pLine_, '\n'))
			apr_file_puts("\n", m_fHandle);
	}

	void XFileInfo::Flush()
	{
		apr_status_t rv;
		if(AprFail(rv=apr_file_sync(m_fHandle)))
			throw AprFileException("apr_file_sync", rv, m_szName);
	}

	apr_off_t XFileInfo::Seek(FSeekMode euMode_, apr_off_t nOffset_)
	{
		apr_status_t rv;
		if(AprFail(rv=apr_file_seek(m_fHandle, (apr_seek_where_t)euMode_, &nOffset_)))
			throw AprFileException("apr_file_seek", rv, m_szName);

		return nOffset_;
	}

	apr_off_t XFileInfo::Tell()
	{
		return Seek(FSeekCur, 0);
	}

	void XFileInfo::Trunc(apr_off_t nOffset_)
	{
		apr_status_t rv;
		if(AprFail(rv=apr_file_trunc(m_fHandle, nOffset_)))
			throw AprFileException("apr_file_trunc", rv, m_szName);
	}

	struct apr_file_printf_data {
		apr_vformatter_buff_t vbuff;
		apr_file_t *fptr;
		char *buf;
	};

	static int file_printf_flush(apr_vformatter_buff_t *buff)
	{
		struct apr_file_printf_data *data = (struct apr_file_printf_data *)buff;

		if (apr_file_write_full(data->fptr, data->buf,
			data->vbuff.curpos - data->buf, NULL)) {
				return -1;
		}

		data->vbuff.curpos = data->buf;
		return 0;
	}

	int XFileInfo::Printf(const char *format, ...)
	{
		struct apr_file_printf_data data;
		va_list ap;
		int count;
		apr_file_t *fptr=m_fHandle;

		data.buf = (char*)malloc(HUGE_STRING_LEN);
		if (data.buf == NULL) {
			return 0;
		}
		data.vbuff.curpos = data.buf;
		data.vbuff.endpos = data.buf + HUGE_STRING_LEN;
		data.fptr = fptr;
		va_start(ap, format);
		count = apr_vformatter(file_printf_flush,
			(apr_vformatter_buff_t *)&data, format, ap);
		/* apr_vformatter does not call flush for the last bits */
		if (count >= 0) file_printf_flush((apr_vformatter_buff_t *)&data);

		va_end(ap);

		free(data.buf);
		return count;
	}
} // apr
} // xugd