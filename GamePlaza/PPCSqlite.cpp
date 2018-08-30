#include "StdAfx.h"
#include "PPCSqlite.h"

string CPPCSqlite::m_sqlitepath;
CPPCSqlite::CPPCSqlite(void)
{
	m_pDB = NULL;
	
	char propathdir[MAX_PATH]={0};
	GetModuleFileName(NULL,propathdir,MAX_PATH);
	CString propathstr = propathdir;
	propathstr = propathstr.Left(propathstr.ReverseFind('\\')+1);
	m_sqlitepath = propathstr + "ppc.db";
}


CPPCSqlite::~CPPCSqlite(void)
{
}

bool CPPCSqlite::open( const char* szFile )
{
	if (!m_pDB)
	{
		close();
		int rc  = sqlite3_open(szFile, &m_pDB);
		if (rc)
		{
			return false;
		}
		//m_curdbfile = szFile;
		sqlite3_busy_timeout(m_pDB,60*1000);
	}

	return true;
}

void CPPCSqlite::close()
{
	if (m_pDB)
	{
		sqlite3_close(m_pDB);
		m_pDB = 0;
	}
}
int CPPCSqlite::exec(const char* szSQL)
{

	if (!m_pDB)
		return DB_ERROR;
	char* szError=0;
	int nRet = sqlite3_exec(m_pDB, szSQL, 0, 0, &szError);
	if (nRet != SQLITE_OK)
	{
		sqlite3_free(szError);
		return DB_ERROR;
	}
	else
	{
		return sqlite3_changes(m_pDB);
	}


}
SQLiteTable* CPPCSqlite::getTable(const char* szSQL)
{
	if (!m_pDB)
		return NULL;
	char* szError=0;
	char** paszResults=0;
	int nRet;
	int nRows(0);
	int nCols(0);
	nRet = sqlite3_get_table(m_pDB, szSQL, &paszResults, &nRows, &nCols, &szError);
	if (nRet == SQLITE_OK)
	{

		return new SQLiteTable(paszResults, nRows, nCols);
	}
	else
	{
		sqlite3_free(szError);
		return NULL;
	}
}
bool CPPCSqlite::CreateServerTabel()
{
	if (open(m_sqlitepath.c_str()))
	{
		CppSQLite3Buffer buf;
		buf.format("create table if not exists %s("
			"server varchar(50),"
			"port INTERGER)","server");
		return (exec(buf) != DB_ERROR);
	}
	return false;
}
bool CPPCSqlite::SaveServer(const char* serverinfo)
{
	if (open(m_sqlitepath.c_str()))
	{
		string sqlstr = "DELETE FROM server";
		exec(sqlstr.c_str());
		CppSQLite3Buffer sql;
		sql.format("insert into server( "
			"server,port) values("
			" '%s',%d)",serverinfo,0);
		return (exec(sql) != DB_ERROR);
	}
	return false;
}
string CPPCSqlite::GetServer()
{
	string retserver = "";
	char sql[4096] = {0};
	sprintf_s(sql,2046,"select * from server");
	SQLiteTable* ptabel2 = getTable(sql);
	if(ptabel2)
	{
		for(int i=0; i<ptabel2->numRows();i++)
		{
			ptabel2->setRow(i);
			retserver  = ptabel2->getStringField(0);
		}
		delete ptabel2;
	}
	return retserver;
}


bool CPPCSqlite::CreateUserTable()
{
	if (open(m_sqlitepath.c_str()))
	{
		CppSQLite3Buffer buf;
		buf.format("create table if not exists %s("
			"uin INTERGER,"
			"pwd varchar(50),"
			"flag INTERGER)","users");
		return (exec(buf) != DB_ERROR);
	}
	return false;
}


bool CPPCSqlite::SaveUser(uint32 uin,const char* pwd,int storepwdflag)
{
	if (open(m_sqlitepath.c_str()))
	{
		CppSQLite3Buffer sqlstr;
		sqlstr.format("DELETE FROM users where uin = %d",uin);
		exec(sqlstr);
		CppSQLite3Buffer sql;
		sql.format("insert into users( "
			"uin,pwd,flag) values("
			" %d,'%s',%d)",uin,pwd,storepwdflag);
		return (exec(sql) != DB_ERROR);
	}
	return false;
}


bool CPPCSqlite::SaveUser(uint32 uin,int storepwdflag)
{
	if (open(m_sqlitepath.c_str()))
	{
		CppSQLite3Buffer sqlstr;
		sqlstr.format("update  users set flag = %d where uin = %d",storepwdflag ,uin);
		return (exec(sqlstr) != DB_ERROR);
	}
	return false;
}

bool CPPCSqlite::DelUser(uint32 uin)
{
	if (open(m_sqlitepath.c_str()))
	{
		CppSQLite3Buffer sqlstr;
		sqlstr.format("DELETE FROM users where uin = %d",uin);
		return (exec(sqlstr) != DB_ERROR);
	}
	return false;
}

bool CPPCSqlite::GetUsers(list<UserLoginItem> &list)
{
	char sql[256] = {0};
	sprintf_s(sql,256,"select * from users");
	
	if (open(m_sqlitepath.c_str()))
	{
		SQLiteTable* ptabel = getTable(sql);
		if (!ptabel)
			return false;
		for (int nrow =0;nrow < ptabel->numRows();nrow++)
		{
			ptabel->setRow(nrow);
			UserLoginItem temitem;
			for (int ncol =0;ncol<ptabel->numFields();ncol++)
			{
				if (strcmp(ptabel->fieldName(ncol),"uin") == 0)
				{
					uint32 id =0;
					id = ptabel->getIntField(ncol);
					temitem.m_uin = id;
				}
				else if (strcmp(ptabel->fieldName(ncol),"pwd")==0)
				{
					string pwd = "";
					pwd = ptabel->getStringField(ncol);
					temitem.m_pwd = pwd;
				}
				else if (strcmp(ptabel->fieldName(ncol),"flag")==0)
				{
				    int flag = 0;
					flag =ptabel->getIntField(ncol);
					temitem.m_flag = flag;
				}
			}
			list.push_back(temitem);
		}
		delete ptabel;
		return true;
	}
	return false;
	
}

bool CPPCSqlite::GetUser(uint32 uin,UserLoginItem& user)
{
	char sql[256] = {0};
	sprintf_s(sql,256,"select * from users where uin=%d",uin);

	if (open(m_sqlitepath.c_str()))
	{
		SQLiteTable* ptabel = getTable(sql);
		if (!ptabel)
			return false;
		for (int nrow =0;nrow < ptabel->numRows();nrow++)
		{
			ptabel->setRow(nrow);
			for (int ncol =0;ncol<ptabel->numFields();ncol++)
			{
				if (strcmp(ptabel->fieldName(ncol),"uin") == 0)
				{
					uint32 id =0;
					id = ptabel->getIntField(ncol);
					user.m_uin = id;
				}
				else if (strcmp(ptabel->fieldName(ncol),"pwd")==0)
				{
					string pwd = "";
					pwd = ptabel->getStringField(ncol);
					user.m_pwd = pwd;
				}
				else if (strcmp(ptabel->fieldName(ncol),"flag")==0)
				{
					int flag = 0;
					flag =ptabel->getIntField(ncol);
					user.m_flag = flag;
				}
			}
		}
		delete ptabel;
		return true;
	}
return false;
}