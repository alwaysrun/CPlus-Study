
// FormTestDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "xugd.xmpp.h"

// CFormTestDlg 对话框
class CFormTestDlg : public CDialogEx
{
	// 构造
public:
	CFormTestDlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_FORMTEST_DIALOG };

	std::string m_strUserJid;
	void CheckResult(xugd::xmpp::XmppResult_t euResult_, CString strInfo_);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strMsg;
	CListCtrl m_ctlListInfo;
	CString m_strName;
	CString m_strIp;
	CString m_strPsw;
	CString m_strFriend;
	CButton m_chkLogFile;
	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnListclear();
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnSubfriend();
	afx_msg void OnBnClickedBtnLoginstart();
	afx_msg void OnBnClickedBtnLoginstop();
	afx_msg void OnBnClickedBtnStop();
};
