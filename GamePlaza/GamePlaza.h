
// GamePlaza.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CGamePlazaApp:
// 有关此类的实现，请参阅 GamePlaza.cpp
//

class CGamePlazaApp : public CWinApp
{
public:
	CGamePlazaApp();

// 重写
public:
	virtual BOOL	InitInstance();
	void			SendMsgToRemoteWnd(int type,uint32 srcuin,uint32 destuin,CString param1,CString param2);
	int GetLogLevel();
	void ReadNetFlag();
	tm GetLogTimer(bool bservertime);
private:
	
// 实现
	 
	DECLARE_MESSAGE_MAP()
public:
	void ReadTestIp();
	virtual int ExitInstance();
	bool LoadLoglibrary(bool bload);

	time_t m_servertime;
	CString m_rootdir;

	uint16 m_loginDetectIpWaitTime;
	uint16 m_afterLoginDetectWaitTime;

	CString m_testIP;
};

extern CGamePlazaApp theApp;