#include "Stdafx.h"
#include "GamePlaza.h"
#include "GameFrame.h"
#include "GlobalUnits.h"
#include "game/ComService/Encrypt.h"
#include "include/md5class.h"
#include "IMUtil/iniFile.h"

#include "coretime.h"
#include "PPCLogIn.h"
#include "PPCSqlite.h"
#include "LastLogusers.h"
HINSTANCE ghInst = NULL;
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGamePlazaApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//应用程序对象说明
CGamePlazaApp theApp;

//构造函数
CGamePlazaApp::CGamePlazaApp()
{

	m_loginDetectIpWaitTime = 1000;
	m_afterLoginDetectWaitTime = 3000;
}
static HANDLE mutexHandle = 0;
//初始化函数
BOOL CGamePlazaApp::InitInstance()
{
	__super::InitInstance();


	//判断是否重入
	//#ifndef ADMIN_PLAZA
	//#ifndef _DEBUG
	//	bool bExistIGame=false;
	//	CMutex Mutex(FALSE,MAIN_DLG_CLASSNAME,NULL);
	//	if (Mutex.Lock(0)==FALSE) bExistIGame=true;
	//	CWnd * pIGameWnd=CWnd::FindWindow(MAIN_DLG_CLASSNAME,NULL);
	//	if (pIGameWnd!=NULL) 
	//	{
	//		bExistIGame=true;
	//		if (pIGameWnd->IsIconic()) pIGameWnd->ShowWindow(SW_RESTORE);
	//		pIGameWnd->SetActiveWindow();
	//		pIGameWnd->BringWindowToTop();
	//		pIGameWnd->SetForegroundWindow();
	//	}
	//	if (bExistIGame==true) return FALSE;
	//#endif
	//#endif

	//AfxMessageBox("test");
	//程序初始化
	AfxOleInit();
	AfxInitRichEdit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//设置注册表
	SetRegistryKey(szSystemRegKey);

	////加载界面库
	//CSkinAttribute::LoadSkinAttribute(QUERY_OBJECT_INTERFACE(m_SkinRecordFile,IUnknownEx));
	//加载配置
	g_GlobalOption.LoadOptionParameter();
	CoreSocket::SocketStartUp();
	/*
	CPPCSqlite sqllist ;
	bool busc = sqllist.CreateServerTabel();
	busc = sqllist.SaveServer("www.baidu.com");
	string server = sqllist.GetServer();
	*/
	/*
	CPPCSqlite sqllist ;
	bool busc = sqllist.CreateUserTable();
	busc = sqllist.SaveUser(100,"sdf",1);
	list<UserLoginItem> listuser;
	busc = sqllist.GetUsers(listuser);
	string server = sqllist.GetServer();
	busc = sqllist.DelUser(100);
	*/
	{
		char propathdir[MAX_PATH]={0};
		GetModuleFileName(NULL,propathdir,MAX_PATH);
		CString propathstr = propathdir;
		propathstr = propathstr.Left(propathstr.ReverseFind('\\')+1);
		CLastLogusers::setDir(propathstr);
		CPPCSqlite sqlite;
		sqlite.CreateServerTabel();
		sqlite.CreateUserTable();

	}



	//全局信息
	if (g_GlobalUnits.InitGlobalUnits()==false)
	{
		ShowInformation(TEXT("游戏初始化失败，程序即将退出！"),0,MB_ICONSTOP);
		return FALSE;
	}

	CPPCLogIn dlg;
	int iRet = dlg.DoModal();
	if (iRet == IDCANCEL)
	{
		return TRUE;
	}
	
//#ifndef _DEBUG
	CString paramstr =m_lpCmdLine;
	//if (paramstr.GetLength()<=0)
	{
		//ShowInformation(TEXT("游戏启动方式出错，程序即将退出！"),0,MB_ICONSTOP);
		//return FALSE;
	}

	CMarkup xml;
	CString para= m_lpCmdLine;
	CString tempstr;
	HWND ParentHandle = NULL;
	uint32 uin = 0;
	CString pwd= "";
	CString ipstr;
	uint16  nprot=0;
	int roomtype = -1;
	uint32 roomid = 0;
	uint32 partnerid = 0;
	UINT64 pamac = 0;
	CString roomname;
	CString proname;
	uint32 prover=0;
	CString sessionkey;
	CString resPath = _T("");
	bool useTcp = false;
	/*
	if (para.GetLength()>0)
	{
		xml.SetDoc(para);
		if (xml.FindElem("GameConfig"))
		{
			while(xml.FindChildElem())
			{
				CString tagName = xml.GetChildTagName();
				if(tagName.CompareNoCase(_T("PARAM1")) == 0)
				{
					tempstr = xml.GetChildData();
					ParentHandle =(HWND)atol(tempstr);

				}
				else if(tagName.CompareNoCase(_T("uin")) == 0)
				{
					uin = atoi(xml.GetChildData());
				}
				else if (tagName.CompareNoCase(_T("password")) ==0)
				{
					pwd = xml.GetChildData();
				}

				else if (tagName.CompareNoCase(_T("AppType")) == 0)
				{
					roomtype = atoi(xml.GetChildData());
				}

				else if (tagName.CompareNoCase("IP") ==0)
				{
					ipstr =xml.GetChildData();
				}

				else if (tagName.CompareNoCase("PORT") ==0)
				{
					nprot = atoi(xml.GetChildData());
				}
				else if (tagName.CompareNoCase("ROOMID") ==0)
				{
					roomid = atoi(xml.GetChildData());
				}
				else if (tagName.CompareNoCase("pid") ==0)
				{
					partnerid = atoi(xml.GetChildData());

				}
				else if (tagName.CompareNoCase("ROOMNAME")==0)
				{
					roomname = xml.GetChildData();

				}
				else if (tagName.CompareNoCase("PROVER") ==0)
				{
					prover = atoi(xml.GetChildData());

				}
				else if (tagName.CompareNoCase("PRONAME") ==0)
				{
					proname = xml.GetChildData();

				}
				else if (tagName.CompareNoCase("SESSIONKEY") ==0)
				{
					sessionkey = xml.GetChildData();
				}
				else if (tagName.CompareNoCase("macaddr") ==0)
				{
					pamac = _strtoui64(xml.GetChildData(),NULL,10);
				}
				else if(tagName.CompareNoCase(_T("ResPath")) == 0)
				{
					resPath = xml.GetChildData();
				}
				else if(tagName.CompareNoCase(_T("USETCP")) ==0)
				{
					useTcp = !!atoi(xml.GetChildData());
				}
				else if(tagName.CompareNoCase(_T("SERVERTIME")) == 0)
					m_servertime = _atoi64(xml.GetChildData());
				else if(tagName.CompareNoCase(_T("ROOTDIR")) == 0)
					m_rootdir = xml.GetChildData();
				else if(tagName.CompareNoCase(_T("PROXYLIST")) == 0)
				{
					g_GlobalUnits.ParseProxyItem(xml.GetChildData());
				}
			}
			
			
		}
		else
		{
			AfxMessageBox("程序启动参数出错!");
			return FALSE;
		}
	}
	else
	{
		AfxMessageBox("程序启动方式出错!");
		return FALSE;
	}*/
	CString pronametemp = proname;
	CString ext ="";
	if (proname.GetLength()>4)
	{
		ext = proname.Right(4);
	}
	if (ext.CompareNoCase(".exe")!=0 || ext=="")
		proname+=".exe";

	
	
	/*CString prodir;
	prodir = proname.Left(proname.ReverseFind('.'));
	
	char propathdir[MAX_PATH]={0};
	GetModuleFileName(NULL,propathdir,MAX_PATH);
	CString propathstr = propathdir;
	propathstr = propathstr.Left(propathstr.ReverseFind('\\')+1);

	prodir = propathstr + prodir + "\\";*/
	/*bool bmultienter=false;
	CMarkup proxml;
	if (proxml.Load(prodir + "config.xml"))
	{
		if (proxml.FindElem("Config"))
		{
			if (proxml.FindChildElem("MultiMode"))
				bmultienter = ( atoi(proxml.GetChildData())==1?true:false);
		}
	}
	CString mutexstr;
	mutexstr.Format("%d_IM_GAMEPOKER_PRO",partnerid);
	mutexHandle = CreateMutex(NULL, FALSE, mutexstr); 
	if (!bmultienter) 
	{
		if (GetLastError()== ERROR_ALREADY_EXISTS)
		{
			AfxMessageBox("已经有程序正在运行当中!");
			return FALSE;
		}
	}*/

	LoadLoglibrary(true);

	int logLv = GetLogLevel();
	if(logLv >0)
	{
		//CString logFileName;
		//logFileName.Format(_T("newppc_%u_%u"),partnerid,uin);
		//Log::open(logFileName,logLv);
	}
	LOG(4)("xml=%s,tcp=%d\n",para.GetBuffer(),useTcp);
	
	

	CString prodir;
	prodir = proname.Left(proname.ReverseFind('.'));

	char propathdir[MAX_PATH]={0};
	GetModuleFileName(NULL,propathdir,MAX_PATH);
	CString propathstr = propathdir;
	propathstr = propathstr.Left(propathstr.ReverseFind('\\')+1);

	prodir = propathstr + "resource\\gameview\\";
	//bool ballowstart = CGameStartCheck::Start(prodir + "startconfig.xml",partnerid,uin);
	
	char pathdir[MAX_PATH]={0};
	GetModuleFileName(NULL,pathdir,MAX_PATH);
	CString pathstr = pathdir;
	pathstr = pathstr.Left(pathstr.ReverseFind('\\'));
	pathstr = pathstr.Left(pathstr.ReverseFind('\\')+1);

	CString parastr;
	parastr.Format("newgamehall_partnerid(%d_%lu)",partnerid,uin);


	//变量定义
	WNDCLASS WndClasss;
	ZeroMemory(&WndClasss,sizeof(WndClasss));

	//注册窗口
	WndClasss.style=CS_DBLCLKS;
	WndClasss.lpfnWndProc=DefWindowProc;
	WndClasss.lpszClassName=MAIN_DLG_CLASSNAME;
	WndClasss.hInstance=AfxGetInstanceHandle();
	WndClasss.hIcon=LoadIcon(MAKEINTRESOURCE(IDR_MAINFRAME));
	WndClasss.hCursor=LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW));
	if (AfxRegisterClass(&WndClasss)==FALSE) AfxThrowResourceException();

	
	CGameFrame * pMainFrame=new CGameFrame();
	pMainFrame->Create(MAIN_DLG_CLASSNAME,TEXT(""),WS_MINIMIZEBOX|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,CRect(0,0,0,0));
	m_pMainWnd=pMainFrame;

	return TRUE;
}



