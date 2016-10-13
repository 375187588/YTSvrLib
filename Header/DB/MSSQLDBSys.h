#pragma once

#pragma   once

#include <winsock2.h>
#include "../Service/SyncObj.h"
#include "MSSQLDB.h"

#include "../Service/Thread.h"
#include "../Service/Utility.h"
//#include "errmsg.h"
#include <vector>
//#include <queue>
#include "..\stl\wqueue.h"

DWORD MSSQLQueryCallBack(BOOL /*bSuc*/,UINT_PTR /*dwKey*/, long* pRetValue, long nRetCount, CMSSQLRecordSet * /*pResSet*/);

//#define  WM_MYSQLEXECUTE   WM_USER+0x1111

typedef DWORD (*MSSQLProcFunc)(BOOL dwSuc,UINT_PTR nKey, long* pRetValue, long nRetCount, CMSSQLRecordSet * pResSet);

class   CMSSQLDBSystem;

typedef struct tagMSDBConnectInfo
{    
    int  m_nSN;                   //DB��������ţ�0~n��
    std::string m_strConnectionInfo;

}MSCONNECT_INFO,*PMSCONNECT_INFO;

class   CMSSQLClient:public CThread
{
    friend   class  CMSSQLDBSystem;

public:

#define  DEFAULTPOOLSIZE  1000
#define	MAX_DATABASE_TRY	3
    typedef  struct  tagSQLStringRequest :public CRecycle //SQL���������Ϣ   
    {
        UINT_PTR      m_nKey;
        unsigned long m_nHash;

        //char          m_strSPName[32];

        CMSSQLCommand* m_pCmd;
        MSSQLProcFunc m_pProcFun; 
		UINT		m_nTry;

        tagSQLStringRequest()
        {
            Init();
        }

        virtual void Init()
        {
            m_nKey = 0;
            m_pCmd = 0;
            m_nHash = 0;
            m_pProcFun  = NULL;
			m_nTry = 0;
        }
        virtual void Clear(){}
    }SQLSTRING_REQUEST;
typedef CWQueue<SQLSTRING_REQUEST*> QueueSQLReq;

public:  

    CMSSQLClient(CMSSQLDBSystem *pSystem);
    CMSSQLClient(CMSSQLDBSystem *pSystem,int nMaxSize);

    virtual ~CMSSQLClient(void);
    ///����MYSQL DB
    BOOL   ConnectDB( const char* pszConnectionString );
    ///����MYSQL DB
    BOOL   ConnectDB(void);
	BOOL	ReconnectDB();
	BOOL	IsConnected() { return ( m_pMSSqlConn != NULL && m_pMSSqlConn->IsConnected()); }

    virtual bool PreLoopFunc();

    virtual void Init(){}
    virtual void Clear(){}

    void SetConnectionInfo( std::string& strConnectionInfo ){ m_strConnectionInfo = strConnectionInfo; }

    SQLSTRING_REQUEST* ApplySQLReq() 
    { 
        //LOG( "ApplySQLReq" );
        return m_ReqObjPool.ApplyObj(); 
    }
    void ReclaimSQLReq( SQLSTRING_REQUEST* pReq )
    { 
        //LOG("ReclaimSQLReq");
        m_ReqObjPool.ReclaimObj(pReq); 
    }
	UINT GetReqCountInQueue()
	{
		m_ReqQueue.Lock();
		UINT nSize = m_ReqQueue.size();
		m_ReqQueue.UnLock();
		return nSize;
	}
	DWORD GetExecuteCount() { return m_dwExecuteCount; }
	void	SetID(size_t n) { m_nID = n; }
	size_t	GetID() { return m_nID; }
	void	NeedReset();
private:

    virtual void OnTerminated(void);
    ///����SQL����
    BOOL   RecvSQLRequest( SQLSTRING_REQUEST* pRequest );
    ///ִ��SQL�������
    BOOL   ExecuteSQL(SQLSTRING_REQUEST *pSqlStr);

private:
    CMSSQLDBSystem   *				m_pDBSystem;
    ///MySQL����	
    CMSSQLConnection *				m_pMSSqlConn;    
    ///SQL��������б� 
    QueueSQLReq						m_ReqQueue; 
    ///SQL�����������
    CPool<SQLSTRING_REQUEST,1024>		m_ReqObjPool;
    ///�ź������   
	YTSvrLib::CSemaphore						m_hSemaphore;

//  CLock							m_QueueLock;
	YTSvrLib::CLock							m_PoolLock;

    std::string                     m_strConnectionInfo;
	DWORD	m_dwExecuteCount;
	size_t	m_nID;
	BOOL	m_bNeedReset;

protected:
    void   OnError(DWORD dwErrCode);
    void   Execute(void);   
};

class  CMSSQLDBSystem : public CThreadPool
{
    friend  class  CMSSQLClient;

public:

