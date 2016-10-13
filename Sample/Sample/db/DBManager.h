#ifndef _DB_MANAGER_H_
#define _DB_MANAGER_H_

DWORD MYSQLQueryCallBack(UINT nErrorCode, ULONG nEffectRows, void* nKey, YTSvrLib::MYSQLLIB::CMYSQLRecordSet * pResSet, BOOL bAsync);

#define MAX_SUB_TABLE 10
class CDBManager :public YTSvrLib::CMYSQLManagerBase, public YTSvrLib::CSingle<CDBManager>
{
public:
#undef DB_PARSE
#define DB_PARSE(emID,func) static void func( YTSvrLib::CMYSQLQueryInfo* pQueryInfo, YTSvrLib::MYSQLRESPONSEINFO* pRespInfo );
#include "DBParseTable.h"

    CDBManager();
    ~CDBManager(){}

public:
    enum ECallBackType
    {
        eInvalid,
		eQueryTest,// �������ݿ�
        eCount //Max 255
    };

	virtual void SetEvent();// �����¼�����

	static void OnDataRecv()
	{
		GetInstance()->OnDataReceive();// �����첽�������,�����ص�
	}
public:
	void SetConnection(LPCSTR lpszDB, LPCSTR lpszSvr, LPCSTR lpszUser, LPCSTR lpszPass, UINT nPort, UINT nCount, UINT nRetryCount = 0);

	BOOL ApplyCmdAndQueryInfo(YTSvrLib::CMYSQLCommand** ppCmd, YTSvrLib::CMYSQLQueryInfo** ppQueryInfo);

	void DoQuery(const char* pSQL, const char* pSPName,ECallBackType eCallBackType, 
				 LONGLONG wParam = 0, LONGLONG lParam = 0, LONGLONG kParam = 0, LONGLONG xParam = 0, LONGLONG yParam = 0, LONGLONG zParam = 0, BOOL bASync = TRUE);

	void OnServerStart();
};

#endif // !_DB_MANAGER_H_