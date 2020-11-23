
// MFCApplication1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

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
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
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
	CString strOutput;
	strOutput.Format(_T("要求-%d < A - B < %d"), n, m);
	SetDlgItemText(IDC_STATIC_1, strOutput);

	//初始化信号量
	sem_a = CreateSemaphore(NULL, m - 1, m + n - 2, NULL);
	sem_b = CreateSemaphore(NULL, n - 1, m + n - 2, NULL);
	sem_s = CreateSemaphore(NULL, 1, 1, NULL);

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

DWORD  WINAPI add_a(LPVOID lpParameter)
{
	CMFCApplication1Dlg* p = (CMFCApplication1Dlg*)lpParameter;
	//对仓库加锁，此时B不可以访问
	if (WaitForSingleObject(p->sem_s, 10) == WAIT_TIMEOUT) {
		CString strOutput;
		strOutput.Format(_T("正在使用仓库中，请稍后"));
		p->SetDlgItemText(IDC_STATIC_2, strOutput);
		return NULL;
	}
	//检查A是否有资源，有则存入一个，并暂停两秒模拟储存过程，并解锁仓库
	//没有则解锁仓库并返回
	if (WaitForSingleObject(p->sem_a, 10) == WAIT_TIMEOUT) {
		//此时没有a的资源，即a-b即将等于m，无法继续存放a
		CString strOutput;
		strOutput.Format(_T("不满足A-B<%d，不可以再存入A"), p->m);
		p->SetDlgItemText(IDC_STATIC_2, strOutput);
	}
	else {
		//存入a，暂停2秒，模拟存入A的时间
		//并给b增加一个资源
		CString strOutput;
		strOutput.Format(_T("正在存入A"));
		p->SetDlgItemText(IDC_STATIC_2, strOutput);
		p->number_a++;
		ReleaseSemaphore(p->sem_b, 1, NULL);
		Sleep(2000);
		strOutput.Format(_T(""));
		p->SetDlgItemText(IDC_STATIC_2, strOutput);
	}
	CString strOutput;
	strOutput.Format(_T("%d"), p->number_a);
	p->SetDlgItemText(IDC_STATIC_a, strOutput);
	//解锁
	ReleaseSemaphore(p->sem_s, 1, NULL);
	return NULL;
}

DWORD  WINAPI add_b(LPVOID lpParameter)
{
	CMFCApplication1Dlg* p = (CMFCApplication1Dlg*)lpParameter;
	//对仓库加锁，此时A不可以访问
	if (WaitForSingleObject(p->sem_s, 10) == WAIT_TIMEOUT) {
		CString strOutput;
		strOutput.Format(_T("正在使用仓库中，请稍后"));
		p->SetDlgItemText(IDC_STATIC_2, strOutput);
		return NULL;
	}
	//检查B是否有资源，有则存入一个，并暂停两秒模拟储存过程，并解锁仓库
	//没有则解锁仓库并返回
	if (WaitForSingleObject(p->sem_b, 10) == WAIT_TIMEOUT) {
		//此时没有b的资源，即b-a即将等于n，无法继续存放b
		CString strOutput;
		strOutput.Format(_T("不满足B-A<%d，不可以再存入B"), p->n);
		p->SetDlgItemText(IDC_STATIC_2, strOutput);
	}
	else {
		//存入b，暂停2秒，模拟存入B的时间
		//并给a增加一个资源
		CString strOutput;
		strOutput.Format(_T("正在存入B"));
		p->SetDlgItemText(IDC_STATIC_2, strOutput);
		p->number_b++;
		ReleaseSemaphore(p->sem_a, 1, NULL);
		Sleep(2000);
		strOutput.Format(_T(""));
		p->SetDlgItemText(IDC_STATIC_2, strOutput);
	}

	CString strOutput;
	strOutput.Format(_T("%d"), p->number_b);
	p->SetDlgItemText(IDC_STATIC_b, strOutput);
	//解锁
	ReleaseSemaphore(p->sem_s, 1, NULL);
	return NULL;
}

void CMFCApplication1Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD dwThreadId = 0; //线程id
	CreateThread(NULL, 0, add_a, this, 0, &dwThreadId); //返回线程句柄
}


void CMFCApplication1Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD dwThreadId = 1000; //线程id
	CreateThread(NULL, 0, add_b, this, 0, &dwThreadId); //返回线程句柄
}