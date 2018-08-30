#include "StdAfx.h"
#include "PCMac.h"
#include <direct.h>
#include <iostream>
#include <fstream>

#define BUFSIZE 1024

static const char macFile[] = "pcmac.db";
static const char systemFile[] = "sysdata.db";

static const char PC_InfoFile[] = "pcmac3.db";
static const char PC_systemFile[] = "sysdata3.db";
CPCMac::CPCMac(void)
{
}


CPCMac::~CPCMac(void)
{
}

bool CPCMac::GetSystemMacInfo( unsigned char *p,int n )
{
	char path[MAX_PATH] = {0};
	GetSystemDirectory(path,MAX_PATH);
	string syspathtemp = path;
	syspathtemp.append( "\\config\\");
	syspathtemp.append(macFile);

	SetFileAttributes(syspathtemp.c_str(),FILE_ATTRIBUTE_NORMAL);

	if (GetDriverMacFile(syspathtemp.c_str(),p,n))
	{
		GetDrivers();
		for ( vector<CString>::size_type i =0;i<m_dirverec.size();i++)
		{
			CString pathtemp = m_dirverec[i];
			CString sysdir = syspathtemp.c_str();
			if (sysdir.Find(pathtemp)>=0)
			{
				continue;
			}
			string path = pathtemp.GetBuffer();
			pathtemp.ReleaseBuffer();
			//path.append( "\\promdata\\");
			//mkdir(path.c_str());
			path.append(systemFile);
			CopyFile(syspathtemp.c_str(),path.c_str(),false);
			SetFileAttributes(path.c_str(),FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_ATTRIBUTE_HIDDEN);
		}
		return true;
	}
	else
	{
		GetDrivers();
		vector<CString>::size_type i=0;
		int onldfileindex=-1;
		CFileStatus oldfilestate;
		oldfilestate.m_ctime = CTime::GetCurrentTime();
		for ( i =0;i<m_dirverec.size();i++)
		{
			CString pathtemp = m_dirverec[i];
			CString sysdir = syspathtemp.c_str();
			if (sysdir.Find(pathtemp)>=0)
			{
				continue;
			}
			string path = pathtemp.GetBuffer();
			pathtemp.ReleaseBuffer();
			//path.append( "\\promdata\\");
			path.append(systemFile);
			CFile file;
			CFileStatus filestate;
			if (file.Open(path.c_str(),CFile::modeRead|CFile::shareDenyNone, NULL))
			{
				file.GetStatus(filestate);
				if (oldfilestate.m_ctime > filestate.m_ctime)
				{
					onldfileindex = i;
					oldfilestate.m_ctime = filestate.m_ctime;
				}
				file.Close();
			}

		}

		if (onldfileindex!=-1)
		{
			CString oldfilepath = m_dirverec[onldfileindex];
			string oldpath = oldfilepath.GetBuffer();
			oldfilepath.ReleaseBuffer();
			//oldpath.append( "\\promdata\\");
			oldpath.append(systemFile);
			for ( i =0;i<m_dirverec.size();i++)
			{
				CString pathtemp = m_dirverec[i];
				CString sysdir = syspathtemp.c_str();
				if (sysdir.Find(pathtemp)>=0 || i ==onldfileindex )
				{
					continue;
				}
				string path = pathtemp.GetBuffer();
				pathtemp.ReleaseBuffer();
				//path.append( "\\promdata\\");
				//mkdir(path.c_str());
				path.append(systemFile);
				CopyFile(oldpath.c_str(),path.c_str(),FALSE);
				SetFileAttributes(path.c_str(),FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_ATTRIBUTE_HIDDEN);
			}
			CopyFile(oldpath.c_str(),syspathtemp.c_str(),FALSE);
			return GetDriverMacFile(syspathtemp.c_str(),p,n);
		}
	}
	return false;

}

bool CPCMac::GetDriverMacFile(CString filepath,unsigned char *p,int n)
{
	char path[MAX_PATH] = {0};
	GetSystemDirectory(path,MAX_PATH);
	string pathtemp = filepath.GetBuffer();
	filepath.ReleaseBuffer();

	std::ifstream file(pathtemp.c_str(),ios::binary|ios::in);
	if(!file)
		return false;
	file.seekg(0,std::ios::end);
	std::streamoff size = file.tellg();
	if(size!=n)
		return false;
	file.seekg(0,std::ios::beg);
	for(int i=0;i<n;i++)
	{
		file >>p[i];
	}
	file.close();
	return true;
}
void CPCMac::GetDrivers()
{
	if (!m_dirverec.empty())
		return;
	char szLogicalDriveStrings[BUFSIZE];
	char* szDrive = NULL;
	ZeroMemory(szLogicalDriveStrings,BUFSIZE);
	GetLogicalDriveStrings(BUFSIZE-1,szLogicalDriveStrings);
	szDrive=(PCHAR)szLogicalDriveStrings;
	do
	{
		if(GetDirverInfo(szDrive))
		{
			m_dirverec.push_back(szDrive);
		}
		szDrive +=(lstrlen(szDrive)+1);
	}
	while(*szDrive!='\x00');
}
bool CPCMac::GetDirverInfo(LPSTR szDrive)
{
	bool ret = false;
	UINT uDriveType;
	uDriveType = GetDriveType(szDrive);//TCHAR的指针类型
	switch(uDriveType)
	{
	case DRIVE_UNKNOWN:
		//printf("the drive type cannot be determined");
		break;
	case DRIVE_NO_ROOT_DIR:
		//printf("the root path is invalid, for example,no volume is mounted at the path");
		break;
	case DRIVE_REMOVABLE:
		//printf("the drive is a type that has removable media,for example,a floppy drive or removable hard disk");
		break;
	case DRIVE_FIXED:
		//printf("the drive is a type that cannot be removed,for example,a fixed hard drive");
		ret = true;
		break;
	case DRIVE_REMOTE:
		//printf("the drive is a remote(network) drive");
		break;
	case DRIVE_CDROM:
		//printf("the drive is a CD_ROM drive");
		break;
	case DRIVE_RAMDISK:
		//printf("the drive is a RAM disk");
		break;
	default:
		break;

	}


	return ret;
}
