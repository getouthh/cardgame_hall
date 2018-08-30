// PPCLogIn.cpp : 实现文件
//

#include "stdafx.h"
#include "GamePlaza.h"
#include "PPCLogIn.h"
#include "afxdialogex.h"
#include "LastLogusers.h"
#include "include\md5class.h"
#include "PPCSqlite.h"
#include "GlobalUnits.h"
#include "PCMac.h"
// CPPCLogIn 对话框
static HANDLE mutexHandle = 0;
IMPLEMENT_DYNAMIC(CPPCLogIn, CSkinDialogEx)

CPPCLogIn::CPPCLogIn(CWnd* pParent /*=NULL*/)
	: CSkinDialogEx(CPPCLogIn::IDD, pParent)
{
	m_curuin = 0;
	m_curpwdtext = "";
	m_bChangePWD = false;
}

CPPCLogIn::~CPPCLogIn()
{
}

void CPPCLogIn::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_COMBO1, m_usercombox);
	DDX_Control(pDX, IDC_EDIT_PWD, m_editpwd);
	DDX_Control(pDX, IDC_EDIT3, m_edtserver);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_btnDelUser);
	DDX_Control(pDX, IDC_CHECK1, m_bRemPWD);
}


BEGIN_MESSAGE_MAP(CPPCLogIn, CSkinDialogEx)
	ON_BN_CLICKED(IDOK, &CPPCLogIn::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CPPCLogIn::OnBnClickedButtonDel)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CPPCLogIn::OnCbnSelchangeCombo1)
	ON_EN_CHANGE(IDC_EDIT_PWD, &CPPCLogIn::OnEnChangeEditPwd)
	ON_CBN_SELENDCANCEL(IDC_COMBO1, &CPPCLogIn::OnCbnSelendcancelCombo1)
	ON_BN_CLICKED(IDC_CHECK1, &CPPCLogIn::OnBnClickedCheck1)
	ON_CBN_EDITCHANGE(IDC_COMBO1, &CPPCLogIn::OnCbnEditchangeCombo1)
	ON_BN_CLICKED(IDCANCEL, &CPPCLogIn::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPPCLogIn 消息处理程序

void LoadUserList()
{

}
void CPPCLogIn::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString uintext;
	m_usercombox.GetWindowText(uintext);
	
    bool brempwd = m_bRemPWD.GetCheck();
	CString pwdstr;
	string md5pwd;
	m_editpwd.GetWindowText(pwdstr);
	CString serverstr ;
	m_edtserver.GetWindowText(serverstr);
	if (uintext.GetLength() == 0)
	{
		MessageBox("帐号不能为空","提示",MB_OK);
		return;
	}

	
	

	if (pwdstr.GetLength() == 0)
	{
		MessageBox("密码不能为空","提示",MB_OK);
		return;
	}
	if (serverstr.GetLength() == 0)
	{
		MessageBox("服务器不能为空","提示",MB_OK);
		return;
	}
	CPPCSqlite sqlite;
	CLastLogusers::addUser(atoi(uintext));
	if (m_bChangePWD)
	{
		md5pwd = CMD5::MD5(pwdstr).c_str();
		m_curpwdtext = md5pwd;
		if (m_bRemPWD)
		{
			sqlite.SaveUser(atoi(uintext),md5pwd.c_str(),brempwd?1:0);
		}	
	}
	m_curuin = atoi(uintext);
	int errocode = SetAppParames();
	if (-1 == errocode)
	{
		MessageBox("请写正确的服务器地址");
		return;
	}
	else if (-2 == errocode)
	{
		MessageBox("已经有同帐号在运行，你退出原来的帐号");
		return;
	}
	CString serstr;
	m_edtserver.GetWindowText(serstr);
	sqlite.SaveServer(serstr);
	__super::OnOK();
	
	//uint32 uin = ;
}


void CPPCLogIn::OnBnClickedButtonDel()
{
	int ncur = m_usercombox.GetCurSel();
	if (ncur >=0)
	{
		CString uintext ;
		m_usercombox.GetLBText(ncur,uintext);
		CLastLogusers::delUser(atoi(uintext));
		CPPCSqlite sqlite;
		sqlite.DelUser(atoi(uintext));
		m_editpwd.SetWindowText("");
		m_usercombox.DeleteString(ncur);
		
	}
	if (m_usercombox.GetCount() >0)
	{
		m_usercombox.SetCurSel(0);
		CPPCSqlite sqllist ;
		CString uintext;
		m_usercombox.GetLBText(m_usercombox.GetCurSel(),uintext);
		if (uintext.GetLength()>0)
		{
			UserLoginItem item;
			sqllist.GetUser(atoi(uintext),item);
			m_curuin =atoi(uintext);
			if (item.m_flag == 1)
			{
				m_bRemPWD.SetCheck(1);
				m_editpwd.SetWindowText("*******");
				m_bChangePWD = false;
				m_curpwdtext = item.m_pwd;
			}
			else
			{
				m_bRemPWD.SetCheck(0);
				m_editpwd.SetWindowText("");
				m_bChangePWD = false;
				m_curpwdtext = "";
			}
		}

	}
	else
		m_usercombox.SetCurSel(-1);
}

