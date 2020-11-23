
// MFCApplication1Dlg.h: 头文件
//

#pragma once
#include <thread>
// CMFCApplication1Dlg 对话框
class CMFCApplication1Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

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
	HANDLE th_a;
	HANDLE th_b;
public:
	//仓库ab中货物的数量
	int number_a = 0;
	int number_b = 0;
	//在这里修改初始的m和n
	static const int m = 4;
	static const int n = 3;
	//ab和仓库的信号量
	HANDLE sem_a = NULL;
	HANDLE sem_b = NULL;
	HANDLE sem_s = NULL;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	
};
//线程及API
DWORD  WINAPI add_a(LPVOID lpParameter);
DWORD  WINAPI add_b(LPVOID lpParameter);