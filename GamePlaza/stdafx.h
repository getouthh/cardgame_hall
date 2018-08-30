
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


//////////////////////////////////////////////////////////////////////////

//系统头文件
#include "Nb30.h"
#include "Afxmt.h"
#include "AfxHtml.h"
#include "AfxInet.h"

//全局头文件
#include "game/Constant.h"
#include "game/GlobalDef.h"
#include "game/GlobalField.h"
#include "game/GlobalFrame.h"
#include "game/GlobalRight.h"

//命令头文件
#include "game/CMD_Game.h"
#include "game/CMD_Plaza.h"
#include "game/CMD_Video.h"

//模板库
#include "game/Template.h"

//组件头文件
#include "game/UserFace/UserFace.h"
#include "game/Download/DownLoad.h"
#include "game/gamerank/GameRank.h"
#include "game/Companion/Companion.h"
#include "game/ChannelService/ChannelModule.h"
#include "game/ClientShare/ClientShare.h"
#include "game/SocketModule.h"
#include "game/PropertyModule/PropertyModule.h"
#include "game/ComService/ComService.h"
#include "game/SkinControl/SkinControls.h"
#include "game/SkinRes/SkinResourceModule.h"
#include "im/zygamedefine.h"
#include "room/RoomCmdDef.h"
#include "im/imdefine.h"

#define WM_LOGIN_ROOM_ERROR  WM_USER + 10001
#define WM_LOGIN_DETECT_RESPONSE WM_USER + 10002
#define WM_REDETECT_PROXY_RES   WM_USER + 10003
//#define LOG_GAME
//////////////////////////////////////////////////////////////////////////