BOOL CPPCLogIn::OnInitDialog()
{
	__super::OnInitDialog();
	list<uint32> uinlist;
	CLastLogusers::getAllUsers(uinlist);
	for (list<uint32>::iterator it = uinlist.begin();it!=uinlist.end();it++)
	{
		CString uinstr;
		uinstr.Format("%lu",*it);
		m_usercombox.AddString(uinstr);
	}
	CPPCSqlite sqllist ;
	if (!uinlist.empty())
	{	
		m_usercombox.SetCurSel(0);
		
		CString uintext;
		m_usercombox.GetWindowText(uintext);
		UserLoginItem item;
		sqllist.GetUser(atoi(uintext),item);
		m_curuin = atoi(uintext);
		if (item.m_flag == 1)
		{
			m_bRemPWD.SetCheck(1);
			m_editpwd.SetWindowText("*******");
			m_bChangePWD = false;
			m_curpwdtext = item.m_pwd;
		}
	}
    m_edtserver.SetWindowText(sqllist.GetServer().c_str());
	GetDlgItem(IDC_STATIC_TIPUIN)->SetWindowText(g_GlobalUnits.GetAppName() + "帐号:");
	GetDlgItem(IDC_STATIC_PWD)->SetWindowText(g_GlobalUnits.GetAppName() + "密码:");
	

	return FALSE;
}



void CPPCLogIn::OnCbnSelchangeCombo1()
{
	if (m_usercombox.GetCount()==0)
		return;
	CPPCSqlite sqllist ;
	CString uintext;
	m_usercombox.GetLBText(m_usercombox.GetCurSel(),uintext);
	if (uintext.GetLength()>0)
	{
		UserLoginItem item;
		sqllist.GetUser(atoi(uintext),item);
		m_curuin =atoi(uintext);
		if (item.m_flag == 1)
		{
			m_bRemPWD.SetCheck(1);
			m_editpwd.SetWindowText("*******");
			m_bChangePWD = false;
			m_curpwdtext = item.m_pwd;
		}
		else
		{
			m_bRemPWD.SetCheck(0);
			m_editpwd.SetWindowText("");
			m_bChangePWD = false;
			m_curpwdtext = "";
		}
	}
	
}


void CPPCLogIn::OnEnChangeEditPwd()
{
	m_bChangePWD = true;
}


void CPPCLogIn::OnCbnSelendcancelCombo1()
{
	
}


void CPPCLogIn::OnBnClickedCheck1()
{
	int ncur = m_usercombox.GetCurSel();
	if (ncur>=0)
	{
		int nstroe = m_bRemPWD.GetCheck();
		CPPCSqlite sqllist ;
		CString uintext;
		m_usercombox.GetLBText(m_usercombox.GetCurSel(),uintext);
		if (uintext.GetLength()>0)
		{
			sqllist.SaveUser(atoi(uintext),nstroe);
		}
	}
	

}


void CPPCLogIn::OnCbnEditchangeCombo1()
{
	m_editpwd.SetWindowText("");
}

int CPPCLogIn::SetAppParames()
{
	CPCMac mac;
	UINT64  pamac;
	bool  readSucess= mac.GetSystemMacInfo((unsigned char*)&pamac,sizeof(pamac));

	g_GlobalUnits.SetUseTcp(false);
	g_GlobalUnits.SetRoomID(20600);
	g_GlobalUnits.SetRemoteHwnd(NULL);
	//g_GlobalUnits.SetPartnerId(1);
	g_GlobalUnits.SetPcMac(pamac);
	g_GlobalUnits.SetPlugType(IM_PLUG_POKERGAME);
	g_GlobalUnits.SetPlugName("游戏");
	g_GlobalUnits.SetUserSessionKey(m_curpwdtext);			///< sessionkey;
	tagGlobalUserData& userdata= g_GlobalUnits.GetGolbalUserData();
	userdata.dwUserID = m_curuin;
	strcpy(userdata.szPassWord,m_curpwdtext.c_str());
	userdata.m_sessionkey = m_curpwdtext.c_str();
	CGameServer *pser = new CGameServer;
	pser->m_kindname = "游戏";
	pser->m_szServerName = "游戏";
	pser->m_roomid = 20600;
	pser->m_processName = "sub.exe";
	CString serverstr;
	m_edtserver.GetWindowText(serverstr);
	unsigned long  add = inet_addr(serverstr);
	if (add == INADDR_NONE)
	  {
		  struct hostent* host=NULL;
		  host = gethostbyname(serverstr);
		  uint32 finalIp = 0;
		  if(NULL != host)
		  {  
				finalIp = *(long*)host->h_addr;
				pser->m_dwServerAddr = finalIp;
		  }
		  else
		  {
			  return -1;
		  }
	  }
	else
	{
		pser->m_dwServerAddr = inet_addr(serverstr);
	
	}
	pser->m_wServerPort = htons(20600);
	g_GlobalUnits.SetRoomType(IMROOM_TYPE_GAME_NEW_POKER);
	g_GlobalUnits.m_gameserver = pser;

	CString mutexstr;
	mutexstr.Format("%lu_%lu",g_GlobalUnits.m_gameserver->m_dwServerAddr,userdata.dwUserID);
	 HANDLE mutexHandle = CreateMutex(NULL, FALSE, mutexstr);
	if (GetLastError()== ERROR_ALREADY_EXISTS)
	{
		CloseHandle(mutexHandle);
		return -2;
	}
	CloseHandle(mutexHandle);
	return 0;
}


void CPPCLogIn::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	__super::OnCancel();
}
