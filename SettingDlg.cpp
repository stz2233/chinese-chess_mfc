// SettingDlg.cpp: 实现文件
//

#include "pch.h"
#include "FiveInARow.h"
#include "afxdialogex.h"
#include "SettingDlg.h"


// SettingDlg 对话框

IMPLEMENT_DYNAMIC(SettingDlg, CDialogEx)

SettingDlg::SettingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTING_DIALOG, pParent)
{
}

SettingDlg::~SettingDlg()
{
}

void SettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK2, CCheck2);
	DDX_Control(pDX, IDC_CHECK3, CCheck3);
	DDX_Control(pDX, IDC_CHECK4, CCheck4);
	DDX_Control(pDX, IDC_COMBO1, CCombo1);
	DDX_Control(pDX, IDC_COMBO2, CCombo2);
}

BOOL SettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CCombo1.AddString(_T("人机对战"));
	CCombo1.AddString(_T("人人对战"));
	//设置字体
	CCombo2.AddString(_T("白"));
	CCombo2.AddString(_T("黑"));
	this->m_font_1.CreatePointFont(300, _T("宋体"));
	this->m_font_2.CreatePointFont(100, _T("宋体"));
	//读取setting.json文件
	try {
		std::ifstream i("setting.json");
		json j = json::parse(i);
		this->ModeOfBattle = CString(std::string(j["ModeOfBattle"]).c_str());
		this->AIColor = CString(std::string(j["AIColor"]).c_str());
		this->EnableRecording = j["EnableRecording"];
		this->EnableRepentant = j["EnableRepentant"];
		this->EnableSound = j["EnableSound"];
		CCheck2.SetCheck(this->EnableRecording);
		CCheck3.SetCheck(this->EnableRepentant);
		CCheck4.SetCheck(this->EnableSound);
		if (this->ModeOfBattle == "man")
			CCombo1.SetCurSel(1);
		else
			CCombo1.SetCurSel(0);
		if (this->AIColor == "white")
			CCombo2.SetCurSel(0);
		else
			CCombo2.SetCurSel(1);
	}
	catch (const std::exception& e) {
		MessageBox(_T("读取配置文件失败，将重新建立配置文件！"));
	}
	return true;
}

BEGIN_MESSAGE_MAP(SettingDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &SettingDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON1, &SettingDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON5, &SettingDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// SettingDlg 消息处理程序


void SettingDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void SettingDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	// 获取控件状态，重新写入setting.json文件
	try {
		this->EnableRecording = CCheck2.GetCheck();
		this->EnableRepentant = CCheck3.GetCheck();
		this->EnableSound = CCheck4.GetCheck();
		std::string temp = CCombo1.GetCurSel() == 0 ? "ai" : "man";
		std::string c = CCombo2.GetCurSel() == 0 ? "white" : "black";
		json j;
		j["ModeOfBattle"] = temp;
		j["AIColor"] = c;
		j["EnableRecording"] = this->EnableRecording;
		j["EnableRepentant"] = this->EnableRepentant;
		j["EnableSound"] = this->EnableSound;
		std::ofstream o("setting.json");
		o << std::setw(4) << j << std::endl;
		this->OnOK();
	}
	catch (const std::exception& e) {
		MessageBox(_T("配置文件保存失败！"));
	}
}


void SettingDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	// 显示未保存，是否返回？
	int nResult = MessageBox(_T("未保存，是否退出？"), _T("警告"), MB_YESNOCANCEL | MB_ICONWARNING);
	switch (nResult)
	{
	case IDYES:
		this->OnOK();
		break;
	case IDNO:
		break;
	case IDCANCEL:
		break;
	};
}