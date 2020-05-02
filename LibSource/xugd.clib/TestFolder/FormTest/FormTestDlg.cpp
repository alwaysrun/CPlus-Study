
// FormTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FormTest.h"
#include "FormTestDlg.h"
#include "afxdialogex.h"
#include "FrmConfig.h"

#include "xugd.time.h"
#include "xugd.thread.h"
#include "xugd.xmpp.h"
#include "xugd.random.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/format.hpp>
#include <boost/smart_ptr.hpp>
#include "xugd.convert.h"

#ifdef _DEBUG
#pragma comment(lib, "xugd.clib_d.lib")
#pragma comment(lib, "xugd.xmpp_d.lib")
#else
#pragma comment(lib, "xugd.clib.lib")
#pragma comment(lib, "xugd.xmpp.lib")
#endif

using namespace xugd::clib;
using namespace xugd::xmpp;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFormTestDlg 对话框




CFormTestDlg::CFormTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFormTestDlg::IDD, pParent)
	, m_strMsg(_T(""))
	, m_strName(_T(""))
	, m_strIp(_T(""))
	, m_strPsw(_T(""))
	, m_strFriend(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFormTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFO, m_ctlListInfo);
	DDX_Text(pDX, IDC_MSG_NAME, m_strName);
	DDX_Text(pDX, IDC_MSG_IP, m_strIp);
	DDX_Text(pDX, IDC_MSG_IP2, m_strPsw);
	DDX_Text(pDX, IDC_MSG_NAME2, m_strFriend);
	DDX_Control(pDX, IDC_CHK_LOGFILE, m_chkLogFile);
}

BEGIN_MESSAGE_MAP(CFormTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SEND, &CFormTestDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CFormTestDlg::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CFormTestDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_LISTCLEAR, &CFormTestDlg::OnBnClickedBtnListclear)
	ON_BN_CLICKED(IDC_BTN_SUBFRIEND, &CFormTestDlg::OnBnClickedBtnSubfriend)
	ON_BN_CLICKED(IDC_BTN_LOGINSTART, &CFormTestDlg::OnBnClickedBtnLoginstart)
	ON_BN_CLICKED(IDC_BTN_LOGINSTOP, &CFormTestDlg::OnBnClickedBtnLoginstop)
	ON_BN_CLICKED(IDC_BTN_STOP, &CFormTestDlg::OnBnClickedBtnStop)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
CFormTestDlg *g_pDlgThis=NULL;
UserInfoConfig g_userInfo;

void ShowInfo(const wchar_t *strInfo_){
	int nCount = g_pDlgThis->m_ctlListInfo.GetItemCount();
	g_pDlgThis->m_ctlListInfo.InsertItem(nCount, strInfo_);
	g_pDlgThis->m_ctlListInfo.EnsureVisible(nCount, FALSE);
}

void To_ShowInfo(const wstring &strInfo_){
	ShowInfo(strInfo_.c_str());
}

BOOL CFormTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	g_pDlgThis = this;
	m_ctlListInfo.InsertColumn(0, L"Info",  LVCFMT_LEFT, 1600);

	g_userInfo.Read(L"FormGloox.xml");
	m_strName = g_userInfo.strName.c_str();
	m_strPsw = g_userInfo.strPsw.c_str();
	m_strIp = g_userInfo.strAddr.c_str();
	m_strFriend = g_userInfo.strFriend.c_str();

	UpdateData(FALSE);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFormTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFormTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFormTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool g_bSend = false;

void SendMsgTest(void *pParam_)
{
	int nTest = 1;
	string strFriend = XConvert::Wstr2A((LPWSTR)(LPCWSTR)g_pDlgThis->m_strFriend);
	string strSelf = g_pDlgThis->m_strUserJid;
	char strMsg[100];

	while(g_bSend)
	{
		sprintf_s(strMsg, 100, "#%d-th Msg From %s", nTest++, strSelf.c_str());
		g_pDlgThis->CheckResult(XmppSendMsg(strSelf.c_str(), strFriend.c_str(), strMsg), L"Send Msg");

		XThread::Sleep(1000);	// 1 second
	}
}

void CFormTestDlg::OnBnClickedBtnSend()
{
	if(m_strUserJid.empty()){
		MessageBox(L"Login first");
		return;
	}
	if(m_strFriend.IsEmpty()){
		MessageBox(L"Input friend first");
		return;
	}

	if(!g_bSend)
	{
		g_bSend = true;
		XThread::Start(SendMsgTest, NULL);
	}
}


void CFormTestDlg::OnBnClickedBtnStop()
{
	g_bSend = false;
}


