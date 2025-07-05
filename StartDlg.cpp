// StartDlg.cpp: 实现文件
//

#include "pch.h"
#include "FiveInARow.h"
#include "afxdialogex.h"
#include "StartDlg.h"
#include "FiveInARowDlg.h"
#include "SettingDlg.h"
// StartDlg 对话框

IMPLEMENT_DYNAMIC(StartDlg, CDialogEx)

StartDlg::StartDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Start_DIALOG, pParent)
{
	this->m_static_font.CreatePointFont(300, _T("宋体"));
	this->m_button_font.CreatePointFont(100, _T("宋体"));	
}
BOOL StartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem(ID_TEXT1)->SetFont(&this->m_static_font);
	GetDlgItem(IDC_BUTTON1)->SetFont(&this->m_button_font);
	GetDlgItem(IDC_BUTTON2)->SetFont(&this->m_button_font);
	GetDlgItem(IDC_BUTTON3)->SetFont(&this->m_button_font);
	GetDlgItem(IDC_BUTTON4)->SetFont(&this->m_button_font);

	return TRUE;  // return TRUE unless you set the focus to a control
}
StartDlg::~StartDlg()
{
}

void StartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(StartDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &StartDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &StartDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &StartDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &StartDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


void StartDlg::OnBnClickedButton1() // 开始按钮 
{
	CFiveInARowDlg dlg;
	dlg.DoModal();
}


void StartDlg::OnBnClickedButton3()	// 关于按钮
{
	CAboutDlg dlg;
	dlg.DoModal();
}


void StartDlg::OnBnClickedButton2()	// 设置按钮
{
	SettingDlg dlg;
	dlg.DoModal();
}


void StartDlg::OnBnClickedButton4() // 退出按钮
{
	exit(0);
}
