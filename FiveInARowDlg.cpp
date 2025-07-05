
// FiveInARowDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "FiveInARow.h"
#include "FiveInARowDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <filesystem>
#include <mmsystem.h>
#include <Windows.h>
#pragma comment(lib,"WINMM.LIB")
namespace fs = std::filesystem;
using json = nlohmann::json;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
void CFiveInARowDlg::StartFFmpegRecording() {	// 开始录屏
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	PROCESS_INFORMATION pi;
	TCHAR cmd[] = _T("ffmpeg.exe -y -f gdigrab -framerate 15 -i desktop -vcodec libx264 -preset ultrafast output.mkv");

	if (!CreateProcess(NULL,   // 没有模块名，使用命令行
		cmd,       // 命令行
		NULL,NULL,FALSE,CREATE_NO_WINDOW, // 不创建窗口
		NULL,NULL,&si,&pi)) {
	}
	else {
		this->hProcess = pi.hProcess;
		// 保存pi，以便以后可以用来终止FFmpeg进程
		CloseHandle(pi.hThread);
	}
	Sleep(1000);
}
void CFiveInARowDlg::TerminateFFmpegRecording() { // 停止录屏
	if (this->hProcess != NULL) {
		TerminateProcess(this->hProcess, 0); // 终止FFmpeg进程
		this->hProcess = NULL; // 重置句柄变量
	}
}
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
void CAboutDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
	ON_COMMAND(ID_32772, &CAboutDlg::On32772)
END_MESSAGE_MAP()

// CFiveInARowDlg 对话框



CFiveInARowDlg::CFiveInARowDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FIVEINAROW_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFiveInARowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFiveInARowDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_COMMAND(ID_VIEW_DETAILS, &CFiveInARowDlg::OnViewDetails)
	ON_COMMAND(ID_32776, &CFiveInARowDlg::On32776)
	ON_COMMAND(ID_32772, &CFiveInARowDlg::On32772)
	ON_COMMAND(ID_32773, &CFiveInARowDlg::On32773)
END_MESSAGE_MAP()


// CFiveInARowDlg 消息处理程序   