void CFormTestDlg::CheckResult(XmppResult_t euResult_, CString strInfo_){
	if(euResult_ != XmppRes_Success){
		CString str;
		str.Format(L"%s failed: %d", strInfo_, (int)euResult_);
		MessageBox(str);
	}
}

void TryLogin(void *pParam_){
	int nChecked = ((int)(pParam_));
	string strName = XConvert::Wstr2A((LPWSTR)(LPCWSTR)g_pDlgThis->m_strName);
	string strPsw = XConvert::Wstr2A((LPWSTR)(LPCWSTR)g_pDlgThis->m_strPsw);
	string strIp = XConvert::Wstr2A((LPWSTR)(LPCWSTR)g_pDlgThis->m_strIp);
	XmppResult_t euResult = XmppLoginDebug(strName.c_str(), strPsw.c_str(), strIp.c_str(), 5222, L"log\\{yyyy}\\{MM}\\Xmpp{dd}.log", To_ShowInfo, BST_CHECKED!=nChecked);
	// XmppResult_t euResult = XmppLoginDebug(strName.c_str(), strPsw.c_str(), strIp.c_str(), 5222, L"Test.log", NULL, false);
	g_pDlgThis->CheckResult(euResult, L"Login");
	To_ShowInfo(L"");
	if(euResult == XmppRes_Success || euResult == XmppRes_HasLogin)
		g_pDlgThis->m_strUserJid = strName;
}

void CFormTestDlg::OnBnClickedBtnLogin()
{
	UpdateData();
	if(m_strName.IsEmpty() || m_strPsw.IsEmpty() || m_strIp.IsEmpty()){
		MessageBox(L"Input login-user info");
		return;
	}

	int nCheck = m_chkLogFile.GetCheck();

	XThread::Start(TryLogin, (void *)nCheck);
	
	SetWindowText(m_strName);
}

BOOL g_bStop = FALSE;
void LoginTest(void *pParam_){
	string strName = XConvert::Wstr2A((LPWSTR)(LPCWSTR)g_pDlgThis->m_strName);
	string strPsw = XConvert::Wstr2A((LPWSTR)(LPCWSTR)g_pDlgThis->m_strPsw);
	string strIp = XConvert::Wstr2A((LPWSTR)(LPCWSTR)g_pDlgThis->m_strIp);

	int nTime = 0;
	while(!g_bStop){
		CString str;
		str.Format(L"--- %d ---", ++nTime);
		To_ShowInfo((LPWSTR)(LPCWSTR)str);

		XmppResult_t euResult = XmppLogin(strName.c_str(), strPsw.c_str(), strIp.c_str(), 5222, L"Log\\{yyyy}\\{MM}\\xmppTest{dd}.log");
		g_pDlgThis->CheckResult(euResult, L"Login");
		if(euResult == XmppRes_Success || euResult == XmppRes_HasLogin){
			g_pDlgThis->m_strUserJid = strName;
			To_ShowInfo(L"Login success");
		}

		XThread::Sleep(0, 0, XRandom::GetInt(5, 10));

		To_ShowInfo(L"To Stop");
		XmppClose(g_pDlgThis->m_strUserJid.c_str());
		g_pDlgThis->m_strUserJid.clear();
		To_ShowInfo(L"Stopped");
		To_ShowInfo(L"");
		XThread::Sleep(0, 0, XRandom::GetInt(2,6));
	}

	To_ShowInfo(L"");
	To_ShowInfo(L"LoginTest finished");
	To_ShowInfo(L"");
}

void TryClose(void *pParam_){
	XmppClose(g_pDlgThis->m_strUserJid.c_str());
	g_pDlgThis->m_strUserJid.clear();
	To_ShowInfo(L"");
}


void CFormTestDlg::OnBnClickedBtnClose()
{
	if(m_strUserJid.empty()){
		MessageBox(L"Login first");
		return;
	}

	XThread::Start(TryClose, NULL);
}

void CFormTestDlg::OnBnClickedBtnListclear()
{
	m_ctlListInfo.DeleteAllItems();
}

void CFormTestDlg::OnBnClickedBtnSubfriend()
{
	if(m_strUserJid.empty()){
		MessageBox(L"Login first");
		return;
	}
	if(m_strFriend.IsEmpty()){
		MessageBox(L"Input friend first");
		return;
	}
	string strFriend = XConvert::Wstr2A((LPWSTR)(LPCWSTR)m_strFriend);
	CheckResult(XmppSubscribe(m_strUserJid.c_str(), strFriend.c_str()), L"Subscribe");
}

void CFormTestDlg::OnBnClickedBtnLoginstart()
{
	g_bStop = FALSE;
	XThread::Start(LoginTest, NULL);
}


void CFormTestDlg::OnBnClickedBtnLoginstop()
{
	g_bStop = TRUE;
}
