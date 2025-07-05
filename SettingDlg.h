#pragma once
#include "afxdialogex.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
using json = nlohmann::json;

// SettingDlg 对话框

class SettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SettingDlg)
private:
	CFont m_font_1, m_font_2;
public:
	CString ModeOfBattle, AIColor;
	bool EnableRecording, EnableSound, EnableRepentant;
	BOOL OnInitDialog();
	SettingDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SettingDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListBox CList1;
	// CCheck2
	CButton CCheck2;
	// CCheck3
	CButton CCheck3;
	// CCheck4
	CButton CCheck4;
	afx_msg void OnCbnSelchangeCombo1();
	// CCombo1
	CComboBox CCombo1;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton5();
	CComboBox CCombo2;
};
