
// FiveInARowDlg.h: 头文件
//

#pragma once
#include "CChessManager.h"
#include "FiveAI.h"
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据HHSJJ(
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void On32772();
};



// CFiveInARowDlg 对话框
class CFiveInARowDlg : public CDialogEx
{
// 构造
	CChessManager m_Manager;
	CFont m_FontTimer;
	CFont m_FontOver;
	int m_iTime;
	bool m_bState;
	COLOR AIColor;
	bool EnableRecording, EnableSound, EnableRepentance, EnableAI;
	HANDLE hProcess;
	FiveAI ai;
public:
	CFiveInARowDlg(CWnd* pParent = nullptr);	// 标准构造函数
	void StartFFmpegRecording();
	void TerminateFFmpegRecording();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FIVEINAROW_DIALOG };
#endif
	int AIDo(CChessManager& m_Manager, FiveAI& ai, COLOR& player);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	bool NewGame(int x, int y);
	bool About(int x, int y);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
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
	afx_msg void OnViewDetails();
	afx_msg void On32776();
	afx_msg void On32772();
	afx_msg void On32773();
};