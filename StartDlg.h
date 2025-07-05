#pragma once
#include "afxdialogex.h"

// StartDlg 对话框

class StartDlg : public CDialogEx
{
	DECLARE_DYNAMIC(StartDlg)
private:
	CFont m_static_font,m_button_font;
public:
	StartDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~StartDlg();
	BOOL OnInitDialog();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Start_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
};
