
// MFCApplication1Dlg.h: 头文件
//

#pragma once



// CMFCApplication1Dlg 对话框
class CMFCApplication1Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 标准构造函数

	bool bind_mos = 0;
	bool bind_key = 0;
	char* key = nullptr;
	HANDLE m_monitorThread_keyboard;
	HANDLE m_monitorThread_mouse;
	HANDLE m_monitorThread_file;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	friend DWORD _stdcall monitorThread_mouse(LPVOID lpThreadParameter);
	friend BOOL CMFCApplication1Dlg::OnInitDialog();
	HICON m_hIcon;

	

	

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnEnChangeEdit2();
};
