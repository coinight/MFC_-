
// MFCApplication1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#include <stdlib.h>
#include <conio.h>
#include <WinUser.h>
#include "Tchar.h"

#define log(fmt, ...) \
CString str; \
str.Format(CString(fmt), __VA_ARGS__); \
AfxMessageBox(str);

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CMFCApplication1Dlg* g_dlg;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

void unbool(bool &data) {
	if (data == 0) data = 1;
	else data = 0;
}

DWORD _stdcall monitorThread_mouse(LPVOID lpThreadParameter) {
	while (1) {
		POINT p_mos;
		bool m_return = ::GetCursorPos(&p_mos);
		if (m_return != 0) {
			//TRACE("x=%d,y=%d\n",p_mos.x,p_mos.y);
			if(g_dlg->bind_mos)mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
		else
		{
			TRACE("HELLO\n");
		}
		Sleep(100);
	}
	return NULL;
}

DWORD _stdcall monitorThread_keyboard(LPVOID lpThreadParameter) {
	auto* pEdit = GetDlgItem(g_dlg->m_hWnd,IDC_EDIT2);
	TCHAR chr1[10];
	CString str1;
	bool keydownF8 = false;
	bool keydownF9 = false;

	while (1) {
		GetWindowText(pEdit, chr1, 10);
		str1.Format(_T("%s"), chr1);
		char* p = (LPSTR)(LPCTSTR)chr1;

		if (GetAsyncKeyState(VK_F8)) {
			
			
			keydownF8 = true;
			TRACE("F8\n");
			
		}

		if (GetAsyncKeyState(VK_F9)) {
			TRACE("F9\n");
			
			keydownF9 = true;

		}

		if (keydownF8 == true && !GetAsyncKeyState(VK_F8)) {
			keydownF8 = false;
			unbool(g_dlg->bind_mos);
		}

		if (keydownF9 == true && !GetAsyncKeyState(VK_F9)) {
			keydownF9 = false;
			unbool(g_dlg->bind_key);
		}

		if (strlen(p) != 0) {
			g_dlg->key = p; 
			
		}
		if (g_dlg->bind_key && g_dlg->key != nullptr) {
			
			keybd_event(*g_dlg->key + 0, 0, 0, 0);
			keybd_event(*g_dlg->key + 0, 0, KEYEVENTF_KEYUP, 0);
		}
		
		//TRACE("char = %s\n", g_dlg->key);
		Sleep(50);
	}
	return NULL;
}
DWORD _stdcall monitorThread_file(LPVOID lpThreadParameter) {
	//FILE* fp = nullptr;
	//char text_255[255];
	//errno_t err;
	//err = fopen_s(&fp,"./data.txt", "a");
	//
	//fputs("Hahaha", fp);
	//fclose(fp);
	return NULL;
}
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CMFCApplication1Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_CHECK3, &CMFCApplication1Dlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK2, &CMFCApplication1Dlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK1, &CMFCApplication1Dlg::OnBnClickedCheck1)
	ON_EN_CHANGE(IDC_EDIT2, &CMFCApplication1Dlg::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	g_dlg = this;

	m_monitorThread_mouse = CreateThread(NULL, NULL, monitorThread_mouse, NULL, NULL, NULL);

	m_monitorThread_keyboard = CreateThread(NULL, NULL, monitorThread_keyboard, NULL, NULL, NULL);

	m_monitorThread_file = CreateThread(NULL, NULL, monitorThread_file, NULL, NULL, NULL);

	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
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
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1Dlg::OnEnChangeEdit1()
{
	TRACE("????????\n");
}


void CMFCApplication1Dlg::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMFCApplication1Dlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMFCApplication1Dlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMFCApplication1Dlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