BOOL CFiveInARowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	this->hProcess = NULL;
	// 加载目录
	CMenu* pMenu = new CMenu();
	pMenu->LoadMenu(IDR_MENU1);
	this->SetMenu(pMenu);

	// 读取配置文件
	try
	{
		std::ifstream ifs("setting.json");
		json j = json::parse(ifs);
		this->EnableSound = j["EnableSound"];
		this->EnableRecording = j["EnableRecording"];
		this->EnableRepentance = j["EnableRepentant"];
		this->AIColor = j["AIColor"] == "white" ? WHITE : BLACK;
		if (j["ModeOfBattle"] == "man")
			this->EnableAI = false;
		else
			this->EnableAI = true;
		ifs.close();
		//根据配置文件修改菜单
		pMenu = GetMenu();
		if (!this->EnableRepentance)
			pMenu->EnableMenuItem(MF_BYCOMMAND | ID_VIEW_DETAILS, MF_DISABLED | MF_GRAYED);
	}
	catch (const std::exception& e)
	{
		// 如果配置文件不存在，创建一个默认的配置文件
		AfxMessageBox(_T("加载配置文件失败!"));
		this->OnOK();
	}


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
	SetWindowPos(NULL, 0, 0, 1024, 768, SWP_NOZORDER | SWP_NOMOVE);
	m_FontTimer.CreatePointFont(250, _T("Segoe UI Semibold"), NULL);
	m_FontOver.CreatePointFont(1666, _T("微软雅黑"), NULL);
	m_bState = false;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFiveInARowDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
		if ((nID & 0xFFF0) == SC_MOVE)
			PostMessage(WM_NCLBUTTONUP, nID, lParam);
	}
}
int CFiveInARowDlg::AIDo(CChessManager& m_Manager, FiveAI& ai, COLOR& player)
{
	std::pair<int, int> p;
	p = ai.findBestMove(m_Manager, player);
	if (p.first == -1 || p.second == -1)
		return 1;
	return m_Manager.Add_Org(p.first, p.second);
}
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFiveInARowDlg::OnPaint()
{

	CPaintDC dc(this);// 用于绘制的设备上下文
	if (IsIconic())
	{
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

	CBitmap bmp;
	BITMAP bm;
	bmp.LoadBitmap(IDB_BITMAP_BK);
	bmp.GetObject(sizeof(BITMAP), &bm);
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	CBitmap* pOldBitmap = MemDC.SelectObject(&bmp);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBitmap);
	m_Manager.Show(&dc);
	AfxMessageBox(_T("请选择“开始”按钮开始新的游戏，按Esc键退出游戏！"));
	CDialog::OnPaint();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFiveInARowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CFiveInARowDlg::OnLButtonDown(UINT nFlags, CPoint point) {
	if (NewGame(point.x, point.y))
	{
		if (this->EnableAI && this->AIColor == m_Manager.GetCurrentColor())
		{
			if (this->AIDo(m_Manager,this->ai,AIColor)) {
				MessageBox(_T("AI落子失败"));
			}
			else {
				if (this->EnableSound)
					PlaySound(MAKEINTRESOURCE(IDR_WAVE2), AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT);
				else
					PlaySound(NULL, AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT);
				CClientDC dc(this);
				m_Manager.Show(&dc);
			}
			return;
		}
	}
	if (About(point.x, point.y))
		return;
	if (!m_bState) {
		AfxMessageBox(_T("请选择“开始”按钮开始新的游戏，按Esc键退出游戏！"));
		return;
	}

	int r = m_Manager.Add(point.x, point.y);
	if (r == 0) {
		if (this->EnableSound)
			PlaySound(MAKEINTRESOURCE(IDR_WAVE2), AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT);
		else
			PlaySound(NULL, AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT);
		CClientDC dc(this);
		m_Manager.Show(&dc);
		if (m_Manager.GameOver()) {
			if (this->EnableRecording)
			{
				MessageBox(_T("正在停止录制..."));
				Sleep(4000);
				this->TerminateFFmpegRecording();
			}
			KillTimer(1);
			KillTimer(2);
			CString csTemp;
			if (m_Manager.GetWinner() == WHITE)
			{
				csTemp.Format(_T("白方胜!"));
			}
			else
			{
				csTemp.Format(_T("黑方胜！"));
			}
			m_bState = false;
			CClientDC dc(this);
			CFont* pOldFont = dc.SelectObject(&m_FontOver);
			int OldBkMode = dc.GetBkMode();
			COLORREF OldColor, NewColor1 = RGB(60, 60, 60), NewColor2 = RGB(250, 50, 50);
			dc.SetBkMode(TRANSPARENT);
			OldColor = dc.SetTextColor(NewColor1);
			dc.TextOut(0, 0, csTemp);
			dc.SetTextColor(NewColor2);
			dc.TextOut(0, 0, csTemp);
			dc.SetTextColor(OldColor);
			dc.SetBkMode(OldBkMode);
			dc.SelectObject(pOldFont);
			return;
		}
	}
	if (r == 1)
		AfxMessageBox(_T("请在棋盘交叉点落子！"));
	else if (r == 2)
		AfxMessageBox(_T("不可以重复落子！"));
	if (this->EnableAI && this->AIColor == m_Manager.GetCurrentColor())
	{
		if (this->AIDo(m_Manager, this->ai, this->AIColor)) {
			MessageBox(_T("AI落子失败"));
		}
		else {
			if (this->EnableSound)
				PlaySound(MAKEINTRESOURCE(IDR_WAVE2), AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT);
			else
				PlaySound(NULL, AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT);
			CClientDC dc(this);
			m_Manager.Show(&dc);
			if (m_Manager.GameOver()) {
				if (this->EnableRecording)
				{
					MessageBox(_T("正在停止录制..."));
					Sleep(4000);
					this->TerminateFFmpegRecording();
				}
				KillTimer(1);
				KillTimer(2);
				CString csTemp;
				if (m_Manager.GetWinner() == WHITE)
				{
					csTemp.Format(_T("白方胜!"));
				}
				else
				{
					csTemp.Format(_T("黑方胜！"));
				}
				m_bState = false;
				CClientDC dc(this);
				CFont* pOldFont = dc.SelectObject(&m_FontOver);
				int OldBkMode = dc.GetBkMode();
				COLORREF OldColor, NewColor1 = RGB(60, 60, 60), NewColor2 = RGB(250, 50, 50);
				dc.SetBkMode(TRANSPARENT);
				OldColor = dc.SetTextColor(NewColor1);
				dc.TextOut(0, 0, csTemp);
				dc.SetTextColor(NewColor2);
				dc.TextOut(0, 0, csTemp);
				dc.SetTextColor(OldColor);
				dc.SetBkMode(OldBkMode);
				dc.SelectObject(pOldFont);
				return;
			}
		}
		return;
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}
bool CFiveInARowDlg::NewGame(int x, int y) {
	int x0 = 35, y0 = 150, x1 = 200, y1 = 185;
	if ((x >= x0 && x <= x1) && (y >= y0 && y <= y1)) {
		try
		{
			if (this->EnableRecording)
				this->StartFFmpegRecording();
		}
		catch (const std::exception& e){
			MessageBox(_T("启动录制失败！"));
		}
		m_Manager.NewGame();
		Invalidate();
		m_iTime = 0;
		SetTimer(1, 1000, NULL);
		SetTimer(2, 500, NULL);
		m_bState = true;
		return true;
	}
	return false;
}
bool CFiveInARowDlg::About(int x, int y) {
	int x0 = 35, y0 = 70, x1 = 200, y1 = 95;
	if ((x >= x0 && x <= x1) && (y >= y0 && y <= y1)) {
		CAboutDlg dlg;
		dlg.DoModal();
		return true;
	}
	return false;
}

void CFiveInARowDlg::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent) {
	case 1: {
		CClientDC dc(this);
		CFont* pOldFont;
		pOldFont = dc.SelectObject(&m_FontTimer);
		m_iTime++;
		CString csTemp;
		csTemp.Format(_T("%04d "), m_iTime);
		COLORREF OldColor, NewColor = RGB(150, 50, 50);
		OldColor = dc.SetTextColor(NewColor);
		dc.TextOutW(725, 20, csTemp);
		dc.SetTextColor(OldColor);
		dc.SelectObject(pOldFont);
		break;
	}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CFiveInARowDlg::OnViewDetails()
{
	// TODO: 在此添加命令处理程序代码
	this->m_Manager.Repentance();
	Invalidate();		// 重新绘制整个窗口
}


void CFiveInARowDlg::On32776()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
	// TODO: 在此添加命令处理程序代码
}


void CAboutDlg::On32772()
{
	CFileDialog FileDlg(FALSE);
	FileDlg.m_ofn.lpstrFilter = _T("所有文件(*.*)\0*.*\0\0");
	if (FileDlg.DoModal() == IDOK) {
		CString csPath = FileDlg.GetPathName();
		std::ifstream ifs(csPath);
		json j = json::parse(ifs);
		ifs.close();
	}
	// TODO: 在此添加命令处理程序代码
}


void CFiveInARowDlg::On32773()
{

	if (!this->m_bState)
	{
		CFileDialog FileDlg(TRUE);
		FileDlg.m_ofn.lpstrFilter = _T("所有文件(*.*)\0*.*\0\0");
		if (FileDlg.DoModal() == IDOK) {
			CString csPath = FileDlg.GetPathName();
			if (this->m_Manager.LoadArchives(csPath))
			{
				MessageBox(_T("加载存档成功"));
				Invalidate();
				m_iTime = 0;
				SetTimer(1, 1000, NULL);
				SetTimer(2, 500, NULL);
				m_bState = true;
			}
			else {
				MessageBox(_T("存档文件有误 加载存档失败"));
			}
		}
	}
	else {
		MessageBox(_T("游戏正在进行中，无法加载存档"));
	}
	// TODO: 在此添加命令处理程序代码
}


void CFiveInARowDlg::On32772() // 保存
{
	CFileDialog FileDlg(FALSE);
	FileDlg.m_ofn.lpstrFilter = _T("所有文件(*.*)\0*.*\0\0");
	if (FileDlg.DoModal() == IDOK) {
		CString csPath = FileDlg.GetPathName();
		if (this->m_Manager.SaveArchives(csPath))
		{
			MessageBox(_T("保存存档成功"));
			Invalidate();
		}
		else {
			MessageBox(_T("保存存档失败"));
		}
	}
	// TODO: 在此添加命令处理程序代码
}
