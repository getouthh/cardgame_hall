#pragma once
#pragma comment(lib,"sqlite3")
#include "include/sqlite3/sqlite3.h"
#include "IMUtil/db/SQLiteTable.h"
#include "IMUtil/db/SQLite3Binary.h"
#include <list>
class UserLoginItem
{
public:
	UserLoginItem(){};
	~UserLoginItem(){};
public:
	int m_uin;
	string m_pwd;
	int m_flag;
};
class CPPCSqlite
{
public:
	enum
	{
		DB_ERROR = -1,
	};
	CPPCSqlite(void);
	~CPPCSqlite(void);
	sqlite3* m_pDB;
	string m_curdbfile;
	static string m_sqlitepath;
public:
	void close();
	bool open( const char* szFile );
	SQLiteTable* getTable(const char* szSQL);
		/*  INSERT, UPDATE, or DELETE         
	/* @return 如果操作成功，返回的SQL操作影响的条数 DB_ERROR是错误
	/************************************************************************/
    int exec(const char* szSQL);


	bool CreateServerTabel();
	bool SaveServer(const char* serverinfo);
	string GetServer();


	bool CreateUserTable();
	bool SaveUser(uint32 uin,const char* pwd,int storepwdflag);
	bool SaveUser(uint32 uin,int storepwdflag);
	bool DelUser(uint32 uin);
	bool GetUsers(list<UserLoginItem> &list);
	bool GetUser(uint32 uin,UserLoginItem& user);
};

