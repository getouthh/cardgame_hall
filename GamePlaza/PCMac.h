#pragma once
class CPCMac
{
public:
	CPCMac(void);
	~CPCMac(void);
	UINT64  g_macAddr;	//48位MAC地址,用6个字符表示
	vector<CString> m_dirverec;
 bool GetSystemMacInfo( unsigned char *p,int n );
 bool CPCMac::GetDriverMacFile(CString filepath,unsigned char *p,int n);
void GetDrivers();
bool GetDirverInfo(LPSTR szDrive);
};