	CMSSQLDBSystem(void) : m_RecSetPool("CMSSQLRecordSet"),m_CommandPool("CMSSQLCommand")
	{ m_nDbCount = 1; m_nHashNum = 1; m_nDbLowCount = 1; m_nDbHighCount = 0; m_bDBHighLowEnable = FALSE;}
    virtual ~CMSSQLDBSystem(void){}

    ///����mysql server�б���Ϣ 
    void   SetMSSqlServerListInfo( std::vector<MSCONNECT_INFO>& vecConnectInfo); 
    ///��������mysql server��Ϣ
    void   AddServerConnectInfo(MSCONNECT_INFO& sConnectInfo);
    ///ϵͳ����	
    BOOL   Startup( BOOL bHighLowEnable = TRUE ,UINT nRetryCount = 0); 
    ///��ȡ����ʧ�ܵ�����vector
    std::vector<int>  GetFailedConnVec(void){ return m_vecFailedConnDB;}
    ///����SQL�������     
    BOOL   AddSQLRequest(UINT_PTR dwKey,long nHashParm, /*const char pSPName[],*/ CMSSQLCommand* pCmd, MSSQLProcFunc pFunc, BOOL bHighLevel = FALSE );

    CMSSQLRecordSet* ApplyRecSet()
    {
        //LOG("ApplyRecSet");
        return m_RecSetPool.ApplyObj(); 
    }
    void   ReclaimRecSet( CMSSQLRecordSet* pRecSet )
    {
//        LOGASync("ReclaimRecSet pRecSet=0x%08x(0x%08x)", pRecSet, pRecSet->m_pRecordSet );
		pRecSet->Close();
        m_RecSetPool.ReclaimObj( pRecSet ); 
    }

    CMSSQLCommand* ApplyCmd()
    { 
		CMSSQLCommand* pCmd = m_CommandPool.ApplyObj();
        /*Removed by Waj at [10/8/2010 ]
       LOGASync("ApplyCmd=0x%08x", pCmd);
       */
        return pCmd;
    }
    void   ReclaimCmd( CMSSQLCommand* pCmd )
    { 
        /*Removed by Waj at [10/8/2010 ]
        LOGASync("ReclaimCmd=0x%08x", pCmd);
        */
        m_CommandPool.ReclaimObj( pCmd ); 
    }
	BOOL	IsAllDBClientIdle();
	size_t	GetAllCmdInQueue();
	size_t	GetCmdInQueue( size_t ayID[32], size_t ayQueue[32] );
	void	ResetClient( size_t nClientID );

	EM_DATABASE_TYPE GetDBType() const{return m_emDatabseType;}
	UINT GetRetryCount() const {return m_nRetryCount;}
protected:

    ///������������Ϣ�б� 
    std::vector< MSCONNECT_INFO  >  m_vecConnectInfo;
	EM_DATABASE_TYPE				m_emDatabseType;
    ///����MySQL DBʧ�ܵ�vector
    std::vector< int >              m_vecFailedConnDB;

    size_t                           m_nDbCount;
	BOOL							m_bDBHighLowEnable;
	size_t							m_nDbLowCount;
	size_t							m_nDbHighCount;
    DWORD                           m_nHashNum;
	UINT							m_nRetryCount;

    CPool<CMSSQLRecordSet,1024>          m_RecSetPool;
    CPool<CMSSQLCommand,1024>            m_CommandPool;    

protected: 

    ///HASH�������ݿ� �±�Ϊ(0~n)
    virtual size_t HashAlloteDBServer( size_t nHashParm, BOOL bHighLevel )
    { 
        /*if ( strlen(pHashParam) < 1 )
        {
            return ( m_nDbCount+atoi(pHashParam));
        }

        int nCount = 0;
        for (int i=0; i< (int)m_nHashNum ; ++i)
        {
            nCount += ::tolower(pHashParam[i]);
        }*/
		if( m_nDbCount<= 0 )
			return 0;
		if( bHighLevel != FALSE && m_nDbHighCount > 0 )
		{
			size_t nRet = nHashParm % m_nDbHighCount + m_nDbLowCount;
			return nRet;
		}
		if( m_nDbLowCount<= 0 )
			return 0;
        size_t nRet = nHashParm % m_nDbLowCount;
        return nRet;
        //return -1;
    }
    ///MySQL�ͻ������ӶϿ�����
    virtual void  OnDisconnect(CMSSQLClient* pClient){ pClient->ConnectDB(); }
    ///MySQL��ִ��SQL��������������ʱ����
    virtual void  OnOtherError(CMSSQLClient* /*pClient*/){}
};


//DWORD SQLQueryCallBackProc(BOOL /*bSuc*/,DWORD /*dwKey*/, CMSSQLRecordSet * /*pResSet*/);