void CGamePlazaApp::SendMsgToRemoteWnd(int type,uint32 srcuin,uint32 destuin,CString param1,CString param2)
{
	if (g_GlobalUnits.GetRemoteHwnd() == NULL)
		return;
	CMarkup xml;
	CString vertext = _T("<?xml version=\"1.0\" encoding=\"GB2312\" ?>\r\n");
	xml.SetDoc(vertext);
	xml.AddElem("MSG");
	xml.AddAttrib("VER",1);
	xml.AddChildElem("CMD",type);
	xml.AddChildElem("TYPE",g_GlobalUnits.GetPlugType());
	xml.AddChildElem("PLUGNAME",g_GlobalUnits.GetPlugName());
	if (type == IM_PLUG_CMD::IMPLUG_OPEN)
	{
		xml.AddChildElem("FROMUIN",0);
		xml.AddChildElem("DESTUIN",0);
		xml.AddChildElem("PARAM1",atol(param1));
		xml.AddChildElem("PARAM2",atoi(param2));
	}
	else if (type == IM_PLUG_CMD::IM_PLUG_GANER)
	{
		xml.AddChildElem("FROMUIN",0);
		xml.AddChildElem("DESTUIN",0);
		xml.AddChildElem("PARAM1",param1);
		xml.AddChildElem("PARAM2",param2);
	}
	else if (type ==IM_PLUG_CMD::IMPLUG_INVITE )
	{
		xml.AddChildElem("FROMUIN",srcuin);
		xml.AddChildElem("DESTUIN",destuin);
		xml.AddChildElem("PARAM1",atoi(param1));
	}
	else if (type == IM_PLUG_CMD::IM_PLUG_VIEWDATA)
	{
		xml.AddChildElem("FROMUIN",srcuin);
		xml.AddChildElem("DESTUIN",destuin);
		xml.AddChildElem("PARAM1",atoi(param1));
	}
	else if (type == IM_PLUG_CMD::IM_PLUG_ADDFREIND)
	{
		xml.AddChildElem("FROMUIN",srcuin);
		xml.AddChildElem("DESTUIN",destuin);
		xml.AddChildElem("PARAM1",atoi(param1));
	}
	else if (type == IM_PLUG_CMD::IM_PLUG_SENDMSG)
	{
		xml.AddChildElem("FROMUIN",srcuin);
		xml.AddChildElem("DESTUIN",destuin);
		xml.AddChildElem("PARAM1",atoi(param1));
	}
	else if (type == IM_PLUG_CMD::IM_PLUG_SENDMONEY)
	{
		xml.AddChildElem("FROMUIN",srcuin);
		xml.AddChildElem("DESTUIN",destuin);
		xml.AddChildElem("PARAM1",atoi(param1));
	}
	else if (type == IM_PLUG_CMD::IM_PLUG_KICKOUT)
	{
		xml.AddChildElem("FROMUIN",srcuin);
		xml.AddChildElem("DESTUIN",destuin);
		xml.AddChildElem("PARAM1",atoi(param1));
	}
	else if(type == IM_PLUG_CMD::IM_NEWPPC_CMD)
	{
		xml.AddChildElem("GameHwnd",param1);
		xml.AddChildElem("PARAM2",param2);
	}
	CString datastr = xml.GetDoc();
	COPYDATASTRUCT data;
	data.cbData = datastr.GetLength();
	data.dwData = datastr.GetLength();
	data.lpData = datastr.GetBuffer();
	SendMessage(g_GlobalUnits.GetRemoteHwnd(),WM_COPYDATA,NULL,(LPARAM)&data);
	datastr.ReleaseBuffer();
}

int CGamePlazaApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	LoadLoglibrary(false);

	CoreSocket::SocketCleanUp();
	return CWinApp::ExitInstance();
}


int CGamePlazaApp::GetLogLevel()
{
	int logLevel = 0;

	char dir[MAX_PATH]={0};
	GetModuleFileName(NULL,dir,MAX_PATH);
	CString path = dir;
	path = path.Left(path.ReverseFind('\\'));
	path = path.Left(path.ReverseFind('\\'));
	path = path.Left(path.ReverseFind('\\')+1);
	path += _T("config\\sys.cfg");
	if(!PathFileExists(path.GetBuffer()))
		return logLevel;

	CIniFile iniFile(path.GetBuffer());
	BOOL ret = iniFile.ReadFile();
	if(!ret)
		return logLevel;

	logLevel = iniFile.GetValueI("Log","Level");
	m_loginDetectIpWaitTime = iniFile.GetValueI("DETECT","beforelogin",1000);
	m_afterLoginDetectWaitTime = iniFile.GetValueI("DETECT","afterlogin",3000);
	return logLevel;
}

void CGamePlazaApp::ReadNetFlag()
{
	char dir[MAX_PATH]={0};
	GetModuleFileName(NULL,dir,MAX_PATH);
	CString path = dir;
	path = path.Left(path.ReverseFind('\\'));
	path = path.Left(path.ReverseFind('\\'));
	path = path.Left(path.ReverseFind('\\')+1);
	path += _T("config\\sys.cfg");
	if(PathFileExists(path.GetBuffer()))
	{
		CIniFile iniFile(path.GetBuffer());
		if(iniFile.ReadFile())
		{
			if(CIniFile::noID != iniFile.FindKey(_T("PPCNET")))
			{
				bool useTcp = iniFile.GetValueB(_T("PPCNET"),_T("usetcp"),false);
				g_GlobalUnits.SetUseTcp(useTcp);
				LOG(4)("CGamePlazaApp::ReadNetFlag Tcp=%d\n",useTcp);
			}
			else
				LOG(4)("CGamePlazaApp::ReadNetFlag not fid PPCNET\n");
		}
	}
}

bool CGamePlazaApp::LoadLoglibrary(bool bload)
{
	if(ghInst) 
	{
		::FreeLibrary(ghInst);//释放Dll函数;
		ghInst = NULL;
	}

	return false;
}

tm CGamePlazaApp::GetLogTimer(bool bservertime)
{
	tm sertime;
	if(bservertime)
	{
		time_t sertm = m_servertime;
		sertm += GetTickCount()/1000;		
		localtime_s(&sertime,&sertm);
	}
	else
	{
		time_t timep;
		time(&timep);
		sertime = *localtime(&timep); 
	}
	return sertime;
}

void CGamePlazaApp::ReadTestIp()
{
	CMarkup testxml;
	char dir[MAX_PATH]={0};
	GetModuleFileName(NULL,dir,MAX_PATH);
	CString path = dir;
	path = path.Left(path.ReverseFind('\\'));
	path = path.Left(path.ReverseFind('\\'));
	path = path.Left(path.ReverseFind('\\')+1);

	if (testxml.Load(path + "gametest.xml"))
	{
		if (testxml.FindChildElem("IP"))
		{
			m_testIP = testxml.GetChildData();
		}
	}
	else
		m_testIP = "";
}